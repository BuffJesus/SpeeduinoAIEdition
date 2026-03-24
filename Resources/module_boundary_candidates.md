# Module Boundary Candidates — Phase 3 Extraction Targets

**Purpose:** Identify high-risk domains with clean vs. scattered boundaries for Phase 3 refactoring
**Date:** 2026-03-23
**Target Domains:** Knock, Engine Protection, Idle Control, Status/Log Export

---

## Domain 1: Knock Detection & Mitigation

### Current State: PARTIALLY CLEAN

#### Module Files:
- **knock.cpp** — Knock detection logic (window calculation, digital/analog input)
- **knock.h** — Public interface

#### Scattered Logic:
| Function | Location | Responsibility | State Dependencies |
|----------|----------|----------------|-------------------|
| `correctionKnockTiming()` | corrections.cpp:975 | Applies knock retard to ignition advance | `currentStatus.knockRetard`, `currentStatus.knockCount`, `currentStatus.knockActive` |
| `_calculateKnockRecovery()` | corrections.cpp:939 | Helper for gradual knock recovery | `configPage10.knock_recoveryRate`, `configPage10.knock_recoveryStepTime` |
| Knock window table construction | init.cpp:70-71 | 2D table setup for window start/duration | `configPage10.knock_window_angle`, `configPage10.knock_window_dur`, `configPage10.knock_window_rpms` |
| Knock interrupt attachment | init.cpp:259-270 | Attaches knock pulse ISR | `configPage10.knock_mode`, `configPage10.knock_pin`, `configPage10.knock_pullup`, `configPage10.knock_trigger` |
| Knock status bit export | logger.cpp:19+ (getTSLogEntry) | Maps `status5` knock bits to TS log | `BIT_STATUS5_KNOCK_ACTIVE` (line 127), `currentStatus.knockRetard` (line 129), `currentStatus.knockCount` (line 128) |

#### Shared State Variables (globals.h):
- `currentStatus.knockRetard` (uint8_t) — Current retard amount in degrees
- `currentStatus.knockCount` (uint8_t) — Knock event counter
- `currentStatus.knockActive` (bool) — Stored in `status5`, bit `BIT_STATUS5_KNOCK_ACTIVE`
- `configPage10.knock_mode` — Detection mode (off/digital/analog)
- `configPage10.knock_recoveryRate` — Recovery rate (degrees per second)
- `configPage10.knock_recoveryStepTime` — Time between recovery steps (ms)
- `configPage10.knock_window_angle[]` — Window start angles by RPM
- `configPage10.knock_window_dur[]` — Window durations by RPM
- `configPage10.knock_window_rpms[]` — RPM bins for window tables
- `configPage10.knock_maxRetard` — Maximum retard limit
- `configPage10.knock_threshold` — Detection threshold (analog mode)

#### Callers:
- **corrections.cpp**: `correctionsIgn()` calls `correctionKnockTiming()`
- **speeduino.ino**: Main loop reads knock state for logging
- **init.cpp**: Initializes knock interrupt if enabled
- **logger.cpp**: Exports knock state to TS log stream

#### Clean Boundary Assessment: 🟡 MEDIUM

**Positives:**
- Knock detection logic is isolated in knock.cpp
- State variables are centralized in `currentStatus` and `configPage10`
- Clear public interface in knock.h

**Issues:**
- Knock retard application is in corrections.cpp (separate from detection)
- Knock recovery calculation is in corrections.cpp (not in knock.cpp)
- Interrupt attachment is in init.cpp (mixed with other interrupt setup)
- Status export is in logger.cpp (not in knock.cpp)

#### Extraction Potential: 🟢 HIGH

**Recommended Refactoring:**
1. ✅ **Already clean:** Knock detection logic in knock.cpp
2. 🔄 **Move to knock.cpp:**
   - `_calculateKnockRecovery()` from corrections.cpp
   - `correctionKnockTiming()` from corrections.cpp (or rename to `applyKnockRetard()`)
3. 🔄 **Extract to knock_init.cpp:**
   - Knock interrupt attachment logic from init.cpp
   - Knock table construction from init.cpp
