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

Phases 3 through 11 are complete or substantially complete.

- Phase 3: substantially complete — knock module fully migrated to `KnockState` struct; legacy globals removed
- Phase 4: complete — board capability channels, interrupt-pin validation, comms helper extraction, DropBear pin-mapping extraction
- Phase 5: complete — updates.cpp coverage, knock observability flags, live-data map, logger byte regression tests
- Phase 6–8: complete — PWM fan, SPI flash persistence, ADC normalization, hardware averaging, transport limit raises
- Phase 9–10: complete — decoder tooth-number resolution, idle/launch/protection test suites, ESP32-C3 coprocessor transport wiring
- Phase 11: software complete — `Serial2` / Airbear TCP bridge wired and documented; remaining work is hardware bench validation
- Firmware Capability API sprint: complete — `FW-001` through `FW-007` all done, `test_comms` suite at 46/46

**Current simulator baseline:**

| Suite | Passing |
|-------|---------|
| test_comms | 46/46 (10 skipped — Teensy/U16P2 guards correct on AVR) |
| test_decoders | 275/275 |
| test_fuel | 88/88 |
| test_ign | 193/193 |
| test_idle | 10/10 |
| test_init | 9/9 |
| test_launch | 6/6 |
| test_math | 44/44 |
| test_protection | 19/19 |
| test_schedules | 26/26 |
| test_sensors | 75/75 |
| test_tables | 24/24 |
| test_updates | 38/38 |

Recent completed work includes:

- firmware capability `'K'` query command — 39-byte fixed binary response (board id, capability flags, feature flags, OCH size, signature)
- stable `BoardId` enum — `getStableBoardId()` maps pin mapping to `BOARD_ID_DROPBEAR_T41`, `BOARD_ID_BEAR_CUB`, or `BOARD_ID_UNKNOWN`
- external page CRC verification — `parseLegacyCrcRequest` / `encodeLegacyCrc32Response` / `calculatePageCRC32ForMode`, production and U16P2 mode-aware
- `OCH_OFFSET_*` named constants in `live_data_map.h` and `CAP_FEATURE_FLASH_HEALTH` / `CAP_FEATURE_U16P2` feature flag bits
- Phase 3 knock cleanup — `knockStartTime` and `knockLastRecoveryStep` legacy globals fully removed; `KnockState` struct is the sole owner
- `PID::Initialize()` promoted to public; `initialiseCorrections()` now calls it directly instead of the triple mode-toggle workaround
- VSS ISR noise filter — `vssPulseIsFiltered()` rejects contact-bounce and electrical noise (500 µs minimum gap); 10 regression tests
- `getSpeed()` race condition fix — `vssTimes[vssIndex]` stale-pulse check now reads under `noInterrupts()`; added divide-by-zero guard
- `setPinHysteresis` GDIR write wrapped in critical section — iMXRT1062 has no atomic alias for GPIO direction register
- Haltech CAN `DATA3` staging duty cycle — bytes 2–3 now report `PW2`-based duty cycle when staging is active
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

Documentation policy in this repo is intentionally narrow:

- `speeduino/FIRMWARE_ROADMAP.md` is the source of truth for project status and remaining work
- `speeduino/SESSION_HANDOFF_*.md` files are kept only when they still support an active decoder evidence/state slice or an active experimental/bench-validation slice
- obsolete handoffs are removed rather than accumulated as a second roadmap

The handoffs that still matter are the ones linked from the roadmap, plus the current experimental validation notes:

- [speeduino/SESSION_HANDOFF_2026-03-26_TS_U16_PAGES.md](speeduino/SESSION_HANDOFF_2026-03-26_TS_U16_PAGES.md)
- [speeduino/SESSION_HANDOFF_2026-03-28_TUNE_LEARN_VALIDATION.md](speeduino/SESSION_HANDOFF_2026-03-28_TUNE_LEARN_VALIDATION.md)

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
