/*
Speeduino - Simple engine management for the Arduino Mega 2560 platform
Copyright (C) Josh Stewart
A full copy of the license may be found in the projects root directory
*/
/** @file
 * Process Incoming and outgoing serial communications.
 */
#include "globals.h"
#include "comms.h"
#include "comms_legacy.h"
#include "comms_secondary.h"
#include "storage.h"
#include "maths.h"
#include "utilities.h"
#include "decoders.h"
#include "TS_CommandButtonHandler.h"
#include "pages.h"
#include "page_crc.h"
#include "logger.h"
#include "idle.h"
#include "corrections.h"
#include "table3d_axis_io.h"
#include BOARD_H
#ifdef RTC_ENABLED
  #include "rtc_common.h"
#endif

static byte currentPage = 1;//Not the same as the speeduino config page numbers
bool firstCommsRequest = true; /**< The number of times the A command has been issued. This is used to track whether a reset has recently been performed on the controller */
static byte currentCommand; /**< The serial command that is currently being processed. This is only useful when cmdPending=True */
static bool chunkPending = false; /**< Whether or not the current chunk write is complete or not */
static uint16_t chunkComplete = 0; /**< The number of bytes in a chunk write that have been written so far */
static uint16_t chunkSize = 0; /**< The complete size of the requested chunk write */
static int valueOffset; /**< The memory offset within a given page for a value to be read from or written to. Note that we cannot use 'offset' as a variable name, it is a reserved word for several teensy libraries */
byte logItemsTransmitted;
byte inProgressLength;
SerialStatus serialStatusFlag;
SerialStatus serialSecondaryStatusFlag;

static bool isMap(void) {
    // Detecting if the current page is a table/map
  return (currentPage == veMapPage) || (currentPage == ignMapPage) || (currentPage == afrMapPage) || (currentPage == fuelMap2Page) || (currentPage == ignMap2Page);
}

static bool hasStepperIdleAlgorithm(void)
{
  return (configPage6.iacAlgorithm == IAC_ALGORITHM_STEP_OL)
      || (configPage6.iacAlgorithm == IAC_ALGORITHM_STEP_CL)
      || (configPage6.iacAlgorithm == IAC_ALGORITHM_STEP_OLCL);
}

static int16_t getLegacyColdAdvance(void)
{
  return ((int16_t)table2D_getValue(&CLTAdvanceTable, currentStatus.coolant + CALIBRATION_TEMPERATURE_OFFSET) - 15) * 10;
}

bool isLegacyCommandByte(byte highByte, bool allowLegacyComms)
{
  if (highByte == 'F')
  {
    return true;
  }

  return allowLegacyComms && ((((highByte >= 'A') && (highByte <= 'z')) || (highByte == '?')));
}

bool shouldRouteSecondaryTunerStudioToPrimary(uint8_t secondaryProtocol, uint8_t status5)
{
  return (secondaryProtocol == SECONDARY_SERIAL_PROTO_TUNERSTUDIO)
      && BIT_CHECK(status5, BIT_STATUS5_ALLOW_TS_ON_SECONDARY_COMMS);
}

LegacyHandlerDispatch getPrimaryLegacyHandlerDispatch(byte command, uint8_t secondaryProtocol)
{
  LegacyHandlerDispatch dispatch = {command, false, false, false};

  switch (command)
  {
    case 'b':
    case 'd':
    case 'M':
    case 'p':
    case 'Q':
    case 'r':
      dispatch.handled = true;
      break;

    case 'B':
      dispatch.handled = true;
      dispatch.enableCompatMode = true;
      break;

    case 'S':
      dispatch.command = getLegacyVersionResponseCommand(command, false, secondaryProtocol);
      dispatch.handled = true;
      dispatch.useSecondaryStatusFlag = true;
      break;

    default:
      break;
  }

  return dispatch;
}

byte decodeLegacyPageIdentifier(byte rawPageValue)
{
  if ((rawPageValue >= '0') && (rawPageValue <= '9'))
  {
    return rawPageValue - 48U;
  }
  if ((rawPageValue >= 'a') && (rawPageValue <= 'f'))
  {
    return rawPageValue - 87U;
  }
  if ((rawPageValue >= 'A') && (rawPageValue <= 'F'))
  {
    return rawPageValue - 55U;
  }

  return rawPageValue;
}

bool parseLegacyCommandButtonRequest(const uint8_t *buffer, size_t bufferSize, LegacyCommandButtonRequest *request)
{
  if ((buffer == nullptr) || (request == nullptr) || (bufferSize < 2U))
  {
    return false;
  }

  request->command = word(buffer[0], buffer[1]);
  return true;
}

bool parseLegacyEepromTransferHeader(const uint8_t *buffer, size_t bufferSize, LegacyEepromTransferHeader *header)
{
  if ((buffer == nullptr) || (header == nullptr) || (bufferSize < 3U))
  {
    return false;
  }

  header->eepromSize = word(buffer[1], buffer[0]);
  return buffer[2] == ',';
}

size_t buildLegacyEepromTransferHeader(uint16_t eepromSize, uint8_t *buffer, size_t bufferSize)
{
  if ((buffer == nullptr) || (bufferSize < 3U))
  {
    return 0U;
  }

  buffer[0] = lowByte(eepromSize);
  buffer[1] = highByte(eepromSize);
  buffer[2] = ',';
  return 3U;
}

bool isLegacyEepromTransferSizeValid(uint16_t requestedSize, uint16_t expectedSize)
{
  return requestedSize == expectedSize;
}

LegacyLoggerCommand getLegacyLoggerCommand(byte command)
{
  LegacyLoggerCommand loggerCommand = {LEGACY_LOGGER_NONE, false};

  switch (command)
  {
    case 'H':
      loggerCommand.action = LEGACY_LOGGER_START_TOOTH;
      loggerCommand.sendAck = true;
      break;

    case 'h':
      loggerCommand.action = LEGACY_LOGGER_STOP_TOOTH;
      break;

    case 'J':
      loggerCommand.action = LEGACY_LOGGER_START_COMPOSITE;
      loggerCommand.sendAck = true;
      break;

    case 'j':
      loggerCommand.action = LEGACY_LOGGER_STOP_COMPOSITE;
      break;

    case 'O':
      loggerCommand.action = LEGACY_LOGGER_START_TERTIARY;
      loggerCommand.sendAck = true;
      break;

    case 'o':
      loggerCommand.action = LEGACY_LOGGER_STOP_TERTIARY;
      break;

    case 'X':
      loggerCommand.action = LEGACY_LOGGER_START_CAMS;
      loggerCommand.sendAck = true;
      break;

    case 'x':
      loggerCommand.action = LEGACY_LOGGER_STOP_CAMS;
      break;

    default:
      break;
  }

  return loggerCommand;
}

LegacyTriggeredLogType getLegacyTriggeredLogType(bool toothLogEnabled, uint8_t compositeTriggerUsed)
{
  if (toothLogEnabled)
  {
    return LEGACY_TRIGGERED_LOG_TOOTH;
  }

  if (compositeTriggerUsed > 0U)
  {
    return LEGACY_TRIGGERED_LOG_COMPOSITE;
  }

  return LEGACY_TRIGGERED_LOG_NONE;
}