4. 🔄 **Extract to knock_export.cpp (or keep in logger.cpp with clear interface):**
   - getTSLogEntry() knock-specific byte mappings

**Estimated Risk Level:** LOW
- Pure calculation (recovery, retard application)
- No new dependencies introduced
- Existing tests (test_ign/test_knock_helpers.cpp) provide regression safety

---

## Domain 2: Engine Protection (Rev/Boost/Oil/AFR Limits)

### Current State: CLEAN BOUNDARY

#### Module Files:
- **engineProtection.cpp** — All protection limit checks
- **engineProtection.h** — Public interface

#### Public Interface (engineProtection.h):
```cpp
bool protectionTimerElapsed(uint8_t currentTime, uint8_t startTime, uint8_t delayTime);
byte checkEngineProtect(void);     // Orchestrator, calls all limit checks
byte checkRevLimit(void);          // Rev limiter with rolling cut
byte checkBoostLimit(void);        // Boost cut protection
byte checkOilPressureLimit(void);  // Oil pressure protection
byte checkAFRLimit(void);          // AFR-based fuel/spark cut
```

#### Internal Helpers (static in engineProtection.cpp):
- `resetOilProtectionTimer()` (line 12)
- `resetAFRProtectionState()` (line 18)
- `afrProtectionConfigured()` (line 26)

#### Scattered Logic:
| Function | Location | Responsibility | State Dependencies |
|----------|----------|----------------|-------------------|
| `calculateMaxAllowedRPM()` | speeduino.ino:1201 | Computes effective RPM limit considering all protection sources | `checkEngineProtect()`, `configPage4.SoftRevLim`, launch/flatshift state |
| `correctionSoftRevLimit()` | corrections.cpp:853 | Applies soft rev limiter (gradual retard) | `currentStatus.RPM`, `configPage4.SoftRevLim`, `configPage2.SoftLimitMode` |
| Engine protection status bit export | logger.cpp:85 | Maps `engineProtectStatus` byte to TS log | `currentStatus.engineProtectStatus`, `ENGINE_PROTECT_BIT_RPM`, `ENGINE_PROTECT_BIT_MAP`, etc. |

#### Shared State Variables:
- `currentStatus.engineProtectStatus` (uint8_t) — Bitfield of active protection sources
  - `ENGINE_PROTECT_BIT_RPM` (0x01)
  - `ENGINE_PROTECT_BIT_MAP` (0x02)
  - `ENGINE_PROTECT_BIT_OIL` (0x04)
  - `ENGINE_PROTECT_BIT_AFR` (0x08)
  - `ENGINE_PROTECT_BIT_COOLANT` (0x10)
- `currentStatus.launchingHard` (bool) — Hard launch active flag
- `currentStatus.flatShiftingHard` (bool) — Hard flat-shift active flag
- `configPage4.SoftRevLim` — Soft rev limit threshold (RPM * 100)
- `configPage9.*ProtRPM` — Protection RPM limits for various sensors
- `configPage15.rollingProtRPMDelta[]` — Rolling cut RPM delta table
- `configPage15.rollingProtCutPercent[]` — Rolling cut percent table

#### Callers:
- **speeduino.ino**: Main loop calls `calculateMaxAllowedRPM()` → `checkEngineProtect()`
- **corrections.cpp**: `correctionsIgn()` calls `correctionSoftRevLimit()`
- **logger.cpp**: Exports `engineProtectStatus` to TS log

#### Clean Boundary Assessment: 🟢 EXCELLENT

**Positives:**
- All core protection logic is in engineProtection.cpp
- Public interface is clean and well-defined
- Internal helpers are static (no leakage)
- State is centralized in `currentStatus.engineProtectStatus` bitfield

**Minor Issues:**
- `calculateMaxAllowedRPM()` is in speeduino.ino (orchestrates protection + launch/flatshift)
- `correctionSoftRevLimit()` is in corrections.cpp (applies soft limiter retard)

#### Extraction Potential: 🟡 MEDIUM

