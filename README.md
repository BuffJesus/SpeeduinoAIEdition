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

**Current decoder replay/runtime coverage** (`196/196` decoder tests passing):

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
- Rover MEMS
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
- Tightened the forum evidence collector so it uses canonical topic URLs, explicit decoder-family matching, and bounded thread crawling instead of raw noisy search-result dumps
- Added project-doc evidence from the official wiki, especially the DropBear board page, to support future board-profile and packaged-tune decisions with primary Speeduino documentation
- Added an external decoder research baseline from local rusEFI and MS3 source trees, narrowing the next blocked decoder research priorities to `Rover MEMS` first and `Honda J32` second, with `36-2-1` now explicitly treated as a real-capture-first problem
- Added a Rover-specific decoder note that aligns the current Speeduino Rover MEMS implementation with the surviving forum evidence and the MS3/rusEFI references, and narrows the next safe Rover step to attachment extraction rather than new synthetic traces
- Downloaded and staged the Rover MEMS forum attachments locally, confirming the archived Rover test project used `Crank Speed` with `5-3-2 cam`, `Wasted Spark`, and `Sequential` injection, while narrowing the remaining Rover blocker to composite-log CSV channel mapping and PDF wheel rendering
- Added a host-side composite-log parser at [tools/parse_speeduino_composite_csv.py](tools/parse_speeduino_composite_csv.py) and used it to resolve the Rover CSV channel names from the archived project INI, narrowing the remaining Rover blocker again to PDF wheel interpretation and exact signal-to-tooth alignment
- Added a host-side PDF image extractor at [tools/extract_pdf_images.py](tools/extract_pdf_images.py) and used it to pull `65` embedded images out of the Rover MEMS PDFs into [Resources/rover_mems_evidence/pdf_images](Resources/rover_mems_evidence/pdf_images), so the remaining Rover blocker is now selecting the right wheel drawings and aligning them to the parsed signal streams
- Added an OCR-based Rover PDF image indexer at [tools/index_pdf_images.py](tools/index_pdf_images.py) and used it to rank the `65` extracted Rover images down to a small set of likely crank/cam diagram candidates in [Resources/rover_mems_evidence/pdf_images/ocr_index.json](Resources/rover_mems_evidence/pdf_images/ocr_index.json)
- Added a PDF readiness inspector at [tools/inspect_pdf_evidence.py](tools/inspect_pdf_evidence.py) and used it to confirm the Rover manuals have no extractable text layer here and that no local page renderer is installed, turning the remaining blocker into an explicit environment limitation rather than a vague parsing gap
- Installed a user-scope Poppler renderer, added [tools/render_pdf_pages.py](tools/render_pdf_pages.py), and rendered the full Rover manual pages into [Resources/rover_mems_evidence/rendered_pages](Resources/rover_mems_evidence/rendered_pages), which let the repo map OEM Rover wheel descriptions onto Speeduino's `11-5-12-4` and `2-14-3-13` patterns even though a first naive replay encoding still had to be backed out
- Added [tools/derive_rover_mems_windows.py](tools/derive_rover_mems_windows.py) to derive the Rover MEMS rolling `32`-bit decoder window from the documented `36`-slot wheels, plus focused tool tests that lock the helper to all five hard-coded Rover patterns
- Added narrow Rover MEMS primary-only replay coverage for the two OEM-backed layouts, proving the current decoder recognizes `2-14-3-13` and `11-5-12-4` and lands in half-sync without speculative timing assumptions
- Verified `test_decoders`: `196/196`
- Verified `test_updates`: `38/38`
- Verified `test_updates_tail`: `5/5`
- Verified `test_ign`: `146/146`
- Verified `test_sensors`: `50/50`

Latest handoff references:

- [SESSION_HANDOFF_2026-03-21_HARLEY_REPLAY.md](speeduino/SESSION_HANDOFF_2026-03-21_HARLEY_REPLAY.md)
- [SESSION_HANDOFF_2026-03-21_STOCK_TUNE_AUDIT.md](speeduino/SESSION_HANDOFF_2026-03-21_STOCK_TUNE_AUDIT.md)
- [SESSION_HANDOFF_2026-03-21_FORK_BASE_TUNE.md](speeduino/SESSION_HANDOFF_2026-03-21_FORK_BASE_TUNE.md)
- [SESSION_HANDOFF_2026-03-21_TUNE_DEFAULT_VALUES.md](speeduino/SESSION_HANDOFF_2026-03-21_TUNE_DEFAULT_VALUES.md)
- [SESSION_HANDOFF_2026-03-21_FORUM_EVIDENCE_CURATION.md](speeduino/SESSION_HANDOFF_2026-03-21_FORUM_EVIDENCE_CURATION.md)
- [SESSION_HANDOFF_2026-03-21_WIKI_BOARD_DOCS.md](speeduino/SESSION_HANDOFF_2026-03-21_WIKI_BOARD_DOCS.md)
- [SESSION_HANDOFF_2026-03-21_EXTERNAL_DECODER_RESEARCH.md](speeduino/SESSION_HANDOFF_2026-03-21_EXTERNAL_DECODER_RESEARCH.md)
- [SESSION_HANDOFF_2026-03-21_ROVER_MEMS_NOTE.md](speeduino/SESSION_HANDOFF_2026-03-21_ROVER_MEMS_NOTE.md)
- [SESSION_HANDOFF_2026-03-22_ROVER_MEMS_BIT_WINDOW.md](speeduino/SESSION_HANDOFF_2026-03-22_ROVER_MEMS_BIT_WINDOW.md)
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