bool parseLegacyPageReadRequest(const uint8_t *buffer, size_t bufferSize, LegacyPageReadRequest *request)
{
  if ((buffer == nullptr) || (request == nullptr) || (bufferSize < 6U))
  {
    return false;
  }

  request->page = buffer[1];
  request->offset = word(buffer[3], buffer[2]);
  request->length = word(buffer[5], buffer[4]);
  return true;
}

bool parseLegacyPageWriteRequest(const uint8_t *buffer, size_t bufferSize, LegacyPageWriteRequest *request)
{
  if ((buffer == nullptr) || (request == nullptr) || (bufferSize < 6U))
  {
    return false;
  }

  request->page = buffer[1];
  request->offset = word(buffer[3], buffer[2]);
  request->length = word(buffer[5], buffer[4]);
  return true;
}

bool parseLegacyCrcRequest(const uint8_t *buffer, size_t bufferSize, LegacyCrcRequest *request)
{
  if ((buffer == nullptr) || (request == nullptr) || (bufferSize < 2U))
  {
    return false;
  }

  request->page = buffer[1];
  return true;
}

bool parseLegacyOutputChannelsRequest(const uint8_t *buffer, size_t bufferSize, LegacyOutputChannelsRequest *request)
{
  if ((buffer == nullptr) || (request == nullptr) || (bufferSize < 6U))
  {
    return false;
  }

  request->tsCanId = buffer[0];
  request->command = buffer[1];
  request->offset = word(buffer[3], buffer[2]);
  request->length = word(buffer[5], buffer[4]);
  return true;
}

void encodeLegacyCrc32Response(uint32_t crcValue, uint8_t *buffer)
{
  if (buffer == nullptr)
  {
    return;
  }

  buffer[0] = ((crcValue >> 24) & 255);
  buffer[1] = ((crcValue >> 16) & 255);
  buffer[2] = ((crcValue >> 8) & 255);
  buffer[3] = (crcValue & 255);
}

SecondarySerialAck buildSecondarySerialAck(uint8_t secondaryProtocol, byte cmd)
{
  SecondarySerialAck ack = {{0U, 0U, 0U}, 0U};

  const bool usesAck = (secondaryProtocol == SECONDARY_SERIAL_PROTO_GENERIC_FIXED)
                    || (secondaryProtocol == SECONDARY_SERIAL_PROTO_GENERIC_INI)
                    || (secondaryProtocol == SECONDARY_SERIAL_PROTO_REALDASH);
  if (!usesAck)
  {
    return ack;
  }

  if (cmd == LEGACY_SEND_OUTPUT_CHANNELS_CMD)
  {
    ack.bytes[0] = 'r';
    ack.bytes[1] = cmd;
    ack.length = 2U;
  }
  else if (cmd == LEGACY_FIXED_OUTPUT_CHANNELS_CMD)
  {
    ack.bytes[0] = 'A';
    ack.length = 1U;
  }
  else if (cmd == LEGACY_NEW_OUTPUT_CHANNELS_CMD)
  {
    ack.bytes[0] = 'n';
    ack.bytes[1] = cmd;
    ack.bytes[2] = NEW_CAN_PACKET_SIZE;
    ack.length = 3U;
  }

  return ack;
}

bool shouldThrottleLegacySerialWrites(const Stream *targetPort)
{
  return targetPort == &Serial;
}

LegacySendProgress calculateLegacySendProgress(uint16_t offset, uint16_t packetLength, uint16_t itemsSent)
{
  LegacySendProgress progress = {0U, 0U};
  const uint16_t nextOffset = offset + itemsSent;
  const uint16_t remainingLength = (packetLength > itemsSent) ? (packetLength - itemsSent) : 0U;

  progress.nextOffset = (nextOffset > UINT8_MAX) ? UINT8_MAX : (uint8_t)nextOffset;
  progress.remainingLength = (remainingLength > UINT8_MAX) ? UINT8_MAX : (uint8_t)remainingLength;
  return progress;
}

LegacySerialResumeRequest buildLegacySerialResumeRequest(uint16_t offset, uint16_t remainingLength)
{
  LegacySerialResumeRequest request = {LEGACY_SEND_OUTPUT_CHANNELS_CMD, 0U, 0U};
  const LegacySendProgress progress = calculateLegacySendProgress(offset, remainingLength, 0U);

  request.offset = progress.nextOffset;
  request.length = progress.remainingLength;
  return request;
}

bool isLegacyOutputChannelsCommandSupported(byte cmd)
{
  return cmd == LEGACY_SEND_OUTPUT_CHANNELS_CMD;
}

SecondaryRealtimeRequest buildSecondaryRealtimeRequest(byte command, uint8_t secondaryProtocol)
{
  SecondaryRealtimeRequest request = {0U, 0U, false, false};

  if (command == 'A')
  {
    request.packetLength = CAN_PACKET_SIZE;
    request.cmd = LEGACY_FIXED_OUTPUT_CHANNELS_CMD;
    request.supported = true;
  }
  else if (command == 'n')
  {
    request.packetLength = NEW_CAN_PACKET_SIZE;
    request.cmd = LEGACY_NEW_OUTPUT_CHANNELS_CMD;
    request.supported = true;
  }

  request.useLegacyLayout = request.supported && (secondaryProtocol == SECONDARY_SERIAL_PROTO_GENERIC_FIXED);
  return request;
}

byte getLegacyVersionResponseCommand(byte requestedCommand, bool isSecondaryPort, uint8_t secondaryProtocol)
{
  if (requestedCommand != 'S')
  {
    return requestedCommand;
  }

  if (isSecondaryPort)
  {
    return (secondaryProtocol == SECONDARY_SERIAL_PROTO_MSDROID) ? 'Q' : 'S';
  }

  return ((secondaryProtocol == SECONDARY_SERIAL_PROTO_MSDROID) || (secondaryProtocol == SECONDARY_SERIAL_PROTO_TUNERSTUDIO)) ? 'Q' : 'S';
}

const char *getLegacyVersionResponse(byte cmd)
{
  switch (cmd)
  {
    case 'Q':
      return "speeduino 202501";
    case 'S':
      return "Speeduino 2025.01.6";
    default:
      return nullptr;
  }
}

#if defined(CORE_AVR)
#pragma GCC push_options
// This minimizes RAM usage at no performance cost
#pragma GCC optimize ("Os") 
#endif

