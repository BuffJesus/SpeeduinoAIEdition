# Continue Prompt — 2026-04-08

Use this as the kickoff prompt for the next session. It captures everything that was decided on 2026-04-07 across the firmware and the desktop tuner so a fresh context can pick up without re-deriving the rationale.

## Context

Two repos are evolving in lockstep:

- **Firmware** — `C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/` — Speeduino fork, primary target is the DropBear board on Teensy 4.1 (`BOARD_ID=55`, MC33810-driven). Roadmap: [FIRMWARE_ROADMAP.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/FIRMWARE_ROADMAP.md).
- **Tuner** — `D:/Documents/JetBrains/Python/Tuner/` — PySide6 desktop application currently mid-pivot to native C++ Qt 6 (Phase 14). Roadmap: [docs/tuning-roadmap.md](D:/Documents/JetBrains/Python/Tuner/docs/tuning-roadmap.md). Top-level guidance: [CLAUDE.md](D:/Documents/JetBrains/Python/Tuner/CLAUDE.md).

The user is the only operator, runs DropBear/Teensy 4.1 themselves, and asked us to focus on improvements to that hardware path specifically (not AVR Mega2560).

## What landed yesterday (2026-04-07)

All of the following are **roadmap-only**. No firmware C++ or Python code was changed. Everything is planning text in the two roadmap documents.

### Firmware roadmap additions

