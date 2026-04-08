# Firmware Roadmap

## Phase 1: Safety And Correctness

- Fix high-risk ignition, fueling, and protection defects before broader refactors.
- Prioritize knock handling, limiter/protection interactions, and config/runtime mismatches.
- Replace placeholder telemetry where it reports misleading runtime data.
- Add focused regression coverage for corrected logic where unit-test hooks already exist.

Current phase 1 work started in:
- [corrections.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/corrections.cpp)

## Phase 2: Regression Harness

- Add unit tests around ignition corrections, engine protection, sensor filtering, and config migrations. ✅
- Use existing `UNIT_TEST` hooks to expose pure or near-pure helpers without changing runtime behavior. ✅
- Build migration tests for [updates.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/updates.cpp). ✅
- Add decoder state-transition tests for the most common trigger patterns in [decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp). ✅
- Add host-side trigger tests that replay real captured crank/cam traces, not just synthetic edge sequences. ✅
- Create a small library of noisy and borderline trigger recordings to regression-test sync, dwell scheduling, and false-trigger handling. ✅
- Add scenario tests for launch, flat shift, hard/soft limiters, and protection reactivation behavior as named state-machine tests instead of only function-local checks. ✅
- Keep using the forum/manual evidence pipeline as a primary source for blocked decoder work:
  - prefer canonical topic-level extraction from [Resources/speeduino_evidence_collector_stable.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/speeduino_evidence_collector_stable.py), not raw search-engine dumps
  - prioritize decoder families with surviving high-confidence forum evidence after topic dedupe and explicit family matching
  - the earlier `36-2-1` tooth-number blocker is now resolved from real-capture evidence; the remaining explicitly deferred high-value decoder blocker is the Rover MEMS cam-phase slice at the end of the roadmap
  - use external decoder references as topology and sync-behavior evidence, not as drop-in waveform sources:
    - rusEFI real-trigger tests in [Resources/rusefi-2026-03-17/unit_tests/tests/trigger/test_real_4b11.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/unit_tests/tests/trigger/test_real_4b11.cpp) are the best current external model for `36-2-1` work because they validate against real CSV captures
    - rusEFI decoder implementations in [Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_honda.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_honda.cpp) and [Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_rover.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_rover.cpp) are useful for `Honda J32` and Rover-family topology review
    - MS3 Rover references in [Resources/ms3-source-master/ms3/core.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master/ms3/core.ini), [Resources/ms3-source-master/ms3/ms3_ign_in.c](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master/ms3/ms3_ign_in.c), and [Resources/ms3-source-master/ms3/ms3_ign_wheel.c](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master/ms3/ms3_ign_wheel.c) are the best current external clues for Rover phase and `poll_level_tooth` semantics
  - before adding new replay traces for `Rover MEMS` or `36-2-1`, first convert the surviving external/forum evidence into an explicit tooth/gap or poll-phase note instead of inferring directly from comments
  - the `36-2-1` physical gap model remains documented in [SESSION_HANDOFF_2026-03-22_36-2-1.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_36-2-1.md), backed by [analyze_36_2_1.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_36_2_1.py), while the maintained current-code ISR contract is now formalized in [SESSION_HANDOFF_2026-03-23_36-2-1_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_36-2-1_STATE.md) and covered directly in [ThirtySixMinus21.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/ThirtySixMinus21/ThirtySixMinus21.cpp); the formerly unresolved single-gap tooth-number assignment is now locked in-tree by the Phase 9 state and replay updates
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
  - the Rover evidence-conversion backlog is now consolidated in [SESSION_HANDOFF_2026-03-28_ROVER_MEMS_FULLSYNC_BLOCKER.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-28_ROVER_MEMS_FULLSYNC_BLOCKER.md), while the maintained primary-only decoder contract remains separated in [SESSION_HANDOFF_2026-03-23_ROVER_MEMS_PRIMARY_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_ROVER_MEMS_PRIMARY_STATE.md). Rover attachments are staged locally in [Resources/rover_mems_evidence](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence), the composite-log CSV channel mapping is solved with [tools/parse_speeduino_composite_csv.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/parse_speeduino_composite_csv.py), the Rover PDF image corpus is extracted with [tools/extract_pdf_images.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/extract_pdf_images.py), the extracted images are OCR-indexed with [tools/index_pdf_images.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/index_pdf_images.py), PDF readiness is checked with [tools/inspect_pdf_evidence.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/inspect_pdf_evidence.py), full Rover pages are rendered with [tools/render_pdf_pages.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/render_pdf_pages.py), the manuals now map OEM Rover layouts onto Speeduino's `11-5-12-4` and `2-14-3-13` patterns, the rolling `roverMEMSTeethSeen` bit window is derived in [derive_rover_mems_windows.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/derive_rover_mems_windows.py), and a real June 25 post-fix tooth logger host-side-proves Rover pattern `#1` via [analyze_rover_mems_tooth_log.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_rover_mems_tooth_log.py); the remaining blocker is still the exact `5-3-2 cam` phase alignment needed for full-sync Rover replay rather than primary-wheel identification itself

### Current Verified Baseline

Most of the phase snapshots below are historical closing baselines. The current spot-checked simulator baseline in this workspace is:

- test_decoders: 275/275 PASSED
- test_fuel: 88/88 PASSED
- test_ign: 193/193 PASSED
- test_init: 9/9 PASSED
- test_launch: 6/6 PASSED
- test_math: 44/44 PASSED
- test_protection: 19/19 PASSED
- test_schedules: 26/26 PASSED
- test_sensors: 75/75 PASSED
- test_tables: 24/24 PASSED
- test_updates: 38/38 PASSED
- test_comms: 46/46 PASSED (10 skipped — Teensy41/U16P2 guards correct on AVR harness)

`test_updates` and `test_ign` did briefly hit AVR LTO/internal build-cache failures during incremental rebuilds, and a broader sweep also hit transient missing-object / file-lock failures when multiple PlatformIO test jobs targeted the same environment concurrently. The same suites passed cleanly on rerun after a rebuild/clean, so treat those as toolchain/build-cache instability notes, not as active source-level regressions.

Separately, the GitHub Actions `Calculate memory deltas` Arduino Teensy 4.1 compile path is back to passing after removing a `globals.h` declaration-order dependency from [board_teensy41.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/board_teensy41.h). The current local Arduino Teensy 4.1 repro only emits the external `SdFat.h` FS-detection `#warning`; there are no active `updates.cpp` warning annotations reproducing in this workspace, and there is no compile blocker.

### Firmware Capability API Sprint — Complete ✅

All seven items from [docs/implementation_backlog_firmware.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/docs/implementation_backlog_firmware.md) are done:

| Item | Summary | Files |
|------|---------|-------|
| FW-001 ✅ | Release manifest generator | `tools/generate_release_manifest.py`, `release/release_manifest.json` |
| FW-002 ✅ | Release artifact classification | `release/README.md`, manifest |
| FW-003 ✅ | Capability query `'K'` command | `comms_legacy.cpp/.h` — 39-byte binary response |
| FW-004 ✅ | Stable `BoardId` enum | `globals.h/.cpp` — `getStableBoardId()`, `BOARD_ID_DROPBEAR_T41=55` |
| FW-005 ✅ | External page verification | `page_crc.cpp`, protocol docs in `docs/page_verification_protocol.md` |
| FW-006 ✅ | Live-data contract metadata | `live_data_map.h` `OCH_OFFSET_*` constants, `CAP_FEATURE_FLASH_HEALTH` |
| FW-007 ✅ | Experimental U16P2 feature flag | `CAP_FEATURE_U16P2` in capability response feature_flags byte |

The `test_comms` suite now covers all five new protocol items:
- `test_comms/test_capability_response.cpp` — FW-003/FW-004/FW-006/FW-007 (packet layout, BoardId mapping, OCH offsets, feature flags)
- `test_comms/test_page_verification.cpp` — FW-005 (parseLegacyCrcRequest, encodeLegacyCrc32Response, production and U16P2 CRC mode-awareness)

### Current Active Remaining Work

After the firmware capability API sprint, the remaining roadmap surface is intentionally narrow:

- hardware/bench validation for the experimental Teensy/DropBear native-`U16` page-2 path, not more transport debugging
- hardware/bench validation for the `runtimeStatusA` tune-learning validity bits and TunerStudio indicator behavior
- **ESP32-C3 Serial2 coprocessor transport** ✅ software complete (Phase 11): `Serial2.begin(115200)` in `beginBoardSerial()`, `serviceBoardSerial()` board hook wired into main loop; remaining work is hardware bench validation per Phase 11 Slice E
- **PID reset cleanup** ✅: `PID::Initialize()` promoted from private to public in `PID_v1.h`; `initialiseCorrections()` now calls `egoPID.Initialize()` + `egoPID.SetMode(AUTOMATIC)` directly instead of the triple mode-toggle workaround — same state reset, no behavior change, TODO removed; 193/193 test_ign PASSED
- **VSS noise filter** ✅: `vssPulse()` ISR now rejects pulses arriving within `VSS_FILTER_MIN_GAP_US` (500µs) of the previous accepted pulse — suppresses contact-bounce and electrical noise; `vssPulseIsFiltered()` extracted as `TESTABLE_INLINE_STATIC` pure helper; 10 regression tests added in `test_sensors/test_vss_filter.cpp`; test_sensors: 65→75/75 PASSED
- **`setPinHysteresis` atomic write** ✅: wrapped the GDIR read-modify-write in `noInterrupts()`/`interrupts()` — iMXRT1062 has no atomic SET/CLR alias for GDIR; brief critical section prevents potential race on shared GPIO port; Teensy 4.1 only, no AVR tests
- **Haltech CAN staging duty cycle** ✅: `CAN_HALTECH_DATA3` bytes 2-3 now report `currentStatus.PW2`-based duty cycle (same formula as PW1) when `BIT_STATUS4_STAGING_ACTIVE` is set; 0 when staging is inactive; resolves the TODO in `comms_CAN.cpp`
- **`getSpeed()` race condition fix** ✅: replaced three separate `vssGetPulseGap()` calls (each with its own `noInterrupts()` pair) with a single atomic snapshot of `vssTimes[]` + `vssIndex`; all gap calculations and the stale-pulse check now use the same consistent buffer view — a pulse arriving mid-loop can no longer mix pre- and post-update timestamps across gaps; also guards `pulseTime > 0 && vssPulsesPerKm > 0` to prevent divide-by-zero on misconfiguration
- evidence conversion for Rover MEMS `Crank Speed + 5-3-2 cam` full replay
- harness isolation work only if someone wants to re-attempt direct AVR state coverage for `4G63`
- optional future design work for high-resolution VE telemetry, if a real logging workflow proves it is worth a separate alternate-signature path
- larger structural refactors such as `IdleState`, deeper `init.cpp` splitting, and broader `setPinMapping()` table conversion remain explicitly deferred rather than active blockers

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

Additional completed board-layer cleanup since the original Phase 4 closeout:
- Shared PWM timing / phase decisions moved behind board hooks (`boardPwmTimerInvertsPhase()`, `boardPwmTimerTickMicros()`)
- Shared flash-backed storage / logger health export routed through board-flash helpers instead of repeated Teensy-only branches
- Shared analog input mode setup moved behind `boardAnalogInputMode()`

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
  1. HIGH priority: Extract DropBear Teensy 4.1 mapping (actual board ID `55`, not `60`) into board_teensy41.cpp data table/helper (~50 lines, low risk, 2-hour effort)
  2. MEDIUM priority: Convert case 3 platform variants into board_* lookups (moderate risk, 4–6 hours)
  3. LOW priority: Consolidate duplicate cases (40/41/42, 50/51) via shared tables (3–4 hours per group)
  4. FUTURE: Full data-driven pin table system (20–30 hours + multi-platform regression testing)
- Recommendation: Defer extraction to Phase 5+ or future maintenance window
- Current repo state update:
  - the highest-value low-risk extraction above has now started: the DropBear Teensy 4.1 mapping is moved behind [setTeensy41DropBearPinMapping()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/board_teensy41.cpp) and called from [setPinMapping()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/init.cpp)
  - this is intentionally behavior-preserving cleanup only; the generic setPinMapping switch still owns the remaining board cases

**Binary Size:**
- Teensy 4.1: 261,708 bytes code + 30,464 data (FLASH: 301,052 total in current Arduino Teensy 4.1 repro)
- RAM1: variables 40,928, code 256,600, padding 5,544 (free for local variables: 221,216)
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
- test_init: 9/9 PASSED (focused init-helper simulator slice)
- **Total passing: 718/718**

**Remaining Phase 4 Work (Future):**
- setPinMapping data-driven conversion beyond the extracted DropBear Teensy 4.1 case (documented, deferred)
- Abstract PWM polarity inversion into board-layer functions ✅ (`boardPwmTimerInvertsPhase()` now hides the countdown-timer PWM phase inversion from `idle.cpp` and `auxiliaries.cpp`)
- Platform-specific code moves into board_* layer (ongoing, incremental)
- Full SPI flash page serialization ✅ (Phase 10: saveTablePageToFlash/loadTablePageFromFlash added; all 8 table pages dual-write wired in storage.cpp; kStructPageIDs expanded to all 15 pages for complete tune bank capture)

Additional current note on remaining Phase 4 work:
- Shared PWM timer-count setup now also uses `boardPwmTimerTickMicros()` in `idle.cpp` and `auxiliaries.cpp`, and analog sensor pin setup now uses `boardAnalogInputMode()` in `init.cpp`
- Shared storage / page serialization / logger export cleanup is substantially complete; remaining Teensy / STM32 branches in those areas are mostly backend linkage or experimental feature guards rather than board-policy leakage
- Cross-build board-header consistency is improved: the Arduino Teensy 4.1 compile path no longer depends on `PIN_LAYOUT_DROPBEAR` being declared earlier via `globals.h`, because [board_teensy41.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/board_teensy41.h) now uses a local DropBear pin-mapping constant inside its board capability helper

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
- test_init: 9/9 PASSED (focused init-helper simulator slice)
- **Total passing: 732/732**