**Recommended Refactoring (if any):**
1. ✅ **Already clean:** Core protection logic in engineProtection.cpp
2. 🤔 **Optional move to engineProtection.cpp:**
   - `calculateMaxAllowedRPM()` from speeduino.ino (only if you want single source of truth for RPM limit)
   - `correctionSoftRevLimit()` from corrections.cpp (rename to `applySoftRevLimiter()`)
3. 🔄 **Extract to protection_export.cpp (or keep in logger.cpp):**
   - engineProtectStatus bit mapping in getTSLogEntry()

**Estimated Risk Level:** LOW
- Module is already well-isolated
- Refactoring is optional (not required for testability)

**Current Test Coverage:**
- test_ign/test_limiter_state_machine.cpp has extensive coverage (156 tests verified in baseline)
- Protection bit export tested in test_corrections.cpp (line 1215, 1218)

---

## Domain 3: Idle Control

### Current State: MODERATELY CLEAN

#### Module Files:
- **idle.cpp** — Idle control logic (stepper, PWM, PID)
- **idle.h** — Public interface

#### Public Interface (idle.h):
```cpp
void initialiseIdle(bool forcehoming);
void initialiseIdleUpOutput(void);
void idleControl(void);  // Main idle control function (called from main loop)
void disableIdle(void);
void idleInterrupt(void); // ISR for stepper motor control
```

#### Internal Helpers (static in idle.cpp):
- `enableIdle()` (line 60)
- `checkForStepping()` (line 294)
- `doStep()` (line 349)
- `isStepperHomed()` (line 386)

#### Scattered Logic:
| Function | Location | Responsibility | State Dependencies |
|----------|----------|----------------|-------------------|
| Idle advance correction | corrections.cpp:808 `correctionIdleAdvance()` | Applies idle-specific ignition advance | `currentStatus.idleUpActive`, `configPage2.idleAdvEnabled`, `configPage2.idleAdvRPM`, `configPage2.idleAdvTPS`, `idleAdvanceTable` |
| Idle output pin setup | init.cpp (within `initialiseAll()`) | Sets idle valve pin modes and initial states | `pinIdle1`, `pinIdle2`, `configPage6.iacAlgorithm`, `configPage6.iacStepperInv` |
| Idle table construction | init.cpp:57 | Constructs `idleTargetTable` 2D lookup | `configPage6.iacCLValues`, `configPage6.iacBins` |
| Idle advance table construction | init.cpp:58 | Constructs `idleAdvanceTable` 2D lookup | `configPage4.idleAdvValues`, `configPage4.idleAdvBins` |

#### Shared State Variables:
- **Idle valve control:**
  - `currentStatus.idleLoad` (uint16_t) — Current idle load value
  - `currentStatus.idleUpActive` (bool) — Idle-up condition active
  - `currentStatus.CTPSActive` (bool) — Closed throttle position switch active
  - `idle_pwm_target_value` (uint8_t, global) — Target PWM duty cycle
  - `idle_pin_port`, `idle_pin_mask` (uint8_t, global) — Idle valve pin hardware
  - `idleStepper` struct (global) — Stepper motor state machine
- **Configuration:**
  - `configPage6.iacAlgorithm` — Idle algorithm (None, PWM on/off, PWM open loop, PWM closed loop, Stepper)
  - `configPage6.iacStepperInv` — Stepper direction inversion
  - `configPage6.iacCLValues[]`, `configPage6.iacBins[]` — Closed-loop target table
  - `configPage4.idleAdvValues[]`, `configPage4.idleAdvBins[]` — Idle advance table
  - `configPage6.idleKP`, `configPage6.idleKI`, `configPage6.idleKD` — PID gains
- **Idle-up inputs:**
  - `pinIdle1`, `pinIdle2` — Idle valve output pins
  - `configPage2.idleUpPin` — Idle-up input pin (A/C compressor, etc.)
  - `configPage2.idleUpAdder` — Idle-up RPM adder

