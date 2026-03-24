# Continue Prompt for 2026-03-24 — SpeeduinoAIEdition Phase 6

## Session Context

**Previous Session Date:** 2026-03-23
**Status:** Phase 2 complete (812/812 tests passing), Phase 6 roadmap created
**Current Phase:** Phase 6.1 — Teensy 4.1 Platform Enablement (Foundation: Storage + Capability Layer)

---

## What Happened Last Session

### ✅ Phase 2 Verified Complete
- **812/812 tests passing** (all suites green, decoder baseline maintained)
- Test breakdown:
  - test_decoders: 263/263 (36 decoder contracts verified)
  - test_ign: 156/156 (knock, protection, limiter state machine)
  - test_sensors: 57/57 (MAP sampling, filtering, calibration)
  - test_updates: 38/38 + test_updates_tail: 5/5 (EEPROM migrations v2→v24)
  - Plus: test_fuel, test_init, test_math, test_schedules, test_schedule_calcs, test_tables
- No code changes (analysis-only session)

### 🚨 Critical Discovery: Roadmap Misalignment
- Attempted Phase 3 refactorings (module extractions, init.cpp helpers)
- **FIRMWARE_ROADMAP.md explicitly prioritizes Phase 6 before Phase 3-5**
- Reason: SPI flash storage + board capabilities are prerequisites for later phases

### 📋 Deliverables Created
1. **`Resources/phase3_pragmatic_summary.md`**
   - Phase 3 refactorings assessed and deferred (all high-risk, low immediate value)
   - Roadmap alignment correction documented
   - Test count corrected (812, not 519)

2. **`Resources/phase6_teensy41_enablement_roadmap.md`** ⭐ **READ THIS FIRST**
   - Complete Phase 6 implementation plan (12-18 weeks estimated)
   - 7 priorities identified (SPI flash, 16-bit tables, PWM fan, 12-bit ADC, 32×32 tables, ISR optimization, capability flags)
   - Dependencies mapped (SPI flash unblocks everything else)
   - Test strategy defined

3. **`Resources/init_cpp_audit.md`**
   - 3867-line init.cpp breakdown (3 hot spots: initialiseAll, setPinMapping, initialiseTriggers)
   - Phase 3 extraction candidates (deferred until Phase 6 complete)

4. **`Resources/module_boundary_candidates.md`**
   - Domain boundary analysis (knock, protection, idle, logger)
   - Extraction priorities (knock = high, protection = optional, idle = defer)

### 🎯 Current Teensy 4.1 State
**Enabled:**
- ✅ Native CAN (FlexCAN_T4, 3 buses)
- ✅ SD logging (SDIO FIFO mode)
- ✅ RTC (TimeLib)
- ✅ 8-channel fuel + 8-channel ignition (Quad-timer)

**Disabled/Constrained:**
- ❌ PWM fan (timer conflict, needs GPT timer — Priority 3)
- ❌ 8-bit table values (should be 16-bit — Priority 2)
- ❌ Emulated EEPROM (should use 8MB SPI flash — Priority 1)
- ❌ 8-bit ADC (should use 12-bit native — Priority 4)
- ❌ 16×16 table max (could be 32×32 — Priority 5)

---

## Phase 6.1 Goal for This Session

**Start Phase 6.1: Foundation (Storage + Capability Layer)**
**Estimated Duration:** 4-6 weeks total, start prototype this session

### Priority 1: SPI Flash Storage Backend (Start Here)

**Objective:** Prototype LittleFS-backed storage to replace emulated EEPROM

