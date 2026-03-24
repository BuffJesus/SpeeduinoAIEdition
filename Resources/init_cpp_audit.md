# init.cpp Responsibility Audit

**File:** `speeduino/init.cpp`
**Total Lines:** 3867
**Purpose:** Speeduino initialization orchestration (called at Arduino setup())

## Function Breakdown

### 1. `construct2dTables()` (Lines 42-75)
**Responsibility:** Config-derived table construction
**Description:** Maps 2D table structures to config page memory locations
**Risk Level:** Low
**Dependencies:** All configPage structures, 2D table infrastructure
**Notes:**
- Pure data structure setup, no I/O
- Called after EEPROM load
- 34 distinct 2D table constructions
- Single responsibility: table pointer initialization

---

### 2. `initialiseAll()` (Lines 97-1258)
**Responsibility:** MIXED — Primary orchestration function with multiple concerns
**Description:** Top-level init entry point performing ~40 distinct initialization steps

#### Block Analysis:

**Lines 99-103:** Board bring-up (LED setup)
- Category: **Startup/Board bring-up**
- Risk: Low

**Lines 105-139:** Optional EEPROM reset via pin hold
- Category: **Startup/Board bring-up**
- Risk: Medium (destructive operation with timing requirements)
- Dependencies: Board-specific pin definitions

**Lines 142-145:** Config load and migration
- Category: **Config-derived setup**
- Risk: High (EEPROM corruption risk)
- Dependencies: storage.cpp, updates.cpp

**Lines 152-164:** Board-specific init, timers, serial
- Category: **Startup/Board bring-up**
- Risk: Medium
- Dependencies: Board files, timers.ino

**Lines 166-169:** 2D table construction and calibration load
- Category: **Config-derived table construction**
- Risk: Low
- Dependencies: construct2dTables(), loadCalibration()

**Lines 172-178:** Pin mapping selection and application
- Category: **Pin and interrupt wiring**
- Risk: High (incorrect pin mapping = hardware damage risk)
- Dependencies: setPinMapping()

**Lines 180-198:** Optional SD, RTC, CAN, secondary serial init
- Category: **Startup/Board bring-up**
- Risk: Medium
- Notes: Board capability-gated

**Lines 201-231:** Safety: End all coil charges and close all injectors
- Category: **Startup/Board bring-up**
- Risk: Critical (safety mechanism)
- Notes: Prevents unintended spark/fuel on startup

**Lines 234-246:** Subsystem initializations
- Category: **Mixed responsibilities**
- Functions called:
  - `initialiseSchedulers()` — scheduler state machine setup
  - `initialiseIdle(true)` — idle control module init
  - `initialiseFan()` — fan control init
  - `initialiseAirCon()` — A/C control init
  - `initialiseAuxPWM()` — auxiliary PWM init
  - `initialiseCorrections()` — correction factors init
  - `initialiseADC()` — ADC subsystem init
  - `initialiseMAPBaro()` — MAP/baro sensor baseline
  - `initialiseProgrammableIO()` — programmable I/O init
- Risk: Medium to High
- Notes: Each function has distinct responsibility, should be reviewed individually

**Lines 248-270:** Interrupt attachment (Flex, VSS, Knock)
- Category: **Pin and interrupt wiring**
- Risk: Medium
- Dependencies: Config-driven pin selections

**Lines 272-291:** One-time fuel calculations
- Category: **Config-derived setup**
- Risk: Low
- Notes: `req_fuel_uS`, `inj_opentime_uS`, staged injection multipliers

**Lines 293-296:** Secondary trigger edge auto-correction for VVT
- Category: **Decoder initialization**
- Risk: Medium
- Notes: Config patch for poll-level VVT compatibility

**Lines 298-322:** Crank trigger baseline state initialization
- Category: **Decoder initialization**
- Risk: High
- Notes: Critical for sync detection
- Sets: RPM, hasSync, runSecs, crankRPM, engineProtectStatus, triggerFilterTime, etc.

**Lines 324-330:** `initialiseTriggers()` call and post-trigger VSS/Flex attachment
- Category: **Decoder initialization**
- Risk: High
- Dependencies: decoders.cpp, interrupt subsystem

