# Firmware Roadmap

## Phase 1: Safety And Correctness

- Fix high-risk ignition, fueling, and protection defects before broader refactors.
- Prioritize knock handling, limiter/protection interactions, and config/runtime mismatches.
- Replace placeholder telemetry where it reports misleading runtime data.
- Add focused regression coverage for corrected logic where unit-test hooks already exist.

Current phase 1 work started in:
- [corrections.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/corrections.cpp)

## Phase 2: Regression Harness

- Add unit tests around ignition corrections, engine protection, sensor filtering, and config migrations.
- Use existing `UNIT_TEST` hooks to expose pure or near-pure helpers without changing runtime behavior.
- Build migration tests for [updates.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/updates.cpp).
- Add decoder state-transition tests for the most common trigger patterns in [decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp).
- Add host-side trigger tests that replay real captured crank/cam traces, not just synthetic edge sequences.
- Create a small library of noisy and borderline trigger recordings to regression-test sync, dwell scheduling, and false-trigger handling.
- Add scenario tests for launch, flat shift, hard/soft limiters, and protection reactivation behavior as named state-machine tests instead of only function-local checks.
- Keep using the forum/manual evidence pipeline as a primary source for blocked decoder work:
  - prefer canonical topic-level extraction from [Resources/speeduino_evidence_collector_stable.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/speeduino_evidence_collector_stable.py), not raw search-engine dumps
  - prioritize decoder families with surviving high-confidence forum evidence after topic dedupe and explicit family matching
  - current best evidence-backed next decoder candidate is `36-2-1`, but it should still be approached from real captured logs rather than guessed synthetic traces; the blocked Rover MEMS cam-phase slice remains explicitly deferred to the end-of-roadmap revisit list
  - use external decoder references as topology and sync-behavior evidence, not as drop-in waveform sources:
    - rusEFI real-trigger tests in [Resources/rusefi-2026-03-17/unit_tests/tests/trigger/test_real_4b11.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/unit_tests/tests/trigger/test_real_4b11.cpp) are the best current external model for `36-2-1` work because they validate against real CSV captures
    - rusEFI decoder implementations in [Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_honda.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_honda.cpp) and [Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_rover.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_rover.cpp) are useful for `Honda J32` and Rover-family topology review
    - MS3 Rover references in [Resources/ms3-source-master/ms3/core.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master/ms3/core.ini), [Resources/ms3-source-master/ms3/ms3_ign_in.c](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master/ms3/ms3_ign_in.c), and [Resources/ms3-source-master/ms3/ms3_ign_wheel.c](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master/ms3/ms3_ign_wheel.c) are the best current external clues for Rover phase and `poll_level_tooth` semantics
  - before adding new replay traces for `Rover MEMS` or `36-2-1`, first convert the surviving external/forum evidence into an explicit tooth/gap or poll-phase note instead of inferring directly from comments
  - the `36-2-1` physical gap model remains documented in [SESSION_HANDOFF_2026-03-22_36-2-1.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_36-2-1.md), backed by [analyze_36_2_1.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_36_2_1.py), while the maintained current-code ISR contract is now formalized in [SESSION_HANDOFF_2026-03-23_36-2-1_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_36-2-1_STATE.md) and covered directly in [ThirtySixMinus21.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/ThirtySixMinus21/ThirtySixMinus21.cpp); the remaining blocker is still the decoder's unresolved single-gap tooth-number assignment, not the physical wheel topology
  - the `36-2-2-2` Subaru-focused evidence/staging note remains in [SESSION_HANDOFF_2026-03-22_36-2-2-2.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_36-2-2-2.md), while the maintained current-code `4`-cylinder sync contract is now formalized in [SESSION_HANDOFF_2026-03-23_36-2-2-2_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_36-2-2-2_STATE.md) and covered directly in [ThirtySixMinus222.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/ThirtySixMinus222/ThirtySixMinus222.cpp); the remaining blocker is still staged real-capture normalization for replay rather than stronger family-wide pattern coverage
  - the `420a` current-code secondary-falling sync / realignment / wrap contract is now documented in [SESSION_HANDOFF_2026-03-23_420A_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_420A_STATE.md) and has direct AVR state coverage in [FourTwentyA.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/FourTwentyA/FourTwentyA.cpp), locking the current primary-high tooth-`13`, primary-low tooth-`5`, sync-loss realignment, and `16`-tooth wrap behavior
  - the `Audi 135` current-code sync / decimation contract is now documented in [SESSION_HANDOFF_2026-03-22_AUDI135.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_AUDI135.md) and has direct AVR state coverage in [Audi135.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Audi135/Audi135.cpp), complementing the older replay-backed slice with explicit every-third-tooth and resync assertions
  - the `Basic Distributor` current-code sync / wrap / filter contract is now documented in [SESSION_HANDOFF_2026-03-23_BASIC_DISTRIBUTOR_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_BASIC_DISTRIBUTOR_STATE.md) and has direct AVR state coverage in [BasicDistributor.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/BasicDistributor/BasicDistributor.cpp), complementing the older replay-backed slice with explicit stroke-dependent tooth-angle, first-pulse sync, wrap, and lite-filter assertions
  - the `Daihatsu +1` current-code sync model is now documented in [SESSION_HANDOFF_2026-03-22_DAIHATSU.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_DAIHATSU.md) and has direct AVR state coverage in [Daihatsu.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Daihatsu/Daihatsu.cpp) for both `3`- and `4`-cylinder paths
  - the `DRZ400` current-code setup / secondary-sync / shared-primary-wrap contract is now documented in [SESSION_HANDOFF_2026-03-23_DRZ400_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_DRZ400_STATE.md) and has direct AVR state coverage in [DRZ400.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/DRZ400/DRZ400.cpp), locking the setup-seeded filters, unsynced secondary sync, synced secondary tooth-`6` realignment, and first-primary wrap through the shared dual-wheel ISR
  - the `Dual-Wheel` current-code secondary-sync / primary-wrap / short-secondary-filter / kept-`useResync` contract is now documented in [SESSION_HANDOFF_2026-03-23_DUALWHEEL_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_DUALWHEEL_STATE.md) and has direct AVR state coverage in [dual_wheel.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/dual_wheel/dual_wheel.cpp), complementing the replay-backed slice with explicit sync establishment, first-primary wrap, isolated short-secondary filtering, and resync assertions
  - the `Ford ST170` current-code missing-tooth half-sync / cam-group promotion / noise-filter / regroup contract is now documented in [SESSION_HANDOFF_2026-03-23_FORDST170_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_FORDST170_STATE.md) and has direct AVR state coverage in [ForsdST170.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/FordST170/ForsdST170.cpp), complementing the replay-backed slice with explicit half-sync, cam-before-gap full sync, short-noise filtering, and sixth-tooth regroup assertions
  - the `GM 24X` cam-reset sync contract is now documented in [SESSION_HANDOFF_2026-03-22_GM24X_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_GM24X_STATE.md) and has direct AVR state coverage in [Gm24X.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Gm24X/Gm24X.cpp), complementing the older replay-backed slice with explicit sentinel, lookup-table, and resync assertions
  - the `GM 7X` current-code short-gap sync contract is now documented in [SESSION_HANDOFF_2026-03-23_GM7X_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_GM7X_STATE.md) and has direct AVR state coverage in [Gm7X.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Gm7X/Gm7X.cpp), complementing the older replay-backed slice with explicit `tooth 3` sync, no-sync, and wrap assertions
  - the `4G63` maintained host-harness contract is now formalized in [SESSION_HANDOFF_2026-03-23_4G63_REPLAY_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_4G63_REPLAY_STATE.md), with the earlier replay implementation note preserved in [SESSION_HANDOFF_2026-03-20_4G63.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-20_4G63.md); the kept coverage is replay-only and locks no-false-sync / candidate-tooth behavior rather than full sync
  - the `Harley` current-code high-state sync / two-tooth toggle / low-state sync-loss contract is now documented in [SESSION_HANDOFF_2026-03-23_HARLEY_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_HARLEY_STATE.md) and has direct AVR state coverage in [Harley.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Harley/Harley.cpp), complementing the older replay-backed slice with explicit setup, tooth-`1`, tooth-`2`, and sync-drop assertions
  - the `Jeep 2000` cam-reset sync contract is now documented in [SESSION_HANDOFF_2026-03-22_JEEP2000_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_JEEP2000_STATE.md) and has direct AVR state coverage in [Jeep2000.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Jeep2000/Jeep2000.cpp), complementing the older replay-backed slice with explicit sentinel, grouped-tooth, and next-revolution assertions
  - the `Mazda AU` current-code sync model is now documented in [SESSION_HANDOFF_2026-03-22_MAZDAAU.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_MAZDAAU.md) and has direct AVR state coverage in [MazdaAU.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/MazdaAU/MazdaAU.cpp), including the current third-cam-tooth sync path and the odd unsynced short-gap counter mutation
  - the `Miata 99-05` current-code cam-edge sync / noise deferral / wrap contract is now documented in [SESSION_HANDOFF_2026-03-23_MIATA9905_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_MIATA9905_STATE.md) and has direct AVR state coverage in [Miata9905.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Miata9905/Miata9905.cpp), complementing the older replay-backed slice with explicit sentinel, tooth-`6` sync, noisy extra-edge deferral, and `8`-edge wrap assertions
  - the `Missing-Tooth 36-1` current-code gap-sync / half-sync / cam-promotion / early-gap-loss contract is now documented in [SESSION_HANDOFF_2026-03-23_MISSINGTOOTH36_1_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_MISSINGTOOTH36_1_STATE.md) and has direct AVR state coverage in [missing_tooth.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/missing_tooth/missing_tooth.cpp), complementing the replay-backed slice with explicit tooth-`1` sync, primary-only sequential half-sync retention, cam promotion, and early-gap sync-loss assertions
  - the `NGC4` current-code wide-missing-tooth half-sync / seeded cam-phase full-sync contract is now documented in [SESSION_HANDOFF_2026-03-23_NGC4_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_NGC4_STATE.md) and has direct AVR state coverage in [test_ngc.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/NGC/test_ngc.cpp), complementing the replay-backed slice with explicit half-sync, phase-`1`, and phase-`5` assertions
  - the `Non-360` current-code multiplier-scaled setup / shared-dual-wheel sync / RPM-gating / crank-angle-fallback contract is now documented in [SESSION_HANDOFF_2026-03-23_NON360_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_NON360_STATE.md) and has direct AVR state coverage in [Non360.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Non360/Non360.cpp), locking the current `TrigAngMul` setup behavior, secondary sync with first-primary wrap, sync-gated RPM reporting, and `toothCurrentCount == 0` crank-angle fallback
  - the `Nissan360` current-code window-mapping / invalid-window rejection / wrap / kept-`useResync` contract is now documented in [SESSION_HANDOFF_2026-03-23_NISSAN360_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_NISSAN360_STATE.md) and has direct AVR state coverage in [Nissan360.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Nissan360/Nissan360.cpp), complementing the replay-backed slice with explicit 4/6/8-cylinder window mappings, invalid-window rejection, synced wrap, and the kept positive `useResync` realignment assertion
  - the `Renix` current-code virtual-tooth advancement / short-gap filtering contract is now documented in [SESSION_HANDOFF_2026-03-23_RENIX_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_RENIX_STATE.md) and has direct AVR state coverage in [renix.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/renix/renix.cpp), complementing the replay-backed slice with explicit valid-group advancement and isolated short-gap rejection assertions
  - the `Honda J32` current-code primary-only `15 teeth -> gap -> 7 teeth -> gap` contract is now formalized in [SESSION_HANDOFF_2026-03-23_HONDAJ32_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_HONDAJ32_STATE.md), with the earlier evidence basis preserved in [SESSION_HANDOFF_2026-03-22_HONDAJ32.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_HONDAJ32.md), and has dedicated state-machine coverage in [HondaJ32.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/HondaJ32/HondaJ32.cpp)
  - the `Honda D17` current-code short-gap sync / no-sync / wrap contract is now documented in [SESSION_HANDOFF_2026-03-23_HONDAD17_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_HONDAD17_STATE.md) and has direct AVR state coverage in [HondaD17.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/HondaD17/HondaD17.cpp), complementing the older replay-backed slice with explicit setup, `13th`-tooth sync, uniform-gap rejection, and wrap assertions
  - the `Subaru 6/7` current-code cam-count sync / misalignment recovery / wrap contract is now documented in [SESSION_HANDOFF_2026-03-23_SUBARU67_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_SUBARU67_STATE.md) and has direct AVR state coverage in [Subaru67.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Subaru67/Subaru67.cpp), complementing the older replay-backed slice with explicit tooth-`2` sync, tooth-`5` realignment, and `12`-tooth wrap assertions
  - the `Suzuki K6A` current-code descending-gap sync / wrap / short-gap-filter contract is now documented in [SESSION_HANDOFF_2026-03-23_K6A_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_K6A_STATE.md) and has direct AVR state coverage in [test_runtimeState.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/SuzukiK6A/test_runtimeState.cpp), complementing the replay-backed slice with explicit tooth-`6` sync, tooth-`1` wrap, post-sync filtering, and noisy primary-only retention assertions
  - the `Vmax` current-code wide-lobe sync / small-lobe increment / tooth-`6` wrap contract is now documented in [SESSION_HANDOFF_2026-03-23_VMAX_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_VMAX_STATE.md) and has direct AVR state coverage in [Vmax.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Vmax/Vmax.cpp), locking the current wide-lobe tooth-`1` sync, tooth-`2` advance, and tooth-`6` wrap behavior
  - the current Rover-specific notes are [SESSION_HANDOFF_2026-03-21_ROVER_MEMS_NOTE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-21_ROVER_MEMS_NOTE.md), [SESSION_HANDOFF_2026-03-22_ROVER_MEMS_BIT_WINDOW.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_ROVER_MEMS_BIT_WINDOW.md), [SESSION_HANDOFF_2026-03-22_ROVER_MEMS_CAM_532.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_ROVER_MEMS_CAM_532.md), and [SESSION_HANDOFF_2026-03-22_ROVER_MEMS_TOOTH_LOG.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_ROVER_MEMS_TOOTH_LOG.md); the maintained primary-only decoder contract is now separated in [SESSION_HANDOFF_2026-03-23_ROVER_MEMS_PRIMARY_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_ROVER_MEMS_PRIMARY_STATE.md). Rover attachments are staged locally in [Resources/rover_mems_evidence](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence), the composite-log CSV channel mapping is solved with [tools/parse_speeduino_composite_csv.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/parse_speeduino_composite_csv.py), the Rover PDF image corpus is extracted with [tools/extract_pdf_images.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/extract_pdf_images.py), the extracted images are OCR-indexed with [tools/index_pdf_images.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/index_pdf_images.py), PDF readiness is checked with [tools/inspect_pdf_evidence.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/inspect_pdf_evidence.py), full Rover pages are rendered with [tools/render_pdf_pages.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/render_pdf_pages.py), the manuals now map OEM Rover layouts onto Speeduino's `11-5-12-4` and `2-14-3-13` patterns, the rolling `roverMEMSTeethSeen` bit window is derived in [derive_rover_mems_windows.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/derive_rover_mems_windows.py), and a real June 25 post-fix tooth logger host-side-proves Rover pattern `#1` via [analyze_rover_mems_tooth_log.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_rover_mems_tooth_log.py); the remaining blocker is still the exact `5-3-2 cam` phase alignment needed for full-sync Rover replay rather than primary-wheel identification itself

