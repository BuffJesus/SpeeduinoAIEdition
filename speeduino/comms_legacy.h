/** \file comms.h
 * @brief File for handling all serial requests 
 * @author Josh Stewart
 * 
 * This file contains all the functions associated with serial comms.
 * This includes sending of live data, sending/receiving current page data, sending CRC values of pages, receiving sensor calibration data etc
 * 
 */

#ifndef COMMS_LEGACY_H
#define COMMS_LEGACY_H

/** \enum SerialStatus
 * @brief The current state of serial communication
 * */
enum SerialStatus {
  /** No serial comms is in progress */
  SERIAL_INACTIVE, 
  /** A partial write is in progress. */
  SERIAL_TRANSMIT_INPROGRESS, 
  /** A partial write is in progress (legacy send). */
  SERIAL_TRANSMIT_INPROGRESS_LEGACY, 
  /** We are part way through transmitting the tooth log */
  SERIAL_TRANSMIT_TOOTH_INPROGRESS, 
  /** We are part way through transmitting the tooth log (legacy send) */
  SERIAL_TRANSMIT_TOOTH_INPROGRESS_LEGACY, 
  /** We are part way through transmitting the composite log */
  SERIAL_TRANSMIT_COMPOSITE_INPROGRESS,
  /** We are part way through transmitting the composite log (legacy send) */
  SERIAL_TRANSMIT_COMPOSITE_INPROGRESS_LEGACY,
  /** Whether or not a serial request has only been partially received.
   * This occurs when a the length has been received in the serial buffer,
   * but not all of the payload or CRC has yet been received. 
   * 
   * Expectation is that ::serialReceive is called  until the status reverts 
   * to SERIAL_INACTIVE
  */
  SERIAL_RECEIVE_INPROGRESS,
  /** We are part way through processing a legacy serial commang: call ::serialReceive */
  SERIAL_COMMAND_INPROGRESS_LEGACY,
};
/** @brief Current status of serial comms. */
extern SerialStatus serialStatusFlag;
extern SerialStatus serialSecondaryStatusFlag;

/**
 * @brief Is a serial write in progress?
 * 
 * Expectation is that ::serialTransmit is called until this
 * returns false
 */
inline bool serialTransmitInProgress(void) {
    return serialStatusFlag==SERIAL_TRANSMIT_INPROGRESS
    || serialStatusFlag==SERIAL_TRANSMIT_INPROGRESS_LEGACY
    || serialStatusFlag==SERIAL_TRANSMIT_TOOTH_INPROGRESS
    || serialStatusFlag==SERIAL_TRANSMIT_TOOTH_INPROGRESS_LEGACY
    || serialStatusFlag==SERIAL_TRANSMIT_COMPOSITE_INPROGRESS
    || serialStatusFlag==SERIAL_TRANSMIT_COMPOSITE_INPROGRESS_LEGACY;
}

/**
 * @brief Is a non-blocking serial receive operation in progress?
 * 
 * Expectation is the ::serialReceive is called until this
 * returns false.
 */
inline bool serialRecieveInProgress(void) {
  return serialStatusFlag==SERIAL_RECEIVE_INPROGRESS
  || serialStatusFlag==SERIAL_COMMAND_INPROGRESS_LEGACY;
}

extern bool firstCommsRequest; /**< The number of times the A command has been issued. This is used to track whether a reset has recently been performed on the controller */
extern byte logItemsTransmitted;
extern byte inProgressLength;

struct LegacyPageReadRequest
{
  uint8_t page;
  uint16_t offset;
  uint16_t length;
};

struct LegacyPageWriteRequest
{
  uint8_t page;
  uint16_t offset;
  uint16_t length;
};

struct LegacyCrcRequest
{
  uint8_t page;
};

struct LegacyOutputChannelsRequest
{
  uint8_t tsCanId;
  uint8_t command;
  uint16_t offset;
  uint16_t length;
};

struct SecondarySerialAck
{
  uint8_t bytes[3];
  size_t length;
};

struct LegacySendProgress
{
  uint8_t nextOffset;
  uint8_t remainingLength;
};

struct LegacySerialResumeRequest
{
  byte cmd;
  uint8_t offset;
  uint8_t length;
};

struct SecondaryRealtimeRequest
{
  uint16_t packetLength;
  byte cmd;
  bool useLegacyLayout;
  bool supported;
};

struct LegacyHandlerDispatch
{
  byte command;
  bool handled;
  bool enableCompatMode;
  bool useSecondaryStatusFlag;
};

struct LegacyCommandButtonRequest
{
  uint16_t command;
};

struct LegacyEepromTransferHeader
{
  uint16_t eepromSize;
};