**Binary Size (Phase 5):**
- Historical closeout note: Phase 5 recorded Teensy 4.1 at 254,060 bytes code + 30,464 data at that point in time
- Current Arduino Teensy 4.1 repro in this workspace is 261,708 bytes code + 30,464 data

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
- No active software-only blockers remain from the original Phase 6 slice.
- Native CAN stabilization is complete in-tree.
- ESP32-C3 coprocessor transport remains hardware-availability work, not a current firmware blocker.

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
  - move primary serial startup behind a board hook instead of a `CORE_TEENSY41` branch in `initialiseAll()` [complete] (Phase 10: `beginBoardSerial()` keeps Teensy 4.1's shortened USB wait inside the board layer)
  - move post-trigger hysteresis setup behind a board hook instead of a `CORE_TEENSY41` branch in `initialiseTriggers()` [complete] (Phase 10: `finaliseBoardTriggerSetup()` keeps Teensy 4.1 interrupt-pin hysteresis in the board layer)
  - move Teensy 4.1 DropBear pin-mapping selection behind a board hook instead of calling a Teensy helper from `setPinMapping()` [complete] (Phase 10: `applyBoardPinMapping()` lets the board layer own the Teensy 4.1-specific DropBear mapping)
- Use the existing ESP32-C3 board hardware as a real secondary transport / coprocessor path for wireless tuning, log offload, and update workflows once the board capability layer exists. ✅ (Capability layer established: `BOARD_CAP_WIFI_TRANSPORT` bit 7 added to board_capability enum; set for PIN_LAYOUT_DROPBEAR on CORE_TEENSY41; `boardCap_wifiTransport` channel exposed in INI at byte 130 [7:7]. Transport implementation moved to Phase 11: schematic confirms Serial2 pins 7/8 at 115200 baud to Airbear ESP32-C3.)

- Improve tune-assist / autotune quality from the firmware side without assuming any changes to TunerStudio internals:
  - treat TunerStudio's built-in autotune algorithm as external / fixed; do not plan work that depends on modifying its private implementation
  - improve the quality of the data TunerStudio sees:
    - cleaner AFR / MAP / TPS / RPM inputs and filtering
    - clearer steady-state vs transient gating
    - more explicit warmup / ASE / accel / decel / DFCO / protection / sync-state signals
  - prefer firmware features that help both TunerStudio autotune and external offline analysis:
    - richer runtime status bits
    - clearer logger / live-data semantics
    - confidence or validity markers for when a cell hit is worth learning from
  - completed narrow slice: reused the existing `runtimeStatusA` byte high bits instead of widening `ochBlockSize`, adding `fullSync`, `transientActive`, `warmupOrASEActive`, and `tuneLearnValid` so TS/offline tools can gate learning on explicit firmware state without a packet-size or signature change
  - focused implementation and future bench-validation details for that tune-assist slice are kept in [SESSION_HANDOFF_2026-03-28_TUNE_LEARN_VALIDATION.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-28_TUNE_LEARN_VALIDATION.md)
  - if higher-resolution VE or tune-assist telemetry is explored later, keep it as an alternate-signature / alternate-INI path rather than mutating the shared production packet contract in place
  - the narrowest future slice here is not "replace TunerStudio autotune"; it is exposing better tune-quality evidence and log semantics so both TunerStudio and external tools can make better decisions

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
  - [SESSION_HANDOFF_2026-03-28_ROVER_MEMS_FULLSYNC_BLOCKER.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-28_ROVER_MEMS_FULLSYNC_BLOCKER.md)
  - [SESSION_HANDOFF_2026-03-23_ROVER_MEMS_PRIMARY_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_ROVER_MEMS_PRIMARY_STATE.md)
- The maintained `megaatmega2560_sim_unittest` decoder baseline now includes the later Phase 9 additions as well; keep it green as new evidence-backed slices land.

## Phase 11: ESP32-C3 Coprocessor Serial2 Transport ✅ **SOFTWARE COMPLETE**

### Prerequisites (all confirmed)

- Schematic (`V2.0.1/comms.kicad_sch`): Teensy 4.1 **Serial2 TX/RX = pins 7/8** → ESP32-C3 Supermini RX/TX at **115200 baud**
- `BOARD_CAP_WIFI_TRANSPORT` (bit 7) set in `board_capability` enum for `PIN_LAYOUT_DROPBEAR` on `CORE_TEENSY41` ✅
- `boardCap_wifiTransport` output channel exposed in INI at byte 130 bit 7 ✅
- Airbear firmware v0.1.2 confirmed: operates in TunerStudio TCP bridge mode on port 2000, speaks full Speeduino comms framing; identified by `BOARD_CAP_WIFI_TRANSPORT` bit when the Python tuner connects

### Slice A: Serial2 Initialisation ✅ **COMPLETE**

- `Serial2.begin(115200)` added to `beginBoardSerial()` in [board_teensy41.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/board_teensy41.cpp) (unconditional for Teensy 4.1; `pSecondarySerial = &Serial2` was already unconditional)
- No change to Serial0 (USB CDC) or Serial1; Serial2 is additive
- Verify via the existing minimal-serial diagnostic firmware (`diagnostics/minimal_teensy41_serial/minimal_teensy41_serial.ino`) before wiring the full comms path — it echoes every non-Q/S byte with `MINIMAL:BYTE:` prefix, making it safe to confirm the physical link without running live ECU code

### Slice B: Comms Dispatcher Routing ✅ **COMPLETE**

- Added `serviceBoardSerial()` board hook declared in [board_teensy41.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/board_teensy41.h), implemented in [board_teensy41.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/board_teensy41.cpp)
- Routing design: temporarily diverts `pPrimarySerial` to `&Serial2` while a command is active; all comms helpers (`serialReceive`, `serialTransmit`, `sendValues`, …) use `primarySerial = *pPrimarySerial`, so the full request/response cycle flows through Serial2 without duplicating protocol logic
- `serviceBoardSerial()` gates on `boardHasCapability(BOARD_CAP_WIFI_TRANSPORT)` — no-op on non-DropBear boards at runtime
- Called from [speeduino.ino](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/speeduino.ino) after the primary serial check under `#if defined(CORE_TEENSY) && defined(__IMXRT1062__)`
- USB CDC and Serial2 sessions share `serialStatusFlag` so they cannot overlap; whichever port is not active waits in its hardware buffer

### Slice C: `ochGetCommand` Alignment Note ✅ **COMPLETE** (documented in board_teensy41.cpp)

The custom INI defines:
```
ochGetCommand = "r\$tsCanId\x30%2o%2c"
```
This is the modern `'r'` command (subcommand `0x30`, 2-byte offset, 2-byte count). The firmware handles both `'A'` (legacy) and `'r'` (modern). Airbear firmware v0.1.2 sends `'A'`. Both paths return the same 148-byte payload, so there is no firmware change required here; documented in [board_teensy41.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/board_teensy41.cpp) that Airbear is expected to migrate to the `'r'` form to stay aligned with the INI contract.

### Slice D: `BLOCKING_FACTOR` Exposure ✅ **COMPLETE** (documented in board_teensy41.cpp)

After Phase 8 Slices C+D, `BLOCKING_FACTOR = 512` and `TABLE_BLOCKING_FACTOR = 512` for Teensy. The Python tuner's `TcpTransport` reads the blocking factors from the `'f'` command response and uses them for write chunking. The Airbear TCP proxy must relay the `'f'` command response verbatim without reinterpreting or capping the reported values. No firmware change needed; constraint documented in [board_teensy41.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/board_teensy41.cpp) in the `beginBoardSerial()` comment block.

### Slice E: Integration Test Path

Hardware-only validation — cannot be covered by the AVR simulator harness:

1. Flash `diagnostics/minimal_teensy41_serial/minimal_teensy41_serial.ino.hex` to Teensy 4.1
2. Connect via Airbear in TunerStudio TCP bridge mode on port 2000
3. Send `Q` from a TCP client — expect `speeduino 202501-T41` echo via Serial2 → TCP
4. Confirm `MINIMAL:ALIVE` appears at 1Hz — proves the physical link and baud rate
5. Flash production `speeduino-dropbear-v2.0.1-teensy41.hex`
6. Connect Python tuner via `TcpTransport("speeduino.local", 2000)` — expect full page read/write/burn round-trip
7. Verify `boardCap_wifiTransport` reads `1` in the Python tuner's capability display

### Slice F: Firmware Variant Handling

Two production signatures in the release bundle:
- `speeduino 202501-T41` — standard byte-mode INI/tune
- `speeduino 202501-T41-U16P2` — experimental native-U16 Page 2

Airbear detects the active variant via the `'Q'` command response at startup. The Serial2 transport is identical for both variants; the U16P2 page-2 difference is in the byte stream of `'p'` / `'M'` / `'d'` commands for page 2 only. Airbear proxies all page bytes verbatim — no reinterpretation — so both variants are handled correctly by the same Airbear firmware without changes.

### Phase 11 Test Baseline Target ✅ **UNCHANGED** (no new simulator tests — hardware-only paths)

No new simulator tests were added (Serial2 init and routing are hardware-only paths). The integration test record in Slice E above is the hardware validation artifact. Current workspace baseline (includes VSS filter and comms tests added post-Phase 11):

- test_decoders: 275/275
- test_fuel: 88/88
- test_ign: 193/193
- test_idle: 10/10
- test_init: 9/9
- test_launch: 6/6
- test_math: 44/44
- test_protection: 19/19
- test_schedules: 26/26
- test_sensors: 75/75 (was 65/65 at Phase 11 close; +10 from VSS noise filter regression suite)
- test_tables: 24/24
- test_updates: 38/38
- test_comms: 46/46 (10 skipped — Teensy41/U16P2 guards correct on AVR harness)

---

## Phase 12: U16 Maps Where Precision Matters (DropBear / Teensy)

**Status: planned.** This is a forward-looking phase committed to in concert with the desktop tuner project's Phase 14 native rewrite (see `D:/Documents/JetBrains/Python/Tuner/docs/tuning-roadmap.md`). The trigger is the desktop pivot to a native C++ Qt application: while we're rewriting the host-side parsers and generators anyway, it's the right time to widen the high-leverage 3D maps on the only board family that has the SRAM and flash headroom for it.

### Goal

Ship a single production firmware variant — `speeduino 202501-T41` — that exposes the **high-precision 3D fueling/ignition/boost maps as native U16 pages** on DropBear / Teensy 4.1, while leaving coarse-natural-step tables (warmup enrichment, ASE tapers, IAT retard, etc.) on the existing byte-serialized contract. The experimental `-U16P2` signature collapses into the default once the targeted tables are validated.

### Where U16 is load-bearing vs where U08 is fine

The original framing of this phase ("all 3D maps → U16") was too broad. The 15 production pages aren't uniform: most 3D maps benefit visibly from U16, but a non-trivial number of tables have natural step sizes coarser than what U08 already gives.

**Tables that get widened to U16** (precision actually matters):

| Table | Why U16 matters |
|---|---|
| `veTable`, `veTable2` | 1% VE granularity in U08 is 0.39% per step at full scale — visible AFR jumps under partial throttle, especially on small forced-induction engines |
| `afrTable` / `lambdaTable` | Stoich gasoline at ~14.7 AFR has 0.058 AFR per U08 step — visible at idle and cruise targets |
| `ignitionTable` (advance) | 90° span in U08 is 0.35° per step — noticeable at light load and on knock-sensitive setups |
| `boostTable` (boost target) | 256 kPa span in U08 is 1 kPa per step — visible boost surging on closed-loop control |
| `vvtTable`, `vvt2Table` (cam target) | Fine cam-angle control needs sub-degree resolution |

**Tables that stay U08** (coarse natural steps or small ranges where U08 is already finer than what an operator tunes to):

- `wueTable` (warmup enrichment) — 16-entry coolant axis, whole-percent corrections, U08 already gives 0.4% precision
- `aseTaperTable`, `aseLoadTable`, `crankingEnrichTable` — after-start enrichment tapers, tuned in 1%/1°C steps
- `injOpenTable` — injector deadtime, microseconds, not finely interpolated
- `iatRetardTable`, `cltAdvanceTable` — small total ranges
- `flexFuelTable`, `flexAdvTable`, `flexBoostTable` — flex fuel trims, small ranges
- `egoTable` (closed-loop bounds), `dwellCompensation` — coarse natural steps
- Idle/boost/etc. PID gain tables — tuning constants, not 3D maps
- Sensor calibration pages 0/1/2 (CLT/IAT/O2) — unchanged; different transport (`'t'` calibration command)

**Why selective is better than blanket:**

1. Smaller firmware delta — roughly 5–7 pages widen instead of all 15
2. Smaller backwards-compat surface — tunes that only edited the byte-mode tables need no migration
3. Faster ship date — fewer pages to validate before promoting from `-U16P2` experimental to default
4. Honest characterization — "we widened the tables that needed it" rather than "we widened everything"
5. Forward-compatible with AVR — if mega2560 has the headroom for just the VE table to be U16 someday, that's a per-table opt-in instead of a board-family-wide flip

### Why this is the right time

1. **Page 2 already proves the seam works.** The experimental `speeduino 202501-T41-U16P2` path has been validated end-to-end on real hardware (TS reads, edits, burns, page CRC, SPI flash mirror — see `SESSION_HANDOFF_2026-03-26_TS_U16_PAGES.md`). The "Trouble Areas" section below documents the work that remains, but the architectural risk is gone — it's now a roll-out problem, not a design problem.
2. **The desktop is being rewritten in C++.** Both the parsers and the table generators (Engine Setup wizard, Hardware Setup wizard, VE/AFR/spark generator services) are being ported anyway. Generating U16 row data from day one is cheaper than generating U08 first and adding U16 support later.
3. **Phase 5's `live_data_map.h` discipline is in place.** The 148-byte log entry is now versioned and asserted at compile time, so widening telemetry channels later is a tractable problem rather than a layout-shuffling exercise.
4. **DropBear has the headroom.** Teensy 4.1 has 1 MB of SRAM and 8 MB of flash; the existing tune fits in a tiny fraction of either. Doubling map storage from byte to U16 is invisible at the platform level.

### Hard rules (locked)

- **DropBear/Teensy only.** AVR (mega2560, etc.) keeps the byte-serialized contract for every table — flash and RAM are too tight there to widen page storage. Phase 12 is gated on `BOARD_CAP_TEENSY41` (or equivalent capability bit) and never affects AVR builds.
- **Selective widening, not blanket.** Only the tables in the "widened to U16" list above get the new contract. Coarse-step tables (warmup, ASE, retard maps, PID gains) stay on the existing byte contract on DropBear too — they don't need the precision and widening them would only inflate the firmware delta.
- **Single signature.** When Phase 12 ships, `speeduino 202501-T41` is the new contract (selective U16) and `-U16P2` is retired. There is no "all-U08 mode" toggle for DropBear — the relevant byte paths are removed from the DropBear build, not just hidden.
- **One firmware build per board family.** No runtime mode switch. The board capability bit is the trigger; the per-table widening list is locked at compile time.
- **No widening of the live-data packet.** The 148-byte `LOG_ENTRY_SIZE` contract stays. High-resolution VE/AFR/spark telemetry is a separate slice in Phase 13B (Native Logging Contract on the desktop side); it does not block Phase 12.
- **CRC and SPI flash mirror cover the new byte stream exactly.** Same discipline as the U16P2 experimental work — page CRC is computed over the actual transmitted bytes (mixed U08/U16 across pages), SPI flash save/load iterates the new per-page sizes correctly.

### Slice plan

1. **Inventory the affected pages and lock the widening list.** Walk the 15 production pages and identify which ones contain tables in the "widened to U16" list. Each affected `[16x16]` U08 map is a 256-byte region in the current contract → 512-byte region in the Phase 12 contract. Pages may be **mixed** (e.g. a page that currently holds the VE table plus a small WUE-style trim table will have one widened entry and several byte-mode entries). Document the new per-page total sizes.
2. **Promote the experimental U16P2 page-2 path to the affected entries.** Apply the same `getPageSize()` / `getPageValue()` / `setPageValue()` / page CRC / SPI flash mirror updates that were validated for page 2 to every page that contains a widened entry. Pages with no widened entries are unchanged.
3. **Update the bundled INI.** `speeduino-dropbear-v2.0.1.ini` adopts the new layout; rename the experimental `speeduino-dropbear-v2.0.1-u16p2-experimental.ini` to retire it. Page sizes, table declarations (`U16` vs `U08` per table), and `ochBlockSize` updated together. Keep `speeduino 202501-T41` as the signature so existing project files continue to load (with a one-time migration on first save — see slice 5).
4. **Update the production base tune.** `speeduino-dropbear-v2.0.1-base-tune.msq` regenerated with the new layout. Widened tables get the existing values upcast (zero-pad high byte). Ford300 reference tune migrated the same way.
5. **Migration path for existing U08 tunes.** First-save handling on the desktop side: when a legacy all-U08 tune is loaded against the new INI, the tuner detects which tables now require U16 and upcasts them in-place (zero-pad high byte → U16, no value change). Tables that stay U08 are untouched, so most of the on-disk MSQ is bit-identical. One-way; no rollback path on the firmware side, since DropBear no longer accepts the legacy widths for those specific tables.
6. **Update test_comms guards.** The currently-skipped Teensy41/U16P2 tests on the AVR harness become unconditional on the Teensy build target *for the affected pages only*; the AVR build target keeps the byte path and its existing tests.
7. **Hardware validation.** Same path as Phase 11 Slice E: flash, connect via Airbear TCP bridge, full read/write/burn round-trip on every affected page (mixed U08/U16 entries on the same page), CRC verified, SPI flash save/load round-trip verified, cold-boot from flash verified. Particular attention to pages that have *both* widened and byte-mode entries — that's the new edge case the experimental U16P2 work didn't exercise (it touched all of page 2).
8. **Retire the `-U16P2` experimental signature.** Remove from the release bundle. Update Airbear's `'Q'`-response variant detection to expect only the single signature (the Airbear bridge itself doesn't change since it proxies bytes verbatim — only its variant logging is affected).