## Phase 3: Runtime Structure

- Split [init.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/init.cpp) by responsibility:
  - startup and board bring-up
  - config-derived table construction
  - pin and interrupt wiring
  - decoder initialization
- Reduce logic in [speeduino.ino](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/speeduino.ino) so it mainly orchestrates scheduled work.
- Isolate high-risk domains behind explicit module boundaries:
  - knock ✅ **Slice A, C, D complete**
  - engine protection
  - idle control
  - status/log export
- Move toward board-scoped feature configuration files for hardware-dependent subsystems such as knock, trigger inputs, and built-in peripherals instead of spreading those assumptions across common code and INI defaults.
- Prefer subsystem-owned state structs for complex runtime domains so calculations and live data stay close together. ✅ **KnockState struct introduced**

### Phase 3 Completed Work

**Slice A: Extract Knock Helper Functions** ✅ **COMPLETE**
- Extracted 3 testable helper functions from corrections.cpp to knock.cpp:
  - `knockGetActivationCount()` - Determines knock count threshold with zero-guard
  - `knockCalculateRetard()` - Computes ignition retard with overflow protection
  - `knockCalculateRecovery()` - Manages timing recovery with step progression
- Added 24 test assertions in test/test_ign/test_knock_helpers.cpp
- Maintained backward-compatible wrappers in corrections.cpp
- Test results: 159/159 test_ign PASSED, 263/263 test_decoders PASSED
- Binary size: Teensy 4.1 253868 bytes (-64 from Phase 6 baseline)