**Why This First:**
- Unblocks 16-bit tables (won't fit in EEPROM layout)
- Enables tune banks (multiple configs)
- Enables migration staging (pre-update snapshots)
- Enables high-rate diagnostic logging

**Steps for This Session:**

1. **Add LittleFS library dependency**
   - Update `platformio.ini` for Teensy 4.1:
     ```ini
     [env:teensy41]
     lib_deps =
         ...existing...
         https://github.com/PaulStoffregen/LittleFS.git
     ```
   - Build to verify library integration

2. **Create storage abstraction layer**
   - New files:
     - `speeduino/storage_spi.h` — Public interface
     - `speeduino/storage_spi.cpp` — LittleFS implementation
   - Functions to implement (start with stubs):
     ```cpp
     // In storage_spi.h
     #ifdef BOARD_CAP_SPI_FLASH

     bool initSPIFlash();  // Initialize LittleFS on onboard flash
     bool loadConfigFromFlash(byte configPage, void* buffer, uint16_t size);
     bool saveConfigToFlash(byte configPage, const void* buffer, uint16_t size);
     bool loadTuneBank(uint8_t bankID);
     bool saveMigrationSnapshot();
     uint32_t getFlashFreeSpace();

     #endif
     ```

3. **Prototype basic file operations**
   - Create test in `test/test_storage_spi/`:
     - Test file creation
     - Test read/write config page
     - Test wear-leveling behavior
     - Test corruption recovery

4. **Add board capability flag**
   - In `board_teensy41.h`:
     ```cpp
     #define BOARD_CAP_SPI_FLASH 1
     ```
   - In `globals.h` (if not already there):
     ```cpp
     extern uint16_t boardCapabilities;
     bool boardHasCapability(uint16_t capability);
     ```

5. **Validate no regression**
   - Run full test suite: `pio test -e megaatmega2560_sim_unittest`
   - Confirm 812/812 tests still passing
   - New storage tests can fail (not implemented yet), but existing tests must remain green

**Expected Outcome:**
- LittleFS library integrated ✅
- Storage abstraction layer skeleton created ✅
- Basic file operations prototyped ✅
- Capability flag infrastructure added ✅
- **No regression in existing 812 tests** ✅

**Defer to Next Session:**
- Full config page migration (EEPROM → flash)
- Tune bank switching implementation
- Migration snapshot implementation

---

## Working Rules

1. **Maintain Phase 2 baseline:** 812/812 tests must remain green
2. **Decoder baseline is sacred:** 263/263 decoder tests must never regress
3. **Teensy-only features:** Use `#ifdef BOARD_CAP_SPI_FLASH` guards (AVR keeps EEPROM)
4. **Test-first for storage:** Corruption = no-start, needs extensive validation
5. **Document as you go:** Add inline comments explaining SPI flash vs EEPROM trade-offs

---

## Phase 6 Roadmap Reference

**Phase 6.1: Foundation (Current)**
- Priority 1: SPI flash storage backend ⬅️ **START HERE**
- Priority 7: Board capability declarations

**Phase 6.2: Table Resolution (Next)**
- Priority 2: 16-bit table values (depends on SPI flash)
- Priority 5: 32×32 table sizes (depends on SPI flash + 16-bit)

**Phase 6.3: Peripheral Enablement**
- Priority 3: PWM fan support (GPT timer)
- Priority 4: 12-bit ADC

**Phase 6.4: Performance Optimization**
- Priority 6: Drain all timer flags per ISR

**See:** `Resources/phase6_teensy41_enablement_roadmap.md` for complete plan

---

## Key Files to Reference

### Must Read Before Starting
1. **`Resources/phase6_teensy41_enablement_roadmap.md`** ⭐
   - Complete Phase 6 plan (priorities, dependencies, test strategy)
2. **`speeduino/FIRMWARE_ROADMAP.md`**
   - Overall project roadmap (confirms Phase 6 priority)
3. **`speeduino/board_teensy41.h`**
   - Current Teensy capabilities and constraints

### Useful for Context
4. **`Resources/phase3_pragmatic_summary.md`**
   - Why Phase 3 was deferred (helpful context)
5. **`speeduino/storage.cpp`**
   - Current EEPROM storage implementation (use as template for SPI flash)

### Decoder Baseline (Keep Green)
6. **`test/test_decoders/`**
   - 263 decoder tests (must remain 263/263 passing)

---

## Verification Commands

```bash
# Baseline check (before starting work)
cd /c/Users/Cornelio/Desktop/speeduino-202501.6
pio test -e megaatmega2560_sim_unittest

# Expected: 812/812 tests passing

# Decoder baseline check (after changes)
pio test -e megaatmega2560_sim_unittest --filter test_decoders

# Expected: 263/263 tests passing

# New storage tests (will fail initially, that's OK)
pio test -e megaatmega2560_sim_unittest --filter test_storage_spi

# Expected: 0/X tests passing (not implemented yet)

# Teensy 4.1 build check (verify library integration)
pio run -e teensy41

# Expected: Successful build (no linker errors)
```

---

## Expected Session Outcome

**By End of Session:**
1. ✅ LittleFS library integrated in platformio.ini
2. ✅ `storage_spi.h` and `storage_spi.cpp` created (function stubs)
3. ✅ `test/test_storage_spi/` directory created with skeleton tests
4. ✅ `BOARD_CAP_SPI_FLASH` flag defined in board_teensy41.h
5. ✅ `boardHasCapability()` helper function added
6. ✅ Teensy 4.1 build succeeds (verify no library conflicts)
7. ✅ **812/812 tests still passing** (no regression)

**Defer to Next Session:**
- Implement actual LittleFS file operations
- Implement config page load/save functions
- Implement tune bank switching
- Implement migration snapshot

**Success Metric:**
- Foundation in place, no regression, prototype validated

---

## If You Encounter Issues

### LittleFS library not found
- Try alternative: `mbed-littlefs` or `littlefs-lib` from PlatformIO registry
- Check Teensy 4.1 board docs for recommended filesystem library

### Teensy build fails after library add
- Check for dependency conflicts in platformio.ini
- Verify library supports Teensy 4.1 (some are AVR-only)

### Tests regress after adding capability flags
- Ensure `#ifdef BOARD_CAP_SPI_FLASH` guards are correct
- Verify AVR build path unchanged (storage.cpp still used)
- Run `pio test -e megaatmega2560_sim_unittest` to isolate issue

### Decoder tests fail after changes
- **STOP IMMEDIATELY** — back out changes
- Decoder baseline is sacred (263/263 must stay green)
- If unavoidable, document blocker in session notes

---

## Prompt for Claude

```
Continue Phase 6.1 for SpeeduinoAIEdition — Teensy 4.1 SPI Flash Storage Prototype

## Context
- Phase 2 complete: 812/812 tests passing, decoder baseline green (263/263)
- Phase 6 prioritized over Phase 3-5 per FIRMWARE_ROADMAP.md
- Goal: Enable Teensy 4.1 as first-class platform (16-bit tables, SPI flash, higher ADC resolution)

## This Session Goal
Start Phase 6.1 Priority 1: SPI flash storage backend prototype

## Tasks
1. Add LittleFS library to platformio.ini (Teensy 4.1 environment)
2. Create storage_spi.h/cpp abstraction layer (function stubs for now)
3. Add BOARD_CAP_SPI_FLASH capability flag in board_teensy41.h
4. Create test/test_storage_spi/ directory with skeleton tests
5. Verify Teensy 4.1 build succeeds (no library conflicts)
6. Confirm 812/812 tests still passing (no regression)

## Working Rules
- Decoder baseline is sacred (263/263 must stay green)
- Use #ifdef BOARD_CAP_SPI_FLASH guards (Teensy-only feature)
- Prototype only (full implementation deferred to next session)
- Test-first for storage (corruption = no-start)

## Key Reference
Read Resources/phase6_teensy41_enablement_roadmap.md for complete Phase 6 plan

## Verification
After each change, run:
- pio test -e megaatmega2560_sim_unittest (expect 812/812)
- pio test -e megaatmega2560_sim_unittest --filter test_decoders (expect 263/263)
- pio run -e teensy41 (expect successful build)

## Success Metric
Foundation in place, no regression, prototype validates LittleFS integration works
```

---

## Summary

**Phase 2:** ✅ Complete (812/812 tests, decoder baseline verified)
**Phase 3-5:** ⏸️ Deferred (per roadmap, Phase 6 takes priority)
**Phase 6.1:** ➡️ **Start Here** (SPI flash storage prototype)

**Next Session Focus:** LittleFS integration + storage abstraction layer + capability flags

**Estimated Effort:** 4-6 hours for prototype, 20-30 hours for full Priority 1 completion

**Read First:** `Resources/phase6_teensy41_enablement_roadmap.md` (complete Phase 6 plan)

**Keep Green:** 812/812 tests, 263/263 decoders

**Target Platform:** Teensy 4.1 + Dropbear (higher-resolution tables, SPI flash, native CAN)
