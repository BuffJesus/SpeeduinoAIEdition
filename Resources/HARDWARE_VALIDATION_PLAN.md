# Hardware Validation Plan - Phase 6 Teensy 4.1 Enablement

**Status**: Ready for bench/vehicle testing
**Target Platform**: Teensy 4.1 with SPI flash storage
**Baseline**: 263/263 decoders passing, all unit tests green

---

## Validation Prerequisites

### Required Hardware
- Teensy 4.1 board (IMXRT1062, 600MHz, 7.75MB flash)
- SPI flash chip (W25Q128, tested configuration)
- Test bench with:
  - Programmable wheel simulator (for decoder testing)
  - Analog signal sources (for ADC validation)
  - Fuel/ignition output monitoring
- TunerStudio software (latest version)

### Firmware Configuration
- Build environment: PlatformIO, `teensy41` target
- Firmware signature: `speeduino 202501-T41`
- INI file: speeduino.ini with `#if mcu_teensy` conditional sections
- Expected flash usage: ~254KB (verified in build)

---

## Test Sequence

### Phase 1: Boot and Storage Verification

**Objective**: Confirm Teensy 4.1 boots, initializes SPI flash, and falls back to EEPROM if needed.

**Tests**:
1. **Cold boot with blank SPI flash**
   - Power on Teensy 4.1 with erased SPI flash
   - Expected: Firmware boots, loads defaults from EEPROM
   - Verify: Serial output shows SPI flash init status
   - Verify: TunerStudio connects and displays default tune

2. **Save configuration to SPI flash**
   - Modify configPage2 values via TunerStudio
   - Click "Burn to Flash"
   - Expected: Tune saved to SPI flash (not EEPROM)
   - Verify: Check serial output for SPI flash write confirmation

3. **Power cycle and load from SPI flash**
   - Power cycle Teensy 4.1
   - Expected: Firmware loads modified tune from SPI flash
   - Verify: TunerStudio displays previously saved values
   - Verify: EEPROM not accessed (check serial output)

4. **SPI flash failure fallback**
   - Disconnect or corrupt SPI flash chip
   - Power cycle
   - Expected: Firmware falls back to EEPROM
   - Verify: Tune loads from EEPROM, serial output shows fallback message

**Pass criteria**: All 4 tests succeed, no boot failures, tune persists across power cycles.

---

### Phase 2: TunerStudio Communication

**Objective**: Confirm TunerStudio recognizes Teensy 4.1 signature and uses correct page sizes.

**Tests**:
1. **Firmware signature detection**
   - Connect TunerStudio to Teensy 4.1
   - Expected: TunerStudio displays "speeduino 202501-T41" in connection dialog
   - Verify: INI file loads correct conditional sections (`#if mcu_teensy`)

2. **Page size validation**
   - Read all 15 configuration pages via TunerStudio
   - Expected: No page size mismatches or truncation errors
   - Verify: Pages with 3D tables use correct sizes (e.g., veMapPage = 544 bytes)
   - Reference: PHASE6_INI_UPDATES_SPEC.md for expected sizes

3. **Table value write/read**
   - Modify veTable (16x16) values via TunerStudio
   - Write values > 255 (e.g., 300, 512) to test 16-bit storage
   - Burn to flash
   - Power cycle
   - Expected: Values > 255 persist correctly
   - Verify: TunerStudio displays correct 16-bit values on reload

**Pass criteria**: TunerStudio communicates without errors, 16-bit table values round-trip correctly.

---

### Phase 3: 12-bit ADC Accuracy

**Objective**: Validate 12-bit ADC provides higher resolution for MAP, TPS, IAT, CLT sensors.

**Tests**:
1. **MAP sensor resolution**
   - Apply stable analog voltage to MAP sensor pin (e.g., 2.5V = 100 kPa)
   - Record MAP reading in TunerStudio live dashboard
   - Expected: MAP value stable within ±0.5 kPa (12-bit resolution ~0.24 kPa)
   - Compare: AVR 10-bit resolution = ±1 kPa

2. **TPS sensor resolution**
   - Sweep TPS voltage from 0V to 5V slowly
   - Record TPS percentage in TunerStudio
   - Expected: Smooth 0-100% sweep with no jumps > 0.5%
   - Verify: 12-bit ADC provides ~0.1% resolution vs. AVR 0.4%

3. **Temperature sensor resolution**
   - Apply stable voltage to CLT/IAT pins
   - Record temperature in TunerStudio
   - Expected: Temperature stable within ±0.5°C
   - Verify: 12-bit ADC improves granularity at cold start (e.g., -20°C to 0°C range)

**Pass criteria**: All sensors show improved resolution vs. AVR, no noise or instability.

---

### Phase 4: ISR Timer Drain Optimization

