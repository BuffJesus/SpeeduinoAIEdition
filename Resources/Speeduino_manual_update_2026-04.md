# Speeduino Manual Update

Revision: 2026-04-02

This document is a maintained update to the base manual in `Resources/Speeduino_manual.pdf`.

Use it as a companion to the main manual, not as a full replacement. The original PDF remains the broad user guide for installation, wiring, and tuning fundamentals. This update captures the current firmware and packaging changes that are present in this workspace and release set.

## What Changed

This update documents:

- the current DropBear / Teensy 4.1 release file pairing
- the alternate experimental native `U16` TunerStudio page-2 path
- the current tune-learning validity indicator and output-channel bits
- the board capability and SPI flash health output channels
- the current boundaries between production behavior, experimental behavior, and deferred work

## Base Manual Reference

Start with `Resources/Speeduino_manual.pdf` for:

- firmware loading and general installation
- TunerStudio project creation
- wiring and sensor setup
- tuning fundamentals

Use this update for the newer branch-specific behavior below.

## Release Artifact Pairing

Use the release files as matched sets. Do not mix production and experimental `.hex`, `.ini`, or `.msq` files in the same TunerStudio project.

### Production DropBear / Teensy 4.1

Use these three files together:

- `release/speeduino-dropbear-v2.0.1-teensy41.hex`
- `release/speeduino-dropbear-v2.0.1.ini`
- `release/speeduino-dropbear-v2.0.1-base-tune.msq`

Expected firmware signature:

- `speeduino 202501-T41`

This is the normal byte-serialized TunerStudio contract. This is the safe default path for normal use.

### Experimental Native-U16 Page 2

Use these three files together:

- `release/speeduino-dropbear-v2.0.1-teensy41-u16p2-experimental.hex`
- `release/speeduino-dropbear-v2.0.1-u16p2-experimental.ini`
- `release/speeduino-dropbear-v2.0.1-u16p2-experimental-base-tune.msq`

Expected firmware signature:

- `speeduino 202501-T41-U16P2`

This is an alternate experimental TunerStudio contract for the DropBear / Teensy 4.1 path only.

Important limits:

- it is intentionally gated behind a different signature and INI
- it only changes the experimental page-2 path
- the production byte-contract path remains the normal supported path
- high-resolution VE telemetry is still deferred

### Existing TunerStudio Projects

Older Teensy 4.1 projects that were created against the generic `speeduino 202501` signature may not attach cleanly after flashing newer Teensy firmware.

If TunerStudio reports a signature mismatch:

1. Verify which `.hex` you flashed.
2. Verify the project is using the matching `.ini`.
3. Re-create the project with the matching INI if the project was built against an older signature.
4. Import the matching `.msq` after the project opens correctly.

## Experimental Native-U16 Page 2

The repo now includes a working Teensy-only, DropBear-gated, page-2-only experimental native `U16` path.

What is proven in the current repo state:

- TunerStudio can read page 2 as native `U16`
- TunerStudio can write and burn page 2
- page CRC matches the same serialized page data
- SPI flash mirroring follows the same page-2 stream
- runtime fueling can consume the high-resolution page-2 VE path in this experimental mode

What is not changed by this experiment:

- the production byte-serialized page contract
- the existing live data packet size
- the byte-sized compatibility view exposed through the normal VE status channels

Practical guidance:

- use the experimental path only with the matching experimental `.hex`, `.ini`, and `.msq`
- do not point an existing production project at the experimental firmware
- do not assume that all tables, logs, or output channels have become `U16`

Current recommendation:

- use the production path for normal tuning and supportable installs
- use the experimental path only for controlled bench or hardware validation on the intended board

## Tune Learn Valid Indicator

The firmware now exposes a tune-learning validity bit through `runtimeStatusA` at output-channel byte `147`.

Current `runtimeStatusA` bit layout:

- bit0: fuel pump on
- bit1: launch hard active
- bit2: flat shift hard active
- bit3: idle-up active
- bit4: full sync
- bit5: transient active
- bit6: warmup or ASE active
- bit7: tune learn valid

TunerStudio now includes a front-page indicator for this bit:

- blocked label: `Tune Learn Blocked`
- ready label: `Tune Learn Valid`

### Tune Learn Valid Logic

The current firmware logic only sets `tune learn valid` when all of the following are true:

- the engine has full sync
- no TPS or MAP accel transient is active
- no TPS or MAP decel transient is active
- warmup is not active
- ASE is not active
- DFCO is not active
- engine protection is not active

This bit is intended to expose a cleaner "safe to learn" window to logs and dashboards. It does not change TunerStudio's internal VE Analyze implementation.

### Minimum Bench Validation

When hardware is available, validate these cases separately:

- key-on with no sync: `fullSync = 0`, `tuneValid = 0`
- cranking before full sync: `tuneValid = 0`
- stable running with no blockers: `fullSync = 1`, `tuneValid = 1`
- warmup active: `tuneValid = 0`
- ASE active: `tuneValid = 0`
- accel or decel transient: `tuneValid = 0`
- DFCO active: `tuneValid = 0`
- engine protection active: `tuneValid = 0`
- sync loss: `tuneValid = 0`
- post-recovery stable running: `tuneValid = 1`

Recommended channels to log during validation:

- `rSA_fullSync`
- `rSA_transient`
- `rSA_warmupASE`
- `rSA_tuneValid`
- `syncLossCounter`
- `egoCorrection`
- `warmupEnrich`
- `ase_enrich`
- `TPSdot`
- `MAPdot`
- `engineProtectStatus`
- `DFCOOn`

## Live Data And Board Capability Channels

The maintained live-data packet size is `148` bytes in the current repo and matching INIs.

Newer branch-specific bytes of interest:

- byte `130`: `boardCapabilityFlags`
- byte `131`: `SPIFlashHealth`
- byte `147`: `runtimeStatusA`

### Board Capability Flags

`boardCapabilityFlags` allows TunerStudio or tooling to understand board-specific capabilities without guessing from board names alone.

Current documented flags include:

- `BOARD_CAP_SPI_FLASH`
- `BOARD_CAP_UNRESTRICTED_INTERRUPTS`

Use case:

- board-aware UI can hide or gate features that the selected hardware does not support cleanly

### SPI Flash Health

`SPIFlashHealth` is currently intended for the Teensy 4.1 path.

Values:

- `1`: SPI flash is present and healthy
- `0`: SPI flash is unavailable or not detected

This is intended as a runtime health/status channel, not as proof that every flash-backed workflow has been validated on the current hardware.

### Interrupt Pin Validation

Trigger pin validation now checks platform interrupt capability against the configured trigger inputs.

Practical effect:

- restricted-interrupt platforms are less likely to fail at runtime because of unsupported trigger pin assignments
- boards that support unrestricted interrupts advertise that capability explicitly

## Current Production Versus Experimental Boundaries

The current split is intentional.

### Production Behavior

These are the normal supported assumptions:

- byte-serialized TunerStudio page contract
- existing output-channel packet size
- existing byte-sized VE telemetry channels
- standard DropBear / Teensy 4.1 release pairing

### Experimental Behavior

These are currently experimental and should be treated that way:

- DropBear / Teensy 4.1 native `U16` page-2 path
- matching alternate signature, alternate INI, and alternate base tune
- high-resolution runtime fueling along the experimental page-2 path only

### Deferred Work

These are still deferred and should not be described to end users as completed features:

- high-resolution VE telemetry in the normal output-channel packet
- broad native `U16` expansion across all TS pages
- replacing the production byte-contract path
- broader hardware validation that has not yet been performed

## Recommended User Guidance

For most users:

- use the production DropBear / Teensy 4.1 package set
- create a fresh TunerStudio project with the matching production INI
- treat the base manual PDF as the main setup guide
- treat this update document as the current release note and behavior supplement

For bench or engineering validation:

- use the experimental package set only as a matched trio
- keep experimental projects separate from production projects
- verify page read, write, burn, reconnect, and flash-backed restore behavior before relying on the setup
- log `runtimeStatusA` and the tune-learning channels when evaluating tune-assist behavior

## Source Of Truth

This update was prepared from the current repo state, primarily:

- `Resources/Speeduino_manual.pdf`
- `speeduino/FIRMWARE_ROADMAP.md`
- `speeduino/SESSION_HANDOFF_2026-03-26_TS_U16_PAGES.md`
- `speeduino/SESSION_HANDOFF_2026-03-28_TUNE_LEARN_VALIDATION.md`
- `speeduino/live_data_map.h`
- `release/README.md`

When this update and older screenshots or manual text disagree, prefer the current release artifacts, INIs, and source-backed handoff documents listed above.
