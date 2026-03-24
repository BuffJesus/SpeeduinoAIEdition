# Phase 6: Teensy 4.1 Platform Enablement Roadmap

**Date:** 2026-03-23
**Status:** Planning
**Prerequisites:** Phase 2 complete (812/812 tests passing)

---

## Executive Summary

Per FIRMWARE_ROADMAP.md, **Phase 6 should be prioritized before Phase 3-5** because:
1. Storage infrastructure (SPI flash) unblocks larger tune pages and migration staging
2. Higher-resolution tables require storage changes first (can't fit in EEPROM)
3. Board capability layer enables Phase 4 board-specific work
4. Teensy-specific optimizations unblock performance features (higher sample rates, better filtering)

**Current Blockers for Phase 3-5:**
- Phase 3 module extractions would be easier with explicit board capabilities
- Phase 4 board consistency requires capability layer (Phase 6 prerequisite)
- Phase 5 tune migration needs larger storage (Phase 6 SPI flash)

---

## Current Teensy 4.1 State Assessment

### ✅ Already Enabled
1. **Native CAN:** FlexCAN_T4 library integrated (board_teensy41.h:184)
   - 3 CAN buses available (CAN1, CAN2, CAN3)
   - 256-entry RX buffer, 16-entry TX buffer
2. **SD Logging:** SDIO FIFO mode enabled by default (board_teensy41.h:30-32)
   - Fastest SD mode on Teensy (hardware-offloaded writes)
3. **RTC:** TimeLib integrated (board_teensy41.h:29, 31)
4. **8-channel fuel + 8-channel ignition:** Quad-timer (TMR) fully wired
   - TMR1 (fuel 1-4), TMR2 (ign 1-4), TMR3 (fuel 5-8), TMR4 (ign 5-8)
5. **Serial buffer:** 517 bytes for SD transfers (board_teensy41.h:22)
6. **Higher memory:** 1MB RAM, 8MB flash vs. AVR's 8KB RAM/256KB flash

### ❌ Disabled or Constrained
1. **PWM Fan:** Commented out, timer conflict (board_teensy41.h:35)
   - Comment: "Ran out of timers, should use GPT timer" (line 146)
   - Currently uses TMR3_CSCTRL1 which conflicts with fuel channels
2. **8-bit table values:** `table3d_value_t = uint8_t` (table3d_typedefs.h:25)
   - VE, spark, AFR tables limited to 0-255 range
   - Teensy has RAM for 16-bit tables but storage/comms not adapted
3. **Emulated EEPROM:** Still using flash-backed EEPROM.h (board_teensy41.h:27)
   - 8MB onboard SPI flash unused
   - Tune storage constrained to EEPROM layout (~4KB total config)
   - No tune banks, no migration staging, no high-rate diagnostics
4. **AVR-shaped timer ISR:** Single `else if` servicing per interrupt
   - Should drain all pending flags per ISR (roadmap line 116)
5. **8-bit ADC:** Using default Arduino ADC interface
   - Teensy 4.1 has 12-bit ADC hardware (4096 steps vs. 256)
   - No averaging, no oversampling
6. **Table size limits:** 16×16 max (table3d_typedefs.h:41)
   - Teensy has RAM for 32×32 or larger
   - Constrained by tune transport (serial packet size, EEPROM layout)

### 🟡 Partially Enabled
1. **Serial comms:** 115200 baud hard-coded
   - Teensy 4.1 USB can handle 1-2 Mbps
   - Limited by TunerStudio protocol expectations
2. **Boost/VVT timers:** PIT timers used (lines 140-144)
   - Works but could use GPT for better resolution

---

## Phase 6 Priority Order (Per Roadmap)

### 🎯 Priority 1: SPI Flash Storage Backend
**Goal:** Use 8MB onboard flash for tune persistence, tune banks, migration staging

**Current Blocker:** No filesystem, all storage goes through emulated EEPROM

**Implementation Path:**
1. Add LittleFS library for Teensy 4.1 (wear-leveled flash filesystem)
2. Create `storage_spi.cpp` abstraction layer:
   - `loadConfigFromFlash()` — replaces `loadConfig()` EEPROM reads
   - `saveConfigToFlash()` — replaces `writeConfig()` EEPROM writes
   - `loadTuneBank(bankID)` — multi-tune support
   - `saveMigrationSnapshot()` — pre-migration backup
   - `loadDiagnosticRing()` — high-rate event capture
3. Add capability flag: `BOARD_CAP_SPI_FLASH`
4. Conditional compilation: use SPI flash on Teensy 4.1, EEPROM on AVR
5. Migrate existing EEPROM layout to flash-backed structure

**Benefits Unlocked:**
- ✅ Tune banks (3-5 tunes switchable at runtime)
- ✅ Migration staging (backup before updates.cpp runs)
- ✅ Larger config pages (no 4KB EEPROM limit)
- ✅ High-rate diagnostic ring buffer (knock events, sync loss, protection triggers)
- ✅ Pre-migration snapshots for safe rollback

**Estimated Effort:** 20-30 hours (filesystem integration + abstraction layer + tests)

**Risk:** Medium (storage corruption = no-start, needs extensive validation)

**Test Strategy:**
- Unit tests: file creation, read/write, wear-leveling
- Integration tests: loadConfig() → flash, writeConfig() → flash
- Migration tests: EEPROM → flash first-boot migration
- Corruption recovery tests: partial write, power-loss simulation

---

### 🎯 Priority 2: Higher-Resolution 3D Tables (16-bit)
**Goal:** Increase table value resolution from 8-bit (0-255) to 16-bit (0-65535)

**Current Blocker:** `table3d_value_t = uint8_t` hardcoded, tune transport assumes 8-bit

**Implementation Path:**
1. Make `table3d_value_t` conditional:
   ```cpp
   #ifdef TEENSY_HIGH_RES_TABLES
     using table3d_value_t = uint16_t;
   #else
     using table3d_value_t = uint8_t;
   #endif
   ```
2. Add scaling factor to table definitions:
   - VE table: 0.1% resolution (0-6553.5% range)
   - Spark table: 0.1° resolution (-3276.8° to +3276.7°)
   - AFR table: 0.01 AFR resolution (0-655.35 AFR)
3. Update comms protocol to handle 16-bit table transfers
4. Update TunerStudio INI for Teensy-specific table definitions
5. Add migration: 8-bit → 16-bit table conversion on first boot

**Benefits Unlocked:**
- ✅ 10× finer VE tuning (0.1% vs. 1% steps)
- ✅ 10× finer spark advance (0.1° vs. 1° steps)
- ✅ 100× finer AFR target (0.01 vs. 0.1 AFR steps)
- ✅ Better idle stability (finer fuel correction)
- ✅ Better transient response (finer accel enrichment)

**Memory Impact:**
- Current: 16×16 table = 256 bytes
- New: 16×16 table = 512 bytes
- Total for 3 main tables (VE, spark, AFR): +768 bytes
- Teensy 4.1 has 1MB RAM → negligible impact

**Estimated Effort:** 15-20 hours (type changes + comms + INI + tests)

**Risk:** Medium (comms protocol change, tune migration complexity)

**Dependencies:**
- **Must have:** SPI flash storage (16-bit tables won't fit in EEPROM layout)
- **Should have:** Updated TunerStudio (custom INI for Teensy)

---

### 🎯 Priority 3: PWM Fan Support (GPT Timer)
**Goal:** Enable PWM fan control using GPT timer (avoid TMR conflict)

**Current Blocker:** TMR3 used for fuel channels, can't also drive fan (board_teensy41.h:146)

**Implementation Path:**
1. Allocate GPT1 or GPT2 timer for fan PWM
2. Implement `initialiseFan_teensy41()` in board_teensy41.cpp:
   - Configure GPT for PWM mode
   - Set frequency (25 kHz typical for fans)
   - Wire to fan output pin
3. Add `ENABLE_FAN_TIMER()` / `DISABLE_FAN_TIMER()` macros for GPT
4. Test fan control at various duty cycles

**Benefits Unlocked:**
- ✅ Thermostatic fan control (currently disabled on Teensy)
- ✅ Quieter cooling (PWM vs. on/off)
- ✅ Longer fan life (soft start)

**Estimated Effort:** 4-6 hours (GPT timer setup + wiring + validation)

**Risk:** Low (isolated feature, doesn't affect fuel/ignition)

---

### 🎯 Priority 4: Higher-Resolution ADC (12-bit)
**Goal:** Use Teensy's 12-bit ADC instead of 8-bit Arduino default

**Current Blocker:** Using Arduino `analogRead()` which returns 10-bit, scaled to 8-bit

**Implementation Path:**
1. Create `adc_teensy41.cpp` with native ADC driver:
   - 12-bit resolution (0-4095)
   - Hardware averaging (2-32 samples)
   - DMA transfer for background sampling
2. Update `readMAP()`, `readTPS()`, `readCLT()`, `readIAT()` to use 12-bit values
3. Add scaling in sensor calibration tables (map 0-4095 to physical units)
4. Optional: Oversample to 14-bit or 16-bit effective resolution

**Benefits Unlocked:**
- ✅ 16× finer MAP resolution (0.025 kPa vs. 0.4 kPa steps)
- ✅ 16× finer TPS resolution (0.025% vs. 0.4% steps)
- ✅ Better idle stability (less ADC noise)
- ✅ Faster sensor reads (DMA vs. blocking analogRead)

**Estimated Effort:** 10-15 hours (ADC driver + DMA + sensor updates + calibration)

**Risk:** Medium (affects all sensor reads, needs extensive validation)

**Test Strategy:**
- Bench test: known voltage → verify ADC count
- Noise test: measure jitter at fixed voltage
- Speed test: verify DMA doesn't block main loop

---

### 🎯 Priority 5: Larger Table Sizes (32×32)
**Goal:** Support 32×32 tables (vs. current 16×16 max)

**Current Blocker:** Table generator only defines 4×4, 6×6, 8×8, 16×16 (table3d_typedefs.h:37-41)

**Implementation Path:**
1. Add 32×32 to `TABLE3D_GENERATOR` macro:
   ```cpp
   GENERATOR(32, Rpm, Load, ##__VA_ARGS__)
   ```
2. Update INI definitions for 32×32 tables
3. Add comms protocol support for larger table transfers
4. Add Teensy-only conditional (AVR doesn't have RAM for 32×32)

**Benefits Unlocked:**
- ✅ Finer RPM resolution (200 RPM bins vs. 400 RPM bins)
- ✅ Finer load resolution (5 kPa bins vs. 10 kPa bins)
- ✅ Better transition smoothness
- ✅ More accurate VE map for turbo engines (wide load range)

**Memory Impact:**
- 16×16 table: 256 bytes
- 32×32 table: 1024 bytes
- +768 bytes per table (×3 main tables = 2.3 KB)
- Teensy 4.1: negligible (1MB RAM available)

**Estimated Effort:** 8-12 hours (macro + comms + INI + memory validation)

**Risk:** Low (additive feature, doesn't affect existing 16×16 tables)

**Dependencies:**
- **Must have:** SPI flash storage (32×32 tables won't fit in EEPROM)
- **Must have:** 16-bit table values (32×32 with 8-bit = still coarse resolution)

---

### 🎯 Priority 6: Drain All Timer Flags Per ISR
**Goal:** Handle multiple pending timer events per interrupt (vs. single `else if`)

**Current Blocker:** AVR-style ISR pattern only services one event per interrupt

**Implementation Path:**
1. Audit current timer ISRs in timers.cpp (Teensy 4.1 section)
2. Replace `else if` chains with `while (pending_flags)` loops
3. Validate that ISR execution time stays within acceptable limits
4. Add ISR execution time monitoring (optional)

**Benefits Unlocked:**
- ✅ Lower latency for back-to-back events
- ✅ Better scheduler accuracy at high RPM
- ✅ Fewer missed events under load

**Estimated Effort:** 6-8 hours (ISR audit + refactor + timing validation)

**Risk:** Medium (ISR changes are safety-critical, need scope validation)

---

### 🎯 Priority 7: Board Capability Declarations
**Goal:** Explicit feature flags so firmware and INI can adapt to board

**Current Implementation:** Scattered `#ifdef CORE_TEENSY41` checks

**Desired Implementation:**
```cpp
// In board_teensy41.h
#define BOARD_CAP_SPI_FLASH         1
#define BOARD_CAP_HIGH_RES_TABLES   1
#define BOARD_CAP_HIGH_RES_ADC      1
#define BOARD_CAP_NATIVE_CAN        1
#define BOARD_CAP_RTC               1
#define BOARD_CAP_SD                1
#define BOARD_CAP_PWM_FAN           1  // After GPT timer implemented
#define BOARD_CAP_32X32_TABLES      1
#define BOARD_CAP_16BIT_TABLE_VALUES 1
```

Then in INI:
```ini
[PcVariables]
  isTeensy41 = { boardCapabilities & 0x0001 }  ; BOARD_CAP_SPI_FLASH

[UserDefined]
  ; Only show 32×32 table option if board supports it
  dialog = tableSize, "Table Size", isTeensy41
    field = "Size", veTableSize, { isTeensy41 }
```

**Benefits Unlocked:**
- ✅ INI hides unsupported features (no 32×32 option on AVR)
- ✅ Runtime code adapts cleanly (no scattered ifdefs)
- ✅ Easier to add new boards (declare capabilities, inherit features)
- ✅ Tune compatibility checking (reject Teensy tune on AVR)

**Estimated Effort:** 6-10 hours (capability flags + INI updates + runtime queries)

**Risk:** Low (infrastructure work, doesn't change behavior)

---

## Implementation Order (Dependency-Driven)

### Phase 6.1: Foundation (Storage + Capability Layer)
**Duration:** 4-6 weeks
1. ✅ SPI flash storage backend (Priority 1) — **Prerequisite for everything else**
2. ✅ Board capability declarations (Priority 7) — **Enables conditional features**

### Phase 6.2: Table Resolution (Core Tuning Improvement)
**Duration:** 3-4 weeks
3. ✅ 16-bit table values (Priority 2) — **Depends on SPI flash**
4. ✅ 32×32 table sizes (Priority 5) — **Depends on SPI flash + 16-bit values**

### Phase 6.3: Peripheral Enablement (Hardware Features)
**Duration:** 2-3 weeks
5. ✅ PWM fan support (Priority 3) — **Independent, low risk**
6. ✅ 12-bit ADC (Priority 4) — **Independent, medium risk**

### Phase 6.4: Performance Optimization (Efficiency)
**Duration:** 1-2 weeks
7. ✅ Drain all timer flags (Priority 6) — **Requires ISR validation**

### Phase 6.5: Validation and Documentation
**Duration:** 2-3 weeks
- Full bench test suite (all features enabled)
- Hardware-in-loop validation (actual Teensy 4.1 + Dropbear)
- Tune migration validation (8-bit → 16-bit, 16×16 → 32×32)
- Documentation updates (wiki, INI help text, migration guide)

**Total Estimated Duration:** 12-18 weeks (3-4.5 months)

---

## Success Metrics

| Metric | Current (AVR baseline) | Phase 6 Goal (Teensy 4.1) |
|--------|----------------------|--------------------------|
| **Table value resolution** | 8-bit (1% VE steps) | 16-bit (0.1% VE steps) |
| **Table size** | 16×16 max | 32×32 available |
| **ADC resolution** | 8-bit (256 steps) | 12-bit (4096 steps) |
| **Tune storage** | 4KB EEPROM | 8MB flash (tune banks) |
| **PWM fan** | Disabled | Enabled (GPT timer) |
| **Native CAN** | Enabled | Enabled (unchanged) |
| **SD logging** | Enabled | Enabled (unchanged) |
| **Tune banks** | 1 tune only | 3-5 tunes switchable |
| **Migration staging** | In-place (risky) | Flash-backed snapshot |

---

## Risk Mitigation

### High-Risk Items
1. **SPI flash storage:** Corruption = no-start
   - Mitigation: Keep EEPROM fallback path, test power-loss scenarios
2. **16-bit table comms:** Protocol change = tune transfer failure
   - Mitigation: Versioned protocol, backward compat for 8-bit tables
3. **12-bit ADC:** Sensor miscalibration = wrong fuel/spark
   - Mitigation: Bench test with known voltages, validate against AVR baseline

### Medium-Risk Items
1. **32×32 tables:** Memory pressure at high cylinder count
   - Mitigation: Conditional compilation, test 8-cyl + 32×32 tables
2. **Timer ISR drain:** Excessive ISR time = missed events
   - Mitigation: Scope validation, execution time limits

### Low-Risk Items
1. **PWM fan:** Isolated feature
2. **Capability flags:** Infrastructure only
3. **Drain timer flags:** Performance optimization

---

## Testing Strategy

### Unit Tests (Host-Side)
- SPI flash: file create/read/write/delete/corruption recovery
- Table conversion: 8-bit → 16-bit value scaling
- ADC scaling: raw counts → physical units

### Integration Tests (Teensy Hardware)
- Full config save/load cycle (flash backend)
- Tune bank switching (load bank 2, verify different config)
- Migration: EEPROM → flash first boot
- Table streaming: 32×32 table upload via serial
- ADC accuracy: known voltage → verify physical unit

### Regression Tests (Existing 812 Tests)
- All decoder tests must remain green
- All protection/knock tests must remain green
- All sensor tests must remain green
- No AVR functionality broken by Teensy additions

---

## Recommended Next Steps

### Immediate (This Session)
1. ✅ Create this roadmap document (DONE)
2. ✅ Update phase3_pragmatic_summary.md with roadmap alignment findings
3. ✅ Add note to FIRMWARE_ROADMAP.md: "Phase 6 in progress, see phase6_teensy41_enablement_roadmap.md"

### Short-Term (Next Session)
1. Prototype SPI flash storage with LittleFS
   - Add library dependency to platformio.ini
   - Create storage_spi.cpp skeleton
   - Test basic file create/read/write
2. Design board capability bitfield
   - Define capability constants
   - Add boardHasCapability() query function
   - Update INI to check capabilities

### Medium-Term (Next 2-3 Sessions)
1. Implement full SPI flash backend
2. Add 16-bit table support (conditional compilation)
3. Update comms protocol for 16-bit table transfers
4. Add Teensy-specific INI definitions

---

## Why Phase 6 Before Phase 3-5

**From FIRMWARE_ROADMAP.md:**

> "Phase 6 should be prioritized before Phase 3-5 because storage infrastructure (SPI flash) unblocks larger tune pages and migration staging."

**Rationale:**
1. **Phase 3 (Runtime Structure):** Module extractions are easier with explicit board capabilities
   - Knock module can check `BOARD_CAP_KNOCK_HARDWARE`
   - Protection module can check `BOARD_CAP_HIGH_RES_ADC`
2. **Phase 4 (Board Consistency):** Requires capability layer (Phase 6 deliverable)
   - Can't push board-specific behavior until capabilities are declared
3. **Phase 5 (Configuration):** Tune migration needs SPI flash staging
   - Pre-migration snapshots require storage (Phase 6 deliverable)
   - Larger config pages require flash (Phase 6 deliverable)

**Conclusion:** **Phase 6 is a prerequisite for Phase 3-5**, not a "nice-to-have"

---

## Summary

**Phase 6 Status:** Ready to begin
**Phase 2 Baseline:** 812/812 tests passing ✅
**Estimated Total Effort:** 12-18 weeks
**Highest Value Feature:** SPI flash storage (unblocks everything else)
**Lowest Hanging Fruit:** PWM fan (4-6 hours, isolated)

**Recommended Approach:**
1. Start with SPI flash prototype (validate LittleFS works)
2. Add board capability layer (infrastructure)
3. Implement 16-bit tables (depends on flash storage)
4. Add PWM fan in parallel (independent work)
5. Defer 12-bit ADC and 32×32 tables until flash + 16-bit tables proven

**Success Criterion:** Teensy 4.1 becomes a **first-class platform** with features that AVR cannot support, enabling higher-resolution tuning and better data logging.
