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

## Phase 5: Configuration And Observability

- Treat [updates.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/updates.cpp) as a first-class subsystem with validation and tests.
- Add clearer runtime observability for feature state, block conditions, and fault reasons.
- Prefer explicit status and counters over ambiguous flags when debugging field issues.
- Add a structured tune-migration pipeline instead of one-off migration edits, with ordered migrators and tests for shape changes, renames, unit conversions, and default filling.
- Add explicit tune-compatibility policy and regression checks for stock Speeduino `.msq` files:
  - keep the stock `speeduino 202501` signature only while the stock base tune imports cleanly into the current INI surface
  - add a host-side compatibility audit against [Resources/Speeduino base tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino%20base%20tune.msq) covering signature, page count, required constants, and fork-added fields that rely on defaults or migrations
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