**Objective**: Confirm all ISR handlers drain pending flags without missed interrupts.

**Tests**:
1. **Sequential fuel injection timing**
   - Configure 8-cylinder sequential fuel mode
   - Run engine at 6000 RPM (high event rate)
   - Expected: All 8 fuel channels fire on time
   - Verify: No missed injections, no timing drift
   - Monitor: TMR1_isr and TMR3_isr handle 4 channels each

2. **Sequential ignition timing**
   - Configure 8-cylinder wasted spark or sequential ignition
   - Run engine at 6000 RPM
   - Expected: All 8 ignition channels fire on time
   - Verify: No missed sparks, no timing errors
   - Monitor: TMR2_isr and TMR4_isr handle 4 channels each

3. **Concurrent timer events**
   - Configure engine with overlapping fuel/ignition events
   - Run at high RPM with boost, VVT, idle control active
   - Expected: PIT_isr, TMR1-4_isr all drain flags correctly
   - Verify: No "skipped interrupt" warnings in serial output

**Pass criteria**: All timer channels fire reliably at high RPM, no missed events.

---

### Phase 5: Decoder Pattern Validation

**Objective**: Confirm decoder patterns work identically on Teensy 4.1 as on AVR.

**Tests**:
1. **Missing tooth (36-1) decoder**
   - Feed 36-1 pattern at 1000 RPM via wheel simulator
   - Expected: Decoder syncs within 2 revolutions
   - Verify: RPM reading stable, no sync loss warnings
   - Verify: Ignition/fuel timing matches AVR behavior

2. **Dual wheel decoder (e.g., 24-1 + cam)**
   - Feed dual wheel pattern at 3000 RPM
   - Expected: Decoder achieves full sync with cam signal
   - Verify: Sequential mode enabled, correct cylinder phasing

3. **High tooth count decoder (60-2)**
   - Feed 60-2 pattern at 8000 RPM
   - Expected: Decoder maintains sync at high event rate
   - Verify: No "too many interrupts" errors
   - Note: Teensy 4.1 @ 600MHz should handle easily

**Pass criteria**: All 263 decoder patterns behave identically to AVR, no sync loss at high RPM.

---

### Phase 6: Full Engine Run

**Objective**: Validate complete ECU operation on running engine.

**Tests**:
1. **Cold start**
   - Start cold engine (< 20°C coolant temp)
   - Expected: Engine cranks, fires, idles stably
   - Verify: Cold start enrichment applied, warmup ramp smooth

2. **Idle stability**
   - Let engine idle for 5 minutes
   - Expected: RPM stable ±50 RPM, no stalling
   - Verify: Idle control, closed-loop O2, fuel trim active

3. **Transient response**
   - Apply sudden throttle changes (0% → 100% → 0%)
   - Expected: Engine responds smoothly, no hesitation or stumble
   - Verify: Acceleration enrichment, MAP-based fuel correction working

4. **High load operation**
   - Run engine at WOT (wide open throttle) for 10 seconds
   - Expected: Engine pulls cleanly to redline
   - Verify: Boost control (if equipped), knock detection, fuel/ignition maps correct

5. **Sustained operation**
   - Run engine for 30 minutes under varying load
   - Expected: No crashes, resets, or anomalies
   - Verify: All sensors read correctly, no memory leaks, flash write OK

**Pass criteria**: Engine runs identically to AVR firmware, no regressions, all features functional.

---

## Known Limitations

1. **SPI flash serialization**: Page-to-buffer serialization deferred to future phase. Current implementation uses infrastructure only. See: Resources/spi_flash_serialization_approach.md

2. **Test coverage**: Unit tests cover core logic, but hardware-specific ISR timing can only be validated on actual hardware.

3. **Conditional compilation**: Some code paths (e.g., `#ifdef CORE_TEENSY41`) only tested via compilation, not runtime, until hardware test.

---

## Rollback Plan

If validation fails:
1. Revert to AVR firmware on production vehicles
2. Document failure mode in GitHub issue
3. Isolate root cause via serial debug output
4. Fix in isolated environment, re-run unit tests, retry hardware test

---

## Success Criteria

**Phase 6 considered complete when**:
- All 6 test phases pass without failures
- TunerStudio 16-bit table values confirmed working
- SPI flash storage persists across power cycles
- Engine runs identically to AVR baseline
- No regressions in decoder sync, fuel/ignition timing, or sensor accuracy

---

## References

- speeduino.ini: Conditional INI with Teensy 4.1 page sizes
- comms_legacy.cpp:397-412: Firmware signature function
- board_teensy41.cpp:217-288: ISR handlers with flag draining
- storage_spi.h/cpp: SPI flash LittleFS backend
- PHASE6_INI_UPDATES_SPEC.md: Page size calculations
- spi_flash_serialization_approach.md: Future serialization work