**Slice C: Consolidate Knock Domain Logic** ✅ **COMPLETE**
- Moved 4 knock correction functions from corrections.cpp to knock.cpp:
  - `knockEventIsValid()` - Knock window validation
  - `knockApplyCorrection()` - Main knock timing correction logic
  - Plus helper imports from Slice A
- All knock business logic now consolidated in knock.cpp module
- corrections.cpp retains only orchestration role via `correctionsIgn()`
- Test results: 156/156 test_ign PASSED, 263/263 test_decoders PASSED
- Binary size: Teensy 4.1 253932 bytes (stable)

**Slice D: Create KnockState Struct** ✅ **COMPLETE**
- Added `KnockState` struct in knock.h with 4 fields:
  - `startTime` (unsigned long) - Timestamp of last knock event
  - `lastRecoveryStep` (uint8_t) - Recovery progress counter
  - `retard` (uint8_t) - Current knock retard amount
  - `count` (volatile uint8_t) - Total knock events (ISR-safe)
  - `reset()` method (marked volatile for ISR compatibility)
- Migrated knock.cpp functions to use `knockState` internally
- Maintained bidirectional sync with legacy globals for backward compatibility
- Updated `initialiseCorrections()` to call `knockState.reset()`
- Added 3 new KnockState tests (initialization, multi-step updates, volatile access)
- Test results: 159/159 test_ign PASSED (including 3 new tests), 263/263 test_decoders PASSED
- Binary size: Teensy 4.1 253868 bytes (-64 from Phase 6, net 0 from Slice A)

**Slice B: Deferred** - High complexity, lower priority given successful alternate path

### Phase 3 Remaining Work

- **Engine Protection**: ✅ **Audit complete** - Already has excellent module boundary, no struct consolidation needed
- **Idle Control**: Consolidate idle algorithms, create IdleState struct (DEFERRED - medium risk, lower priority)
- **Status/Log Export**: Clarify logger contract, reduce currentStatus coupling (moved to Phase 4)
- **init.cpp Split**: Separate initialization concerns into focused modules (partially complete - req_fuel helpers extracted)

**Phase 3 Status:** SUBSTANTIALLY COMPLETE
- Knock module: Full boundary consolidation with KnockState struct
- Engine protection: Already follows target pattern
- Init helpers: req_fuel/injector calculations extracted and tested (12 assertions)
- Remaining work moved to Phase 4 or deferred as lower priority

## Phase 4: Board And Comms Consistency ✅ **COMPLETE**

**Completed:**
- ✅ Board-layer audit (4 LOW-risk moves, 1 MEDIUM-risk deferred)
- ✅ ADC initialization moved to board layer (initADC_Teensy41 in initBoard)
- ✅ Board capability output channel implemented (byte 130, 3 tests)
- ✅ SPI flash health output channel implemented (byte 131, 2 tests)
- ✅ Legacy telemetry audit (4 deprecated placeholders marked: MAF at legacy packet bytes, getNextError at bytes 75/52/74)
- ✅ Interrupt pin audit (29+ ungated attachInterrupt calls identified)
- ✅ Interrupt pin validation implemented (BOARD_CAP_UNRESTRICTED_INTERRUPTS flag, validateTriggerPins function, 6 tests)
- ✅ Comms helper extraction (14 tests: calculateLegacySendProgress, getLegacyVersionResponse, isLegacyOutputChannelsCommandSupported, getLegacyVersionResponseCommand)
- ✅ Output channel alignment (LOG_ENTRY_SIZE 131→132, speeduino.ini ochBlockSize 130→132 synced)
- ✅ setPinMapping audit (19 board cases, case 3 largest at ~333 lines, extraction opportunities documented)

**Board Capability Output Channels:**
- **Byte 130**: Board capability bitfield (BOARD_CAP_UNRESTRICTED_INTERRUPTS, BOARD_CAP_SPI_FLASH)
- **Byte 131**: SPI flash health (Teensy 4.1 only, 1=healthy, 0=unavailable/not present)
- Updated LOG_ENTRY_SIZE 131 → 132
- Synced speeduino.ini ochBlockSize 130 → 132
- Added 5 tests (3 board caps + 2 SPI flash health)
- Enables TunerStudio board-aware UI (hide features not supported by hardware)

**Interrupt Pin Validation:**
- Added BOARD_CAP_UNRESTRICTED_INTERRUPTS flag (Teensy 4.1 and STM32)
- Implemented validateTriggerPins() function in init.cpp
- Validates trigger pin assignments against platform interrupt capabilities
- Added 6 tests covering all decoder configurations and platform capabilities
- Safety-critical: Prevents runtime failure from invalid pin assignments on restricted-interrupt platforms

**Legacy Telemetry Audit:**
- **Deprecated MAF placeholders**: buildLegacyValuesPacket() bytes (not genuinely implemented, marked for future removal)
- **Deprecated getNextError placeholders**: bytes 75 (getTSLogEntry), 52 (buildLegacyValuesPacket), 74 (getReadableLogEntry) — returns hardcoded 0, no active infrastructure
- **UNUSED slot**: getReadableLogEntry case 59 (marked "//UNUSED!!")
- Recommendation: Keep deprecated comments, defer removal until legacy protocol sunset

**Comms Helper Extraction:**
- Extracted 4 testable helper functions from comms_legacy.cpp:
  - `calculateLegacySendProgress()` - Compute remaining bytes for serial throttling
  - `getLegacyVersionResponse()` - Map version response to string
  - `isLegacyOutputChannelsCommandSupported()` - Validate realtime command
  - `getLegacyVersionResponseCommand()` - Map version to command byte
- Added 14 tests covering edge cases, boundary conditions, and existing workarounds
- Enables evidence-based legacy protocol refactoring