#### Callers:
- **speeduino.ino**: Main loop calls `idleControl()` every 200ms
- **corrections.cpp**: `correctionsIgn()` calls `correctionIdleAdvance()`
- **init.cpp**: Calls `initialiseIdle(true)` during startup
- **timers.cpp**: Calls `idleInterrupt()` if stepper mode is active

#### Clean Boundary Assessment: 🟡 MEDIUM

**Positives:**
- Core idle control logic is in idle.cpp
- Idle valve actuation is encapsulated
- Stepper state machine is internal
- PID object is managed within idle.cpp

**Issues:**
- Idle advance correction is in corrections.cpp (separate from idle control)
- Idle pin setup is scattered in init.cpp (not in `initialiseIdle()`)
- Idle table construction is in init.cpp (not in idle.cpp)
- Some idle state is global (not encapsulated in idle.cpp)

#### Extraction Potential: 🟡 MEDIUM

**Recommended Refactoring:**
1. ✅ **Already clean:** Idle valve control logic in idle.cpp
2. 🔄 **Move to idle.cpp:**
   - `correctionIdleAdvance()` from corrections.cpp (rename to `calculateIdleAdvance()`)
3. 🔄 **Move to idle_init.cpp (or add to idle.cpp):**
   - Idle table construction from init.cpp
   - Idle pin setup from init.cpp (add to `initialiseIdle()`)
4. 🔄 **Consider encapsulation:**
   - Move `idle_pwm_target_value`, `idle_pin_port`, `idle_pin_mask` into idle.cpp static variables
   - Move `idleStepper` struct into idle.cpp static variable

**Estimated Risk Level:** MEDIUM
- Idle control has timing requirements (stepper ISR)
- Pin setup changes could affect hardware initialization order
- PID tuning is sensitive to refactoring

**Current Test Coverage:**
- No dedicated idle control tests (gap!)
- Idle advance correction tested indirectly in test_corrections.cpp

---

## Domain 4: Status/Log Export (getTSLogEntry, getLegacySerialLogEntry)

### Current State: CLEAN BOUNDARY

#### Module Files:
- **logger.cpp** — All log export functions
- **logger.h** — Public interface

#### Public Interface (logger.h):
```cpp
byte getTSLogEntry(uint16_t byteNum);  // TunerStudio log format
int16_t getReadableLogEntry(uint16_t logIndex);  // Human-readable format
uint8_t getLegacySecondarySerialLogEntry(uint16_t byteNum);  // Legacy serial format
bool is2ByteEntry(uint8_t key);  // Helper for 2-byte fields

// Tooth/composite logging
void startToothLogger(void);
void stopToothLogger(void);
void startCompositeLogger(void);
void stopCompositeLogger(void);
void startCompositeLoggerTertiary(void);
void stopCompositeLoggerTertiary(void);
void startCompositeLoggerCams(void);
void stopCompositeLoggerCams(void);
```

#### Internal Logic:
- `getTSLogEntry()` (line 19): 171-line switch statement mapping byte offsets to `currentStatus` fields
- `getLegacySecondarySerialLogEntry()` (line 341): 148-line switch statement for legacy serial protocol
- `getReadableLogEntry()` (line 189): 153-line switch statement for human-readable format

#### Scattered Logic:
| Function | Location | Responsibility | Dependencies |
|----------|----------|----------------|--------------|
| ❌ NONE | — | — | — |

**All log export logic is in logger.cpp!**

#### Shared State Variables:
- **Read-only access to `currentStatus` struct** (all fields)
- **Read-only access to `configPage*` structs** (for gauge calibration, e.g., TPS range)
- **Tooth logging state:**
  - `toothHistory[]` (uint16_t array, global) — Tooth time history
  - `toothHistoryIndex` (uint8_t, global) — Current index in tooth history
- **Composite logging state:**
  - `compositeLogHistory[]` (uint16_t array, global) — Composite signal time history
  - `compositeLogHistoryIndex` (uint8_t, global) — Current index

#### Callers:
- **comms.cpp**: Calls `getTSLogEntry()` to serve TS log requests
- **comms_legacy.cpp**: Calls `getLegacySecondarySerialLogEntry()` for legacy serial
- **utilities.cpp**: Calls `getReadableLogEntry()` for human-readable logs
- **SD_logger.cpp**: Uses log export functions for SD card logging

