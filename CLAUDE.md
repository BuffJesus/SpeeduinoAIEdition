# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this repo is

Speeduino AI Edition — a maintenance-focused fork of Speeduino 202501.6. The single operator runs the **DropBear board on Teensy 4.1** (`BOARD_ID=55`, MC33810-driven), so all new feature work targets that hardware path. AVR Mega2560 and other boards are supported as a regression-safety baseline only.

The companion desktop tuner application (separate repo) lives at `D:/Documents/JetBrains/Python/Tuner/` and is moving from Python/PySide6 to native C++ Qt 6. Several roadmap items in this repo are explicitly joint commitments with that tuner — see [FIRMWARE_ROADMAP.md](FIRMWARE_ROADMAP.md) Phase 12 and Phase 14.

## Build and test

PlatformIO project. Common environments in [platformio.ini](platformio.ini):

| Env | Purpose |
|---|---|
| `megaatmega2560` | Production AVR build, the regression baseline |
| `megaatmega2560_sim_unittest` | simavr-based test runner used by the entire `test/` suite |
| `teensy41` | DropBear/Teensy 4.1 production build (`-DTEENSY_INIT_USB_DELAY_AFTER=40`) |
| `teensy35`, `teensy36`, `black_F407VE` | Other supported boards |

```bash
# Compile-only (no upload)
pio run -e teensy41
pio run -e megaatmega2560

# Run all unit tests under simavr
pio test -e megaatmega2560_sim_unittest

# Run a single test suite
pio test -e megaatmega2560_sim_unittest -f test_decoders
pio test -e megaatmega2560_sim_unittest -f test_comms

# Run a single test file inside a suite — append the filename to the filter
pio test -e megaatmega2560_sim_unittest -f test_decoders/missing_tooth
```

`test_table3d_native` is `test_ignore`d for every embedded environment because it's a host-native suite — leave it that way.

If `pio test` hits transient missing-object or file-lock failures during incremental rebuilds, that's an LTO/build-cache issue, not a source regression. Rerun after `pio run -e megaatmega2560_sim_unittest -t clean`. The `FIRMWARE_ROADMAP.md` "Current Verified Baseline" section calls this out explicitly.

## Architecture — the parts that matter

### Board layer is the seam

Each supported board has a `board_*.{h,cpp}` pair under `speeduino/`. The board layer owns:

- timer/PWM setup, ISR vector wiring, and the `*_TIMER_*` macros consumed by [scheduler.cpp](speeduino/scheduler.cpp)
- pin mapping for known board IDs via `applyBoardPinMapping(boardID)` (called from [init.cpp](speeduino/init.cpp) `setPinMapping()`)
- board capability flags returned by `getBoardSpecificCapabilityFlags(pinMapping)` — consumed by the `'K'` capability response and by feature gates throughout the firmware
- secondary serial / CAN / SD / RTC / ADC initialization

**DropBear is the only `applyBoardPinMapping()` consumer in [board_teensy41.cpp](speeduino/board_teensy41.cpp)**, so any per-board policy work for the user's hardware can stay scoped to `setTeensy41DropBearPinMapping()` without touching shared init.

New Teensy-only feature work goes inside `#if defined(CORE_TEENSY41)` so the AVR path stays bit-for-bit identical and the simavr suites remain the safety net. Phases 12, 13, 14 in the roadmap are all DropBear/Teensy 4.1 only.

### Live data packet is a header-derived contract

[live_data_map.h](speeduino/live_data_map.h) is a 148-row declarative table mapping each byte of the `'A'`/`'r'` response to its semantic name, encoding, and the source struct field. `LIVE_DATA_MAP_SIZE = 148` is enforced as a hard ceiling — **never grow the packet**. Add new state by repurposing reserved bytes or extending bits in the existing `runtimeStatusA` packed-status pattern (see roadmap Phase 13 Slice N).

The desktop tuner's `LiveDataMapParser` reads this header directly and canaries against `LIVE_DATA_MAP_SIZE`, so any reorder/width change is a cross-repo break, not a free firmware-side edit.

`OCH_OFFSET_*` constants in the same header are the named anchors for fields the desktop, dashboards, and CAN broadcast all need; treat them as load-bearing.

### Capability response (`'K'` command)