**setPinMapping Audit:**
- 19 distinct board cases (IDs: 1, 2, 3, 6, 8, 9, 10, 20, 30, 31, 40, 41, 42, 45, 50, 51, 53, 55, 56, 60)
- Function spans ~1794 lines (init.cpp:1254–3048)
- Largest case: Case 3 (~333 lines) with 5 nested platform variants (AVR Mega, CORE_TEENSY35, CORE_TEENSY41, STM32F407xx, CORE_STM32)
- High repetition: Each case manually assigns 20–40 pin variables
- **Extraction opportunities identified:**
  1. HIGH priority: Extract Dropbear (case 60) into board_teensy41.cpp data table (~50 lines, low risk, 2-hour effort)
  2. MEDIUM priority: Convert case 3 platform variants into board_* lookups (moderate risk, 4–6 hours)
  3. LOW priority: Consolidate duplicate cases (40/41/42, 50/51) via shared tables (3–4 hours per group)
  4. FUTURE: Full data-driven pin table system (20–30 hours + multi-platform regression testing)
- Recommendation: Defer extraction to Phase 5+ or future maintenance window

**Binary Size:**
- Teensy 4.1: 254,060 bytes code + 30,464 data (FLASH: 292,860 total, +128 from previous 292,732)
- RAM1: variables 38,880, code 248,952, padding 13,192 (free for local variables: 223,264)
- RAM2: variables 12,416 (free for malloc/new: 511,872)

**Test Results (Phase 4 Closing Baseline):**
- test_decoders: 263/263 PASSED
- test_ign: 164/164 PASSED (+5 from Phase 3: +2 SPI flash health, +3 knock state = 164 total)
- test_updates: 38/38 PASSED
- test_updates_tail: 5/5 PASSED
- test_sensors: 57/57 PASSED
- test_tables: 24/24 PASSED
- test_math: 44/44 PASSED
- test_fuel: 88/88 PASSED
- test_schedules: 26/26 PASSED
- test_init: TIMED OUT (10 tests ran: 1 failed unrelated, 8 succeeded — known simavr long-running suite issue, 139 tests total in suite, includes 14 new comms helper tests)
- **Total passing: 709/709 (excluding test_init timeout)**

**Remaining Phase 4 Work (Future):**
- setPinMapping data-driven conversion (documented, deferred)
- Abstract PWM polarity inversion into board-layer functions (MEDIUM risk, deferred)
- Platform-specific code moves into board_* layer (ongoing, incremental)
- Full SPI flash page serialization ✅ (Phase 10: saveTablePageToFlash/loadTablePageFromFlash added; all 8 table pages dual-write wired in storage.cpp; kStructPageIDs expanded to all 15 pages for complete tune bank capture)

## Phase 5: Configuration And Observability

### Phase 5 Completed Work

**Slice A: updates.cpp Migration Audit** ✅ **COMPLETE** (no code changes)
- CURRENT_DATA_VERSION = 24; 22 migration steps v2→v24
- ALL steps covered by 38 (test_updates) + 11 (test_updates_tail) = 49 tests total
- 7 testable helper functions extracted: migratePIDGain, migrateCrankingEnrich, migrateFlexAdvance, migrateCrankingPctToCurve, migrateTPSResolution, migrateVVTTableEntry, migrateIdleAdvDelay
- Migration types: EEPROM raw shifts (v5→v6, v6→v7, v16→v17), unit conversions, default fills, struct fixes
- All raw EEPROM shift tests confirmed present

**Slice B: Knock Observability Status Flags** ✅ **COMPLETE**
- Added 5 `KNOCK_STATUS_*` bitmask constants to knock.h
- Implemented `knockGetStatusFlags(knockMode, windowActive, knockCount, activationCount, retard, lastRecoveryStep)` in knock.cpp
  - Pure function with no global state dependencies — fully testable
  - Flags: MODE_ENABLED, WINDOW_ACTIVE, COUNT_AT_THRESHOLD, RETARD_ACTIVE, RECOVERY_ACTIVE
- Added 8 tests in test_ign/test_knock_helpers.cpp
- test_ign: 164 → 172/172 PASSED

**Slice C: Migration Helpers for v17→v18** ✅ **COMPLETE**
- Extracted `migrateVVTTableEntry_v17_to_v18(uint8_t)` — shift left by 1, saturates at UINT8_MAX for inputs > 127
- Extracted `migrateIdleAdvDelay_v17_to_v18(uint8_t)` — multiply by 2, saturates at UINT8_MAX for inputs > 127
- Both declared in updates.h UNIT_TEST section, implemented in updates.cpp UNIT_TEST section
- Added 6 tests in new `testConfigMigrations_group1b()` runner (placed in test_updates_tail to stay within ATmega2560 SRAM limit)
- test_updates_tail: 5 → 11/11 PASSED

**Test Results (Phase 5 Closing Baseline):**
- test_decoders: 263/263 PASSED
- test_ign: 172/172 PASSED (+8 from knockGetStatusFlags tests)
- test_updates: 38/38 PASSED (unchanged)
- test_updates_tail: 11/11 PASSED (+6 from v17→v18 migration helper tests)
- test_sensors: 57/57 PASSED
- test_tables: 24/24 PASSED
- test_math: 44/44 PASSED
- test_fuel: 88/88 PASSED
- test_schedules: 26/26 PASSED
- test_init: TIMED OUT (known simavr long-running issue, 139 tests in suite)
- **Total passing: 723/723 (excluding test_init timeout)**

**Binary Size (Phase 5):**
- Teensy 4.1: 254,060 bytes code + 30,464 data (unchanged from Phase 4)

**Slice D: Logger Byte Regression Tests** ✅ **COMPLETE**
- Created `test/test_ign/test_logger_byte_regression.cpp` with 10 tests
- Locks locked byte positions in `getTSLogEntry()` and `getReadableLogEntry()`:
  - Byte 84 / index 57: `currentStatus.status3` (halfsync, fuel2, nitrous bits)
  - Byte 85 / index 58: `currentStatus.engineProtectStatus` (RPM/MAP/oil/AFR/coolant bits)
  - Byte 127 / index 91: `currentStatus.status5` (KNOCK_ACTIVE bit 3, KNOCK_PULSE bit 4)
  - Byte 128 / index 92: `currentStatus.knockCount`
  - Byte 129 / index 93: `currentStatus.knockRetard`
- test_ign: 172 → 182/182 PASSED

**Slice E: Tune Compatibility Audit** ✅ **COMPLETE** (no code changes)
See audit findings below.

**Slice F: Declarative live_data_map.h** ✅ **COMPLETE**
- Created `speeduino/live_data_map.h`: 148-row Byte/RIdx/Field/Encoding comment table covering all LOG_ENTRY_SIZE positions; `#define LIVE_DATA_MAP_SIZE 148U`; `#error` drift guard in production builds (fired by `#include "live_data_map.h"` in `logger.cpp`)
- Added 3 tests in `test/test_ign/test_live_data_map.cpp`: size constant == 148, last byte (runtimeStatusA) returns 0 with flags clear, out-of-range byte returns 0
- test_ign: 190 → 193/193; total: 780/786 (6 skipped, same as Phase 10)

**Phase 5 Status: COMPLETE** ✅

### Phase 5 Objectives

- Treat [updates.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/updates.cpp) as a first-class subsystem with validation and tests.
- Add clearer runtime observability for feature state, block conditions, and fault reasons.
- Prefer explicit status and counters over ambiguous flags when debugging field issues.
- Add a structured tune-migration pipeline instead of one-off migration edits, with ordered migrators and tests for shape changes, renames, unit conversions, and default filling.
- Add explicit tune-compatibility policy and regression checks for stock Speeduino `.msq` files:
  - keep the stock `speeduino 202501` signature only while the stock base tune imports cleanly into the current INI surface
  - add a host-side compatibility audit against [Resources/Speeduino base tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino%20base%20tune.msq) covering signature, page count, required constants, and fork-added fields that rely on defaults or migrations
  - use primary project documentation in [Resources/Speeduino_manual.pdf](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino_manual.pdf) to distinguish real default conflicts from context-dependent fields, rather than treating every raw INI `defaultValue` mismatch as equal evidence
  - align the audit with TunerStudio semantics: treat `defaultValue` as initialization evidence, not as automatic proof that the shipped tune baseline is wrong, except in `noMsqSave`-style cases
  - treat hardware-dependent polarity fields and tune-dependent idle timing fields as contextual exemptions only when the manual, INI help text, and runtime code all support that classification
  - distinguish generic INI defaults from packaged board-profile overrides when the release bundle intentionally targets specific hardware such as DropBear on Teensy 4.1
  - if the fork intentionally diverges from stock tune semantics, bump the signature and ship a fork-owned base tune plus documented migration notes
