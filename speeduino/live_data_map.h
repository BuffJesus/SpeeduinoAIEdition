/**
 * @file live_data_map.h
 * @brief Authoritative byte-position reference for getTSLogEntry() output channels
 *
 * This file documents every byte position in the 148-byte live-data packet sent to
 * TunerStudio via the 'A' command. It is the single source of truth for:
 *   - speeduino.ini  ochBlockSize  (must equal LIVE_DATA_MAP_SIZE)
 *   - logger.cpp     getTSLogEntry() switch cases
 *   - logger.cpp     getReadableLogEntry() switch cases
 *   - test_logger_byte_regression.cpp  (locks high-risk bytes)
 *
 * HOW TO EXTEND THE LOGGER
 * 1. Add new case(s) at the next available byte in getTSLogEntry() and getReadableLogEntry().
 * 2. Bump LOG_ENTRY_SIZE in logger.h and LIVE_DATA_MAP_SIZE below (both must stay equal).
 * 3. Bump ochBlockSize in speeduino.ini and the release INI to match.
 * 4. Add the new row(s) to the table below.
 * 5. Add a regression test in test_logger_byte_regression.cpp for any high-risk channel.
 *
 * BYTE TABLE
 * ----------
 * Columns:
 *   Byte(s)  - position(s) in getTSLogEntry() output (0-indexed)
 *   RIdx     - getReadableLogEntry() index; '-' = no direct readable entry
 *   Field                         - currentStatus member or expression
 *   Encoding                      - wire encoding sent to TS
 *   Notes / INI channel name
 *
 * Byte(s)  RIdx  Field                          Encoding       Notes
 * -------- ----- ------------------------------ -------------- ---------------------------
 * 0        0     secl                           U08            secl
 * 1        1     status1                        U08 bits       status1
 * 2        2     engine                         U08 bits       engine
 * 3        3     syncLossCounter                U08            syncLossCounter
 * 4-5      4     MAP                            U16 LE         map (kPa)
 * 6        5     IAT + CALIBRATION_OFFSET       U08            mat
 * 7        6     coolant + CALIBRATION_OFFSET   U08            clt
 * 8        7     batCorrection                  U08            batCorrection
 * 9        8     battery10                      U08            battery10 (V×10)
 * 10       9     O2                             U08            O2 (AFR×10)
 * 11       10    egoCorrection                  U08            egoCorrection
 * 12       11    iatCorrection                  U08            iatCorrection
 * 13       12    wueCorrection                  U08            wueCorrection
 * 14-15    13    RPM                            U16 LE         RPM
 * 16       14    AEamount >> 1 [low]            U08            tpsaccel (×½ for byte fit)
 * 17-18    15    corrections                    U16 LE         gammaEnrich
 * 19       16    VE1                            U08            ve1
 * 20       17    VE2                            U08            ve2
 * 21       18    afrTarget                      U08            afrTarget (×10)
 * 22-23    19    tpsDOT                         S16 LE         tpsDOT
 * 24       20    advance                        U08            advance (deg)
 * 25       21    TPS                            U08            tps (×½ = 0-100%)
 * 26-27    22    loopsPerSecond                 U16 LE         loopsPerSecond
 * 28-29    23    freeRAM                        U16 LE         freeRAM
 * 30       24    boostTarget >> 1 [low]         U08            boostTarget (×½)
 * 31       25    boostDuty / 100                U08            boostDuty
 * 32       26    status2                        U08 bits       status2
 * 33-34    27    rpmDOT                         S16 LE         rpmDOT
 * 35       28    ethanolPct                     U08            ethanolPct
 * 36       29    flexCorrection                 U08            flexCorrection
 * 37       30    flexIgnCorrection              U08            flexIgnCorrection
 * 38       31    idleLoad                       U08            idleLoad
 * 39       32    testOutputs                    U08 bits       testOutputs
 * 40       33    O2_2                           U08            O2_2 (AFR×10)
 * 41       34    baro                           U08            baro (kPa)
 * 42-43    35    canin[0]                       U16 LE         canin0
 * 44-45    36    canin[1]                       U16 LE         canin1
 * 46-47    37    canin[2]                       U16 LE         canin2
 * 48-49    38    canin[3]                       U16 LE         canin3
 * 50-51    39    canin[4]                       U16 LE         canin4
 * 52-53    40    canin[5]                       U16 LE         canin5
 * 54-55    41    canin[6]                       U16 LE         canin6
 * 56-57    42    canin[7]                       U16 LE         canin7
 * 58-59    43    canin[8]                       U16 LE         canin8
 * 60-61    44    canin[9]                       U16 LE         canin9
 * 62-63    45    canin[10]                      U16 LE         canin10
 * 64-65    46    canin[11]                      U16 LE         canin11
 * 66-67    47    canin[12]                      U16 LE         canin12
 * 68-69    48    canin[13]                      U16 LE         canin13
 * 70-71    49    canin[14]                      U16 LE         canin14
 * 72-73    50    canin[15]                      U16 LE         canin15
 * 74       51    tpsADC                         U08            tpsADC
 * 75       52    0 (deprecated getNextError)    U08            DEPRECATED: always 0
 * 76-77    53    PW1                            U16 LE         PW1 (µs×10)
 * 78-79    54    PW2                            U16 LE         PW2 (µs×10)
 * 80-81    55    PW3                            U16 LE         PW3 (µs×10)
 * 82-83    56    PW4                            U16 LE         PW4 (µs×10)
 * 84       57    status3                        U08 bits       status3 [LOCKED]
 * 85       58    engineProtectStatus            U08 bits       engineProtectStatus [LOCKED]
 * 86-87    60    fuelLoad                       S16 LE         fuelLoad  (RIdx 59 = UNUSED)
 * 88-89    61    ignLoad                        S16 LE         ignLoad
 * 90-91    62    dwell                          U16 LE         dwell (ms×10)
 * 92       63    CLIdleTarget                   U08            CLIdleTarget
 * 93-94    64    mapDOT                         S16 LE         mapDOT
 * 95-96    65    vvt1Angle                      S16 LE         vvt1Angle (deg×10)
 * 97       66    vvt1TargetAngle                U08            vvt1TargetAngle
 * 98       67    vvt1Duty [low]                 U08            vvt1Duty
 * 99-100   68    flexBoostCorrection            S16 LE         flexBoostCorrection
 * 101      69    baroCorrection                 U08            baroCorrection
 * 102      70    VE                             U08            ve (active VE%)
 * 103      71    ASEValue                       U08            aseValue
 * 104-105  72    vss                            U16 LE         vss (km/h or mph)
 * 106      73    gear                           U08            gear
 * 107      74    fuelPressure                   U08            fuelPressure
 * 108      75    oilPressure                    U08            oilPressure
 * 109      76    wmiPW                          U08            wmiPW
 * 110      77    status4                        U08 bits       status4
 * 111-112  78    vvt2Angle                      S16 LE         vvt2Angle (deg×10)
 * 113      79    vvt2TargetAngle                U08            vvt2TargetAngle
 * 114      80    vvt2Duty [low]                 U08            vvt2Duty
 * 115      81    outputsStatus                  U08 bits       outputsStatus
 * 116      82    fuelTemp + CALIBRATION_OFFSET  U08            fuelTemp
 * 117      83    fuelTempCorrection             U08            fuelTempCorrection
 * 118      84    advance1                       U08            advance1 (deg)
 * 119      85    advance2                       U08            advance2 (deg)
 * 120      86    TS_SD_Status                   U08            TS_SD_Status
 * 121-122  87    EMAP                           U16 LE         EMAP (kPa)
 * 123      88    fanDuty                        U08            fanDuty
 * 124      89    airConStatus                   U08 bits       airConStatus
 * 125-126  90    actualDwell                    U16 LE         actualDwell (µs)
 * 127      91    status5                        U08 bits       status5 [LOCKED]
 * 128      92    knockCount                     U08            knockCount [LOCKED]
 * 129      93    knockRetard                    U08            knockRetard [LOCKED]
 * 130      94    getBoardCapabilityFlags()      U08 bits       boardCapabilityFlags (Phase 4)
 * 131      95    isSPIFlashHealthy() Teensy41   U08            SPIFlashHealth (Phase 4)
 * 132-133  96    PW5                            U16 LE         PW5 (µs×10) [LOCKED]
 * 134-135  97    PW6                            U16 LE         PW6 (µs×10) [LOCKED]
 * 136-137  98    PW7                            U16 LE         PW7 (µs×10) [LOCKED]
 * 138-139  99    PW8                            U16 LE         PW8 (µs×10) [LOCKED]
 * 140      100   launchCorrection               U08            launchCorrection [LOCKED]
 * 141-142  101   injAngle                       U16 LE         injAngle [LOCKED]
 * 143-146  102   startRevolutions [31:0] LE     U32 LE         startRevolutions (Phase 10) [LOCKED]
 *          103   startRevolutions [31:16]       -              (readable hi-half of above)
 * 147      104   runtimeStatusA packed          U08 bits       runtimeStatusA (Phase 10) [LOCKED]
 *                  bit0 = fuelPumpOn
 *                  bit1 = launchingHard
 *                  bit2 = flatShiftingHard
 *                  bit3 = idleUpActive
 *                  bit4 = fullSync
 *                  bit5 = transientActive (TPS/MAP accel or decel)
 *                  bit6 = warmupOrASEActive
 *                  bit7 = tuneLearnValid (full sync, no transient, no warmup/ASE, no DFCO, no protection)
 */

#ifndef LIVE_DATA_MAP_H
#define LIVE_DATA_MAP_H

/**
 * Number of bytes in the live-data packet. Must equal LOG_ENTRY_SIZE (production builds)
 * and ochBlockSize in speeduino.ini. Update all three together when extending the logger.
 */
#define LIVE_DATA_MAP_SIZE  148U

/*
 * Compile-time guard: if LOG_ENTRY_SIZE is bumped without updating this header,
 * the build will fail with a descriptive error.
 *
 * Excluded from UNIT_TEST builds because LOG_ENTRY_SIZE is defined as 1 there
 * (simavr memory constraint in logger.h).
 */
#if !defined(UNIT_TEST)
  #include "logger.h"
  #if (LOG_ENTRY_SIZE) != (LIVE_DATA_MAP_SIZE)
    #error "LOG_ENTRY_SIZE != LIVE_DATA_MAP_SIZE: update live_data_map.h table, LIVE_DATA_MAP_SIZE, and speeduino.ini ochBlockSize together."
  #endif
#endif

#endif /* LIVE_DATA_MAP_H */