### Desktop / tuner application coordination

The desktop tuner application's Phase 14 (native C++ Qt app) is the natural consumer of this firmware change. Decisions committed in concert:

- **Per-table capability-driven generation.** When the desktop's Hardware Setup Wizard or Engine Setup Wizard generates a table, it looks up the **table's `data_type` from the active definition** rather than guessing from board capability alone. Definitions for DropBear-class boards declare `veTable`, `afrTable`, `ignitionTable`, `boostTable`, etc. as `U16`; everything else (`wueTable`, `aseTaperTable`, retard tables, etc.) stays `U08`. AVR-class definitions declare every table as `U08`. The generator just respects whatever the definition says.
- **Native definition format encodes the data type per table.** The desktop's owned `NativeDefinition` schema (already shipped in v1) carries `data_type` per `NativeTable`. This lets the native format describe a mixed-width contract directly — no special-cases for DropBear vs AVR in the generators, just "read what the definition says".
- **No legacy U08 generation path for tables that should be U16.** Once Phase 12 ships, the desktop's VE/AFR/spark/boost generators never produce U08 output when the active definition declares that table as U16. The byte path stays alive in the codebase as an INI/MSQ import compatibility layer (so existing TunerStudio U08 tunes can still be loaded and migrated) and for tables that genuinely stay U08 even on DropBear.
- **Selective migration on first save.** Loading a legacy all-U08 tune against the new mixed-width definition triggers a one-time in-memory upcast of *only the tables that widened* (zero-pad high byte → U16, no value change). Tables that stayed U08 are untouched. The next save writes the new format. No prompt; no opt-out; the operator sees a status-bar message noting which tables migrated.
- **Live telemetry stays byte-resolution.** Dashboard gauges that read live VE / AFR / advance / etc. continue to use the 148-byte live-data block. Only the *interpretation of stored map values* changes; the live channels that report active VE/AFR/etc. are still U08 in the packet (Phase 13B widens those separately if needed).

### What stays out of scope

- **High-resolution VE/AFR/spark logging.** The 148-byte live-data packet is unchanged. Hi-res telemetry is a separate Phase 13B slice and depends on a versioned `ChannelContract` that the desktop already has the infrastructure for.
- **AVR widening.** mega2560 etc. continue to use the byte contract. No conditional U08-or-U16 builds — the board family decides the contract at build time.
- **TunerStudio support.** TunerStudio reads INI files; if it loads the new INI it gets U16 tables for free. We don't gate Phase 12 on TS-side validation — the desktop tuner becomes the primary host once Phase 14 is shipped, and TS becomes a compatibility consumer.

### Test baseline target

- All existing Teensy41-skipped tests under `test_comms` become unconditional on the Teensy build target (10 tests promoted).
- New Teensy-only tests covering U16 page round-trip for every affected page (`test_pages_u16_dropbear`).
- Hardware validation record in this file (same shape as Phase 11 Slice E).
- Desktop-side parity tests (`tests/unit/test_cpp_*_parity.py`) updated to use the U16 INI as the production fixture once Phase 12 ships; the existing U08 fixtures stay around as legacy import-compatibility test cases.

### Sequencing

Phase 12 lands **after** the desktop tuner's Phase 14 first wave is far enough along that the C++ table generators exist to consume the new contract. Otherwise we'd ship a firmware variant nothing on the host side knows how to populate. The dependency order is:

1. Desktop Phase 14: port `[Constants]`, `[OutputChannels]`, `[TableEditor]`, generator services to C++ (in progress)
2. Desktop Phase 14: native Hardware Setup wizard producing tables ← *the consumer*
3. Firmware Phase 12: U16 by default on Teensy build target ← *the producer*
4. Joint hardware validation: bench with DropBear + native tuner end-to-end

Until step 1 is complete, the existing experimental `-U16P2` path remains the canonical way to test U16 page handling on Teensy. Phase 12 promotes that path; it doesn't replace it ahead of schedule.