enum LegacyLoggerAction : uint8_t
{
  LEGACY_LOGGER_NONE = 0U,
  LEGACY_LOGGER_START_TOOTH,
  LEGACY_LOGGER_STOP_TOOTH,
  LEGACY_LOGGER_START_COMPOSITE,
  LEGACY_LOGGER_STOP_COMPOSITE,
  LEGACY_LOGGER_START_TERTIARY,
  LEGACY_LOGGER_STOP_TERTIARY,
  LEGACY_LOGGER_START_CAMS,
  LEGACY_LOGGER_STOP_CAMS,
};

struct LegacyLoggerCommand
{
  LegacyLoggerAction action;
  bool sendAck;
};

enum LegacyTriggeredLogType : uint8_t
{
  LEGACY_TRIGGERED_LOG_NONE = 0U,
  LEGACY_TRIGGERED_LOG_TOOTH,
  LEGACY_TRIGGERED_LOG_COMPOSITE,
};

static constexpr byte LEGACY_SEND_OUTPUT_CHANNELS_CMD = 0x30U;
static constexpr byte LEGACY_FIXED_OUTPUT_CHANNELS_CMD = 0x31U;
static constexpr byte LEGACY_NEW_OUTPUT_CHANNELS_CMD = 0x32U;

void legacySerialCommand(void);//This is the heart of the Command Line Interpreter.  All that needed to be done was to make it human readable.
void legacySerialHandler(byte cmd, Stream &targetPort, SerialStatus &targetStatusFlag);
bool isLegacyCommandByte(byte highByte, bool allowLegacyComms);
bool shouldRouteSecondaryTunerStudioToPrimary(uint8_t secondaryProtocol, uint8_t status5);
LegacyHandlerDispatch getPrimaryLegacyHandlerDispatch(byte command, uint8_t secondaryProtocol);
byte decodeLegacyPageIdentifier(byte rawPageValue);
bool parseLegacyCommandButtonRequest(const uint8_t *buffer, size_t bufferSize, LegacyCommandButtonRequest *request);
bool parseLegacyEepromTransferHeader(const uint8_t *buffer, size_t bufferSize, LegacyEepromTransferHeader *header);
size_t buildLegacyEepromTransferHeader(uint16_t eepromSize, uint8_t *buffer, size_t bufferSize);
bool isLegacyEepromTransferSizeValid(uint16_t requestedSize, uint16_t expectedSize);
LegacyLoggerCommand getLegacyLoggerCommand(byte command);
LegacyTriggeredLogType getLegacyTriggeredLogType(bool toothLogEnabled, uint8_t compositeTriggerUsed);
bool parseLegacyPageReadRequest(const uint8_t *buffer, size_t bufferSize, LegacyPageReadRequest *request);
bool parseLegacyPageWriteRequest(const uint8_t *buffer, size_t bufferSize, LegacyPageWriteRequest *request);
bool parseLegacyCrcRequest(const uint8_t *buffer, size_t bufferSize, LegacyCrcRequest *request);
bool parseLegacyOutputChannelsRequest(const uint8_t *buffer, size_t bufferSize, LegacyOutputChannelsRequest *request);
void encodeLegacyCrc32Response(uint32_t crcValue, uint8_t *buffer);
SecondarySerialAck buildSecondarySerialAck(uint8_t secondaryProtocol, byte cmd);
bool shouldThrottleLegacySerialWrites(const Stream *targetPort);
LegacySendProgress calculateLegacySendProgress(uint16_t offset, uint16_t packetLength, uint16_t itemsSent);
LegacySerialResumeRequest buildLegacySerialResumeRequest(uint16_t offset, uint16_t remainingLength);
bool isLegacyOutputChannelsCommandSupported(byte cmd);
SecondaryRealtimeRequest buildSecondaryRealtimeRequest(byte command, uint8_t secondaryProtocol);
byte getLegacyVersionResponseCommand(byte requestedCommand, bool isSecondaryPort, uint8_t secondaryProtocol);
const char *getLegacyVersionResponse(byte cmd);
void sendValues(uint16_t offset, uint16_t packetLength, byte cmd, Stream &targetPort, SerialStatus &targetStatusFlag);
void sendValues(uint16_t offset, uint16_t packetLength, byte cmd, Stream &targetPort, SerialStatus &targetStatusFlag, uint8_t (*logFunction)(uint16_t));
void sendValuesLegacy(void);
size_t buildLegacyValuesPacket(uint8_t *buffer, size_t bufferSize);
void sendPage(void);
void sendPageASCII(void);
void receiveCalibration(byte tableID);
void testComm(void);
void sendToothLog_legacy(byte startOffset);
void sendCompositeLog_legacy(byte startOffset);

#endif // COMMS_LEGACY_H