/** Processes the incoming data on the serial buffer based on the command sent.
Can be either data for a new command or a continuation of data for command that is already in progress:
- cmdPending = If a command has started but is waiting on further data to complete
- chunkPending = Specifically for the new receive value method where TS will send a known number of contiguous bytes to be written to a table

Commands are single byte (letter symbol) commands.
*/
void legacySerialCommand(void)
{
  serialReceiveStartTime = millis();
  if ( serialStatusFlag == SERIAL_INACTIVE )  { currentCommand = primarySerial.read(); }

  const LegacyHandlerDispatch handlerDispatch = getPrimaryLegacyHandlerDispatch(currentCommand, configPage9.secondarySerialProtocol);
  if (handlerDispatch.handled)
  {
    if (handlerDispatch.enableCompatMode)
    {
      BIT_SET(currentStatus.status4, BIT_STATUS4_COMMS_COMPAT);
    }

    legacySerialHandler(handlerDispatch.command, primarySerial, handlerDispatch.useSecondaryStatusFlag ? serialSecondaryStatusFlag : serialStatusFlag);
    if (currentCommand == 'S')
    {
      currentStatus.secl = 0;
    }
    return;
  }

  const LegacyLoggerCommand loggerCommand = getLegacyLoggerCommand(currentCommand);
  if (loggerCommand.action != LEGACY_LOGGER_NONE)
  {
    switch (loggerCommand.action)
    {
      case LEGACY_LOGGER_START_TOOTH:
        startToothLogger();
        break;

      case LEGACY_LOGGER_STOP_TOOTH:
        stopToothLogger();
        break;

      case LEGACY_LOGGER_START_COMPOSITE:
        startCompositeLogger();
        break;

      case LEGACY_LOGGER_STOP_COMPOSITE:
        stopCompositeLogger();
        break;

      case LEGACY_LOGGER_START_TERTIARY:
        startCompositeLoggerTertiary();
        break;

      case LEGACY_LOGGER_STOP_TERTIARY:
        stopCompositeLoggerTertiary();
        break;

      case LEGACY_LOGGER_START_CAMS:
        startCompositeLoggerCams();
        break;

      case LEGACY_LOGGER_STOP_CAMS:
        stopCompositeLoggerCams();
        break;

      default:
        break;
    }

    if (loggerCommand.sendAck)
    {
      primarySerial.write(1);
    }
    return;
  }

  switch (currentCommand)
  {

    case 'a':
      serialStatusFlag = SERIAL_COMMAND_INPROGRESS_LEGACY;
      if (primarySerial.available() >= 2)
      {
        primarySerial.read(); //Ignore the first value, it's always 0
        primarySerial.read(); //Ignore the second value, it's always 6
        sendValuesLegacy();
        serialStatusFlag = SERIAL_INACTIVE;
      }
      break;

    case 'A': // send x bytes of realtime values
      sendValues(0, LOG_ENTRY_SIZE, LEGACY_FIXED_OUTPUT_CHANNELS_CMD, primarySerial, serialStatusFlag);   //send values to serial0
      firstCommsRequest = false;
      break;

    case 'C': // test communications. This is used by Tunerstudio to see whether there is an ECU on a given serial port
      testComm();
      break;

    case 'c': //Send the current loops/sec value
      primarySerial.write(lowByte(currentStatus.loopsPerSecond));
      primarySerial.write(highByte(currentStatus.loopsPerSecond));
      break;

    case 'E': // receive command button commands
      serialStatusFlag = SERIAL_COMMAND_INPROGRESS_LEGACY;

      if(primarySerial.available() >= 2)
      {
        uint8_t requestBuffer[2];
        LegacyCommandButtonRequest request;
        requestBuffer[0] = (uint8_t)primarySerial.read();
        requestBuffer[1] = (uint8_t)primarySerial.read();

        if (parseLegacyCommandButtonRequest(requestBuffer, sizeof(requestBuffer), &request))
        {
          (void)TS_CommandButtonsHandler(request.command);
          serialStatusFlag = SERIAL_INACTIVE;
        }
      }
      break;

    case 'F': // send serial protocol version
      primarySerial.print(F("002"));
      break;

    //The G/g commands are used for bulk reading and writing to the EEPROM directly. This is typically a non-user feature but will be incorporated into SpeedyLoader for anyone programming many boards at once
    case 'G': // Dumps the EEPROM values to serial
    {
      //The format is 2 bytes for the overall EEPROM size, a comma and then a raw dump of the EEPROM values
      uint8_t headerBuffer[3];
      const uint16_t eepromSize = getEEPROMSize();
      const size_t headerSize = buildLegacyEepromTransferHeader(eepromSize, headerBuffer, sizeof(headerBuffer));
      primarySerial.write(headerBuffer, headerSize);

      for(uint16_t x = 0; x < eepromSize; x++)
      {
        primarySerial.write(EEPROMReadRaw(x));
      }
      serialStatusFlag = SERIAL_INACTIVE;
      break;
    }

    case 'g': // Receive a dump of raw EEPROM values from the user
    {
      serialStatusFlag = SERIAL_COMMAND_INPROGRESS_LEGACY;
      //Format is similar to the above command. 2 bytes for the EEPROM size that is about to be transmitted, a comma and then a raw dump of the EEPROM values
      while( (primarySerial.available() < 3) && (!isRxTimeout()) ) { delay(1); }
      if(primarySerial.available() >= 3)
      {
        uint8_t headerBuffer[3];
        LegacyEepromTransferHeader header;
        for (uint8_t i = 0; i < 3U; i++)
        {
          headerBuffer[i] = (uint8_t)primarySerial.read();
        }

        if (!parseLegacyEepromTransferHeader(headerBuffer, sizeof(headerBuffer), &header))
        {
          break;
        }

        if(!isLegacyEepromTransferSizeValid(header.eepromSize, getEEPROMSize()))
        {
          //Client is trying to send the wrong EEPROM size. Don't let it 
          primarySerial.println(F("ERR; Incorrect EEPROM size"));
          break;
        }
        else
        {
          for(uint16_t x = 0; x < header.eepromSize; x++)
          {
            while( (primarySerial.available() == 0) && (!isRxTimeout()) ) { delay(1); }
            if(primarySerial.available()) 
            { 
              EEPROMWriteRaw(x, primarySerial.read());
            }
            else 
            {
              //Timed out, abort the write
              serialStatusFlag = SERIAL_INACTIVE;
              break;
            }
          }
        }
        serialStatusFlag = SERIAL_INACTIVE;
      }
      break;
    }

    case 'L': // List the contents of current page in human readable form
      #ifndef SMALL_FLASH_MODE
      sendPageASCII();
      #endif
      break;

    case 'm': //Send the current free memory
      currentStatus.freeRAM = freeRam();
      primarySerial.write(lowByte(currentStatus.freeRAM));
      primarySerial.write(highByte(currentStatus.freeRAM));
      break;

    case 'N': // Displays a new line.  Like pushing enter in a text editor
      primarySerial.println();
      break;

    case 'P': // set the current page
      //This is a legacy function and is no longer used by TunerStudio. It is maintained for compatibility with other systems
      //A 2nd byte of data is required after the 'P' specifying the new page number.
      serialStatusFlag = SERIAL_COMMAND_INPROGRESS_LEGACY;

      if (primarySerial.available() > 0)
      {
        currentPage = decodeLegacyPageIdentifier(primarySerial.read());
        serialStatusFlag = SERIAL_INACTIVE;
      }
      break;

    case 'T': //Send 256 tooth log entries to Tuner Studios tooth logger
      //6 bytes required:
      //2 - Page identifier
      //2 - offset
      //2 - Length
      serialStatusFlag = SERIAL_COMMAND_INPROGRESS_LEGACY;
      if(primarySerial.available() >= 6)
      {
        primarySerial.read(); // First byte of the page identifier can be ignored. It's always 0
        primarySerial.read(); // First byte of the page identifier can be ignored. It's always 0
        primarySerial.read(); // First byte of the page identifier can be ignored. It's always 0
        primarySerial.read(); // First byte of the page identifier can be ignored. It's always 0
        primarySerial.read(); // First byte of the page identifier can be ignored. It's always 0
        primarySerial.read(); // First byte of the page identifier can be ignored. It's always 0

        switch (getLegacyTriggeredLogType(currentStatus.toothLogEnabled, currentStatus.compositeTriggerUsed))
        {
          case LEGACY_TRIGGERED_LOG_TOOTH:
            sendToothLog_legacy(0);
            break;

          case LEGACY_TRIGGERED_LOG_COMPOSITE:
            sendCompositeLog_legacy(0);
            break;

          default:
            break;
        }
        serialStatusFlag = SERIAL_INACTIVE;
      }
      break;

    case 't': // receive new Calibration info. Command structure: "t", <tble_idx> <data array>.
      byte tableID;
      //byte canID;

      //The first 2 bytes sent represent the canID and tableID
      while (primarySerial.available() == 0) { }
      tableID = primarySerial.read(); //Not currently used for anything

      receiveCalibration(tableID); //Receive new values and store in memory
      writeCalibration(); //Store received values in EEPROM

      break;

    case 'U': //User wants to reset the Arduino (probably for FW update)
      if (resetControl != RESET_CONTROL_DISABLED)
      {
      #ifndef SMALL_FLASH_MODE
        if (serialStatusFlag == SERIAL_INACTIVE) { primarySerial.println(F("Comms halted. Next byte will reset the Arduino.")); }
      #endif

        while (primarySerial.available() == 0) { }
        digitalWrite(pinResetControl, LOW);
      }
      else
      {
      #ifndef SMALL_FLASH_MODE
        if (serialStatusFlag == SERIAL_INACTIVE) { primarySerial.println(F("Reset control is currently disabled.")); }
      #endif
      }
      break;

    case 'V': // send VE table and constants in binary
      sendPage();
      break;

    case 'W': // receive new VE obr constant at 'W'+<offset>+<newbyte>
      serialStatusFlag = SERIAL_COMMAND_INPROGRESS_LEGACY;

      if (isMap())
      {
        if(primarySerial.available() >= 3) // 1 additional byte is required on the MAP pages which are larger than 255 bytes
        {
          byte offset1, offset2;
          offset1 = primarySerial.read();
          offset2 = primarySerial.read();
          valueOffset = word(offset2, offset1);
          setPageValue(currentPage, valueOffset, primarySerial.read());
          serialStatusFlag = SERIAL_INACTIVE;
        }
      }
      else
      {
        if(primarySerial.available() >= 2)
        {
          valueOffset = primarySerial.read();
          setPageValue(currentPage, valueOffset, primarySerial.read());
          serialStatusFlag = SERIAL_INACTIVE;
        }
      }

      break;

    case 'w':
      //No w commands are supported in legacy mode. This should never be called
      if(primarySerial.available() >= 7)
      {
        byte offset1, offset2, length1, length2;

        primarySerial.read(); // First byte of the page identifier can be ignored. It's always 0
        currentPage = primarySerial.read();
        //currentPage = 1;
        offset1 = primarySerial.read();
        offset2 = primarySerial.read();
        valueOffset = word(offset2, offset1);
        length1 = primarySerial.read();
        length2 = primarySerial.read();
        chunkSize = word(length2, length1);
      }
      break;

    case 'Z': //Totally non-standard testing function. Will be removed once calibration testing is completed. This function takes 1.5kb of program space! :S
    #ifndef SMALL_FLASH_MODE
      primarySerial.println(F("Coolant"));
      for (int x = 0; x < 32; x++)
      {
        primarySerial.print(cltCalibration_bins[x]);
        primarySerial.print(", ");
        primarySerial.println(cltCalibration_values[x]);
      }
      primarySerial.println(F("Inlet temp"));
      for (int x = 0; x < 32; x++)
      {
        primarySerial.print(iatCalibration_bins[x]);
        primarySerial.print(", ");
        primarySerial.println(iatCalibration_values[x]);
      }
      primarySerial.println(F("O2"));
      for (int x = 0; x < 32; x++)
      {
        primarySerial.print(o2Calibration_bins[x]);
        primarySerial.print(", ");
        primarySerial.println(o2Calibration_values[x]);
      }
      primarySerial.println(F("WUE"));
      for (int x = 0; x < 10; x++)
      {
        primarySerial.print(configPage4.wueBins[x]);
        primarySerial.print(F(", "));
        primarySerial.println(configPage2.wueValues[x]);
      }
      primarySerial.flush();
    #endif
      break;

    case 'z': //Send 256 tooth log entries to a terminal emulator
      sendToothLog_legacy(0); //Sends tooth log values as chars
      break;

    case '`': //Custom 16u2 firmware is making its presence known
      serialStatusFlag = SERIAL_COMMAND_INPROGRESS_LEGACY;

      if (primarySerial.available() >= 1) {
        configPage4.bootloaderCaps = primarySerial.read();
        serialStatusFlag = SERIAL_INACTIVE;
      }
      break;


    case '?':
    #ifndef SMALL_FLASH_MODE
      primarySerial.println
      (F(
         "\n"
         "===Command Help===\n\n"
         "All commands are single character and are concatenated with their parameters \n"
         "without spaces."
         "Syntax:  <command>+<parameter1>+<parameter2>+<parameterN>\n\n"
         "===List of Commands===\n\n"
         "A - Displays 31 bytes of currentStatus values in binary (live data)\n"
         "B - Burn current map and configPage values to eeprom\n"
         "C - Test COM port.  Used by Tunerstudio to see whether an ECU is on a given serial \n"
         "    port. Returns a binary number.\n"
         "N - Print new line.\n"
         "P - Set current page.  Syntax:  P+<pageNumber>\n"
         "R - Same as A command\n"
         "S - Display signature number\n"
         "Q - Same as S command\n"
         "V - Display map or configPage values in binary\n"
         "W - Set one byte in map or configPage.  Expects binary parameters. \n"
         "    Syntax:  W+<offset>+<newbyte>\n"
         "t - Set calibration values.  Expects binary parameters.  Table index is either 0, \n"
         "    1, or 2.  Syntax:  t+<tble_idx>+<newValue1>+<newValue2>+<newValueN>\n"
         "Z - Display calibration values\n"
         "T - Displays 256 tooth log entries in binary\n"
         "r - Displays 256 tooth log entries\n"
         "U - Prepare for firmware update. The next byte received will cause the Arduino to reset.\n"
         "? - Displays this help page"
       ));
     #endif

      break;

    default:
      //Serial.println(F("Err: Unknown cmd"));
      //while(primarySerial.available() && primarySerial.peek()!='A') { primarySerial.read(); }
      serialStatusFlag = SERIAL_INACTIVE;
      break;
  }
}