---

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
    - the experimental milestone above is now complete for page `2` only: there is a Teensy-only, DropBear-gated, alternate-signature native-`U16` TS transport seam with matching read/write/CRC/SPI-flash behavior
    - implementation details and the end-to-end validation record for that experimental page-2 path are kept in [SESSION_HANDOFF_2026-03-26_TS_U16_PAGES.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-26_TS_U16_PAGES.md)
    - the experimental runtime fueling slice is also in place for page `2`: fueling uses a high-resolution runtime VE path while `currentStatus.VE*` remain byte-sized compatibility/display channels
    - the final protocol blocker was not TunerStudio itself; it was an internal type-width mismatch where `table3d_value_t` could still compile as byte-sized on Teensy in early include paths. That is now fixed in [table3d_typedefs.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/table3d_typedefs.h), with matching serializer fixes in [pages.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/pages.cpp) and [comms_legacy.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/comms_legacy.cpp)
    - the experimental path has now been smoke-tested in real TunerStudio: page 2 displays correctly and burns cleanly with the alternate experimental hex/INI/tune flow
    - what remains deferred is **high-resolution VE telemetry/output-channel exposure**, because logger/live-data/legacy packet layouts are still byte-oriented and shared by existing consumers
    - treat telemetry widening as a separate design slice from TS page transport and fueling; do not conflate "TS can edit `U16` page 2" or "fueling consumes hi-res VE" with "all external status/log contracts expose hi-res VE"
    - the current recommendation is to avoid widening the existing `148`-byte output-channel packet in place; any real hi-res VE telemetry experiment should use an alternate signature + alternate INI + alternate `ochBlockSize`/logger map rather than mutating the shared production packet contract
    - the best next milestone is no longer transport debugging; it is either:
      - controlled bench / hardware validation of the experimental artifacts, or
      - a separate design for optional hi-res VE telemetry if a real logging workflow requires it
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

## Phase 12: U16 maps where precision matters (DropBear / Teensy)

This phase is the firmware side of a **joint commitment** with the desktop tuner application at [D:/Documents/JetBrains/Python/Tuner](D:/Documents/JetBrains/Python/Tuner) (its Phase 14). The desktop already references this firmware phase by name in [docs/tuning-roadmap.md](D:/Documents/JetBrains/Python/Tuner/docs/tuning-roadmap.md) ("Phase 14 / Firmware Phase 12 joint commitment") and the Tuner [CLAUDE.md](D:/Documents/JetBrains/Python/Tuner/CLAUDE.md) ("Joint U16-where-precision-matters commitment with the Speeduino firmware (Firmware Phase 12)"). The earlier all-3D-tables-to-U16 framing was too broad; the refined plan widens only the tables where U08 quantization is actually visible to the operator and leaves coarse-natural-step tables on byte mode.

### Tables that widen to U16 on DropBear / Teensy 4.1

- `veTable`, `veTable2` — 1% VE per U08 step is ~0.39% per step at full scale; visible AFR jumps under partial throttle on small forced-induction engines
- `afrTable` — lambda/AFR target resolution becomes meaningful for closed-loop EGO and lambda overlays
- `ignitionTable`, `ignitionTable2` — 0.5° advance per step is too coarse on knock-limited turbo engines near MBT
- `boostTable` — duty-cycle target resolution dominates closed-loop boost overshoot/undershoot
- `vvtTable`, `vvtTable2` — cam target degrees benefit from sub-degree resolution for closed-loop cam PID

### Tables that stay U08 even on DropBear (intentionally)

- `wueTable`, ASE tapers, cranking enrichment — coarse natural step
- Retard maps, flex trims, knock retard tables — coarse natural step
- PID gains, sensor calibrations, dwell tables — coarse natural step
- All AVR-class boards: every table stays U08

### Firmware-side scope

- Single production signature `speeduino 202501-T41` exposes the targeted tables as native U16 on DropBear; the experimental `-U16P2` signature collapses into the default once these tables are validated
- Pages with **mixed widened + byte-mode entries** are valid — page CRC and SPI flash mirror already handle mixed widths the same way the experimental U16P2 path handled the all-widened case (see Phase 8 closing notes and the existing CRC/page-verification protocol in [page_crc.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/page_crc.cpp))
- Hot fueling lookups (`getVE1`, `getAdvance1`, etc.) need to consume native U16 cells without an intermediate byte downcast — this is the surface that the experimental U16P2 INI noted was not yet end-to-end ([CLAUDE.md](D:/Documents/JetBrains/Python/Tuner/CLAUDE.md) "Known Fragile Areas" item 4)
- BLOCKING_FACTOR / TABLE_BLOCKING_FACTOR are already 512 from Phase 8 — no transport change needed
- Capability response (`'K'` command, FW-003) already advertises `CAP_FEATURE_U16P2`; once Phase 12 lands the flag's meaning shifts from "experimental U16 page-2 path" to "native U16 high-leverage tables"; the desktop reads `data_type` from the active definition so the firmware change is invisible to the generator code

### Sequencing (joint with desktop Phase 14)

1. Desktop Phase 14 lands the per-table U16 generators first (already in progress in [Tuner](D:/Documents/JetBrains/Python/Tuner))
2. Firmware Phase 12 ships native U16 fueling/ignition/boost/VVT lookups on Teensy 4.1
3. Joint hardware/bench validation including a page that holds both widened and byte-mode entries
4. `-U16P2` experimental signature retired; production becomes the only DropBear variant

### Known fragile area

The Tuner [CLAUDE.md](D:/Documents/JetBrains/Python/Tuner/CLAUDE.md) "Known Fragile Areas" item 4 calls out that the experimental INI itself notes "the TS page-2 transport is native U16, but the main fueling path still consumes VE through byte-returning lookups, so this is not yet a proven end-to-end high-resolution fueling mode." Closing this gap is the core firmware deliverable for Phase 12 — the transport already works; the **lookup** path is what is missing.

## Phase 13: Teensy 4.1 / DropBear Enhancement Backlog

Current Teensy 4.1 board layer ([board_teensy41.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/board_teensy41.h), [board_teensy41.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/board_teensy41.cpp)) leaves significant iMXRT1062 capability unused vs what rusEFI ([Resources/rusefi-2026-03-17](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17)) and MS3 ([Resources/ms3-source-master](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master)) exploit on comparable Cortex-M class hardware. The DropBear (`BOARD_ID=55`, MC33810-driven) is the only consumer of this layer and the user's daily driver, so the items below are scoped to that target. Each is research-only until called out as ✅ implemented. None of the following block the current narrow Phase 1 surface — they are net-new capability work.

### Slice A — Trigger ISR jitter (HIGH value, MEDIUM risk)

- **Problem**: `pinTrigger`/`pinTrigger2`/`pinTrigger3` use `attachInterrupt()` + a software `micros()` timestamp inside the decoder ISR. On iMXRT1062 the GPIO IRQ entry latency plus `micros()` read introduces ≥1µs of jitter, which dominates ignition timing error above ~6000 RPM on high-tooth-count wheels (60-2, NGC4).
- **rusEFI reference**: `firmware/hw_layer/drivers/gpio/...` and the trigger input layer use **hardware input capture** on the QuadTimer (TMR) capture inputs, latching the bus-clock counter in hardware at the edge — sub-100ns timestamp resolution, ISR only reads the latched value.
- **Speeduino fit**: TMR1–TMR4 are already claimed for fuel/ign compares, but each Quad-Timer channel has an independent **secondary input function** that can capture into `CAPT` while the channel still runs as a compare timer. Alternatively use FlexPWM input capture, which is unused.
- **Impact**: removes the largest single source of timing scatter on DropBear, directly improves dwell consistency at high RPM.
- **How to apply**: prototype on `pinTrigger` only first, behind a `TEENSY41_HW_TRIGGER_CAPTURE` opt-in; keep the `attachInterrupt` path as fallback. Validate against existing decoder tests by feeding the captured timestamp into the same `triggerHandler` callback.

### Slice B — Software knock detection (HIGH value, HIGH effort)

- **Problem**: `KNOCK_MODE_DIGITAL` only consumes a comparator-output pin from an external knock IC (TPIC8101 / HIP9011). DropBear users without that IC fall back to no knock protection.
- **rusEFI reference**: `firmware/controllers/sensors/software_knock.cpp` runs a windowed ADC capture (typically 64–128 samples around TDC), then a CMSIS-DSP biquad bandpass at the cylinder-bore-resonant frequency, then RMS — entirely on the Cortex-M7 FPU.
- **Speeduino fit**: Teensy 4.1 has the same FPU + access to `arm_math.h` (CMSIS-DSP) via Teensyduino. ADC1 can sample at >1 MSPS in single-shot mode, easily covering one cylinder window per ignition event.
- **How to apply**: add `adc_teensy41_knock.{h,cpp}` with a ring buffer + biquad; expose RMS via existing `currentStatus.knockCount`/`knockRetard` consumers so no logic in `corrections.cpp` changes. Gate behind `KNOCK_MODE_ANALOG_DSP` (new) so AVR never sees it.

### Slice C — Native Ethernet TS transport (HIGH value, LOW risk)

- **Problem**: WiFi is currently delivered via the ESP32-C3 Airbear coprocessor on `Serial2` at 115200 baud (Phase 11). 115200 caps tune-load throughput and adds two hops of latency for live data.
- **Teensy 4.1 native**: built-in 10/100 Ethernet PHY pads, supported by `NativeEthernet` (Teensyduino library). A single magjack + RJ45 on the DropBear v2 carrier exposes wired TS-over-TCP at full link speed.
- **How to apply**: extend `boardHasCapability(BOARD_CAP_WIFI_TRANSPORT)` to also cover an ETH transport flag; in `serviceBoardSerial()` add a `pPrimarySerial = (Stream*)&ethClient` divert that mirrors the existing `Serial2` divert pattern. Zero protocol changes needed because the diverted-pointer comms architecture already supports it.
- **Why it lands cleanly**: this is the same shape as the Phase 11 ESP32-C3 path. The plumbing is already proven; only the transport object changes.

### Slice D — ADC pipeline upgrades (MEDIUM value, LOW risk)