#### Clean Boundary Assessment: 🟢 EXCELLENT

**Positives:**
- All log export logic is in logger.cpp
- Clear public interface
- No scattered dependencies
- Read-only access to status (no side effects)

**Minor Issues:**
- `getTSLogEntry()` is a 171-line switch statement (not modular, but acceptable)
- Tooth logging state is global (not encapsulated in logger.cpp)

#### Extraction Potential: ✅ NONE NEEDED

**Recommended Refactoring:**
- ✅ **Already clean:** No refactoring needed
- 🤔 **Optional cleanup:** Break `getTSLogEntry()` into sub-functions by category (status bytes, sensor bytes, fuel bytes, ignition bytes, etc.)
- 🤔 **Optional encapsulation:** Move tooth/composite logging state into logger.cpp static variables

**Estimated Risk Level:** N/A (no refactoring needed)

**Current Test Coverage:**
- test_ign/test_corrections.cpp tests getTSLogEntry() for key bytes:
  - Line 1182: `status1` mapping
  - Line 1198: `status2` mapping
  - Line 1215: `engineProtectStatus` mapping
  - Line 1231-1236: `status5`, `knockRetard`, `knockCount`, `knockActive` mapping

---

## Summary Table: Module Boundary Quality

| Domain | Files | Boundary Quality | Scattered Functions | Extraction Priority | Risk Level |
|--------|-------|------------------|---------------------|---------------------|------------|
| **Knock** | knock.cpp/h | 🟡 MEDIUM | 4 (corrections.cpp, init.cpp, logger.cpp) | 🟢 HIGH | LOW |
| **Engine Protection** | engineProtection.cpp/h | 🟢 EXCELLENT | 2 (speeduino.ino, corrections.cpp) | 🟡 MEDIUM (optional) | LOW |
| **Idle Control** | idle.cpp/h | 🟡 MEDIUM | 3 (corrections.cpp, init.cpp) | 🟡 MEDIUM | MEDIUM |
| **Status/Log Export** | logger.cpp/h | 🟢 EXCELLENT | 0 | ✅ NONE NEEDED | N/A |

---

## Phase 3 Recommended Extraction Order

### 🎯 Priority 1: Knock Domain (Low Risk, High Value)
**Target:** Consolidate knock logic into knock.cpp

**Steps:**
1. Move `_calculateKnockRecovery()` from corrections.cpp to knock.cpp as `knockCalculateRecovery()`
2. Move `correctionKnockTiming()` from corrections.cpp to knock.cpp as `knockApplyRetard()`
3. Update corrections.cpp to call knock.h public interface
4. Add unit tests for new public functions in test_ign/test_knock_helpers.cpp

**Expected Outcome:**
- ✅ 100% of knock logic in knock.cpp
- ✅ Knock module fully testable in isolation
- ✅ Existing test coverage provides regression safety

**Estimated Effort:** 2-4 hours
**Estimated Risk:** LOW (pure calculation, no hardware dependencies)

---

### 🎯 Priority 2: Engine Protection Domain (Optional Cleanup)
**Target:** Move `calculateMaxAllowedRPM()` and `correctionSoftRevLimit()` to engineProtection.cpp

**Steps:**
1. Move `calculateMaxAllowedRPM()` from speeduino.ino to engineProtection.cpp as `calculateEffectiveRPMLimit()`
2. Move `correctionSoftRevLimit()` from corrections.cpp to engineProtection.cpp as `applySoftRevLimiter()`
3. Update callers to use engineProtection.h interface
4. Add unit tests in test_ign/test_limiter_state_machine.cpp (or create new file)

**Expected Outcome:**
- ✅ 100% of protection logic in engineProtection.cpp
- ✅ Single source of truth for RPM limits
- ✅ Cleaner corrections.cpp (less mixed responsibility)

**Estimated Effort:** 3-5 hours
**Estimated Risk:** LOW (well-tested domain, clear interface)

**Note:** This is optional — engineProtection.cpp is already well-isolated.