void legacySerialHandler(byte cmd, Stream &targetPort, SerialStatus &targetStatusFlag)
{
  switch (cmd)
  {

    case 'b': // New EEPROM burn command to only burn a single page at a time
      targetStatusFlag = SERIAL_COMMAND_INPROGRESS_LEGACY;

      if (targetPort.available() >= 2)
      {
        targetPort.read(); //Ignore the first table value, it's always 0
        writeConfig(targetPort.read());
        targetStatusFlag = SERIAL_INACTIVE;
      }
      break;

    case 'B': // AS above but for the serial compatibility mode. 
      targetStatusFlag = SERIAL_COMMAND_INPROGRESS_LEGACY;

      if (targetPort.available() >= 2)
      {
        targetPort.read(); //Ignore the first table value, it's always 0
        writeConfig(targetPort.read());
        targetStatusFlag = SERIAL_INACTIVE;
      }
      break;

    case 'd':
      targetStatusFlag = SERIAL_COMMAND_INPROGRESS_LEGACY;

      if (targetPort.available() >= 2)
      {
        uint8_t requestBuffer[2];
        LegacyCrcRequest request;
        uint8_t responseBuffer[4];
        for (uint8_t i = 0; i < 2U; i++)
        {
          requestBuffer[i] = targetPort.read();
        }

        if (!parseLegacyCrcRequest(requestBuffer, sizeof(requestBuffer), &request))
        {
          break;
        }

        encodeLegacyCrc32Response(calculatePageCRC32(request.page), responseBuffer);
        targetPort.write(responseBuffer, sizeof(responseBuffer));
        
        targetStatusFlag = SERIAL_INACTIVE;
      }
      break;

    case 'M':
      targetStatusFlag = SERIAL_COMMAND_INPROGRESS_LEGACY;

      if(chunkPending == false)
      {
        //This means it's a new request
        //7 bytes required:
        //2 - Page identifier
        //2 - offset
        //2 - Length
        //1 - 1st New value
        if(targetPort.available() >= 7)
        {
          uint8_t requestBuffer[6];
          LegacyPageWriteRequest request;
          for (uint8_t i = 0; i < 6U; i++)
          {
            requestBuffer[i] = targetPort.read();
          }

          if (!parseLegacyPageWriteRequest(requestBuffer, sizeof(requestBuffer), &request))
          {
            break;
          }

          currentPage = request.page;
          valueOffset = request.offset;
          chunkSize = request.length;

          //Regular page data
          chunkPending = true;
          chunkComplete = 0;
        }
      }
      //This CANNOT be an else of the above if statement as chunkPending gets set to true above
      if(chunkPending == true)
      { 
        while( (targetPort.available() > 0) && (chunkComplete < chunkSize) )
        {
          setPageValue(currentPage, (valueOffset + chunkComplete), targetPort.read());
          chunkComplete++;
        }
        if(chunkComplete >= chunkSize) { targetStatusFlag = SERIAL_INACTIVE; chunkPending = false; }
      }
      break;

    case 'p':
      targetStatusFlag = SERIAL_COMMAND_INPROGRESS_LEGACY;

      //6 bytes required:
      //2 - Page identifier
      //2 - offset
      //2 - Length
      if(targetPort.available() >= 6)
      {
        uint8_t requestBuffer[6];
        LegacyPageReadRequest request;
        for (uint8_t i = 0; i < 6U; i++)
        {
          requestBuffer[i] = targetPort.read();
        }

        if (!parseLegacyPageReadRequest(requestBuffer, sizeof(requestBuffer), &request))
        {
          break;
        }

        valueOffset = request.offset;
        for(uint16_t i = 0; i < request.length; i++)
        {
          targetPort.write( getPageValue(request.page, valueOffset + i) );
        }

        targetStatusFlag = SERIAL_INACTIVE;
      }
      break;

    case 'Q': // send code version
    case 'S': // send code version
    {
      const char *response = getLegacyVersionResponse(cmd);
      if (response != nullptr)
      {
        targetPort.print(response);
      }
      break;
    }

    case 'r': //New format for the optimised OutputChannels
      targetStatusFlag = SERIAL_COMMAND_INPROGRESS_LEGACY;
      byte cmd;
      if (targetPort.available() >= 6)
      {
        uint8_t requestBuffer[6];
        LegacyOutputChannelsRequest request;
        for (uint8_t i = 0; i < 6U; i++)
        {
          requestBuffer[i] = targetPort.read();
        }

        if (!parseLegacyOutputChannelsRequest(requestBuffer, sizeof(requestBuffer), &request))
        {
          break;
        }
        cmd = request.command;

        targetStatusFlag = SERIAL_INACTIVE;

        if(isLegacyOutputChannelsCommandSupported(cmd))
        {
          sendValues(request.offset, request.length, cmd, targetPort, targetStatusFlag);
        }
        else
        {
          //No other r/ commands are supported in legacy mode
        }
      }
      break;

  }
}