**Lines 332-371:** Stroke-based fuel adjustment and injection angle setup
- Category: **Config-derived setup**
- Risk: Medium
- Notes: CRANK_ANGLE_MAX_IGN, CRANK_ANGLE_MAX_INJ baseline

**Lines 373-1117:** MASSIVE cylinder-count-based switch statement (1-12 cylinders)
- Category: **Config-derived setup** + **Pin and interrupt wiring**
- Risk: High (complexity, many edge cases)
- Notes:
  - Sets ignition and injection timing degrees per channel
  - Configures sequential, semi-sequential, paired, simultaneous modes
  - Handles odd-fire engines
  - Handles staged injection
  - Handles rotary engines
  - Each case is 50-150 lines of angle calculations and output assignments

**Lines 1119-1155:** Additional fuel mode and injection setup
- Category: **Config-derived setup**
- Risk: Medium
- Notes: Handles fuel2 (secondary fuel table) mode and channel assignments

**Lines 1157-1177:** Function pointer assignments for injector control
- Category: **Config-derived setup**
- Risk: Critical (wrong function = hardware failure)
- Notes: Maps inj1StartFunction, inj1EndFunction based on pin layout and channel count

**Lines 1179-1211:** Function pointer assignments for ignition control
- Category: **Config-derived setup**
- Risk: Critical (wrong function = hardware failure or misfire)
- Notes: Maps ign1StartFunction, ign1EndFunction based on pin layout and mode

**Lines 1213-1230:** Fuel pump priming sequence
- Category: **Startup/Board bring-up**
- Risk: Medium
- Notes: Turns on fuel pump for configurable duration, respects fpPrimed flag

**Lines 1232-1254:** Final sensor reads and init completion flag
- Category: **Startup/Board bring-up**
- Risk: Low
- Notes: Reads CLT, TPS to seed cranking PW calculation; sets `initialisationComplete = true`

---

### 3. `setPinMapping(byte boardID)` (Lines 1260-3059)
**Responsibility:** Pin and interrupt wiring
**Description:** Board-specific pin assignment based on boardID
**Risk Level:** Critical (hardware interface)
**Lines:** 1800 lines
**Structure:** Giant switch statement with 40+ board configurations

#### Key Observations:
- Each board case assigns 30-60 pins
- Includes: injector pins, coil pins, sensor pins, tach, boost, idle, fan, etc.
- No validation of pin conflicts (relies on board files)
- Some boards set additional config (e.g., trigger edge polarity, VR conditioner enables)
- **Refactoring Risk:** High — any mistake = potential hardware damage

#### Responsibilities Within setPinMapping:
1. **Pin assignment** (primary)
2. **Config patching** (secondary — should be separated)
3. **Board capability flagging** (scattered)

---

### 4. `initialiseTriggers()` (Lines 3061-3628)
**Responsibility:** Decoder initialization
**Description:** Attaches crank/cam interrupts and calls decoder-specific setup
**Risk Level:** Critical (incorrect decoder = no-start or misfire)

#### Key Operations:
- **Lines 3061-3100:** Interrupt handler selection based on `configPage4.TrigPattern`
- **Lines 3102-3400:** Decoder-specific setup calls (e.g., `triggerSetup_missingTooth()`)
- **Lines 3402-3550:** Primary trigger interrupt attachment (handles VR conditioner enable)
- **Lines 3552-3628:** Secondary trigger interrupt attachment (cam, if enabled)

#### Responsibilities:
1. **Decoder initialization:** Calls decoder setup functions
2. **Interrupt wiring:** Attaches ISRs to hardware pins
3. **Config-derived setup:** Configures trigger polarity, secondary trigger mode

#### Mixed Responsibility Issues:
- Decoder setup (pure logic) mixed with interrupt attachment (hardware wiring)
- Some decoders patch global state (e.g., `revolutionOne` for 36-1)

---

### 5. `isAnyFuelScheduleRunning()` (Lines 3630-3648)
**Responsibility:** Runtime query helper
**Description:** Checks if any fuel schedule is active
**Risk Level:** Low
**Notes:** Pure query, no side effects

---

### 6. `isAnyIgnScheduleRunning()` (Lines 3650-3677)
**Responsibility:** Runtime query helper
**Description:** Checks if any ignition schedule is active
**Risk Level:** Low
**Notes:** Pure query, no side effects

---