---

### 🎯 Priority 3: Idle Control Domain (Medium Risk)
**Target:** Consolidate idle-related logic into idle.cpp

**Steps:**
1. Move `correctionIdleAdvance()` from corrections.cpp to idle.cpp as `idleCalculateAdvance()`
2. Move idle table construction from init.cpp to idle.cpp (add to `initialiseIdle()` or new function)
3. Move idle pin setup from init.cpp to idle.cpp (add to `initialiseIdle()`)
4. Encapsulate idle state variables (make static in idle.cpp, expose via getters if needed)
5. Add unit tests for idle control logic in new test_idle/ suite

**Expected Outcome:**
- ✅ 90%+ of idle logic in idle.cpp (some init.cpp coupling unavoidable)
- ✅ Idle module testable in isolation
- ✅ Clearer separation of concerns

**Estimated Effort:** 6-10 hours
**Estimated Risk:** MEDIUM (timing-sensitive, hardware-dependent)

**Blockers:**
- Idle control has timing requirements (stepper ISR)
- Pin setup changes could affect hardware initialization order
- No existing idle control tests (need to build from scratch)

**Defer to Phase 4?** Recommended if time-constrained.

---

### 🎯 Priority 4: Status/Log Export (No Action Needed)
**Target:** N/A

**Recommendation:** ✅ **Already clean — no refactoring needed**

**Optional Cleanup (low priority):**
- Break `getTSLogEntry()` switch statement into category-specific functions
- Encapsulate tooth/composite logging state

---

## Extraction Risk Matrix

| Domain | Scattered Functions | Callers | Shared State Variables | Hardware Dependencies | Test Coverage | Overall Risk |
|--------|---------------------|---------|------------------------|----------------------|---------------|--------------|
| **Knock** | 4 | 4 | 11 | Low (interrupt attachment only) | Good (test_ign) | 🟢 LOW |
| **Engine Protection** | 2 | 3 | 9 | None | Excellent (test_ign) | 🟢 LOW |
| **Idle Control** | 3 | 4 | 15 | High (PWM, stepper ISR) | None (gap!) | 🟡 MEDIUM |
| **Status/Log Export** | 0 | 4 | 2 (read-only) | None | Good (test_ign) | 🟢 LOW |

---

## Recommendations for Phase 3 Refactoring Session

### ✅ DO:
1. **Start with Knock domain** (lowest risk, highest value)
2. **Move pure calculation functions first** (e.g., `_calculateKnockRecovery()`)
3. **Update callers incrementally** (one function at a time)
4. **Run tests after each move** to catch regressions early
5. **Document extracted functions** with contracts and unit tests

### ❌ DON'T:
1. **Don't move hardware-dependent code** without careful analysis
2. **Don't refactor idle control** until you have test coverage
3. **Don't touch logger.cpp** (it's already clean)
4. **Don't move init.cpp pin setup** without understanding initialization order

### 🤔 DEFER TO PHASE 4:
1. Idle control consolidation (medium risk, requires new tests)
2. Table-driven pin mapping (high risk, high value, requires hardware validation)
3. Decoder setup vs. interrupt attachment separation (high risk, decoder-specific)

---

## Conclusion

**Boundary Quality Summary:**
- ✅ **Engine Protection** and **Status/Log Export** have excellent boundaries
- 🟡 **Knock** and **Idle Control** have scattered logic but are extractable

**Phase 3 Strategy:**
1. **Focus on Knock domain first** (low-hanging fruit)
2. **Optionally clean up Engine Protection** (if time permits)
3. **Defer Idle Control to Phase 4** (requires new tests, medium risk)
4. **Leave logger.cpp alone** (already clean)

**Expected Outcome:**
- 2-3 high-risk domains with cleaner boundaries
- Increased testability (100% of knock logic testable in isolation)
- Reduced risk for future firmware changes (easier to validate module changes)

**Success Metric:**
- **Current:** ~70% of high-risk logic scattered across multiple files
- **Phase 3 Goal:** ~90% of knock + protection logic consolidated in dedicated modules