- **Current state**: `initADC_Teensy41()` sets 12-bit + `analogReadAveraging(4)`, then `readAnalogPin()` shifts to 10-bit so the rest of the firmware's `fastMap10Bit()` math is unchanged.
- **Headroom**:
  - **Bump averaging to 16**: sqrt(4) → sqrt(16) = +6 dB SNR for ~12µs/read on a 600 MHz core — still negligible.
  - **Native 12-bit MAP/TPS path**: introduce `fastMap12Bit()` and an opt-in 12-bit branch in `readAnalogPin()` for `pinMAP`/`pinTPS` only. Improves boost-control resolution from 1 kPa/count to 0.25 kPa/count and TPS from 0.1% to 0.025%. Needs INI/feature-flag work to expose to TS without breaking AVR.
  - **DMA-driven background sampling**: rusEFI runs ADC1 in continuous DMA mode into a sample buffer the main loop reads — no per-call ADC stall. Worth doing only if profiling shows `analogRead()` cost is meaningful in the main loop on Teensy 4.1 (it usually isn't, so this is the lowest-priority ADC item).

### Slice E — Cortex-M7 cache & TCM placement (MEDIUM value, LOW risk)

- **Headroom**: iMXRT1062 has 512 KB FlexRAM split between **ITCM** (zero-wait-state instruction TCM) and **DTCM** (zero-wait-state data TCM). Hot-path functions and structs not placed in TCM run from cached OCRAM and incur cache-miss stalls during ISR storms.
- **Targets**: schedule ISRs (`fuelScheduleNInterrupt`, `ignitionScheduleNInterrupt`), the active decoder's `triggerHandler*`, the `currentStatus` struct, and the active page CRC scratch. Add `FASTRUN` / `DMAMEM` placement attributes inside `#if defined(CORE_TEENSY41)` only.
- **Why it matters more under MC33810**: every fuel/ign event on DropBear includes an SPI write to the MC33810; cache misses on the schedule callback compound with SPI latency.

### Slice F — MC33810 SPI via DMA (MEDIUM value, MEDIUM risk)

- **Current state**: [acc_mc33810.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/acc_mc33810.cpp) issues blocking `SPI.transfer()` calls inside the schedule ISR for every injector/coil state change.
- **Improvement**: iMXRT1062 LPSPI supports DMA-backed transfers via Teensyduino's `SPI.transfer(buf, n, evt)` async API. A 1-byte injector/ign update through DMA frees ~3–6µs per event — meaningful at 8000 RPM 8-cyl sequential.
- **Risk**: ordering across overlapping schedules; needs a per-CS queue or a "coalesce all bits dirty in this tick → 1 SPI burst" pattern. Prototype on a single channel before generalizing.

### Slice G — Multi-bus + CAN-FD (MEDIUM value, LOW risk)

- **Current state**: only `Can0` (CAN1) is used via `FlexCAN_T4` ([comms_CAN.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/comms_CAN.cpp), Haltech broadcast, etc.).
- **Teensy 4.1 native**: three CAN controllers; `CAN3` is **CAN-FD** capable (8 Mbit data phase). Useful for modern OEM dashes (Haltech IC-7, AiM, OBD2 gateways) and for offloading high-rate logging to a CAN-FD recorder without choking CAN1.
- **How to apply**: extend `comms_CAN` with a second `Can1` instance behind a build flag; expose a TS-side selector via the capability response (`CAP_FEATURE_CANFD`).

### Slice H — PSRAM-backed extended logging (LOW value, LOW risk)

- **Hardware**: Teensy 4.1 has solder pads for 8 MB QSPI PSRAM (`EXTMEM`).
- **Use**: a circular high-rate composite-log ring buffer (10 kHz tooth + ADC capture) flushed to SD on trigger event — useful for diagnosing intermittent decoder sync loss without keeping the SD card streaming continuously. The composite-log infrastructure already exists; only the buffer placement and flush trigger change.

### Slice I — Watchdog + MPU hardening (LOW value, LOW risk)

- **WDOG**: iMXRT1062 has both `WDOG1` (128s max) and `RTWDOG` (windowed). Neither is currently armed. Arm `WDOG1` with a ~2s timeout, kicked from `oneMSInterval()`; provides automatic recovery from any ISR deadlock.
- **MPU**: configure Cortex-M7 MPU to mark the stack-overflow guard region as no-execute / no-write. Cheap insurance against memory corruption from unbounded recursion under unusual decoder states.

### Slice J — USB CDC at high speed for tune transfer (LOW value, LOW risk)

- **Current state**: USB CDC default config. Tune writes still chunked at `BLOCKING_FACTOR=512` (Phase 8).
- **Headroom**: Teensyduino exposes `USB_DUAL_SERIAL` and `USB_TRIPLE_SERIAL` build modes. A second CDC interface could carry the secondary serial channel without the `Serial2` UART entirely, eliminating the 115200 cap for the Airbear use case if the user runs Airbear off the Teensy USB host instead of via UART.

### Slice K — Closed-loop tightening (LOW value, MEDIUM effort)

- **VVT/idle PID**: rusEFI's `pid.cpp` includes integrator clamping and derivative-on-measurement; Speeduino's `PID_v1` uses derivative-on-error which spikes on setpoint changes. The recent `PID::Initialize()` cleanup makes a drop-in replacement easier than before. Teensy-only at first (FPU-friendly) so AVR is unaffected.

### Slice L — UDP broadcast dashboard channel (LOW value, LOW risk)

- **Evidence**: the decompiled TSDash sources at [C:/Users/Cornelio/Desktop/TSDash_decompiled/TSDash](C:/Users/Cornelio/Desktop/TSDash_decompiled/TSDash) show TSDash itself uses **UDP broadcast** (`DatagramSocket.setBroadcast(true)` in `aD/h.java`, `aM/k.java`, and the `UdpCylPressureSimulator` family) as the primary protocol for live telemetry to remote dashboards, *not* the same TCP framing that TunerStudio uses for tuning. EFI Analytics dashboards on the local network expect a periodic UDP broadcast they can subscribe to without holding a tuning session open.
- **Speeduino fit**: when Slice C (Native Ethernet) is in place, expose a low-rate UDP broadcast (e.g. 25 Hz) of the same 148-byte live-data packet that the `'r'`/`'A'` command already produces. Reuses existing `live_data_map.h` byte layout — no protocol design needed. Lets a TSDash-style remote dash run alongside the Tuner without arbitrating for the comms session.
- **Why it lands cleanly**: the live-data packet is already byte-stable (FW-006 / `OCH_OFFSET_*` constants); the UDP path is purely a transport addition, no payload change.

### Slice M — Live-data packet stability guarantees for the desktop ChannelContract (DOC-only, NO risk)

- **Evidence**: the desktop tuner has shipped a `ChannelContract` parser (`tuner.domain.channel_contract.ChannelContract` + `LiveDataMapParser`) that consumes [live_data_map.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/live_data_map.h) line-by-line and **derives byte offsets directly from the firmware header**. A canary test sums all parsed entry widths against `LIVE_DATA_MAP_SIZE = 148` so any future schema change that breaks the parser fails loudly (Tuner [CLAUDE.md](D:/Documents/JetBrains/Python/Tuner/CLAUDE.md) Important Services / VE Analyze section).
- **Implication for firmware work**: any reordering, insertion, or width change in `live_data_map.h` is now a **cross-repo break**, not a free firmware-side edit. The desktop will still parse the file because it reads it directly, but channel offsets used by historical logs and replay sessions will silently drift.
- **How to apply**: add a comment block at the top of `live_data_map.h` documenting the contract: appends-only at the end of the byte table, never reorder/widen existing rows, bump `LIVE_DATA_MAP_SIZE` only when adding to the end. No code change. Optional follow-up: bake a compile-time `static_assert` on the running total for the existing rows so a non-additive edit won't compile.

### Slice N — Output channel count limits (LOW value, LOW risk)

- **Evidence**: the decompiled TSDash sources at `bA/aD.java` and `bA/N.java` warn the user that "A maximum of N Output Channels can be logged. Your current selection of fields requires X Output Channels — only the fields based on the first N OutputChannels will be added." TunerStudio-family dashboards have a hard upper bound on the number of channels they will pull per packet, and any firmware that grows the live-data packet past that ceiling silently truncates from the user's perspective.
- **How to apply**: when Phase 12 lands and any new U16-resolution logging entries are considered, **stay within the existing 148-byte `LIVE_DATA_MAP_SIZE`** rather than appending more channels. If hi-res telemetry is genuinely needed, do it as a separate "extended logging" command (the Tuner's planned Phase 13B "Native Logging Contract" — see Tuner roadmap) instead of inflating the standard 'r'/'A' packet. Do not break TSDash compatibility for the sake of telemetry that the dash can't display anyway.

### Cross-cutting notes

- All slices are gated `#if defined(CORE_TEENSY41)` so the AVR Mega2560 path stays bit-for-bit identical and the existing simavr suites (275/275 decoders, 88/88 fuel, 193/193 ign, 75/75 sensors, 46/46 comms) remain the safety net.
- DropBear is the only `applyBoardPinMapping()` consumer in [board_teensy41.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/board_teensy41.cpp), so per-board capability flag work can stay scoped to `BOARD_TEENSY41_DROPBEAR_PIN_MAPPING` without a wider table refactor.
- Ordering: Slices A (HW input capture) and C (Ethernet) are the highest-value low-risk wins and should land first; Slice B (software knock) is the highest-effort item and should wait until A is stable so trigger jitter cannot contaminate knock-window sampling.

## Phase 14: Firmware as native-contract producer (joint with Tuner Phase 13A)

This phase is the firmware side of the desktop tuner's **Future Phase 13A: Firmware Contract Modernization** at [Tuner/docs/tuning-roadmap.md](D:/Documents/JetBrains/Python/Tuner/docs/tuning-roadmap.md). The desktop has already shipped v1 of `NativeDefinition`, `NativeTune`, and `ChannelContract` ([Tuner/CLAUDE.md](D:/Documents/JetBrains/Python/Tuner/CLAUDE.md), `tuner.domain.native_format` and `tuner.domain.channel_contract`). The model the desktop currently uses is **"reverse-engineer the contract from the legacy INI"**. The opportunity in this phase is to flip the dependency: **the firmware becomes the authoritative producer of its own native contract**, generated at build time from declarative C++ headers, and the desktop consumes those headers/exports directly. The legacy INI/MSQ paths stay as a one-way compatibility/export layer, not the source of truth.

### Why this matters for both sides

- The desktop `LiveDataMapParser` already reads `live_data_map.h` directly (Tuner roadmap Phase 12 deliverable 4) — this is the **proof** that header-derived contracts work. Same pattern, applied to tune storage and definition metadata, eliminates the entire class of "INI says one thing, firmware does another" bugs.
- Tune-flash mismatch is currently caught only by `'Q'` signature string compare. A schema fingerprint embedded in the firmware lets the desktop refuse to burn an incompatible tune **before** the operator clicks burn — instead of after the engine bricks.
- The Tuner's `NativeTune` v1 is intentionally flat keyed by semantic id (Tuner roadmap deliverable 2). Once the firmware produces that semantic id table itself, the legacy `lastOffset` indirection chain (Tuner CLAUDE.md "Known Fragile Areas" item 2: `afrTable` → `afrTable1Tbl` → `afrTable1Map` → `zBins = afrTable`) collapses.

### Slice 14A — `tune_storage_map.h` declarative table (HIGH value, LOW risk)

- **What**: a sibling header to [live_data_map.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/live_data_map.h), structured the same way: one declarative comment row per tune parameter with `{semantic_id, page, offset, type, scale, units, axis_ref}`. Tables get `{semantic_id, page, offset, rows, cols, x_axis_id, y_axis_id, data_type, units}`. Axes get `{semantic_id, page, offset, length, type, units}`.
- **Source of truth**: this header. The legacy INI's `[Constants]` block becomes a generated artifact (build script reads `tune_storage_map.h` and emits the INI).
- **Desktop fit**: identical pattern to the existing `LiveDataMapParser` — the Tuner adds `TuneStorageMapParser` and the Phase 14 native C++ desktop reads it directly. No INI required for new boards.
- **Migration**: legacy INIs continue to work via the existing parser; the new header-derived path is the canonical one for `speeduino 202501-T41` going forward.

### Slice 14B — Schema fingerprint in capability response (HIGH value, LOW risk)

- **What**: extend the `'K'` capability response (FW-003) with a 32-bit `schema_fingerprint` — a SHA-256-based truncated hash of the concatenation of `live_data_map.h` + `tune_storage_map.h` + `BOARD_ID`, computed at build time and baked into a `const` symbol.
- **How the desktop uses it**: on connect, the desktop's `NativeTune` carries a `definition_signature` (already in v1). The desktop refuses to burn a tune unless `tune.definition_signature == ecu.schema_fingerprint`. Eliminates the "wrong INI loaded against right firmware" failure mode that signature-string matching can't catch when only offsets shift.
- **How the firmware uses it**: optional second-layer guard — incoming page writes can be cross-checked against an in-flash fingerprint stamp before commit. Out of scope for v1; the desktop guard alone is enough for the first slice.
- **Build-side**: `tools/generate_schema_fingerprint.py` already has the right shape (the existing release manifest generator hashes files); extend it to emit `schema_fingerprint.h` with the constant.

### Slice 14C — Semantic-id stamps on packed status bytes (MEDIUM value, LOW risk)

- **What**: extend the existing `runtimeStatusA` pattern to additional packed-status surfaces (engine protection, knock, transient, sync state). Each new packed byte is declared in `live_data_map.h` with explicit per-bit semantic ids, not opaque names like `status3`/`status5`.
- **Why**: the Tuner roadmap Phase 12 finding explicitly calls this out: "the `runtimeStatusA` packed byte (`tuneLearnValid`, `fullSync`, `transientActive`, `warmupOrASEActive`) is a preview of what explicit firmware-owned logging semantics look like — extend this pattern rather than adding more opaque bytes". The firmware roadmap's existing FW-006 work already proves the pattern lands cleanly.
- **No packet growth**: extends meaning of existing bytes that the desktop currently has to special-case in `live_data_map_parser.py`, doesn't add bytes (Phase 13 Slice N: stay within `LIVE_DATA_MAP_SIZE = 148`).

### Slice 14D — Native tune ingest from SD card (MEDIUM value, MEDIUM risk, Teensy 4.1 only)

- **What**: at boot (or via a TS command), the firmware looks for `tune.json` on the SD card, parses it as a `NativeTune`, validates `definition_signature == schema_fingerprint`, and applies it to RAM pages without going through the `'w'` page-write protocol.
- **Why**: race-team workflow — swap tunes between rounds without a laptop. Currently impossible; the only path is TS-over-USB.
- **Risk**: needs a minimal JSON parser in firmware (no Arduino-friendly streaming JSON parser is small enough by default). Could vendor `nanopb`-style JSON or, simpler, define a `tune.bin` binary native format that's just `{header, schema_fingerprint, packed_pages}` — the desktop can produce both formats from the same `NativeTune` model.
- **Defer**: probably ship as `tune.bin` first; `.ntune.json` is a Phase 13B logging-contract follow-up.

### Slice 14E — Boot manifest emission (LOW value, LOW risk)

- **What**: at first USB CDC connect, before any TS handshake, the firmware emits a single newline-terminated JSON line: `{"board": "DropBear-T41", "schema_fingerprint": "0xABCD1234", "fw": "202501.6", "caps": [...], "page_widths": {...}}`. The desktop reads this opportunistically; absent the line, falls back to the current `'Q'`/`'F'`/`'S'` probe sequence.
- **Why**: replaces ~6 round-trip probes with one read on the connect path; speeds up reconnect noticeably on slow links (Airbear ESP32-C3 over 115200 UART).
- **Compatibility**: legacy TS clients ignore unknown lines, so the line is invisible to existing tools.

### Slice 14F — Build-time INI generation from native definition (LOW value, MEDIUM effort)

- **What**: invert the dependency. Once `tune_storage_map.h` is the source of truth (Slice 14A), `tools/generate_ini.py` reads it and emits `speeduino-dropbear.ini` as a generated artifact. Removes hand-maintained INI drift entirely.
- **Why**: every release currently risks INI/firmware divergence because the INI is hand-edited. A generated INI is byte-stable per build and the diff against the previous release's INI becomes a meaningful changelog instead of churn.
- **Sequencing**: requires Slice 14A first. Lowest priority of the Phase 14 slices because the existing hand-maintained INI works; this is a maintenance/CI win, not a capability win.

### Slice 14G — Boot-time multi-tune slot selection (MEDIUM value, LOW-MEDIUM risk, Teensy 4.1 only)

- **What**: store N (target: 4) complete tune images in SPI flash and/or on the SD card. At boot, read a **dash-mounted rotary selector switch** (the same form factor diesel-truck tuners use — EFILive, HP Tuners, MM3 on Cummins/Duramax/Powerstroke) to pick which slot to load into the in-RAM tune pages. Each slot carries its own `schema_fingerprint` (Slice 14B) and is validated independently before being applied. If the selected slot fails fingerprint check, fall back to slot 0 and log the mismatch.
- **Why**: standard ECU-class feature, directly modeled on the diesel-truck rotary-selector workflow (Position 0 = Stock/Tow, Position 1 = Daily, Position 2 = Performance, Position 3 = Race). Also covers the gasoline-tuner use cases: pump gas vs race gas, valet mode, E85 vs pump, wet vs dry. The Tuner roadmap explicitly notes it as a TunerStudio gap (G10) and a firmware-side responsibility. Slice 14D (SD ingest) does most of the plumbing already; this slice generalises it from "one tune" to "N tunes with a selector".
- **Switch form factor — rotary is the primary target**: the operator-facing input is a dash-mounted rotary selector switch, *not* a DIP switch or single SPDT toggle. Two electrical wiring conventions are supported, both pure digital input with internal pull-ups (no ADC, no extra hardware on DropBear v2):
  - **Binary-encoded rotary** (preferred): the switch presents 2 or 3 output pins that form a binary number. 2 pins → 4 positions (matches the target slot count); 3 pins → 8 positions for future expansion. This is how most automotive rotary selectors are wired internally.
  - **One-hot rotary** (simpler switches): N output pins, exactly one grounded per position. Teensy reads N input pins, picks the one that is low. Uses more GPIO but supports cheap mechanical rotaries with no internal encoding.
  - The firmware reads either convention via a configurable `tuneSlotSelectorMode: BINARY | ONEHOT` flag and a small `pinTuneSlot[]` pin array in the pin map. A single-pin SPDT toggle is just the degenerate `BINARY` case with 1 pin / 2 slots, so the same code path covers it for free.
  - **Debounce / glitch handling**: read at boot only (per the boot-time-only contract below), so a single 5 ms settle delay before sampling is enough — no need for a full debounce state machine. The selector is mechanical and the operator turns it with the engine off.
  - **Position-0 default**: if the selector pins are all floating (no switch wired), pull-ups put the read at the all-high state, which maps to slot 0. Boards without a rotary installed get slot-0-only behaviour automatically.
- **Storage**: prefer SPI flash slots over SD for the primary path because the SPI flash is already used for the page mirror, has known wear-levelling behaviour, and doesn't depend on SD card presence at boot. SD becomes the "import a new slot" surface — drop `slot1.bin` on the card, firmware copies it into SPI flash slot 1 on next boot, then runs from flash.
- **Schema fingerprint per slot**: critical. Different slots can be from different firmware builds (e.g. you flashed a new firmware after tuning slot 2 but slot 0 is stale) — each slot gets validated against the running firmware's fingerprint independently and a stale slot is just refused, not run.
- **Out of scope (explicitly)**: live switching while the engine is running. The double-buffered page swap that would let the operator flip a dash switch at idle is structurally harder (fuel/ign ISRs can't see a torn tune mid-stroke), needs an atomic swap point, and gives only marginal additional value over "key-off, flip switch, key-on". Deferred to a hypothetical Phase 15 if real demand surfaces. **Boot-time-only is the contract for Slice 14G.**
- **UI surface for the operator**: a small slot indicator on the live-data packet (1 byte: `activeTuneSlot`) so the desktop and dashboards can show which slot is currently running. Fits within existing `LIVE_DATA_MAP_SIZE = 148` by repurposing one of the reserved bytes — no packet growth (Phase 13 Slice N constraint preserved).
- **Dependencies**: 14B (per-slot fingerprint validation needs the fingerprint to exist) and 14D (binary tune format and SD ingest are reused per slot). Should not start until both have landed.
- **Hardware**: DropBear v2 has spare digital inputs; two get repurposed as `pinTuneSlot[0..1]` in the pin map for the target 4-position binary rotary. The rotary switch itself is an off-the-shelf dash-mounted automotive part (e.g. a 4-position SP4T rotary, ~$10–20). No DropBear board changes — it's just two more wires from the harness to the dash. For users who only want 2 slots, the second pin can be left unwired and slot selection collapses to a single-pin toggle.

### Joint sequencing with Tuner Phase 13A

| Step | Repo | Slice |
|---|---|---|
| 1 | Firmware | 14A — `tune_storage_map.h` lands, sibling to `live_data_map.h`; legacy INI unchanged |
| 2 | Tuner | `TuneStorageMapParser` consumes the new header (mirrors existing `LiveDataMapParser`); `NativeDefinition` v2 builds directly from it instead of from INI |
| 3 | Firmware | 14B — schema fingerprint baked into capability response |
| 4 | Tuner | Burn-time guard: refuse to burn if `tune.definition_signature != ecu.schema_fingerprint`; surface mismatch in the connect dialog |
| 5 | Firmware | 14C — semantic-id extensions to packed status bytes (no packet growth) |
| 6 | Tuner | Phase 13B Native Logging Contract consumes the new packed-status semantics directly |
| 7 | Firmware | 14E — boot manifest line over USB CDC |
| 8 | Tuner | Connect path reads the manifest line; legacy probe path becomes a fallback |
| 9 | Firmware | 14D — `tune.bin` ingest from SD (Teensy 4.1 only) |
| 10 | Firmware | 14F — generated INI from `tune_storage_map.h`, retire hand-maintained INI on the DropBear/Teensy variant |
| 11 | Firmware | 14G — boot-time multi-tune slot selection (depends on 14B + 14D) |
| 12 | Tuner | Multi-tune UI: per-slot edit/burn target, slot status indicator from `activeTuneSlot` byte, "Copy slot X → slot Y" action, fingerprint mismatch surfacing per slot |
| ongoing | Both | 14H — updated DropBear/Teensy 4.1 operator manual, with each new slice landing with its manual section; cross-linked to the desktop Tuner manual via a shared topic index |

### Slice 14H — Updated end-user documentation (MEDIUM value, LOW risk, ongoing)

- **What**: an updated, DropBear/Teensy-4.1-aware operator manual that supersedes the stock Speeduino documentation at [Resources/Speeduino_manual.pdf](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino_manual.pdf) for the features added across Phases 6–14. The stock manual predates Phase 6 enablement; the existing [Resources/Speeduino_manual_update_2026-04.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino_manual_update_2026-04.md) is a partial start that should be folded in and extended.
- **Why now**: Phases 12 (U16 high-leverage tables), 13 (Teensy enablement backlog), and 14 (native-contract producer + multi-tune) introduce a meaningful number of operator-visible behaviours that have **no documentation anywhere**. Operators currently learn these by reading the roadmap, which is the wrong medium — the roadmap is a planning document, not an instruction manual.
- **Scope of the new manual** (additive to the stock Speeduino docs, not a replacement for them):
  - DropBear/Teensy 4.1 board-specific wiring, pinout, and reserved-pin notes (drawn from `setTeensy41DropBearPinMapping()` and the Phase 6 enablement work)
  - U16 high-leverage tables: which tables are widened, what the operator-visible precision change is, how to recognise a U16 vs U08 tune in the desktop, migration notes for legacy U08 tunes
  - SPI flash health monitoring (Phase 6 byte 131 of the live-data packet) — what "healthy" / "unavailable" means and what to do about it
  - PWM fan control (Phase 6) — wiring, frequency selection, configuration
  - Hardware ADC averaging (Phase 7) — what the operator gains from it, no configuration required
  - Native Ethernet TS transport (Phase 13 Slice C, when shipped) — wiring, IP/DHCP behaviour, operator setup
  - Software knock detection (Phase 13 Slice B, when shipped) — sensor wiring, frequency-window configuration, comparison vs the existing TPIC8101/HIP9011 path
  - **Multi-tune slot management (Slice 14G)** — rotary selector wiring (binary vs one-hot), how to build per-slot tunes, fingerprint mismatch behaviour, the boot-time-only contract, the diesel-truck-style 4-position layout
  - SD-card tune ingest (Slice 14D) — file format, naming convention, fingerprint validation, race-team workflow
  - Schema fingerprint (Slice 14B) — what it is, what "fingerprint mismatch" means in operator language, how to recover
  - The capability response (`'K'` command, FW-003) and what each capability bit means in operator-facing terms
- **Format**: Markdown source under `docs/manual/` in the firmware repo (one file per topic), generated to a single PDF at release time via `tools/generate_manual.py` (new). The Markdown source is the canonical edit surface; the PDF is the artefact users actually download. Same model as the existing `Speeduino_manual_update_2026-04.{md,pdf}` pair, just structured per-topic instead of one monolithic update file.
- **Source-of-truth rule**: every documented behaviour must reference either a `live_data_map.h` row, a `tune_storage_map.h` row (once Slice 14A ships), a `FIRMWARE_ROADMAP.md` slice, or a specific source file + line. No prose-only claims about what the firmware does — if a fact isn't pinned to source, it goes stale. The doc is regenerated against the source on every release.
- **What it does NOT replace**:
  - The stock Speeduino wiki / forum / `Speeduino_manual.pdf` for the AVR Mega2560 path — those stay authoritative for AVR. The new manual is explicitly the *DropBear/Teensy 4.1* manual, not a fork of the AVR manual.
  - The roadmap. The roadmap stays a planning document; the manual is the operator-facing companion.
  - The desktop tuner's own docs (those live in the [Tuner](D:/Documents/JetBrains/Python/Tuner) repo and are updated on the desktop side — see Tuner roadmap for the corresponding desktop docs slice).
- **Joint with the desktop**: each topic in the firmware manual that has an operator-facing desktop counterpart (multi-tune slot picker, schema fingerprint mismatch dialog, U16 table editing, etc.) cross-links to the equivalent section in the Tuner manual rather than duplicating UI screenshots in the firmware repo. The two manuals share a topic index so the cross-links are stable.
- **Effort**: ongoing rather than a single sprint. Each new firmware slice from Phase 12+ that ships should land **with** its manual section, not before/after. Backfill for already-shipped Phase 6/7/8 work is a one-time pass.
- **Stock-doc references for fact-checking and structure**: [Resources/Speeduino_manual.pdf](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino_manual.pdf) for the canonical Speeduino topic structure (so the new manual reads like a continuation, not a fork), and the existing partial [Resources/Speeduino_manual_update_2026-04.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino_manual_update_2026-04.md) as the seed content to fold in.

### Out of scope for Phase 14

- AVR Mega2560 stays on the legacy INI/MSQ path forever — there is no flash budget for native-contract producers on AVR, and the desktop already has a working compatibility layer.
- JSON5 hand-authoring of native definitions stays on the desktop side; firmware only ever consumes machine-generated binary or JSON, never hand-authored JSON5.
- Schema migration tooling (`v1` → `v2` rename pass) stays on the desktop side; firmware only ever knows its own compiled-in fingerprint.
