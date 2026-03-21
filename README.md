# Speeduino AI Edition

[![License](https://img.shields.io/badge/license-GPLv3-blue.svg)](LICENSE)
[![Based on Speeduino](https://img.shields.io/badge/based%20on-Speeduino%20202501.6-green.svg)](https://github.com/noisymime/speeduino)
[![Platform](https://img.shields.io/badge/platform-PlatformIO-orange.svg)](https://platformio.org/)

An AI-enhanced fork of Speeduino focused on safety, correctness, regression coverage, and long-term maintainability.

## Project Overview

**Speeduino AI Edition** is a research and development fork of the [Speeduino Engine Management System](https://speeduino.com). This fork prioritizes defensive fixes, systematic testing, and structured roadmap-driven development over feature churn.

### Key Differences from Upstream

- Enhanced safety and correctness work in ignition, fueling, knock, and protection logic
- Broad unit-test coverage for decoder behavior, helpers, corrections, and config migrations
- A trigger trace replay harness for host-side decoder regression testing
- Ongoing Teensy 4.1 platform enablement work
- A phased development roadmap in [FIRMWARE_ROADMAP.md](speeduino/FIRMWARE_ROADMAP.md)

## Table of Contents

- [What is Speeduino?](#what-is-speeduino)
- [AI Edition Features](#ai-edition-features)
- [Current Development Phase](#current-development-phase)
- [Quick Start](#quick-start)
- [Building the Firmware](#building-the-firmware)
- [Running Tests](#running-tests)
- [Project Structure](#project-structure)
- [Development Roadmap](#development-roadmap)
- [Session Handoffs](#session-handoffs)
- [Contributing](#contributing)
- [Important Warnings](#important-warnings)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## What is Speeduino?

Speeduino is an open-source, Arduino-based engine management system that provides:

- Fuel and ignition control
- Support for 1-12 cylinder engines
- 60+ trigger decoder patterns
- TunerStudio tuning support
- Low-cost DIY-friendly hardware

Upstream references:

- Official website: https://speeduino.com
- Documentation: https://wiki.speeduino.com
- Original repository: https://github.com/noisymime/speeduino

## AI Edition Features

### Phase 1: Safety and Correctness

- Fixed high-risk ignition, fueling, and protection defects
- Corrected knock and limiter/protection interaction issues
- Replaced misleading placeholder telemetry in key paths
- Added focused regression coverage for corrected logic

### Phase 2: Regression Harness

- **Decoder Trace Replay System**
  - Externalized trace headers stored in flash/program memory
  - Support for simple traces and compressed repeated-event traces
  - Coverage for 17 decoder families with real or semi-real waveform patterns
  - Unit-test trigger-state overrides for decoders that sample live input levels during the ISR path
- **Config Migration Tests**
  - Direct host-side coverage across the EEPROM update chain from `v2 -> v24`
  - Split `test_updates` / `test_updates_tail` harness to keep full migration coverage stable under `simavr`
- **Runtime State-Machine Tests**
  - Direct validation of decoder sync, wrap, filtering, and resync behavior
- **Ignition / Protection Interaction Tests**
  - Launch, flat-shift, rolling cut, AFR protection, and combined protection precedence coverage
- **Sensor State-Machine Tests**
  - MAP sampling fallback/reset behavior for cycle-average, cycle-minimum, and event-average modes
  - `mapSwitchPoint` boundary transitions and EMAP-disabled sentinel preservation
  - MAP / EMAP / baro calibration helper coverage for clamping and enabled/disabled source behavior

**Current decoder replay/runtime coverage** (`194/194` decoder tests passing):

- Basic Distributor
- Dual-Wheel
- Ford ST170
- Suzuki K6A
- Missing-Tooth 36-1
- NGC4
- Nissan360
- Renix
- Miata 99-05
- Jeep 2000
- Audi 135
- GM 24X
- 4G63
- GM 7X
- Honda D17
- Harley
- Subaru 6/7

Highlighted behaviors now covered include:

- clean sync establishment
- noisy edge rejection and short-gap filtering
- wrap and revolution counting
- resync boundaries
- no-cam / no-sync rejection
- mixed secondary-edge behavior
- Miata cam-edge noise deferral without false sync

### Phase 3-6: Planned

- Runtime structure improvements
- Board and comms consistency work
- Configuration and observability improvements
- Teensy 4.1 platform enablement and board-capability cleanup

See [FIRMWARE_ROADMAP.md](speeduino/FIRMWARE_ROADMAP.md) for the full roadmap.

## Current Development Phase

**Phase 2: Regression Harness** is still the active phase.

Recent work as of `2026-03-21`:

- Completed direct migration coverage across the EEPROM update chain from `v2 -> v24`
- Split migration coverage into stable `test_updates` and `test_updates_tail` suites
- Expanded replay-backed decoder coverage to 15+ decoder families
- Added Subaru 6/7 replay coverage for three-cam sync, wrap, and single-cam misalignment behavior
- Added ignition/protection state-machine coverage for rolling cut, VSS-gated launch, AFR target-table mode, and combined boost/AFR precedence
- Added MAP sampling reset/fallback coverage for sensor state transitions, including `mapSwitchPoint` boundary resets, EMAP-disabled sentinel handling, and MAP/baro calibration helper behavior
- Added a host-side stock-base-tune compatibility audit for [Resources/Speeduino base tune.msq](Resources/Speeduino%20base%20tune.msq) against [speeduino.ini](speeduino.ini), now covering the full round-trippable constant surface plus selected fork-critical default values
- Added a fork-owned compatible base tune at [Resources/Speeduino AI base tune.msq](Resources/Speeduino%20AI%20base%20tune.msq) that closes the current stock-tune drift
- Verified `test_decoders`: `194/194`
- Verified `test_updates`: `38/38`
- Verified `test_updates_tail`: `5/5`
- Verified `test_ign`: `146/146`
- Verified `test_sensors`: `50/50`

Latest handoff references:

- [SESSION_HANDOFF_2026-03-21_HARLEY_REPLAY.md](speeduino/SESSION_HANDOFF_2026-03-21_HARLEY_REPLAY.md)
- [SESSION_HANDOFF_2026-03-21_STOCK_TUNE_AUDIT.md](speeduino/SESSION_HANDOFF_2026-03-21_STOCK_TUNE_AUDIT.md)
- [SESSION_HANDOFF_2026-03-21_FORK_BASE_TUNE.md](speeduino/SESSION_HANDOFF_2026-03-21_FORK_BASE_TUNE.md)
- [SESSION_HANDOFF_2026-03-21_TUNE_DEFAULT_VALUES.md](speeduino/SESSION_HANDOFF_2026-03-21_TUNE_DEFAULT_VALUES.md)
- [SESSION_HANDOFF_2026-03-21_SUBARU67_REPLAY.md](speeduino/SESSION_HANDOFF_2026-03-21_SUBARU67_REPLAY.md)
- [SESSION_HANDOFF_2026-03-21_BASIC_DISTRIBUTOR.md](speeduino/SESSION_HANDOFF_2026-03-21_BASIC_DISTRIBUTOR.md)
- [SESSION_HANDOFF_2026-03-21_HONDAD17.md](speeduino/SESSION_HANDOFF_2026-03-21_HONDAD17.md)
- [SESSION_HANDOFF_2026-03-21_MAP_SWITCHPOINT_EMAP.md](speeduino/SESSION_HANDOFF_2026-03-21_MAP_SWITCHPOINT_EMAP.md)
- [SESSION_HANDOFF_2026-03-21_MAP_BARO_HELPERS.md](speeduino/SESSION_HANDOFF_2026-03-21_MAP_BARO_HELPERS.md)
- [SESSION_HANDOFF_2026-03-21_MAP_SAMPLING.md](speeduino/SESSION_HANDOFF_2026-03-21_MAP_SAMPLING.md)
- [SESSION_HANDOFF_2026-03-21_LIMITER_AFR_VSS.md](speeduino/SESSION_HANDOFF_2026-03-21_LIMITER_AFR_VSS.md)
- [SESSION_HANDOFF_2026-03-21_UPDATES_V15_V18.md](speeduino/SESSION_HANDOFF_2026-03-21_UPDATES_V15_V18.md)

## Quick Start

### Prerequisites

- [PlatformIO Core](https://platformio.org/) or PlatformIO IDE
- Git
- Optional: [TunerStudio](https://www.tunerstudio.com/)

### Clone the Repository

```bash
git clone https://github.com/BuffJesus/SpeeduinoAIEdition.git
cd SpeeduinoAIEdition
```

## Building the Firmware

### Teensy 4.1

```bash
pio run -e teensy41
```

Output: `.pio/build/teensy41/firmware.hex`

Release bundle for the current checked-in Teensy 4.1 packaging:
- [speeduino-dropbear-v2.0.1-teensy41.hex](release/speeduino-dropbear-v2.0.1-teensy41.hex)
- [speeduino-dropbear-v2.0.1.ini](release/speeduino-dropbear-v2.0.1.ini)
- [speeduino-dropbear-v2.0.1-base-tune.msq](release/speeduino-dropbear-v2.0.1-base-tune.msq)

### Arduino Mega 2560

```bash
pio run -e megaatmega2560
```

See `platformio.ini` for the full environment list.

## Running Tests

### Run All Unit Tests

```bash
pio test -e megaatmega2560_sim_unittest
```

### Run Specific Test Suites

```bash
# Decoder tests
pio test -e megaatmega2560_sim_unittest --filter test_decoders

# Config migration tests
pio test -e megaatmega2560_sim_unittest --filter test_updates
pio test -e megaatmega2560_sim_unittest --filter test_updates_tail

# Ignition/protection tests
pio test -e megaatmega2560_sim_unittest --filter test_ign

# Sensor tests
pio test -e megaatmega2560_sim_unittest --filter test_sensors

# Stock base tune compatibility audit
python tools/check_stock_base_tune_compat.py

# Fork-owned base tune compatibility audit
python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq"

# Release bundle base tune compatibility audit
python tools/check_stock_base_tune_compat.py --msq "release/speeduino-dropbear-v2.0.1-base-tune.msq" --ini "release/speeduino-dropbear-v2.0.1.ini"
```

### Current Test Status

- `194/194` decoder tests passing
- `38/38` config migration tests passing
- `5/5` migration tail tests passing
- `146/146` ignition/protection tests passing
- `50/50` sensor tests passing
- Stock base tune audit currently flags one remaining fork-surface drift: missing `knock_limiterDisable` in [Resources/Speeduino base tune.msq](Resources/Speeduino%20base%20tune.msq)
- Stock base tune also fails the 92-check fork-default value contract for knock / rolling-cut / DFCO / launch / idle-advance / idle-up / VSS / WMI / oil-pressure / fan / air-con defaults, as expected for the unchanged stock artifact
- Fork-owned base tune audit passes for [Resources/Speeduino AI base tune.msq](Resources/Speeduino%20AI%20base%20tune.msq), including the 92 selected fork-critical default values
- Release bundle base tune audit passes for [speeduino-dropbear-v2.0.1-base-tune.msq](release/speeduino-dropbear-v2.0.1-base-tune.msq) against [speeduino-dropbear-v2.0.1.ini](release/speeduino-dropbear-v2.0.1.ini), including the same 92 selected fork-critical default values
- Other unit-test suites remain in regular use for regression checking

Note: local Windows `pio test` invocations in this workspace can still hit wrapper/file-lock issues intermittently even when the produced simulator binary itself runs cleanly.

## Project Structure

```text
SpeeduinoAIEdition/
|-- speeduino/                  # Main firmware source
|   |-- decoders.cpp           # Trigger decoder implementations
|   |-- corrections.cpp        # Fueling/ignition corrections
|   |-- updates.cpp            # Config migration logic
|   |-- FIRMWARE_ROADMAP.md    # Development roadmap
|   `-- SESSION_HANDOFF_*.md   # Development session notes
|-- test/                      # Unit test suite
|   |-- test_decoders/         # Decoder validation tests
|   |   `-- traces/            # Externalized trigger trace headers
|   |-- test_updates/          # Config migration tests
|   |-- test_updates_tail/     # Late migration tail split for simulator stability
|   |-- test_ign/             # Ignition / protection interaction tests
|   |-- test_sensors/         # Sensor filtering and MAP sampling tests
|   `-- test_maths/           # Math/correction tests
|-- release/                   # Release artifacts (.hex, .ini)
|-- reference/                 # Hardware designs and documentation
|-- platformio.ini             # Build configuration
|-- README.md
`-- LICENSE
```

## Development Roadmap

This fork follows a structured phase-based roadmap:

1. **Phase 1: Safety and Correctness** - complete
2. **Phase 2: Regression Harness** - in progress
3. **Phase 3: Runtime Structure** - planned
4. **Phase 4: Board and Comms Consistency** - planned
5. **Phase 5: Configuration and Observability** - planned
6. **Phase 6: Teensy 4.1 Platform Enablement** - planned

See [FIRMWARE_ROADMAP.md](speeduino/FIRMWARE_ROADMAP.md) for details.

## Session Handoffs

Development progress is tracked in session handoff documents that capture:

- changes made
- verification performed
- discovered constraints or bugs
- recommended next steps

Recent handoffs:

- [SESSION_HANDOFF_2026-03-21_HARLEY_REPLAY.md](speeduino/SESSION_HANDOFF_2026-03-21_HARLEY_REPLAY.md) - trigger-state override seam and Harley replay coverage
- [SESSION_HANDOFF_2026-03-21_BASIC_DISTRIBUTOR.md](speeduino/SESSION_HANDOFF_2026-03-21_BASIC_DISTRIBUTOR.md) - Basic distributor replay coverage
- [SESSION_HANDOFF_2026-03-21_HONDAD17.md](speeduino/SESSION_HANDOFF_2026-03-21_HONDAD17.md) - Honda D17 replay coverage and startup-gap harness fix
- [SESSION_HANDOFF_2026-03-21_MAP_BARO_HELPERS.md](speeduino/SESSION_HANDOFF_2026-03-21_MAP_BARO_HELPERS.md) - MAP / EMAP / baro conversion helper coverage
- [SESSION_HANDOFF_2026-03-21_MAP_SWITCHPOINT_EMAP.md](speeduino/SESSION_HANDOFF_2026-03-21_MAP_SWITCHPOINT_EMAP.md) - MAP switch-point boundary and EMAP sentinel coverage
- [SESSION_HANDOFF_2026-03-21_MAP_SAMPLING.md](speeduino/SESSION_HANDOFF_2026-03-21_MAP_SAMPLING.md) - MAP sampling reset/fallback edge cases
- [SESSION_HANDOFF_2026-03-21_LIMITER_AFR_VSS.md](speeduino/SESSION_HANDOFF_2026-03-21_LIMITER_AFR_VSS.md) - AFR target-table and VSS-gated limiter coverage
- [SESSION_HANDOFF_2026-03-21_UPDATES_V15_V18.md](speeduino/SESSION_HANDOFF_2026-03-21_UPDATES_V15_V18.md) - split migration harness and mid-chain migration coverage
- [SESSION_HANDOFF_2026-03-20_MIATA_NOISE.md](speeduino/SESSION_HANDOFF_2026-03-20_MIATA_NOISE.md) - Miata 99-05 cam-noise replay coverage
- [SESSION_HANDOFF_2026-03-20_UPDATES_V5_V7.md](speeduino/SESSION_HANDOFF_2026-03-20_UPDATES_V5_V7.md) - final EEPROM relocation migration coverage
- [SESSION_HANDOFF_2026-03-20_UPDATES_V2_V10.md](speeduino/SESSION_HANDOFF_2026-03-20_UPDATES_V2_V10.md) - early migration coverage
- [SESSION_HANDOFF_2026-03-20_UPDATES_V10_V13.md](speeduino/SESSION_HANDOFF_2026-03-20_UPDATES_V10_V13.md) - mid-chain migration coverage
- [SESSION_HANDOFF_2026-03-20_UPDATES_V13_V15.md](speeduino/SESSION_HANDOFF_2026-03-20_UPDATES_V13_V15.md) - PID and calibration migration coverage
- [SESSION_HANDOFF_2026-03-20_UPDATES_V18_V19.md](speeduino/SESSION_HANDOFF_2026-03-20_UPDATES_V18_V19.md) - TPS migration coverage and Spark 2 migration bug fix
- [SESSION_HANDOFF_2026-03-20_GM7X.md](speeduino/SESSION_HANDOFF_2026-03-20_GM7X.md) - GM 7X replay coverage
- [SESSION_HANDOFF_2026-03-20_4G63.md](speeduino/SESSION_HANDOFF_2026-03-20_4G63.md) - 4G63 replay coverage
- [SESSION_HANDOFF_2026-03-20_24X.md](speeduino/SESSION_HANDOFF_2026-03-20_24X.md) - GM 24X replay coverage
- [SESSION_HANDOFF_2026-03-21.md](speeduino/SESSION_HANDOFF_2026-03-21.md) - Miata 99-05 baseline replay coverage
- [SESSION_HANDOFF_2026-03-20.md](speeduino/SESSION_HANDOFF_2026-03-20.md) - trace replay harness expansion
- [SESSION_HANDOFF_2026-03-19.md](speeduino/SESSION_HANDOFF_2026-03-19.md) - config migration coverage groundwork
- [SESSION_HANDOFF_2026-03-16.md](speeduino/SESSION_HANDOFF_2026-03-16.md) - decoder state-machine tests

## Contributing

This is an experimental research fork. Contributions are welcome, but changes should follow these rules:

1. Safety and correctness come first.
2. New behavior should come with regression coverage.
3. Work should align with the active roadmap phase where practical.
4. Documentation should be updated when the repo state materially changes.

Typical workflow:

```bash
git checkout -b feature/your-change
pio test -e megaatmega2560_sim_unittest
pio run -e teensy41
git commit -m "Describe the change"
git push origin feature/your-change
```

See [contributing.md](contributing.md) for upstream-inherited contribution details.

## Important Warnings

### Experimental Software

- Do not use this fork on a real engine without substantial bench validation.
- This repository intentionally carries experimental changes that may diverge from upstream assumptions.
- Always keep a backup ECU and safety plan.

### Compatibility Notes

- This fork is not guaranteed to stay tune-compatible with upstream Speeduino.
- INI and migration behavior may diverge when safety/testability requires it.
- Teensy 4.1 remains the primary platform direction, but the simulator-based AVR harness is still heavily used for testing.

### Liability

This software is provided "as is" without warranty. The authors and contributors are not responsible for damage resulting from its use.

## License

This project inherits the GPLv3 license from upstream Speeduino.

See [LICENSE](LICENSE) for the full text.

## Acknowledgments

- The upstream Speeduino project and contributors
- PlatformIO and Unity test infrastructure
- The rusEFI project for verification-discipline inspiration in trigger and migration testing
