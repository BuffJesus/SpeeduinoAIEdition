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
  - Coverage for 12+ decoder families with real or semi-real waveform patterns
- **Config Migration Tests**
  - Direct host-side coverage across the EEPROM update chain from `v2 -> v24`
- **Runtime State-Machine Tests**
  - Direct validation of decoder sync, wrap, filtering, and resync behavior

**Current decoder replay/runtime coverage** (`182/182` decoder tests passing):

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

Recent work as of `2026-03-20`:

- Completed direct migration coverage across the EEPROM update chain from `v2 -> v24`
- Added replay-backed Miata 99-05 cam-noise coverage
- Expanded replay-backed decoder coverage to 12+ decoder families
- Verified `test_decoders`: `182/182`
- Verified `test_updates`: `38/38`

Latest handoff references:

- [SESSION_HANDOFF_2026-03-20_MIATA_NOISE.md](speeduino/SESSION_HANDOFF_2026-03-20_MIATA_NOISE.md)
- [SESSION_HANDOFF_2026-03-20_UPDATES_V5_V7.md](speeduino/SESSION_HANDOFF_2026-03-20_UPDATES_V5_V7.md)

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

# Math/correction tests
pio test -e megaatmega2560_sim_unittest --filter test_maths
```

### Current Test Status

- `182/182` decoder tests passing
- `38/38` config migration tests passing
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
|   `-- test_maths/            # Math/correction tests
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