/** Send a status record back to tuning/logging SW.
 * This will "live" information from @ref currentStatus struct.
 * @param offset - Start field number
 * @param packetLength - Length of actual message (after possible ack/confirm headers)
 * @param cmd - ??? - Will be used as some kind of ack on secondarySerial
 * @param targetPort - The HardwareSerial device that will be transmitted to
 * @param targetStatusFlag - The status flag that will be set to indicate the status of the transmission
 * @param logFunction - The function that should be called to retrieve the log value
 * E.g. tuning sw command 'A' (Send all values) will send data from field number 0, LOG_ENTRY_SIZE fields.
 * @return the current values of a fixed group of variables
 */
void sendValues(uint16_t offset, uint16_t packetLength, byte cmd, Stream &targetPort, SerialStatus &targetStatusFlag) { sendValues(offset, packetLength, cmd, targetPort, targetStatusFlag, &getTSLogEntry); } //Defaults to using the standard TS log function
void sendValues(uint16_t offset, uint16_t packetLength, byte cmd, Stream &targetPort, SerialStatus &targetStatusFlag, uint8_t (*logFunction)(uint16_t))
{  
  #if defined(secondarySerial_AVAILABLE)
  if (&targetPort == &secondarySerial)
  {
    //Using Secondary serial, check if selected protocol requires the echo back of the command
    const SecondarySerialAck ack = buildSecondarySerialAck(configPage9.secondarySerialProtocol, cmd);
    for (size_t i = 0; i < ack.length; i++)
    {
      secondarySerial.write(ack.bytes[i]);
    }
  }
  else
  #endif
  {
    if(firstCommsRequest) 
    { 
      firstCommsRequest = false;
      currentStatus.secl = 0; 
    }
  }

  //
  targetStatusFlag = SERIAL_TRANSMIT_INPROGRESS_LEGACY;
  currentStatus.status2 ^= (-currentStatus.hasSync ^ currentStatus.status2) & (1U << BIT_STATUS2_SYNC); //Set the sync bit of the Spark variable to match the hasSync variable

  for(byte x=0; x<packetLength; x++)
  {
    bool bufferFull = false;

    //targetPort.write(getTSLogEntry(offset+x));
    targetPort.write(logFunction(offset+x));

    if(shouldThrottleLegacySerialWrites(&targetPort)) 
    { 
      //If the transmit buffer is full, wait for it to clear. This cannot be used with Read Dash as it will cause a timeout
      if(targetPort.availableForWrite() < 1) { bufferFull = true; }
    }

    //Check whether the tx buffer still has space
    if(bufferFull == true) 
    { 
      //tx buffer is full. Store the current state so it can be resumed later
      const LegacySendProgress progress = calculateLegacySendProgress(offset, packetLength, (uint16_t)x + 1U);
      logItemsTransmitted = progress.nextOffset;
      inProgressLength = progress.remainingLength;
      return;
    }
    
  }

  targetStatusFlag = SERIAL_INACTIVE;
  while(targetPort.available()) { targetPort.read(); }
  // Reset any flags that are being used to trigger page refreshes
  BIT_CLEAR(currentStatus.status3, BIT_STATUS3_VSS_REFRESH);

}