- **Phase 12 — U16 maps where precision matters (DropBear/Teensy)**. Joint commitment with the desktop's Phase 14. Widens `veTable`/`veTable2`, `afrTable`, `ignitionTable`/`ignitionTable2`, `boostTable`, `vvtTable`/`vvtTable2` to native U16 on DropBear; everything else stays U08; AVR stays all-U08 forever. Single production signature `speeduino 202501-T41` once it ships; experimental `-U16P2` retired. The actual missing piece is the byte-returning fueling lookup path called out in Tuner CLAUDE.md "Known Fragile Areas" item 4 — the U16P2 transport already works.
- **Phase 13 — Teensy 4.1 / DropBear enhancement backlog**. Eleven slices A–N drawing on rusEFI ([Resources/rusefi-2026-03-17](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17)) and MS3 ([Resources/ms3-source-master](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master)) as references. Highest-value low-risk wins are **Slice A (hardware input capture for triggers using QuadTimer secondary input)** and **Slice C (Native Ethernet TS transport using the built-in PHY pads)**. Highest-effort item is **Slice B (software knock detection via CMSIS-DSP biquad on ADC1)** — must wait until A is stable so trigger jitter can't contaminate knock-window sampling. Slices L/M/N are TSDash-derived (UDP broadcast for dashboards, packet stability guarantees, output channel count limits — stay within `LIVE_DATA_MAP_SIZE = 148`).
- **Phase 14 — Firmware as native-contract producer**. Joint with the tuner's Phase 13A. Eight slices:
  - **14A** — `tune_storage_map.h` declarative table, sibling to `live_data_map.h`, source of truth for tune storage layout. Collapses the legacy `afrTable → afrTable1Tbl → afrTable1Map → zBins=afrTable` 4-hop indirection (Tuner CLAUDE.md fragile area #2).
  - **14B** — 32-bit `schema_fingerprint` baked into the `'K'` capability response (truncated SHA-256 of `live_data_map.h` + `tune_storage_map.h` + `BOARD_ID`). Lets the desktop refuse to burn an incompatible tune *before* commit instead of after the engine bricks.
  - **14C** — semantic-id stamps on packed status bytes (extends the `runtimeStatusA` pattern to `engineProtectStatus`/`status3`/`status5`). No packet growth.
  - **14D** — `tune.bin` ingest from SD card at boot (Teensy 4.1 only). Race-team workflow — swap tunes between rounds without a laptop.
  - **14E** — one-line JSON boot manifest over USB CDC before TS handshake. Skips ~6 round-trip probes.
  - **14F** — generated INI from `tune_storage_map.h` (DropBear only). Retires the hand-maintained INI on the DropBear/Teensy variant.
  - **14G** — boot-time multi-tune slot selection via dash-mounted rotary selector switch (the diesel-truck workflow — EFILive/HP Tuners/MM3 pattern). Target 4 slots. Two electrical conventions: binary-encoded rotary (preferred, 2 pins → 4 positions) and one-hot rotary. Per-slot fingerprint validation. New `activeTuneSlot` byte in the live-data packet (within the existing 148-byte ceiling). Live switching while running is **explicitly out of scope** — boot-time only is the contract.
  - **14H** — updated DropBear/Teensy 4.1 operator manual under `docs/manual/` in the firmware repo. Markdown source → PDF artefact. Each new slice ships with its manual section. Source-of-truth rule: every documented behavior must reference a `live_data_map.h` row, a `tune_storage_map.h` row, a roadmap slice, or a specific source file + line. Joint with the desktop manual via a shared `topic_index.md`.

### Tuner roadmap additions

All under "Future Phase 13A: Firmware Contract Modernization" → new "Joint commitment with Speeduino Firmware Phase 14" subsection:

- A consumer-side mapping table for each firmware slice (14A–14G) showing what the desktop builds to consume it.
- **Multi-tune slot management** subsection explicitly closes TunerStudio gap **G10** (line 139 of the tuner roadmap). Concrete deliverables:
  1. `NativeTune` v1.1: add `slot_index: int = 0` and `slot_name: str | None = None` (forward-compatible minor bump — existing v1 tunes load as slot 0)
  2. Live-data parser: surface `activeTuneSlot` byte once Firmware 14G ships
  3. Toolbar slot indicator (read-only)
  4. Slot picker dropdown + "burn to slot N" routing in `TuningWorkspacePresenter`
  5. "Copy slot X → slot Y" workspace action
  6. Per-slot fingerprint mismatch surfacing in the connect dialog
  7. SD-card import flow as a file-picker action
- Steps 1–3 are **cleared to start now** because they're backwards-compatible (slot 0 default = current single-tune workflow). Steps 4–7 sequence after firmware bench validation of 14G.
- **Updated end-user documentation** subsection — joint with Firmware Slice 14H. Split into operator manual (`docs/manual/operator/`) and developer manual (`docs/manual/developer/`), the latter modeled after the [TunerStudio dev manuals](https://www.tunerstudio.com/index.php/support/manuals/tsdevmanuals). Reference structure (not content) drawn from the stock [Speeduino_manual.pdf](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino_manual.pdf), the [TunerStudio operator manuals](https://www.tunerstudio.com/index.php/support/manuals), and the decompiled TunerStudio sources at [C:/Users/Cornelio/Desktop/Decompiled/TunerStudioMS](C:/Users/Cornelio/Desktop/Decompiled/TunerStudioMS).

### Cross-cutting joint sequencing (firmware + tuner)

| Step | Repo | Slice |
|---|---|---|
| 1 | Firmware | 14A — `tune_storage_map.h` lands |
| 2 | Tuner | `TuneStorageMapParser` consumes the new header; `NativeDefinition` v2 builds from it instead of INI |
| 3 | Firmware | 14B — schema fingerprint in capability response |
| 4 | Tuner | Burn-time guard refusing fingerprint mismatches |
| 5 | Firmware | 14C — semantic-id extensions to packed status bytes |
| 6 | Tuner | Phase 13B Native Logging Contract consumes new packed-status semantics |
| 7 | Firmware | 14E — boot manifest line over USB CDC |
| 8 | Tuner | Connect path reads manifest; legacy probe is fallback |
| 9 | Firmware | 14D — `tune.bin` ingest from SD |
| 10 | Firmware | 14F — generated INI, retire hand-maintained INI on DropBear/Teensy |
| 11 | Firmware | 14G — boot-time multi-tune slot selection |
| 12 | Tuner | Multi-tune UI (steps 4–7 of the desktop ordering) |
| ongoing | Both | 14H — operator manual, each slice ships with its section |

## What to do next session

**Pick exactly one of the following starting points** depending on what the user wants:

1. **Start implementing Slice 14A** (highest leverage, unblocks the most other work). Concretely: design the `tune_storage_map.h` schema as a sibling header to [live_data_map.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/live_data_map.h); produce a first-pass declarative table for the existing DropBear pages; verify the existing INI's offsets match. Do not modify the INI generator yet — Slice 14F is later.
2. **Start implementing the desktop's `NativeTune` v1.1** (Tuner steps 1–3 of multi-tune; backwards-compatible, can land before any firmware changes). Concretely: add `slot_index` and `slot_name` to `tuner.domain.native_format.NativeTune`, bump the schema version to `1.1` per the existing `_check_version` minor-bump rules, add tests, run `python -m pytest --collect-only -q` to confirm the suite still collects cleanly. The `NativeFormatService.from_tune_file` / `to_tune_file` round-trip tests need updating to cover the new fields.
3. **Start implementing Phase 13 Slice A** (hardware input capture for triggers — the highest single-event-quality win on the firmware side, but the most invasive). Concretely: prototype on `pinTrigger` only behind a `TEENSY41_HW_TRIGGER_CAPTURE` opt-in; keep the existing `attachInterrupt` path as fallback; validate the captured timestamp against the existing decoder tests by feeding it into the same `triggerHandler` callback. Read `board_teensy41.cpp` and the QuadTimer setup carefully before touching anything — TMR1–TMR4 are already claimed for fuel/ign compares, the secondary input function on each channel is what's free.
4. **Start implementing Phase 13 Slice C** (Native Ethernet TS transport — the cleanest low-risk operator-visible win). Concretely: extend `boardHasCapability(BOARD_CAP_WIFI_TRANSPORT)` semantics (or add a new `BOARD_CAP_ETHERNET_TRANSPORT`); in `serviceBoardSerial()` add a `pPrimarySerial = (Stream*)&ethClient` divert mirroring the existing `Serial2`/Airbear pattern from Phase 11. Zero protocol changes — the diverted-pointer comms architecture already supports it. Hardware check first: confirm the magjack/RJ45 wiring on the user's DropBear before writing any code.
5. **Start backfilling Slice 14H** (operator manual). Concretely: create `docs/manual/` in the firmware repo, add a `topic_index.md` skeleton, write the first manual section for the most operator-visible already-shipped feature (suggestion: SPI flash health monitoring from Phase 6, since the live-data packet byte 131 is already locked and the desktop already surfaces it). Mirror with the desktop side by creating `docs/manual/operator/` in the Tuner repo with a matching topic-index entry.

**Default recommendation**: option **1 (Slice 14A)** unless the user has a different priority. It's the producer-side foundation for Slices 14B/14F/14G/14H consumers and unblocks the most downstream work. Option 2 is a fine parallel desktop-side track if the user wants a low-risk warm-up that doesn't touch the firmware at all.

## Constraints to respect

- **Never touch AVR Mega2560 paths.** Phases 12, 13, 14 are DropBear/Teensy 4.1 only. Every change goes inside `#if defined(CORE_TEENSY41)`. The 750+ simavr test suite is the safety net — it must keep passing.
- **Live-data packet stays at 148 bytes.** No growth. New byte-level information goes in the existing `LIVE_DATA_MAP_SIZE = 148` budget by repurposing reserved bytes or adding bits to existing packed-status bytes (the `runtimeStatusA` pattern). Documented in Phase 13 Slice N. The desktop's `LiveDataMapParser` enforces this with a structural canary test.
- **Never invent code paths the user did not approve.** This was a roadmap-planning conversation; do not silently start implementing unless the user explicitly says so. If asked to implement, follow the option list above and confirm which one before touching code.
- **Use the dedicated tools** (`Read`, `Edit`, `Grep`, `Glob`) instead of `cat`/`grep`/`sed`. Bash is for system commands only.
- **Do not duplicate work between repos.** The two roadmaps reference each other; the two manuals reference each other via a shared topic index. If a fact belongs in one repo, do not paste it into the other — link to it.
- **The user is the only operator.** They run the firmware on their own DropBear and use the desktop tuner themselves. Frame suggestions accordingly — race-team workflows, dash-mounted rotary, SD swaps, single-operator UX. There is no team.

## Files to read first when picking up

In this order:

1. This file — full context for what was decided
2. [FIRMWARE_ROADMAP.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/FIRMWARE_ROADMAP.md) — search for `## Phase 12`, `## Phase 13`, `## Phase 14` to see the actual roadmap text (each phase has more detail than this summary)
3. [Tuner tuning-roadmap.md](D:/Documents/JetBrains/Python/Tuner/docs/tuning-roadmap.md) — search for `Future Phase 13A` and the `Joint commitment with Speeduino Firmware Phase 14` subsection
4. [Tuner CLAUDE.md](D:/Documents/JetBrains/Python/Tuner/CLAUDE.md) — for the desktop architecture, fragile areas, and the existing native-format v1 surface
5. [live_data_map.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/live_data_map.h) — the existence proof for the header-derived contract pattern Slice 14A is based on
6. [board_teensy41.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/board_teensy41.cpp) and [board_teensy41.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/board_teensy41.h) — only if picking option 3 or 4 above

Do **not** read the whole firmware roadmap in one go — it's ~1000 lines. Use `Grep`/`Read` with offsets on the specific phase headings you need.

## One-line summary

Phase 12 widens DropBear's high-leverage tables to U16, Phase 13 is the Teensy hardware enhancement backlog, Phase 14 makes the firmware the producer of its own native contracts (with multi-tune via dash rotary as the headline operator feature) and ships an updated manual alongside — and the desktop tuner has a matching consumer-side commitment in its Phase 13A. Tomorrow, pick one of the five starting points above (default: Slice 14A).