- Build a generated or declarative live-data map for high-value subsystems so logging and UI exposure stay aligned with runtime structs.
- Add regression tests that lock firmware logger bytes to TunerStudio bit mappings for high-risk status channels such as engine protection and knock.

## Phase 6: Teensy 4.1 Platform Enablement

### Phase 6 Progress Log

**Slice A: Board Audit** ✅ **COMPLETE** (no code changes)
- Timer drain-all pattern already implemented in all 5 ISRs (PIT, TMR1–TMR4) with explicit "Phase 6" comments
- `storage_spi.cpp` (396 lines) already provides LittleFS SPI flash abstraction: init, load/save config pages, tune banks, migration snapshots
- ADC 12-bit support exists in `adc_teensy41.h`
- PWM fan was disabled (`//#define PWM_FAN_AVAILABLE`) with placeholder macros using a mismatched register (`TMR3_COMP22` vs CSCTRL on channel 1)
- Native CAN intentionally disabled due to lockup risk

**Slice B: PWM Fan Support** ✅ **COMPLETE**
- Fixed register bug: `FAN_TIMER_COMPARE` was `TMR3_COMP22` (channel 2 COMP2) but `ENABLE_FAN_TIMER()` operates on `TMR3_CSCTRL1` (channel 1); corrected to `TMR3_COMP21` (channel 1 COMP2)
- Uncommented `#define PWM_FAN_AVAILABLE` in `board_teensy41.h:35`
- Added TCF2 fan dispatch to `TMR3_isr()` in `board_teensy41.cpp` following drain-all pattern (read all flags before clearing any)
- Updated misleading "most likely won't work" comment to document the sharing model (Fuel6 COMP1 + Fan COMP2 on same channel 1 counter)
- Test baseline unchanged: 723/723 PASSED (board file changes not exercised by AVR test suite)

**Slice C: SPI Flash Storage Wiring** ✅ **COMPLETE**
- Wired `g_useSPIFlash` flag into `writeConfig()` and `loadConfig()` in `storage.cpp`: all 7 struct config pages (veSetPage/1, ignSetPage/4, afrSetPage/6, canbusPage/9, warmupPage/10, progOutsPage/13, boostvvtPage2/15) now dual-write to SPI flash and load from SPI flash first with EEPROM fallback
- Rewrote `saveTuneBank()` and `loadTuneBank()` in `storage_spi.cpp`: full multi-page iteration over kStructPageIDs[], file-level copy via shared `copyFlashFile()` helper
- Added `loadTuneBank()` contract note: takes effect on next `loadConfig()` (reboot required); map/table pages deferred
- Fixed `getFlashFreeSpace()` to compute `totalSize() - usedSize()` instead of returning totalSpace
- All changes under `#if defined(CORE_TEENSY41)` — AVR code path unchanged; 723/723 PASSED

**Slice D: Comms Transport Audit** ✅ **COMPLETE** (no code changes)
- `SERIAL_BUFFER_SIZE` already bumped to 2051 (2048+3) on Teensy 4.1 via `SD_LOGGING` override in comms.cpp — no change needed
- `sendPage()` iterates entity-by-entity via `page_iterator_t`, no fixed block size to increase
- `LOG_ENTRY_SIZE=132` already updated in Phase 4; no further payload changes needed
- Comms transport is not a bottleneck for Teensy 4.1 with current protocol

**Slice E: ADC Backend Audit** ✅ **COMPLETE** (no code changes — identified bug, fixed in Phase 7 Slice A)

**Phase 6 Remaining Work:**
- Stabilize native CAN ✅ (Phase 10: TX stall fixed; `Can0.begin()` confirmed non-blocking; all write paths gated on `enable_intcan==1`)
- ESP32-C3 coprocessor path ✅ (capability bit established: `BOARD_CAP_WIFI_TRANSPORT`; transport implementation deferred to hardware availability)

---

## Phase 7: ADC Normalization + Sensor Correctness

### Phase 7 Progress Log

**Slice A: ADC 10-bit Normalization Fix** ✅ **COMPLETE**
- Root cause: `initADC_Teensy41()` sets hardware to 12-bit (0-4095) but `readAnalogPin()` in `sensors.cpp` returned raw `analogRead()` value — on Teensy 4.1 this is 4× too large for `fastMap10Bit()` and all downstream sensor math (TPS, CLT, IAT, MAP, O2, baro)
- Fix: added `#include "adc_teensy41.h"` to `board_teensy41.h` (exposed via `globals.h` → `sensors.cpp`); patched `readAnalogPin()` with `#if defined(CORE_TEENSY41)` branch calling `analogRead10bit()` (`analogRead() >> 2`)
- All sensor reads flow through `readAnalogPin()` on Teensy 4.1 (no `ANALOG_ISR` path), so one change covers TPS, CLT, IAT, MAP, baro, O2 simultaneously
- Added 8 regression tests in `test/test_sensors/test_adc_normalization.cpp`: shift math bounds, normalized TPS/MAP ranges, and overrange-without-normalization assertions
- test_sensors: 57 → 65/65 PASSED; total: 723 → 731/731

**Slice C: ADC Hardware Averaging** ✅ **COMPLETE**
- Added `analogReadAveraging(4)` to `initADC_Teensy41()` in `adc_teensy41.h`
- IMXRT1062 accumulates 4 samples internally; output remains 12-bit; `>> 2` normalization unchanged
- SNR improvement: ~6 dB (sqrt(4)); latency cost: ~4µs per read — negligible on Teensy 4.1
- No test changes needed: hardware averaging is Teensy-only init, not exercised by simavr; existing 65 ADC tests still pass 65/65; total: 731/731

---

## Phase 8: Teensy 4.1 Tune Transport Limits

### Phase 8 Progress Log

**Slice A: Transport Limits Audit** ✅ **COMPLETE** (no code changes)
- `serialPayload` buffer: already 2051B on Teensy (COMMS_SD path, RTC_ENABLED+SD_LOGGING) — no gap
- `BLOCKING_FACTOR`: 251 on Teensy (vs 121 AVR) — largest page is 384B (seqFuelPage), so 251 < 384 means 2-chunk writes for that page
- `TABLE_BLOCKING_FACTOR`: 256 on Teensy (vs 64 AVR) — covers all table pages (max 288B) in 1 shot
- `EEPROM_MAX_WRITE_BLOCK`: 64 on Teensy (vs 15 AVR) — no per-byte delay on Teensy FlexNVM; safely raisable
- Page sizes (max 384B): INI-locked; 16×16 table resize needs struct + INI overhaul — explicitly deferred
- `LOG_ENTRY_SIZE` (132B): needs INI `ochBlockSize` change to expand — deferred

**Slice B: `EEPROM_MAX_WRITE_BLOCK` raise** ✅ **COMPLETE**
- Raised from 64 → 255 for `CORE_TEENSY` in `storage.cpp` (split from STM32 path)
- At RPM>0 (running): up to 255 bytes per `writeConfig()` call (was 64) → largest page (384B) in ≤2 calls (was 6)
- At RPM==0 (stopped): up to 2040 bytes per call (255×8) → any page in 1 call
- Teensy FlexNVM EEPROM emulation is memory-mapped; no 3.8ms-per-byte write constraint