void sendValuesLegacy(void)
{
  uint8_t packet[114];
  size_t packetSize = buildLegacyValuesPacket(packet, sizeof(packet));

  primarySerial.write(packet, packetSize);
}

size_t buildLegacyValuesPacket(uint8_t *buffer, size_t bufferSize)
{
  if ((buffer == nullptr) || (bufferSize < 114U)) { return 0U; }

  size_t offset = 0U;
  uint16_t temp;
  auto appendByte = [&](uint8_t value) {
    buffer[offset++] = value;
  };
  auto appendWord = [&](uint16_t value) {
    appendByte(highByte(value));
    appendByte(lowByte(value));
  };

  appendWord(currentStatus.secl);
  appendWord(currentStatus.PW1);
  appendWord(currentStatus.PW2);
  appendWord(currentStatus.RPM);

  temp = currentStatus.advance * 10;
  appendWord(temp);

  appendByte(currentStatus.nSquirts);
  appendByte(currentStatus.engine);
  appendByte(currentStatus.afrTarget);
  appendByte(currentStatus.afrTarget); // send twice so afrtgt1 == afrtgt2
  appendByte((configPage6.egoType == EGO_TYPE_WIDE && currentStatus.runSecs > configPage6.ego_sdelay) ? 1 : 0); // wbo2_en1
  appendByte((configPage6.egoType == EGO_TYPE_WIDE && currentStatus.runSecs > configPage6.ego_sdelay) ? 1 : 0); // wbo2_en2

  temp = currentStatus.baro * 10;
  appendWord(temp);

  temp = currentStatus.MAP * 10;
  appendWord(temp);

  temp = currentStatus.IAT * 10;
  appendWord(temp);

  temp = currentStatus.coolant * 10;
  appendWord(temp);

  temp = currentStatus.TPS * 10;
  appendWord(temp);

  appendWord(currentStatus.battery10);
  appendWord(currentStatus.O2);
  appendWord(currentStatus.O2_2);

  appendByte(currentStatus.knockCount);
  appendByte(currentStatus.knockRetard);

  temp = currentStatus.egoCorrection * 10;
  appendWord(temp); // egocor1
  appendWord(temp); // egocor2

  temp = currentStatus.iatCorrection * 10;
  appendWord(temp); // aircor

  temp = currentStatus.wueCorrection * 10;
  appendWord(temp); // warmcor

  appendByte(lowByte(currentStatus.AEamount >> 1U)); // accelEnrich
  appendByte(lowByte(currentStatus.AEamount >> 1U)); // accelEnrich
  byte dfcoTaperValue = correctionDFCOfuel();
  appendByte(dfcoTaperValue); // tpsFuelCut (100% = normal, <100% = fuel cut)
  appendByte(dfcoTaperValue); // tpsFuelCut

  temp = currentStatus.baroCorrection * 10U;
  appendWord(temp); // baroCorrection

  temp = currentStatus.corrections * 10;
  appendWord(temp); // gammaEnrich

  temp = currentStatus.VE * 10;
  appendWord(temp); // ve1
  temp = currentStatus.VE2 * 10;
  appendWord(temp); // ve2

  if (hasStepperIdleAlgorithm())
  {
    appendWord((uint16_t)idleStepper.curIdleStep); // iacstep
  }
  else
  {
    appendByte(99); // iacstep
    appendByte(99); // iacstep
  }

  temp = getLegacyColdAdvance();
  appendWord(temp); // cold_adv_deg

  temp = currentStatus.tpsDOT;
  appendWord(temp); // TPSdot

  temp = currentStatus.mapDOT;
  appendWord(temp); // MAPdot

  temp = currentStatus.dwell * 10U;
  appendWord(temp); // dwell

  appendByte(99); // MAF
  appendByte(99); // MAF
  appendByte(currentStatus.fuelLoad * 10); // fuelload
  appendByte(currentStatus.flexCorrection); // fuelcor (flex fuel correction %)
  appendByte(currentStatus.flexCorrection); // fuelcor
  appendByte(currentStatus.outputsStatus); // portStatus

  temp = currentStatus.advance1 * 10;
  appendWord(temp);
  temp = currentStatus.advance2 * 10;
  appendWord(temp);

  while (offset < 114U)
  {
    buffer[offset++] = 99U;
  }

  return offset;
}

namespace {

  void send_raw_entity(const page_iterator_t &entity)
  {
    primarySerial.write((byte *)entity.pData, entity.size);
  }

  inline void send_table_values(table_value_iterator it)
  {
    while (!it.at_end())
    {
      auto row = *it;
      primarySerial.write(&*row, row.size());
      ++it;
    }
  }

  inline void send_table_axis(table_axis_iterator it)
  {
    const table3d_axis_io_converter converter = get_table3d_axis_converter(it.get_domain());
    while (!it.at_end())
    {
      primarySerial.write(converter.to_byte(*it));
      ++it;
    }
  }

  void send_table_entity(const page_iterator_t &entity)
  {
    send_table_values(rows_begin(entity));
    send_table_axis(x_begin(entity));
    send_table_axis(y_begin(entity));
  }

  void send_entity(const page_iterator_t &entity)
  {
    switch (entity.type)
    {
    case Raw:
      return send_raw_entity(entity);
      break;

    case Table:
      return send_table_entity(entity);
      break;
    
    case NoEntity:
      // No-op
      break;

    default:
      abort();
      break;
    }
  }
}

/** Pack the data within the current page (As set with the 'P' command) into a buffer and send it.
 * 
 * Creates a page iterator by @ref page_begin() (See: pages.cpp). Sends page given in @ref currentPage.
 * 
 * Note that some translation of the data is required to lay it out in the way Megasquirt / TunerStudio expect it.
 * Data is sent in binary format, as defined by in each page in the speeduino.ini.
 */
void sendPage(void)
{
  page_iterator_t entity = page_begin(currentPage);

  while (entity.type!=End)
  {
    send_entity(entity);
    entity = advance(entity);
  }
}

namespace {

  /// Prints each element in the memory byte range (*first, *last).
  void serial_println_range(const byte *first, const byte *last)
  {
    while (first!=last)
    {
      primarySerial.println(*first);
      ++first;
    }
  }
  void serial_println_range(const uint16_t *first, const uint16_t *last)
  {
    while (first!=last)
    {
      primarySerial.println(*first);
      ++first;
    }
  }