# Explicit INI defaultValue mismatch report for ambiguous fields
python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --report-explicit-default-mismatches idleAdvStartDelay idleTaperTime boostCutEnabled boostMinDuty boostMaxDuty vvtCL0DutyAng vvtMinClt vvtDelay

# The same mismatches, but labeled as TunerStudio initialization-only evidence
python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --report-initialization-default-evidence idleAdvStartDelay idleTaperTime boostCutEnabled boostMinDuty boostMaxDuty vvtCL0DutyAng vvtMinClt vvtDelay

# Fork contract vs INI defaultValue conflicts
python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --report-contract-default-conflicts

# Classify contract/default conflicts against the stock base tune
python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --stock-msq "Resources/Speeduino base tune.msq" --report-contract-conflict-origins

# Show only fork-specific contract/default conflicts
python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --stock-msq "Resources/Speeduino base tune.msq" --report-contract-conflict-origins --contract-origin-filter fork_and_stock_both_differ_from_ini_default

# Verify the current classified conflict baseline
python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --stock-msq "Resources/Speeduino base tune.msq" --verify-expected-contract-conflicts

# Show context-dependent contract/default differences backed by docs/runtime behavior
python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --report-contextual-contract-exemptions

# Show classified policy items with embedded evidence notes
python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --stock-msq "Resources/Speeduino base tune.msq" --report-policy-evidence

# Show packaged hardware-profile overrides that intentionally differ from generic INI defaults
python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --report-packaged-profile-overrides

# Verify the packaged hardware-profile override baseline
python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --verify-expected-packaged-profile-overrides

# Collect curated forum evidence for one roadmap area using the phpBB forum backend
python Resources/speeduino_evidence_collector_stable.py --mode roadmap --search-engine forum --roadmap-area "Knock and pin/default policy" --per-query-limit 2 --limit-results 12 --max-thread-pages 12

# Collect curated forum evidence for targeted blocked decoder families
python Resources/speeduino_evidence_collector_stable.py --mode decoder --search-engine forum --decoder-family "Honda J32" --decoder-family "Rover MEMS" --per-query-limit 2 --limit-results 18 --max-thread-pages 12

# Review the current external decoder-research baseline before synthesizing new replay traces
# See speeduino/SESSION_HANDOFF_2026-03-21_EXTERNAL_DECODER_RESEARCH.md

# Review the Rover-specific note before any new Rover MEMS replay work
# See speeduino/SESSION_HANDOFF_2026-03-21_ROVER_MEMS_NOTE.md

# Rebuild the OCR-ranked Rover image candidate index
python tools/index_pdf_images.py --image-dir Resources/rover_mems_evidence/pdf_images --output Resources/rover_mems_evidence/pdf_images/ocr_index.json --top 20

# Check whether the local environment can extract text or render Rover PDF pages
python tools/inspect_pdf_evidence.py --pdf-dir Resources/rover_mems_evidence --output Resources/rover_mems_evidence/pdf_inspection.json

# Render full Rover PDF pages now that pdftoppm/Poppler is available
python tools/render_pdf_pages.py --pdf-dir Resources/rover_mems_evidence --output-dir Resources/rover_mems_evidence/rendered_pages --manifest Resources/rover_mems_evidence/rendered_pages/manifest.json --dpi 200