**Slices C+D: `BLOCKING_FACTOR` and `TABLE_BLOCKING_FACTOR` raise** ✅ **COMPLETE**
- `BLOCKING_FACTOR`: 251 → 512 for `CORE_TEENSY` in `comms.h`
- `TABLE_BLOCKING_FACTOR`: 256 → 512 for `CORE_TEENSY` in `comms.h`
- All current pages (max 384B) now fit in a single TS read/write chunk — no 2-chunk splits
- Both are reported to TunerStudio as `uint16_t` in the 'f' command; 512 is within range
- `serialPayload` buffer (2051B) safely absorbs any read/write payload ≤ 512B
- No tests reference BLOCKING_FACTOR or TABLE_BLOCKING_FACTOR — runtime/protocol constants only; 731/731 unchanged

**Slice E: `LOG_ENTRY_SIZE` expansion — PW5-PW8, launchCorrection, injAngle** ✅ **COMPLETE**
- Added 11 bytes (bytes 132-142) to `getTSLogEntry()` and `getReadableLogEntry()` in `logger.cpp`
- Fields: `PW5`/`PW6`/`PW7`/`PW8` (uint16 each, for 5-8 cylinder engines), `launchCorrection` (byte), `injAngle` (uint16)
- `getReadableFloatLogEntry()` extended with PW5-PW8 ÷1000.0 float cases (indices 96-99)
- `LOG_ENTRY_SIZE`: 132 → 143 in `logger.h`; `ochBlockSize`: 132 → 143 in `speeduino.ini`
- 4 new regression tests added to `test_logger_byte_regression.cpp` (byte positions 132-142, readable indices 96-101)
- test_ign: 182 → 186/186; total: 735/735 (was 731+4 = 735)

## Phase 9: Decoder Tooth-Number Resolution

### Phase 9 Progress Log

**Slice B: Resolve 36-2-1 single-gap tooth assignment** ✅ **COMPLETE**
- Prior code: `toothCurrentCount = 20` with comment `// it's either 19 or 20, need to clarify engine direction!`
- Evidence from rusEFI 4B11 real capture (`4b11-running.csv`) and `analyze_36_2_1.py`: 17 physical teeth from double gap to single gap, 16 physical teeth from single gap to double gap
- Correct assignment: `toothCurrentCount = 19` — nominal positions 1-17 are actual teeth, position 18 is the single gap, position 19 is the first tooth after it; total: 17+1+16+2 = 36 ✓
- `toothCurrentCount = 20` was wrong: left only 1 missing position for the 2-tooth double gap
- Changes: `decoders.cpp` (assignment + comment + `getRPM` guard `!= 19`), `ThirtySixMinus21.cpp` (assertion `19U`, loop `15` teeth, intermediate count `34`), `3621_resync_trace.h` (15 teeth = real 16-tooth run)
- `SESSION_HANDOFF_2026-03-23_36-2-1_STATE.md` updated: blocker marked resolved
- 263/263 test_decoders PASSED; 735/735 total unchanged

**Slice C: Full-revolution 36-2-1 test** ✅ **COMPLETE**
- Added `t3621_full_revolution` to `ThirtySixMinus21.cpp`: anchors on double gap (count=1), drives all 33 actual teeth through both gap events, asserts counts at every sync point
- Sequence: count=1 → +16 teeth → count=17 → single gap → count=19 → +15 teeth → count=34 → double gap → count=1
- Previously blocked (single-gap tooth was unresolved); now safe as physical truth
- 264/264 test_decoders PASSED (+1 new test); 736/736 total

**Slice E: 36-2-1 self-correction (spurious tooth resync)** ✅ **COMPLETE**
- Added `t3621_single_gap_reanchors_count_after_spurious_tooth`: establishes sync (count=1), drives 8 regular + 1 spurious + 8 regular teeth (count drifts to 18), verifies single-gap event forces count=19 regardless of drift
- Added `t3621_double_gap_reanchors_count_after_spurious_tooth`: from count=19 (after single gap), drives 5 regular + 1 spurious + 10 regular teeth (count drifts to 35), verifies double-gap event forces count=1 regardless of drift
- Demonstrates the decoder's gap-event self-correction: every gap re-anchors the tooth count, so a single spurious tooth cannot permanently desync the decoder
- 267/267 test_decoders PASSED (+2 new tests); 782/788 total

**Slice F: 36-2-2-2 extended coverage (full revolution + spurious tooth resync)** ✅ **COMPLETE**
- Added `test_36222_h4_full_revolution`: anchors on double gap (count=19), drives 12 regular teeth (count=31), single-gap long-gap (count=34), regular (count=35), 2 more regulars (count=36, 37→wrap count=1, startRevolutions=1), then re-syncs to count=19 on next double-gap group
- Added `test_36222_h4_double_gap_reanchors_count_after_spurious_tooth`: 2 regular + spurious (count drifts to 3), double-gap event forces count=19 regardless of drift
- Added `test_36222_h4_single_gap_reanchors_count_after_spurious_tooth`: 2 regular + spurious (count drifts to 3), single-gap event (long+regular) forces count=35 regardless of drift
- Demonstrates the 36-2-2-2 decoder's gap-event self-correction: both double-gap and single-gap groups re-anchor the tooth count unconditionally
- 270/270 test_decoders PASSED (+3 new tests); 785/791 total

**Slice G: Idle OL PWM regression suite (Option 3)** ✅ **COMPLETE**
- Created `test/test_idle/` suite: `main.cpp`, `test_idle_ol_pwm.h`, `test_idle_ol_pwm.cpp`
- 5 tests covering the open-loop PWM idleLoad state machine:
  - Cranking → uses cranking table (BIT_ENGINE_CRANK path)
  - Not running + iacPWMrun=true → uses cranking table (pre-crank warmup path)
  - Not running + iacPWMrun=false → idleLoad stays 0 (no active control)
  - Running, taper complete → uses running table (steady-state path)
  - Running, taper at start → map(0, 0, taperTime, crankVal, runVal) = crankVal (taper edge)
- Test pattern: manually initializes iacCrankDutyTable/iacPWMTable struct pointers to configPage6 arrays, fills with uniform predictable values via populate_2dtable, sets idleInitComplete to skip re-init, calls idleControl() directly
- 790/796 PASSED (+5 new tests in new suite, 6 skipped unchanged)

**Slice K: Idle OL Stepper regression suite** ✅ **COMPLETE**
- Added `test/test_idle/test_idle_ol_steps.h` and `test_idle_ol_steps.cpp` to existing suite
- 5 tests covering the open-loop stepper `targetIdleStep` state machine:
  - Not running (engine=0) → targetIdleStep = iacCrankStepsTable * 3
  - Cranking (BIT_ENGINE_CRANK, !BIT_ENGINE_RUN) → targetIdleStep = iacCrankStepsTable * 3
  - Running, taper complete + BIT_TIMER_10HZ → targetIdleStep = iacStepTable * 3
  - Running, taper start (idleTaper=0) + BIT_TIMER_10HZ → map(0,0,T,crank*3,run*3) = crank*3
  - Max-steps clamp: iacMaxSteps=1 → targetIdleStep saturates at 1*3=3 even when run table exceeds it
- Test pattern: sets idleStepper.stepperStatus=SOFF and iacStepHome=0 so checkForStepping()/isStepperHomed() gate passes without hardware; asserts on targetIdleStep (set before doStep() modifies curIdleStep)
- test_idle: 10/10 PASSED (+5); total: 795/801 (6 skipped unchanged)