  void serial_print_space_delimited(const byte *first, const byte *last)
  {
    while (first!=last)
    {
      primarySerial.print(*first);// This displays the values horizontally on the screen
      primarySerial.print(F(" "));
      ++first;
    }
    primarySerial.println();
  }
  #define serial_print_space_delimited_array(array) serial_print_space_delimited(array, _end_range_address(array))

  void serial_print_prepadding(byte value)
  {
    if (value < 100)
    {
      primarySerial.print(F(" "));
      if (value < 10)
      {
        primarySerial.print(F(" "));
      }
    }
  }

  void serial_print_prepadded_value(byte value)
  {
      serial_print_prepadding(value);
      primarySerial.print(value);
      primarySerial.print(F(" "));
  }

  void print_row(const table_axis_iterator &y_it, table_row_iterator row)
  {
    serial_print_prepadded_value(get_table3d_axis_converter(y_it.get_domain()).to_byte(*y_it));

    while (!row.at_end())
    {
      serial_print_prepadded_value(*row);
      ++row;
    }
    primarySerial.println();
  }

  void print_x_axis(void *pTable, table_type_t key)
  {
    primarySerial.print(F("    "));

    auto x_it = x_begin(pTable, key);
    const table3d_axis_io_converter converter = get_table3d_axis_converter(x_it.get_domain());

    while(!x_it.at_end())
    {
      serial_print_prepadded_value(converter.to_byte(*x_it));
      ++x_it;
    }
  }

  void serial_print_3dtable(void *pTable, table_type_t key)
  {
    auto y_it = y_begin(pTable, key);
    auto row_it = rows_begin(pTable, key);

    while (!row_it.at_end())
    {
      print_row(y_it, *row_it);
      ++y_it;
      ++row_it;
    }

    print_x_axis(pTable, key);
    primarySerial.println();
  }
}

/** Send page as ASCII for debugging purposes.
 * Similar to sendPage(), however data is sent in human readable format. Sends page given in @ref currentPage.
 * 
 * This is used for testing only (Not used by TunerStudio) in order to see current map and config data without the need for TunerStudio. 
 */
void sendPageASCII(void)
{
  switch (currentPage)
  {
    case veMapPage:
      primarySerial.println(F("\nVE Map"));
      serial_print_3dtable(&fuelTable, fuelTable.type_key);
      break;

    case veSetPage:
      primarySerial.println(F("\nPg 2 Cfg"));
      // The following loop displays in human readable form of all byte values in config page 1 up to but not including the first array.
      serial_println_range((byte *)&configPage2, configPage2.wueValues);
      serial_print_space_delimited_array(configPage2.wueValues);
      // This displays all the byte values between the last array up to but not including the first unsigned int on config page 1
      serial_println_range(_end_range_byte_address(configPage2.wueValues), (byte*)&configPage2.injAng);
      // The following loop displays four unsigned ints
      serial_println_range(configPage2.injAng, configPage2.injAng + _countof(configPage2.injAng));
      // Following loop displays byte values between the unsigned ints
      serial_println_range(_end_range_byte_address(configPage2.injAng), (byte*)&configPage2.mapMax);
      primarySerial.println(configPage2.mapMax);
      // Following loop displays remaining byte values of the page
      serial_println_range(&configPage2.fpPrime, (byte *)&configPage2 + sizeof(configPage2));
      break;

    case ignMapPage:
      primarySerial.println(F("\nIgnition Map"));
      serial_print_3dtable(&ignitionTable, ignitionTable.type_key);
      break;

    case ignSetPage:
      primarySerial.println(F("\nPg 4 Cfg"));
      primarySerial.println(configPage4.triggerAngle);// configPage4.triggerAngle is an int so just display it without complication
      // Following loop displays byte values after that first int up to but not including the first array in config page 2
      serial_println_range((byte*)&configPage4.FixAng, configPage4.taeBins);
      serial_print_space_delimited_array(configPage4.taeBins);
      serial_print_space_delimited_array(configPage4.taeValues);
      serial_print_space_delimited_array(configPage4.wueBins);
      primarySerial.println(configPage4.dwellLimit);// Little lonely byte stuck between two arrays. No complications just display it.
      serial_print_space_delimited_array(configPage4.dwellCorrectionValues);
      serial_println_range(_end_range_byte_address(configPage4.dwellCorrectionValues), (byte *)&configPage4 + sizeof(configPage4));
      break;

    case afrMapPage:
      primarySerial.println(F("\nAFR Map"));
      serial_print_3dtable(&afrTable, afrTable.type_key);
      break;

    case afrSetPage:
      primarySerial.println(F("\nPg 6 Config"));
      serial_println_range((byte *)&configPage6, configPage6.voltageCorrectionBins);
      serial_print_space_delimited_array(configPage6.voltageCorrectionBins);
      serial_print_space_delimited_array(configPage6.injVoltageCorrectionValues);
      serial_print_space_delimited_array(configPage6.airDenBins);
      serial_print_space_delimited_array(configPage6.airDenRates);
      serial_println_range(_end_range_byte_address(configPage6.airDenRates), configPage6.iacCLValues);
      serial_print_space_delimited_array(configPage6.iacCLValues);
      serial_print_space_delimited_array(configPage6.iacOLStepVal);
      serial_print_space_delimited_array(configPage6.iacOLPWMVal);
      serial_print_space_delimited_array(configPage6.iacBins);
      serial_print_space_delimited_array(configPage6.iacCrankSteps);
      serial_print_space_delimited_array(configPage6.iacCrankDuty);
      serial_print_space_delimited_array(configPage6.iacCrankBins);
      // Following loop is for remaining byte value of page
      serial_println_range(_end_range_byte_address(configPage6.iacCrankBins), (byte *)&configPage6 + sizeof(configPage6));
      break;

    case boostvvtPage:
      primarySerial.println(F("\nBoost Map"));
      serial_print_3dtable(&boostTable, boostTable.type_key);
      primarySerial.println(F("\nVVT Map"));
      serial_print_3dtable(&vvtTable, vvtTable.type_key);
      break;

    case seqFuelPage:
      primarySerial.println(F("\nTrim 1 Table"));
      serial_print_3dtable(&trim1Table, trim1Table.type_key);
      break;

    case canbusPage:
      primarySerial.println(F("\nPage 9 Cfg"));
      serial_println_range((byte *)&configPage9, (byte *)&configPage9 + sizeof(configPage9));
      break;

    case fuelMap2Page:
      primarySerial.println(F("\n2nd Fuel Map"));
      serial_print_3dtable(&fuelTable2, fuelTable2.type_key);
      break;
   
    case ignMap2Page:
      primarySerial.println(F("\n2nd Ignition Map"));
      serial_print_3dtable(&ignitionTable2, ignitionTable2.type_key);
      break;

    case boostvvtPage2:
      primarySerial.println(F("\nBoost lookup table"));
      serial_print_3dtable(&boostTableLookupDuty, boostTableLookupDuty.type_key);
      break;

    case warmupPage:
    case progOutsPage:
    default:
    #ifndef SMALL_FLASH_MODE
        primarySerial.println(F("\nPage has not been implemented yet"));
    #endif
        break;
  }
}


