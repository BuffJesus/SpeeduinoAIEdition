# Speeduino AI Edition

[![License](https://img.shields.io/badge/license-GPLv3-blue.svg)](LICENSE)
[![Based on Speeduino](https://img.shields.io/badge/based%20on-Speeduino%20202501.6-green.svg)](https://github.com/noisymime/speeduino)
[![Platform](https://img.shields.io/badge/platform-PlatformIO-orange.svg)](https://platformio.org/)

Speeduino AI Edition is a maintenance-focused fork of Speeduino. The goal is straightforward: improve safety, make behavior easier to verify, and keep the firmware easier to evolve without relying on guesswork.

This repository tracks real firmware work, test infrastructure, board support work, and supporting research notes. It is not a minimal fork and it is not intended to be a drop-in "same as upstream, plus a few patches" repo.

## What This Fork Prioritizes

- Safety and correctness fixes before feature growth
- Regression coverage for decoder, sensor, migration, idle, launch, and protection behavior
- Better board-layer separation, especially for Teensy 4.1 and DropBear-specific behavior
- Explicit documentation of roadmap decisions and blocked areas
- Evidence-driven work when decoder behavior or tune compatibility is unclear

## Current State

The repository has moved well past the older "Phase 2 only" state.

- Phase 3: substantially complete
- Phase 4: complete
- Phase 5: complete
- Phase 6 through Phase 10: active roadmap work with multiple completed slices already documented in [speeduino/FIRMWARE_ROADMAP.md](speeduino/FIRMWARE_ROADMAP.md)

Recent completed work includes:

- board capability output channels and interrupt-pin validation
- SPI flash-backed persistence work for Teensy 4.1
- Teensy 4.1 ADC normalization and averaging fixes
- larger Teensy transport limits and expanded logger payloads
- broader state-machine tests for idle, launch/flat shift, and engine protection
- experimental native `U16` TunerStudio transport for page `2` on the Teensy 4.1 / DropBear path

For the authoritative status, read [speeduino/FIRMWARE_ROADMAP.md](speeduino/FIRMWARE_ROADMAP.md).

## Repository Layout

```text
speeduino-202501.6/
|-- speeduino/          # Main firmware sources
|-- test/               # PlatformIO unit/integration-style test suites
|-- tools/              # Host-side analysis and validation tools
|-- Resources/          # Notes, evidence, manuals, tunes, and helper scripts
|-- release/            # Packaged firmware / INI / tune artifacts
|-- reference/          # Hardware references
|-- platformio.ini      # PlatformIO environments
|-- speeduino.ini       # Main TunerStudio INI
`-- README.md
```

Key firmware files:

- [speeduino/speeduino.ino](speeduino/speeduino.ino)
- [speeduino/init.cpp](speeduino/init.cpp)
- [speeduino/decoders.cpp](speeduino/decoders.cpp)
- [speeduino/storage.cpp](speeduino/storage.cpp)
- [speeduino/logger.cpp](speeduino/logger.cpp)

## Building

Prerequisites:

- PlatformIO Core
- Git
- Python for host-side tooling

Clone:

```bash
git clone https://github.com/BuffJesus/SpeeduinoAIEdition.git
cd SpeeduinoAIEdition
```

Common firmware builds:

```bash
# AVR Mega 2560
pio run -e megaatmega2560

# Teensy 4.1
pio run -e teensy41
```

Other supported environments are listed in [platformio.ini](platformio.ini), including AVR, STM32, SAMD21, SAME51, Teensy 3.x, and the simulator test environment.

## Running Tests

Primary simulator environment:

```bash
pio test -e megaatmega2560_sim_unittest
```

Useful targeted runs:

```bash
pio test -e megaatmega2560_sim_unittest --filter test_decoders
pio test -e megaatmega2560_sim_unittest --filter test_ign
pio test -e megaatmega2560_sim_unittest --filter test_idle
pio test -e megaatmega2560_sim_unittest --filter test_launch
pio test -e megaatmega2560_sim_unittest --filter test_protection
pio test -e megaatmega2560_sim_unittest --filter test_sensors
pio test -e megaatmega2560_sim_unittest --filter test_updates
pio test -e megaatmega2560_sim_unittest --filter test_updates_tail
```

The repo also includes host-side validation tools in [tools](tools) and supporting workflows in [Resources](Resources).

## Tuning And Release Artifacts

Important repo-level artifacts:

- [speeduino.ini](speeduino.ini)
- [speeduino-u16p2-experimental.ini](speeduino-u16p2-experimental.ini)
- [release](release)
- [Resources/Speeduino AI base tune.msq](Resources/Speeduino%20AI%20base%20tune.msq)

The packaged release artifacts in `release/` are the safest starting point for the fork-specific Teensy 4.1 / DropBear path. The experimental `U16` page-2 flow is separate and should be treated as experimental, not as the default production contract.

## Documentation

Start here:

- [speeduino/FIRMWARE_ROADMAP.md](speeduino/FIRMWARE_ROADMAP.md)
- [contributing.md](contributing.md)

The `speeduino/SESSION_HANDOFF_*.md` files are working notes. They are useful when continuing a specific slice, but they are not a replacement for the roadmap.

## Warnings

- Do not run this firmware on a real engine without proper bench validation.
- This fork intentionally diverges from upstream behavior in some areas.
- Tune compatibility, logger layout, and board behavior may differ from upstream where the fork has explicitly chosen safety or maintainability over strict parity.
- Experimental Teensy-only features should be treated as opt-in until validated on the target hardware.

## License

This project inherits the GPLv3 license from upstream Speeduino. See [LICENSE](LICENSE).

## Acknowledgments

- The upstream Speeduino project and contributors
- PlatformIO and Unity test tooling
- rusEFI, where its testing and verification discipline provided useful patterns