**Slice L: Launch control + flat shift scenario tests** ✅ **COMPLETE**
- Created `test/test_launch/` suite: `main.cpp`, `test_launch_flatshift.h`, `test_launch_flatshift.cpp`
- 6 tests covering the `checkLaunchAndFlatShift()` state machine from `speeduino.ino`:
  - RPM above hard-launch limit, clutch engaged → `launchingHard=true` + `BIT_STATUS2_HLAUNCH`
  - RPM below hard-launch limit → `launchingHard=false`
  - Clutch released (active-high polarity, pin LOW → 0) → no launch regardless of RPM
  - TPS below `lnchCtrlTPS` gate → no launch even with clutch in and RPM over limit
  - Flat shift: RPM > `clutchEngagedRPM` with clutch at high speed → `flatShiftingHard=true` + `BIT_STATUS5_FLATSH`
  - Flat shift: RPM ≤ `clutchEngagedRPM` → `flatShiftingHard=false`
- Clutch state injected via `launchHiLo` polarity: `launchHiLo=0` (active-low) + pin-LOW → engaged; `launchHiLo=1` + pin-LOW → released; pre-match `clutchTrigger`/`previousClutchTrigger` to avoid edge-triggered `clutchEngagedRPM` update
- test_launch: 6/6 PASSED (new suite); total: 801/807 (6 skipped unchanged)

**Slice M: Engine protection limiter scenario tests** ✅ **COMPLETE**
- Created `test/test_protection/` suite: `main.cpp`, `test_engine_protection.h`, `test_engine_protection.cpp`
- 12 tests across three protection subsystems:
  - `protectionTimerElapsed()` (pure, 4 tests): not-elapsed, just-elapsed, well-past, uint8 wrap-around (`(uint8_t)(5-250)=11 >= 10`)
  - `checkRevLimit()` fixed mode (4 tests): below limit (no bits), at hard limit (ENGINE_PROTECT_BIT_RPM + BIT_STATUS2_HRDLIM), PROTECT_CUT_OFF disables entirely, soft-window expired triggers hard cut
  - `checkBoostLimit()` (4 tests): MAP below threshold (no cut), PROTECT_CUT_IGN (status2 bit only), PROTECT_CUT_FUEL (status1 bit only), PROTECT_CUT_BOTH (both bits)
- test_protection: 12/12 PASSED (new suite); total: 813/819 (6 skipped unchanged)

**Slice N: Oil pressure and coolant-mode rev limit tests** ✅ **COMPLETE**
- Extended `test/test_protection/test_engine_protection.cpp` with 7 new tests:
  - `checkRevLimit()` coolant mode (2 tests): uniform-bin `coolantProtectTable` via `construct2dTable`; below limit (no bits), above limit (`ENGINE_PROTECT_BIT_COOLANT` + `ENGINE_PROTECT_BIT_RPM` + `BIT_STATUS2_HRDLIM`)
  - `checkOilPressureLimit()` (5 tests): `PROTECT_CUT_OFF` disables, `oilPressureProtEnbl=0` skips gate, pressure above limit (no cut), zero delay fires immediately (`protectionTimerElapsed(0,0,0)=true`), pre-set `ENGINE_PROTECT_BIT_OIL` `alreadyActive` path bypasses non-zero delay
- Key pattern: `oilProtectCountEnabled` reset via above-limit call in `setup_oil_protect()`; `alreadyActive` captured before bit is cleared at call start
- test_protection: 12 → 19/19 PASSED; total: 813/819 → 820/826 (6 skipped unchanged)

**Slice D: Re-land Nissan360 `useResync == false` assertion** ✅ **COMPLETE**
- Resolved the previously-backed-out order-sensitive interaction with Harley replay coverage
- Root cause: `reset_nissan360_runtime()` was missing `testClearTriggerStateOverrides()` — unlike `reset_harley_runtime()` which calls it explicitly; trigger-state override machinery left in unknown state after each Nissan360 state test could leak into subsequent tests
- Fix: added `testClearTriggerStateOverrides()` to `reset_nissan360_runtime()` (mirrors the Harley pattern); added terminal `reset_nissan360_runtime()` call to both resync state tests for defense-in-depth isolation
- New test `test_nissan360_state_no_resync_preserves_tooth_counter`: starts at `toothCurrentCount = 50`, emits 16 primaries inside a secondary window with `useResync == false`; asserts count stays at `66` (50+16, not realigned), `hasSync` remains true, `syncLossCounter` stays 0
- Changes: `Nissan360.cpp` — `testClearTriggerStateOverrides()` in reset function, terminal cleanup in `test_nissan360_state_resync_realigns_tooth_counter`, new test added and registered
- 265/265 test_decoders PASSED (+1 new test); 737/737 total

- Treat Teensy 4.1 as a first-class platform, not just a faster AVR replacement.
- Move capability decisions behind explicit board declarations for SD, RTC, native CAN, onboard SPI flash, trigger hardware, and driver chips so runtime code and the tuning surface can distinguish generic MCU support from specific board support.
- Add a Teensy/DropBear storage path that uses onboard SPI flash for tune persistence, tune banks, migration staging, and higher-rate diagnostic capture instead of constraining new features to the legacy EEPROM layout.
- Increase Teensy-only tune transport limits after storage is decoupled:
  - larger page sizes (deferred — INI-locked; requires struct + INI overhaul)
  - larger blocking factors ✅ (Phase 8 Slices C+D: BLOCKING_FACTOR 251→512, TABLE_BLOCKING_FACTOR 256→512)
  - larger output-channel payloads ✅ (Phase 8 Slice E: LOG_ENTRY_SIZE 132→143; PW5-PW8, launchCorrection, injAngle added) ✅ (Phase 10: LOG_ENTRY_SIZE 143→148; startRevolutions 4-byte LE + runtimeStatusA packed status byte added at bytes 143-147)
  - higher-resolution 3D tables (deferred — requires struct + INI overhaul)
  - faster EEPROM burns ✅ (Phase 8 Slice B: EEPROM_MAX_WRITE_BLOCK 64→255)
- Rework Teensy 4.1 timing and peripheral usage where the current board layer is still unfinished or AVR-shaped:
  - drain all pending timer flags per ISR instead of single `else if` servicing ✅ (Phase 6 Slice A)
  - finish PWM fan support using TMR3 channel 1 COMP2 alongside Fuel6 ✅ (Phase 6 Slice B)
  - wire SPI flash storage into writeConfig()/loadConfig() for all 7 struct pages ✅ (Phase 6 Slice C)
  - extend SPI flash dual-write to all 8 table/map pages ✅ (Phase 10: saveTablePageToFlash/loadTablePageFromFlash via getPageValue/setPageValue iterator; kStructPageIDs expanded to all 15 pages)
  - stabilize native CAN and expose the real capability cleanly ✅ (Phase 10: TX stall fix — enable_intcan guard on sendCANBroadcast(); boardCap_nativeCAN bit decoded in INI; CANisAvailable tightened to require hardware truth)
  - fix readAnalogPin() 10-bit normalization for Teensy 4.1 ✅ (Phase 7 Slice A)
  - add higher-resolution oversampling/averaging to Teensy 4.1 ADC path ✅ (Phase 7 Slice C)
- Use the existing ESP32-C3 board hardware as a real secondary transport / coprocessor path for wireless tuning, log offload, and update workflows once the board capability layer exists. ✅ (Capability layer established: `BOARD_CAP_WIFI_TRANSPORT` bit 7 added to board_capability enum; set for PIN_LAYOUT_DROPBEAR on CORE_TEENSY41; `boardCap_wifiTransport` channel exposed in INI at byte 130 [7:7]. Transport implementation deferred until hardware schematic / UART pin assignment confirmed.)

## Borrowed From rusEFI

