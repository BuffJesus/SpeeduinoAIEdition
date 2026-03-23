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
  - the `36-2-1` gap model is now documented in [SESSION_HANDOFF_2026-03-22_36-2-1.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_36-2-1.md), backed by [analyze_36_2_1.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_36_2_1.py), and narrowed into a current-code AVR state test in [ThirtySixMinus21.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/ThirtySixMinus21/ThirtySixMinus21.cpp); the remaining blocker is still the decoder's unresolved single-gap tooth-number assignment, not the physical wheel topology
  - the `36-2-2-2` H4 state model is now documented in [SESSION_HANDOFF_2026-03-22_36-2-2-2.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_36-2-2-2.md) and narrowed into a current-code AVR state suite in [ThirtySixMinus222.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/ThirtySixMinus222/ThirtySixMinus222.cpp); the remaining blocker is now staged real-capture normalization for replay rather than basic H4 sync-path coverage
  - the `420a` current-code secondary-falling sync / realignment / wrap contract is now documented in [SESSION_HANDOFF_2026-03-23_420A_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_420A_STATE.md) and has direct AVR state coverage in [FourTwentyA.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/FourTwentyA/FourTwentyA.cpp), locking the current primary-high tooth-`13`, primary-low tooth-`5`, sync-loss realignment, and `16`-tooth wrap behavior
  - the `Audi 135` current-code sync / decimation contract is now documented in [SESSION_HANDOFF_2026-03-22_AUDI135.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_AUDI135.md) and has direct AVR state coverage in [Audi135.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Audi135/Audi135.cpp), complementing the older replay-backed slice with explicit every-third-tooth and resync assertions
  - the `Basic Distributor` current-code sync / wrap / filter contract is now documented in [SESSION_HANDOFF_2026-03-23_BASIC_DISTRIBUTOR_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_BASIC_DISTRIBUTOR_STATE.md) and has direct AVR state coverage in [BasicDistributor.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/BasicDistributor/BasicDistributor.cpp), complementing the older replay-backed slice with explicit stroke-dependent tooth-angle, first-pulse sync, wrap, and lite-filter assertions
  - the `Daihatsu +1` current-code sync model is now documented in [SESSION_HANDOFF_2026-03-22_DAIHATSU.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_DAIHATSU.md) and has direct AVR state coverage in [Daihatsu.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Daihatsu/Daihatsu.cpp) for both `3`- and `4`-cylinder paths
  - the `Ford ST170` current-code missing-tooth half-sync / cam-group promotion / noise-filter / regroup contract is now documented in [SESSION_HANDOFF_2026-03-23_FORDST170_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_FORDST170_STATE.md) and has direct AVR state coverage in [ForsdST170.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/FordST170/ForsdST170.cpp), complementing the replay-backed slice with explicit half-sync, cam-before-gap full sync, short-noise filtering, and sixth-tooth regroup assertions
  - the `GM 24X` cam-reset sync contract is now documented in [SESSION_HANDOFF_2026-03-22_GM24X_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_GM24X_STATE.md) and has direct AVR state coverage in [Gm24X.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Gm24X/Gm24X.cpp), complementing the older replay-backed slice with explicit sentinel, lookup-table, and resync assertions
  - the `GM 7X` current-code short-gap sync contract is now documented in [SESSION_HANDOFF_2026-03-23_GM7X_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_GM7X_STATE.md) and has direct AVR state coverage in [Gm7X.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Gm7X/Gm7X.cpp), complementing the older replay-backed slice with explicit `tooth 3` sync, no-sync, and wrap assertions
  - the `Harley` current-code high-state sync / two-tooth toggle / low-state sync-loss contract is now documented in [SESSION_HANDOFF_2026-03-23_HARLEY_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_HARLEY_STATE.md) and has direct AVR state coverage in [Harley.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Harley/Harley.cpp), complementing the older replay-backed slice with explicit setup, tooth-`1`, tooth-`2`, and sync-drop assertions
  - the `Jeep 2000` cam-reset sync contract is now documented in [SESSION_HANDOFF_2026-03-22_JEEP2000_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_JEEP2000_STATE.md) and has direct AVR state coverage in [Jeep2000.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Jeep2000/Jeep2000.cpp), complementing the older replay-backed slice with explicit sentinel, grouped-tooth, and next-revolution assertions
  - the `Mazda AU` current-code sync model is now documented in [SESSION_HANDOFF_2026-03-22_MAZDAAU.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_MAZDAAU.md) and has direct AVR state coverage in [MazdaAU.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/MazdaAU/MazdaAU.cpp), including the current third-cam-tooth sync path and the odd unsynced short-gap counter mutation
  - the `Miata 99-05` current-code cam-edge sync / noise deferral / wrap contract is now documented in [SESSION_HANDOFF_2026-03-23_MIATA9905_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_MIATA9905_STATE.md) and has direct AVR state coverage in [Miata9905.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Miata9905/Miata9905.cpp), complementing the older replay-backed slice with explicit sentinel, tooth-`6` sync, noisy extra-edge deferral, and `8`-edge wrap assertions
  - the `Missing-Tooth 36-1` current-code gap-sync / half-sync / cam-promotion / early-gap-loss contract is now documented in [SESSION_HANDOFF_2026-03-23_MISSINGTOOTH36_1_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_MISSINGTOOTH36_1_STATE.md) and has direct AVR state coverage in [missing_tooth.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/missing_tooth/missing_tooth.cpp), complementing the replay-backed slice with explicit tooth-`1` sync, primary-only sequential half-sync retention, cam promotion, and early-gap sync-loss assertions
  - the `NGC4` current-code wide-missing-tooth half-sync / seeded cam-phase full-sync contract is now documented in [SESSION_HANDOFF_2026-03-23_NGC4_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_NGC4_STATE.md) and has direct AVR state coverage in [test_ngc.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/NGC/test_ngc.cpp), complementing the replay-backed slice with explicit half-sync, phase-`1`, and phase-`5` assertions
  - the `Nissan360` current-code window-mapping / invalid-window rejection / wrap / kept-`useResync` contract is now documented in [SESSION_HANDOFF_2026-03-23_NISSAN360_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_NISSAN360_STATE.md) and has direct AVR state coverage in [Nissan360.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Nissan360/Nissan360.cpp), complementing the replay-backed slice with explicit 4/6/8-cylinder window mappings, invalid-window rejection, synced wrap, and the kept positive `useResync` realignment assertion
  - the `Renix` current-code virtual-tooth advancement / short-gap filtering contract is now documented in [SESSION_HANDOFF_2026-03-23_RENIX_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_RENIX_STATE.md) and has direct AVR state coverage in [renix.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/renix/renix.cpp), complementing the replay-backed slice with explicit valid-group advancement and isolated short-gap rejection assertions
  - the Honda J32 primary-only slice is now documented in [SESSION_HANDOFF_2026-03-22_HONDAJ32.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_HONDAJ32.md) and has dedicated state-machine coverage in [HondaJ32.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/HondaJ32/HondaJ32.cpp)
  - the `Honda D17` current-code short-gap sync / no-sync / wrap contract is now documented in [SESSION_HANDOFF_2026-03-23_HONDAD17_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_HONDAD17_STATE.md) and has direct AVR state coverage in [HondaD17.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/HondaD17/HondaD17.cpp), complementing the older replay-backed slice with explicit setup, `13th`-tooth sync, uniform-gap rejection, and wrap assertions
  - the `Subaru 6/7` current-code cam-count sync / misalignment recovery / wrap contract is now documented in [SESSION_HANDOFF_2026-03-23_SUBARU67_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_SUBARU67_STATE.md) and has direct AVR state coverage in [Subaru67.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Subaru67/Subaru67.cpp), complementing the older replay-backed slice with explicit tooth-`2` sync, tooth-`5` realignment, and `12`-tooth wrap assertions
  - the `Vmax` current-code wide-lobe sync / small-lobe increment / tooth-`6` wrap contract is now documented in [SESSION_HANDOFF_2026-03-23_VMAX_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_VMAX_STATE.md) and has direct AVR state coverage in [Vmax.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Vmax/Vmax.cpp), locking the current wide-lobe tooth-`1` sync, tooth-`2` advance, and tooth-`6` wrap behavior
  - the current Rover-specific notes are [SESSION_HANDOFF_2026-03-21_ROVER_MEMS_NOTE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-21_ROVER_MEMS_NOTE.md), [SESSION_HANDOFF_2026-03-22_ROVER_MEMS_BIT_WINDOW.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_ROVER_MEMS_BIT_WINDOW.md), [SESSION_HANDOFF_2026-03-22_ROVER_MEMS_CAM_532.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_ROVER_MEMS_CAM_532.md), and [SESSION_HANDOFF_2026-03-22_ROVER_MEMS_TOOTH_LOG.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_ROVER_MEMS_TOOTH_LOG.md); Rover attachments are now staged locally in [Resources/rover_mems_evidence](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence), the composite-log CSV channel mapping is solved with [tools/parse_speeduino_composite_csv.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/parse_speeduino_composite_csv.py), the Rover PDF image corpus is extracted with [tools/extract_pdf_images.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/extract_pdf_images.py), the extracted images are OCR-indexed with [tools/index_pdf_images.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/index_pdf_images.py), PDF readiness is checked with [tools/inspect_pdf_evidence.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/inspect_pdf_evidence.py), full Rover pages are rendered with [tools/render_pdf_pages.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/render_pdf_pages.py), the manuals now map OEM Rover layouts onto Speeduino's `11-5-12-4` and `2-14-3-13` patterns, the rolling `roverMEMSTeethSeen` bit window is now derived in [derive_rover_mems_windows.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/derive_rover_mems_windows.py), and a real June 25 post-fix tooth logger now host-side-proves Rover pattern `#1` via [analyze_rover_mems_tooth_log.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_rover_mems_tooth_log.py); the remaining blocker is still the exact `5-3-2 cam` phase alignment needed for full-sync Rover replay rather than primary-wheel identification itself