/** Processes an incoming stream of calibration data (for CLT, IAT or O2) from TunerStudio.
 * Result is store in EEPROM and memory.
 * 
 * @param tableID - calibration table to process. 0 = Coolant Sensor. 1 = IAT Sensor. 2 = O2 Sensor.
 */
void receiveCalibration(byte tableID)
{
  void* pnt_TargetTable_values; //Pointer that will be used to point to the required target table values
  uint16_t* pnt_TargetTable_bins;   //Pointer that will be used to point to the required target table bins
  int OFFSET, DIVISION_FACTOR;

  switch (tableID)
  {
    case 0:
      //coolant table
      pnt_TargetTable_values = (uint16_t *)&cltCalibration_values;
      pnt_TargetTable_bins = (uint16_t *)&cltCalibration_bins;
      OFFSET = CALIBRATION_TEMPERATURE_OFFSET; //
      DIVISION_FACTOR = 10;
      break;
    case 1:
      //Inlet air temp table
      pnt_TargetTable_values = (uint16_t *)&iatCalibration_values;
      pnt_TargetTable_bins = (uint16_t *)&iatCalibration_bins;
      OFFSET = CALIBRATION_TEMPERATURE_OFFSET;
      DIVISION_FACTOR = 10;
      break;
    case 2:
      //O2 table
      //pnt_TargetTable = (byte *)&o2CalibrationTable;
      pnt_TargetTable_values = (uint8_t *)&o2Calibration_values;
      pnt_TargetTable_bins = (uint16_t *)&o2Calibration_bins;
      OFFSET = 0;
      DIVISION_FACTOR = 1;
      break;

    default:
      OFFSET = 0;
      pnt_TargetTable_values = (uint16_t *)&iatCalibration_values;
      pnt_TargetTable_bins = (uint16_t *)&iatCalibration_bins;
      DIVISION_FACTOR = 10;
      break; //Should never get here, but if we do, just fail back to main loop
  }

  int16_t tempValue;
  byte tempBuffer[2];

  if(tableID == 2)
  {
    //O2 calibration. Comes through as 1024 8-bit values of which we use every 32nd
    for (int x = 0; x < 1024; x++)
    {
      while ( primarySerial.available() < 1 ) {}
      tempValue = primarySerial.read();

      if( (x % 32) == 0)
      {
        ((uint8_t*)pnt_TargetTable_values)[(x/32)] = (byte)tempValue; //O2 table stores 8 bit values
        pnt_TargetTable_bins[(x/32)] = (x);
      }
      
    }
  }
  else
  {
    //Temperature calibrations are sent as 32 16-bit values
    for (uint16_t x = 0; x < 32; x++)
    {
      while ( primarySerial.available() < 2 ) {}
      tempBuffer[0] = primarySerial.read();
      tempBuffer[1] = primarySerial.read();

      tempValue = (int16_t)(word(tempBuffer[1], tempBuffer[0])); //Combine the 2 bytes into a single, signed 16-bit value
      tempValue = div(tempValue, DIVISION_FACTOR).quot; //TS sends values multiplied by 10 so divide back to whole degrees. 
      tempValue = ((tempValue - 32) * 5) / 9; //Convert from F to C
      
      //Apply the temp offset and check that it results in all values being positive
      tempValue = tempValue + OFFSET;
      if (tempValue < 0) { tempValue = 0; }

      
      ((uint16_t*)pnt_TargetTable_values)[x] = tempValue; //Both temp tables have 16-bit values
      pnt_TargetTable_bins[x] = (x * 32U);
      writeCalibration();
    }
  }

  writeCalibration();
}

/** Send 256 tooth log entries to primarySerial.
 * if useChar is true, the values are sent as chars to be printed out by a terminal emulator
 * if useChar is false, the values are sent as a 2 byte integer which is readable by TunerStudios tooth logger
*/
void sendToothLog_legacy(byte startOffset) /* Blocking */
{
  //We need TOOTH_LOG_SIZE number of records to send to TunerStudio. If there aren't that many in the buffer then we just return and wait for the next call
  if (BIT_CHECK(currentStatus.status1, BIT_STATUS1_TOOTHLOG1READY)) //Sanity check. Flagging system means this should always be true
  {
      serialStatusFlag = SERIAL_TRANSMIT_TOOTH_INPROGRESS_LEGACY; 
      for (uint8_t x = startOffset; x < TOOTH_LOG_SIZE; ++x)
      {
        primarySerial.write(toothHistory[x] >> 24);
        primarySerial.write(toothHistory[x] >> 16);
        primarySerial.write(toothHistory[x] >> 8);
        primarySerial.write(toothHistory[x]);
      }
      BIT_CLEAR(currentStatus.status1, BIT_STATUS1_TOOTHLOG1READY);
      serialStatusFlag = SERIAL_INACTIVE; 
      toothHistoryIndex = 0;
  }
  else 
  { 
    //TunerStudio has timed out, send a LOG of all 0s
    for(uint16_t x = 0U; x < (4U*TOOTH_LOG_SIZE); ++x)
    {
      primarySerial.write(static_cast<byte>(0x00)); //GCC9 fix
    }
    serialStatusFlag = SERIAL_INACTIVE; 
  } 
}

void sendCompositeLog_legacy(byte startOffset) /* Non-blocking */
{
  if (BIT_CHECK(currentStatus.status1, BIT_STATUS1_TOOTHLOG1READY)) //Sanity check. Flagging system means this should always be true
  {
      serialStatusFlag = SERIAL_TRANSMIT_COMPOSITE_INPROGRESS_LEGACY;

      for (uint8_t x = startOffset; x < TOOTH_LOG_SIZE; ++x)
      {
        //Check whether the tx buffer still has space
        if(primarySerial.availableForWrite() < 4) 
        { 
          //tx buffer is full. Store the current state so it can be resumed later
          logItemsTransmitted = x;
          return;
        }

        uint32_t inProgressCompositeTime = toothHistory[x]; //This combined runtime (in us) that the log was going for by this record)
        
        primarySerial.write(inProgressCompositeTime >> 24);
        primarySerial.write(inProgressCompositeTime >> 16);
        primarySerial.write(inProgressCompositeTime >> 8);
        primarySerial.write(inProgressCompositeTime);

        primarySerial.write(compositeLogHistory[x]); //The status byte (Indicates the trigger edge, whether it was a pri/sec pulse, the sync status)
      }
      BIT_CLEAR(currentStatus.status1, BIT_STATUS1_TOOTHLOG1READY);
      toothHistoryIndex = 0;
      serialStatusFlag = SERIAL_INACTIVE; 
  }
  else 
  { 
    //TunerStudio has timed out, send a LOG of all 0s
    for(uint16_t x = 0U; x < (5U*TOOTH_LOG_SIZE); ++x)
    {
      primarySerial.write(static_cast<byte>(0x00)); //GCC9 fix
    }
    serialStatusFlag = SERIAL_INACTIVE; 
  } 
}

void testComm(void)
{
  primarySerial.write(1);
  return;
}

#if defined(CORE_AVR)
#pragma GCC pop_options
#endif