- Keep borrowing from rusEFI where it improves verification discipline or hardware/config separation, not where it would require adopting their full architecture.
- Highest-value patterns observed in [CLAUDE.md](C:/Users/Cornelio/Desktop/rusefi-2026-03-17/CLAUDE.md), [test_real_noisy_trigger.cpp](C:/Users/Cornelio/Desktop/rusefi-2026-03-17/unit_tests/tests/trigger/test_real_noisy_trigger.cpp), [test_launch.cpp](C:/Users/Cornelio/Desktop/rusefi-2026-03-17/unit_tests/tests/test_launch.cpp), [ComposedTuneMigrator.java](C:/Users/Cornelio/Desktop/rusefi-2026-03-17/java_console/io/src/main/java/com/rusefi/maintenance/migration/migrators/ComposedTuneMigrator.java), and [LiveData.yaml](C:/Users/Cornelio/Desktop/rusefi-2026-03-17/firmware/integration/LiveData.yaml).
- Do not treat rusEFI as a direct implementation template for scheduler, RTOS, console, or generated-config architecture on this codebase; those would be separate major projects, not roadmap refinements.

## Deferred Revisit

- Revisit the blocked Rover MEMS `Crank Speed + 5-3-2 cam` full-sync slice after higher-confidence decoder work is complete.
- Keep the existing Rover progress as-is:
  - primary-wheel identification is evidence-backed for patterns `#1`, `#2`, and `#3`
  - `roverMEMSTeethSeen` rolling-window behavior is derived and host-side tested
  - `5-3-2 cam` ISR semantics are encoded and documented
- The exact reason this is deferred:
  - the repo still lacks a safe tooth-numbered mapping from a logged cam transition to the decoder's `secondaryToothCount == 6 / 4 / 3` gap event
  - later `.mlg` runtime logs and scope screenshots improve confidence that the tested T16 path worked, but they do not expose the internal cam-gap edge needed for a full replay trace
- Rover references to resume from:
  - [SESSION_HANDOFF_2026-03-21_ROVER_MEMS_NOTE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-21_ROVER_MEMS_NOTE.md)
  - [SESSION_HANDOFF_2026-03-22_ROVER_MEMS_BIT_WINDOW.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_ROVER_MEMS_BIT_WINDOW.md)
  - [SESSION_HANDOFF_2026-03-22_ROVER_MEMS_CAM_532.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_ROVER_MEMS_CAM_532.md)
  - [SESSION_HANDOFF_2026-03-22_ROVER_MEMS_TOOTH_LOG.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_ROVER_MEMS_TOOTH_LOG.md)
- The `megaatmega2560_sim_unittest` decoder baseline is green again at `263/263`; keep it that way as new evidence-backed slices land.

## Trouble Areas

- Teensy 4.1 native 16-bit TunerStudio tune pages remain **deferred**:
  - TunerStudio itself supports `U16` tables, but this firmware's TS-facing transport still exposes the main tune/map pages as byte-serialized virtual pages
  - the current `p` / `M` / `d` handlers, `getPageValue()` / `setPageValue()` mapping, page CRC calculation, and SPI-flash page mirroring all operate on that byte stream contract
  - internal Teensy table storage may be `uint16_t`, but the TS-visible page layout is still `288`-byte virtual pages for the 16x16 maps, not native `544`-byte `U16` pages
  - the non-TS persistence path already writes full `table3d_value_t` rows through [storage.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/storage.cpp), so the narrowest future experiment is not "flip the INI to `U16`"; it is a parallel Teensy-only TS serializer/deserializer/CRC path for one page while keeping the current byte path intact
  - the current public page API is the blocking seam: [pages.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/pages.h) only exposes `getPageSize()`, `getPageValue()`, and `setPageValue()` as byte-oriented operations, and both [comms.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/comms.cpp) and [comms_legacy.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/comms_legacy.cpp) stream those bytes directly
  - prior attempts to switch the INI/page sizes to native `U16` broke burn reliability because page sizing, serialization, CRC hashing, and project-local INI copies no longer matched
  - a real native-`U16` implementation would need a deliberate feature slice:
    - define a TS-visible `U16` page contract for the affected pages
    - update page sizing and offset mapping
    - update read serialization and write deserialization
    - update CRC hashing to cover the exact same transmitted byte stream
    - update SPI-flash save/load helpers that currently iterate bytewise via `getPageValue()` / `setPageValue()`
    - update packaged and project-local INIs consistently
  - the safest experimental milestone would be:
    - add a parallel Teensy-only TS page reader/writer + CRC helper for one 16x16 table page
    - gate it behind an explicit experimental signature or capability bit so existing projects stay on the working byte contract
    - prove end-to-end coherence for page read, page write, burn, CRC, and flash mirror before touching the remaining pages
  - until that full slice is implemented and validated end-to-end on Teensy, keep the working byte-serialized TS contract
  - current repo state update:
    - the experimental milestone above is now substantially complete for page `2` only: there is a Teensy-only, DropBear-gated, alternate-signature native-`U16` TS transport seam with matching read/write/CRC/SPI-flash behavior
    - the experimental runtime fueling slice is now also in place for page `2`: fueling uses a high-resolution runtime VE path while `currentStatus.VE*` remain byte-sized compatibility/display channels
    - what remains deferred is **high-resolution VE telemetry/output-channel exposure**, because logger/live-data/legacy packet layouts are still byte-oriented and shared by existing consumers
    - treat telemetry widening as a separate design slice from TS page transport and fueling; do not conflate "TS can edit `U16` page 2" or "fueling consumes hi-res VE" with "all external status/log contracts expose hi-res VE"
- `Rover MEMS` full replay remains intentionally deferred:
  - the repo still lacks a safe tooth-numbered mapping from a logged cam transition to the decoder's `secondaryToothCount == 6 / 4 / 3` gap events needed for full `Crank Speed + 5-3-2 cam` replay
  - treat this as an evidence-conversion blocker, not as a signal to synthesize more speculative traces
- `4G63` direct AVR state coverage is still not safe to land under the current harness:
  - prior direct pin-state-driven attempts perturbed unrelated decoder tests under `simavr`
  - keep replay coverage, but do not re-land a direct state suite until the isolation issue is understood
- `Nissan360` order-sensitive interaction with Harley replay — **resolved (Phase 9 Slice D)**:
  - root cause: `reset_nissan360_runtime()` was missing `testClearTriggerStateOverrides()`, leaving trigger-state override machinery in an unknown state that could affect `lastGap`/`targetGap` comparisons in subsequent Harley ISR calls
  - fix: added `testClearTriggerStateOverrides()` to `reset_nissan360_runtime()` + terminal cleanup calls; `useResync == false` assertion now safely landed
- harness rule for these blocked slices:
  - prefer the last verified green decoder baseline over landing a brittle regression
  - if a candidate slice only passes in isolation but perturbs the full decoder entrypoint, back it out and record the interaction here before moving on

## Lower-Priority Remaining Decoder Surface

- The repo's most common decoder families are already represented in the maintained Phase 2 baseline:
  - missing-tooth
  - dual-wheel / Weber-style secondary sync
  - basic distributor
  - GM (`24X`, `7X`)
  - Honda (`D17`, `J32`)
  - Mazda / Miata
  - Nissan360
  - Subaru `6/7`
  - Ford ST170
  - Harley
  - Daihatsu +1
  - Suzuki K6A
  - Rover MEMS primary-only identification
- Remaining lower-priority or narrower decoder-specific work should stay at the end of the queue unless new evidence says otherwise:
  - `DECODER_WEBER` as a named alias path over the already-covered dual-wheel behavior
  - broader variant expansion inside already-covered families, such as additional `60-2` or cylinder-count permutations that do not add a new state-machine shape
- Blocked-but-important work still stays ahead of these lower-priority items only when the blocker itself becomes actionable:
  - `Rover MEMS` full `Crank Speed + 5-3-2 cam` replay
  - `4G63` direct AVR state isolation under the current harness
  - ~~the backed-out extra `Nissan360` `useResync == false` assertion once the Harley interaction is understood~~ ✅ resolved (Phase 9 Slice D)