## Phase 3: Runtime Structure

- Split [init.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/init.cpp) by responsibility:
  - startup and board bring-up
  - config-derived table construction
  - pin and interrupt wiring
  - decoder initialization
- Reduce logic in [speeduino.ino](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/speeduino.ino) so it mainly orchestrates scheduled work.
- Isolate high-risk domains behind explicit module boundaries:
  - knock
  - engine protection
  - idle control
  - status/log export
- Move toward board-scoped feature configuration files for hardware-dependent subsystems such as knock, trigger inputs, and built-in peripherals instead of spreading those assumptions across common code and INI defaults.
- Prefer subsystem-owned state structs for complex runtime domains so calculations and live data stay close together.

## Phase 4: Board And Comms Consistency

- Push board-specific behavior further into the `board_*` layer.
- Audit interrupt-capable pin assumptions and ADC differences across supported MCUs.
- Align status/logging/comms outputs so new runtime features are exposed consistently across protocols.
- Deprecate or clearly mark partial legacy telemetry paths.
- Continue extracting testable helpers for legacy packet/framing paths before replacing more placeholder fields, so future cleanup stays evidence-based instead of ad hoc.
- Add explicit board capability declarations for features like onboard knock hardware, trigger channels, CAN variants, and logging support so the firmware and tuning surface can hide unsupported options instead of exposing unsafe generic defaults.
- Use official board docs such as [wiki.speeduino.com/en/boards/Dropbear](https://wiki.speeduino.com/en/boards/Dropbear) as a primary source when extracting board-scoped defaults and capabilities:
  - DropBear docs explicitly document board pin roles for `Boost`, `Idle`, `VVT`, and spare digital inputs usable for `VSS` / `Idle Up`
  - the same page documents board-specific crank hardware filtering guidance and MAP-source switching
  - future packaged-tune and board-profile work should prefer these board docs over generic INI assumptions wherever they disagree

## Phase 5: Configuration And Observability

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

- Treat Teensy 4.1 as a first-class platform, not just a faster AVR replacement.
- Move capability decisions behind explicit board declarations for SD, RTC, native CAN, onboard SPI flash, trigger hardware, and driver chips so runtime code and the tuning surface can distinguish generic MCU support from specific board support.
- Add a Teensy/DropBear storage path that uses onboard SPI flash for tune persistence, tune banks, migration staging, and higher-rate diagnostic capture instead of constraining new features to the legacy EEPROM layout.
- Increase Teensy-only tune transport limits after storage is decoupled:
  - larger page sizes
  - larger blocking factors
  - larger output-channel payloads
  - higher-resolution 3D tables
- Rework Teensy 4.1 timing and peripheral usage where the current board layer is still unfinished or AVR-shaped:
  - drain all pending timer flags per ISR instead of single `else if` servicing
  - stabilize native CAN and expose the real capability cleanly
  - finish PWM fan support with a proper Teensy timer instead of leaving it disabled
  - add a Teensy-specific ADC backend with higher-resolution sampling / averaging
- Use the existing ESP32-C3 board hardware as a real secondary transport / coprocessor path for wireless tuning, log offload, and update workflows once the board capability layer exists.

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
- The `megaatmega2560_sim_unittest` decoder baseline is green again at `255/255`; keep it that way as new evidence-backed slices land.

## Trouble Areas

- `Rover MEMS` full replay remains intentionally deferred:
  - the repo still lacks a safe tooth-numbered mapping from a logged cam transition to the decoder's `secondaryToothCount == 6 / 4 / 3` gap events needed for full `Crank Speed + 5-3-2 cam` replay
  - treat this as an evidence-conversion blocker, not as a signal to synthesize more speculative traces
- `4G63` direct AVR state coverage is still not safe to land under the current harness:
  - prior direct pin-state-driven attempts perturbed unrelated decoder tests under `simavr`
  - keep replay coverage, but do not re-land a direct state suite until the isolation issue is understood
- `Nissan360` has a currently unresolved order-sensitive interaction with existing replay coverage:
  - an added direct `useResync` state assertion was backed out after it caused a later Harley replay failure in the full `test_decoders` entrypoint
  - the Nissan360 behavior itself was plausible, but the harness interaction was not understood well enough to keep
- harness rule for these blocked slices:
  - prefer the last verified green decoder baseline over landing a brittle regression
  - if a candidate slice only passes in isolation but perturbs the full decoder entrypoint, back it out and record the interaction here before moving on