### 7. `changeHalfToFullSync()` (Lines 3679-3777)
**Responsibility:** Decoder runtime reconfiguration
**Description:** Transitions decoder from half-sync (missing cam) to full-sync
**Risk Level:** High
**Dependencies:** Decoder state machine, triggerHandler globals

#### Key Operations:
- Disables interrupts
- Adjusts `toothAngles[]` array (doubles angles for sequential operation)
- Adjusts `revolutionOne` detection
- Clears half-sync status bit
- Re-enables interrupts

#### Refactoring Potential:
- Could be moved to decoders.cpp as decoder-agnostic utility
- Currently called from: scheduledIO.cpp (during first cam pulse after startup)

---

### 8. `changeFullToHalfSync()` (Lines 3779-3867)
**Responsibility:** Decoder runtime reconfiguration
**Description:** Transitions decoder from full-sync to half-sync (cam loss)
**Risk Level:** High
**Dependencies:** Decoder state machine, triggerHandler globals

#### Key Operations:
- Disables interrupts
- Adjusts `toothAngles[]` array (halves angles for crank-only operation)
- Adjusts trigger angle scaling
- Sets half-sync status bit
- Stops any running ignition schedules
- Re-enables interrupts

#### Refactoring Potential:
- Could be moved to decoders.cpp as decoder-agnostic utility
- Currently called from: decoders.cpp (on cam signal loss detection)

---

## Responsibility Summary

| Responsibility Category | Line Ranges | Functions Involved | Risk Level |
|------------------------|-------------|-------------------|------------|
| **Pure startup/board bring-up** | 99-103, 152-164, 201-231, 1213-1254 | `initialiseAll()` segments, board-specific init | Medium |
| **Config-derived table construction** | 42-75, 166 | `construct2dTables()` | Low |
| **EEPROM load and migration** | 142-145 | `loadConfig()`, `doUpdates()` | High |
| **Pin and interrupt wiring** | 1260-3059, 172-178, 248-270, 3402-3628 | `setPinMapping()`, `initialiseTriggers()` segments | Critical |
| **Decoder initialization** | 293-322, 324-330, 3061-3628 | `initialiseTriggers()`, decoder setup functions | Critical |
| **Config-derived setup (angles, modes, function pointers)** | 272-291, 332-1211 | `initialiseAll()` cylinder-count switch | Critical |
| **Subsystem initialization** | 234-246 | `initialiseSchedulers()`, `initialiseIdle()`, etc. | Medium-High |
| **Runtime reconfiguration** | 3679-3867 | `changeHalfToFullSync()`, `changeFullToHalfSync()` | High |
| **Runtime query helpers** | 3630-3677 | `isAnyFuelScheduleRunning()`, `isAnyIgnScheduleRunning()` | Low |

---

## Mixed Responsibility Hot Spots

### 🔥 `initialiseAll()` (Lines 97-1258)
**Issue:** Monolithic function performing 40+ distinct initialization steps
**Specific Problems:**
1. **Board bring-up** mixed with **config-derived setup**
2. **Pin wiring** mixed with **fuel calculations**
3. **Decoder init** mixed with **sensor reading**
4. **Safety operations** (coil/injector shutdown) buried in middle
5. **Cylinder-count switch** (800+ lines) dominates function

**Refactoring Potential:** HIGH
**Suggested Decomposition:**
- `initialiseHardware()` — Board, pins, interrupts, timers
- `initialiseConfigDerivedState()` — Angles, function pointers, fuel calcs
- `initialiseSubsystems()` — Schedulers, idle, sensors, corrections
- `performStartupSafety()` — Coil/injector shutdown, protection status clear

---

### 🔥 `setPinMapping(byte boardID)` (Lines 1260-3059)
**Issue:** 1800-line switch statement with 40+ board cases
**Specific Problems:**
1. **Pin assignment** mixed with **config patching**
2. **No abstraction** — each board case is copy-paste-modify
3. **Hard to test** — requires full board definitions
4. **Hard to extend** — new board = 100+ line case

**Refactoring Potential:** MEDIUM (high risk, high value)
**Suggested Decomposition:**
- Move board definitions to structured data (e.g., `board_defs.cpp`)
- Use table-driven approach: `const BoardPinMap boardMaps[]`
- Separate config patching into `applyBoardConfigOverrides()`