# Derive the Rover MEMS rolling decoder windows from documented wheel layouts
python tools/derive_rover_mems_windows.py 2-14-3-13 11-5-12-4
```

### Current Test Status

- `196/196` decoder tests passing
- `38/38` config migration tests passing
- `5/5` migration tail tests passing
- `146/146` ignition/protection tests passing
- `50/50` sensor tests passing
- Stock base tune audit currently flags one remaining fork-surface drift: missing `knock_limiterDisable` in [Resources/Speeduino base tune.msq](Resources/Speeduino%20base%20tune.msq)
- Stock base tune also fails the 92-check fork-default value contract for knock / rolling-cut / DFCO / launch / idle-advance / idle-up / VSS / WMI / oil-pressure / fan / air-con defaults, as expected for the unchanged stock artifact
- Fork-owned base tune audit passes for [Resources/Speeduino AI base tune.msq](Resources/Speeduino%20AI%20base%20tune.msq), including the 92 selected fork-critical default values
- Release bundle base tune audit passes for [speeduino-dropbear-v2.0.1-base-tune.msq](release/speeduino-dropbear-v2.0.1-base-tune.msq) against [speeduino-dropbear-v2.0.1.ini](release/speeduino-dropbear-v2.0.1.ini), including the same 92 selected fork-critical default values
- The audit tool now also parses 230 explicit INI `defaultValue` entries, decodes bitfield defaults to their display values, and can report tune-vs-INI-default mismatches separately from the fork-owned contract
- Those raw `defaultValue` mismatches are now explicitly treated as TunerStudio initialization evidence rather than automatic proof that the shipped tune baseline is wrong; the tool can print that semantic note directly
- Current explicit-default mismatches are narrowed to `idleAdvStartDelay`, `idleTaperTime`, `boostCutEnabled`, `boostMinDuty`, `boostMaxDuty`, `vvtCL0DutyAng`, `vvtMinClt`, and `vvtDelay`, with unit-aware defaults now shown where applicable such as `vvtMinClt = 70 / 160`
- The only remaining active contract-vs-INI-default conflict inside the enforced contract is now `knock_pin`
- `airConCompPol`, `airConReqPol`, `idleAdvStartDelay`, `idleTaperTime`, and `vssPulsesPerKm` are all treated as contextual exemptions rather than active conflicts, because they are wiring-dependent or tune-dependent settings with direct support from the manual, INI help text, and runtime behavior
- A stock-origin classification report now shows only one active conflict, and it is fork-specific: `knock_pin`
- The origin report now prints counts as well, so the current active policy surface is immediately visible as `fork_and_stock_both_differ_from_ini_default=1`
- The repo now machine-checks that exact classification baseline, so future work must either preserve it or update the expected policy map deliberately
- A policy-evidence report now ties the one active conflict plus all contextual exemptions to the concrete project source that justifies them: manual pages, INI help text, and runtime code paths
- The audit now also distinguishes packaged hardware-profile overrides from generic defaults; today that set is just `knock_pin`, where the generic INI default intentionally avoids DropBear-sensitive pins while the packaged DropBear tunes intentionally ship `A8`
- Official project wiki pages are now treated as a primary evidence source alongside the bundled manual and forum threads; the current highest-value board doc is [Dropbear | Speeduino Manual](https://wiki.speeduino.com/en/boards/Dropbear), which documents board pin roles for `Boost`, `Idle`, `VVT`, `VSS` / `Idle Up`, MAP source selection, and crank hardware filtering
- External decoder references from the bundled rusEFI and MS3 source trees are now part of the documented research baseline; current highest-value references are rusEFI's real `36-2-1` test in [test_real_4b11.cpp](Resources/rusefi-2026-03-17/unit_tests/tests/trigger/test_real_4b11.cpp), rusEFI's [trigger_honda.cpp](Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_honda.cpp), rusEFI's [trigger_rover.cpp](Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_rover.cpp), and MS3's Rover notes in [core.ini](Resources/ms3-source-master/ms3/core.ini)
- Rover MEMS now has a dedicated evidence note tying together the current Speeduino decoder, the maintainer-provided `32 poles / 4 missing` forum description, the real composite-log thread, the extracted Rover test project, and MS3's `poll_level_tooth` guidance; the remaining blocker is now specifically CSV channel mapping and PDF wheel rendering, not general topology or missing source files
- Rover MEMS CSV channel mapping is now resolved from the archived project INI via [parse_speeduino_composite_csv.py](tools/parse_speeduino_composite_csv.py); the remaining blocker is the last step from named signals to exact tooth positions, which still requires interpreting the Rover PDF wheel drawings
- Rover MEMS PDF assets are now extracted into a local image corpus with [extract_pdf_images.py](tools/extract_pdf_images.py); the remaining blocker is no longer PDF access, but identifying which images are the relevant wheel diagrams and aligning them with the parsed Rover signal streams
- Rover MEMS extracted PDF images are now OCR-ranked with [index_pdf_images.py](tools/index_pdf_images.py); the remaining blocker is no longer "which of the 65 images matter", but visually confirming the top-ranked candidate diagrams and aligning them with the parsed Rover signal streams
- Rover MEMS PDF readiness is now explicitly inspected with [inspect_pdf_evidence.py](tools/inspect_pdf_evidence.py); in this environment the manuals still have `0` extractable text-layer characters, but `pdftoppm` is now available and full pages are rendered locally with [render_pdf_pages.py](tools/render_pdf_pages.py)
- The rendered Rover manual pages now map `MEMS 1.9 MPi` to Speeduino's `11-5-12-4` Rover layout and `MEMS 3 MPi` / `MEMS 2 VVC` to `2-14-3-13`
- A first primary-only Rover replay attempt based on naive long-gap traces for those layouts was intentionally backed out because the decoder's rolling `roverMEMSTeethSeen` match stayed at pattern `0`; the remaining blocker is now accurate bit-window modeling, not missing documents or rendering capability
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