`comms_legacy.cpp` implements the 39-byte fixed-layout binary capability response (board id, capability flags, feature flags, OCH size, signature). `getStableBoardId()` in [globals.h](speeduino/globals.h)/[globals.cpp](speeduino/globals.cpp) maps a runtime pin mapping to the stable `BoardId` enum (`BOARD_ID_DROPBEAR_T41=55`, `BOARD_ID_BEAR_CUB`, `BOARD_ID_UNKNOWN`). This is the contract the desktop and any future native-format work depend on — extend it additively.

### External page CRC verification

`page_crc.cpp` (`parseLegacyCrcRequest`, `encodeLegacyCrc32Response`, `calculatePageCRC32ForMode`) is mode-aware for the experimental U16P2 path. The protocol is documented in [docs/page_verification_protocol.md](speeduino/docs/page_verification_protocol.md).

### Decoder/test split

`speeduino/decoders.cpp` is the runtime; `test/test_decoders/<DecoderName>/` is the per-decoder state-machine coverage. Many decoders also have a `SESSION_HANDOFF_*_STATE.md` file under `speeduino/` formalising the **current-code** sync/wrap/filter contract — when changing a decoder, read the matching state handoff first; it locks the behavior the tests assert.

The Phase 2 evidence pipeline ([Resources/speeduino_evidence_collector_stable.py](Resources/speeduino_evidence_collector_stable.py)) is the canonical source for forum/manual evidence used in decoder work. External references for cross-checking:

- rusEFI decoder implementations in [Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/](Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/) — topology and sync-behavior cross-reference, **not** drop-in waveform sources
- MS3 references in [Resources/ms3-source-master/ms3/](Resources/ms3-source-master/ms3/) — best current external clue for Rover phase and `poll_level_tooth` semantics

## Roadmap and session handoffs

[FIRMWARE_ROADMAP.md](FIRMWARE_ROADMAP.md) is the authoritative planning document (~1000 lines). It is **not an instruction manual** — it's a planning surface. Search by phase heading (`## Phase 12`, `## Phase 13`, `## Phase 14`) rather than reading top-to-bottom. The "Current Verified Baseline" and "Current Active Remaining Work" sections near the top are the right starting point for picking up active work.

`speeduino/CONTINUE_PROMPT_*.md` files capture session-end state for the next pickup. The most recent one is the right starting context.

`speeduino/SESSION_HANDOFF_*.md` files document the locked current-code contract for individual decoders, the Rover MEMS evidence backlog, and similar narrow-scope state. Read the matching handoff before touching the corresponding decoder.

## Constraints to respect

- **AVR Mega2560 path is bit-for-bit frozen** for new feature work. Phases 12+ are DropBear/Teensy 4.1 only. The simavr test suite (~750 tests) is the safety net — it must keep passing.
- **148-byte live-data packet ceiling.** New telemetry fits inside the existing budget by repurposing reserved bytes or extending packed-status bits.
- **Roadmap and code stay in sync.** When implementing a roadmap slice, mark it ✅ in the same change with a one-line note pointing at the file/function that landed it.
- **Decoder changes require reading the matching `SESSION_HANDOFF_*_STATE.md`** first. The handoffs lock the contract the tests assert; changing decoder behavior without updating both is how Phase 9 regressions happened.
- **Cross-repo coordination:** Phase 12 (U16 high-leverage tables) and Phase 14 (firmware as native-contract producer, including multi-tune via dash rotary) are joint commitments with the desktop tuner. Don't unilaterally change the contracts the desktop reads (`live_data_map.h`, the `'K'` response layout, future `tune_storage_map.h`).

## Useful reference paths

- [platformio.ini](platformio.ini) — env definitions and build flags
- [speeduino/board_teensy41.cpp](speeduino/board_teensy41.cpp) and [speeduino/board_teensy41.h](speeduino/board_teensy41.h) — the DropBear/Teensy 4.1 board layer
- [speeduino/live_data_map.h](speeduino/live_data_map.h) — load-bearing packet contract
- [speeduino/comms_legacy.cpp](speeduino/comms_legacy.cpp) — `'K'` capability response, page-CRC protocol
- [Resources/Speeduino_manual.pdf](Resources/Speeduino_manual.pdf) — stock operator manual (AVR-era, predates Phase 6)
- [Resources/rusefi-2026-03-17/](Resources/rusefi-2026-03-17/) and [Resources/ms3-source-master/](Resources/ms3-source-master/) — external references for decoder/feature cross-checking