**Blockers:**
- Requires changes to 40+ board case statements
- Risk of pin mapping errors = hardware damage
- Would need extensive hardware validation

---

### 🔥 `initialiseTriggers()` (Lines 3061-3628)
**Issue:** Decoder setup mixed with interrupt attachment
**Specific Problems:**
1. **Decoder-specific logic** mixed with **hardware interrupt wiring**
2. **No clear boundary** between decoder init and ISR attachment
3. **VR conditioner enable** logic scattered

**Refactoring Potential:** MEDIUM
**Suggested Decomposition:**
- `setupDecoder()` — Pure decoder state initialization (callable from tests)
- `attachTriggerInterrupts()` — Hardware interrupt wiring only
- `configureVRConditioner()` — Isolated VR conditioner setup

---

## Testability Assessment

### Currently Testable (without hardware)
1. ✅ `construct2dTables()` — Pure pointer setup
2. ✅ Cylinder-count angle calculations (if extracted)
3. ✅ `isAnyFuelScheduleRunning()` / `isAnyIgnScheduleRunning()` helpers
4. ✅ Fuel calculation helpers (if extracted)

### Untestable (requires hardware or heavy mocking)
1. ❌ `setPinMapping()` — Requires real pin definitions
2. ❌ `initialiseTriggers()` — Attaches hardware interrupts
3. ❌ Most of `initialiseAll()` — Calls hardware-dependent subsystem inits
4. ❌ `changeHalfToFullSync()` / `changeFullToHalfSync()` — Modifies interrupt state

### Could Be Testable (with refactoring)
1. 🟡 **Angle calculation logic** — Extract to `calculateCylinderAngles(nCylinders, mode, ...)`
2. 🟡 **Function pointer assignment logic** — Extract to `assignInjectorFunctions(layout, nChannels, ...)`
3. 🟡 **Decoder setup** — Separate pure setup from interrupt attachment
4. 🟡 **req_fuel calculations** — Extract to `calculateRequiredFuel(configPage2, ...)`

---

## Recommended Phase 3 Priorities

### 🎯 High-Value, Low-Risk Extractions
1. **Extract angle calculation helpers** from `initialiseAll()` cylinder-count switch
   - Target: Create `calculateChannelAngles()` in new `init_helpers.cpp`
   - Benefit: ~800 lines become unit-testable
   - Risk: Low (pure calculation, no side effects)

2. **Extract fuel calculation helpers**
   - Target: Create `calculateRequiredFuelMicroseconds()`, `calculateStagedMultipliers()`
   - Benefit: Critical fuel math becomes testable
   - Risk: Low (pure calculation)

3. **Extract function pointer assignment logic**
   - Target: Create `assignOutputFunctions()` in `init_helpers.cpp`
   - Benefit: Critical safety logic becomes testable
   - Risk: Medium (function pointers, must preserve exact behavior)

### 🎯 High-Value, High-Risk Refactorings (Phase 4+)
1. **Decompose `initialiseAll()` into logical stages**
   - Requires careful sequencing analysis
   - High regression risk if order changes

2. **Table-driven pin mapping**
   - Extremely high value (eliminates 1800 lines of switch cases)
   - Extremely high risk (hardware damage if wrong)
   - Requires extensive hardware validation

3. **Separate decoder setup from interrupt attachment**
   - Enables host-side decoder tests
   - Requires coordination with decoders.cpp refactor

---

## Conclusion

**init.cpp is Speeduino's "God Object" orchestrator with severe mixed-responsibility issues.**

**Key Findings:**
1. **Three critical hot spots:** `initialiseAll()`, `setPinMapping()`, `initialiseTriggers()`
2. **Mixed responsibilities** at every level — board I/O mixed with pure calculation
3. **Low testability** — ~90% of code requires hardware or heavy mocking
4. **High complexity** — Cylinder-count switch alone is 800+ lines

**Phase 3 Strategy:**
- **Start with calculation extraction** (low risk, high value)
- **Build up init_helpers.cpp** with testable pure functions
- **Defer pin mapping and interrupt refactors** until Phase 4+ (high risk)

**Success Metric:**
- **Current:** ~10% of init.cpp logic is testable
- **Phase 3 Goal:** ~40% of init.cpp logic is testable via extracted helpers
