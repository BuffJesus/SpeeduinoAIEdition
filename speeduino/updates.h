#ifndef UPDATES_H
#define UPDATES_H

#include "table3d.h"

void doUpdates(void);
void multiplyTableLoad(void *pTable, table_type_t key, uint8_t multiplier); //Added 202201 - to update the table Y axis as TPS now works at 0.5% increments. Multiplies the load axis values by 4 (most tables) or by 2 (VVT table)
void divideTableLoad(void *pTable, table_type_t key, uint8_t divisor); //Added 202201 - to update the table Y axis as TPS now works at 0.5% increments. This should only be needed by the VVT tables when using MAP as load.
void multiplyTableValue(uint8_t pageNum, uint8_t multiplier); //Added to update the table values. Multiplies the value by the multiplier
void divideTableValue(uint8_t pageNum, uint8_t divisor); //Added to update the table values. Divide the value by divisor

#ifdef UNIT_TEST
struct updates_test_state {
  uint8_t eepromVersion;
  uint8_t lastStoredVersion;
  uint16_t readVersionCalls;
  uint8_t storeVersionCalls;
  uint8_t writeAllConfigCalls;
  uint8_t loadConfigCalls;
  uint8_t writeCalibrationCalls;
  bool useFakeVersion;
  bool interceptStorageCalls;
  bool stopAfterStore;
};

void updatesTestResetState(void);
void updatesTestSetInitialVersion(uint8_t version);
void updatesTestSetInterceptStorageCalls(bool interceptStorageCalls);
void updatesTestSetStopAfterStore(bool stopAfterStore);
updates_test_state updatesTestGetState(void);
uint8_t updatesTestGetCurrentDataVersion(void);

// Testable migration helpers exposed for unit testing
uint8_t migratePIDGain_v13_to_v14(uint8_t oldGain); // v13→v14: Rescale PID gains from old resolution (100% per unit) to new (100% = 32)
uint8_t migrateCrankingEnrich_v13_to_v14(uint8_t oldValue); // v13→v14: Rescale cranking enrichment from 0-255% to 0-1275% range (divide by 5)
uint8_t migrateFlexAdvance_v13_to_v14(uint8_t oldAdvance); // v13→v14: Add 40 degree offset to flex fuel advance values
uint8_t migrateCrankingPctToCurve_v4_to_v5(uint8_t oldCrankingPct); // v4→v5: Convert single cranking % to curve value (100 + old %)
uint8_t migrateTPSResolution_v18_to_v19(uint8_t oldTPSValue); // v18→v19: Multiply TPS values by 2 for 0.5% resolution upgrade
uint8_t migrateVVTTableEntry_v17_to_v18(uint8_t oldValue);  // v17→v18: Double VVT target resolution (shift left by 1), saturates at UINT8_MAX for inputs > 127
uint8_t migrateIdleAdvDelay_v17_to_v18(uint8_t oldValue);   // v17→v18: Double idle advance delay resolution (multiply by 2), saturates at UINT8_MAX for inputs > 127
#endif

#endif
