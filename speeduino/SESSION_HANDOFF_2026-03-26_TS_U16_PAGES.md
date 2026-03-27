# TunerStudio Native U16 Page Investigation

## Outcome

Native `U16` TunerStudio tune pages were **not** implemented in this session.

The current Teensy 4.1 / DropBear TunerStudio path is working because it preserves the existing **byte-serialized virtual page contract**. A safe native-`U16` experiment would require a **parallel Teensy-only TS page path**, not an in-place conversion of the existing page API.

Update from the follow-on experimental seam work:
- there is now a **Teensy-only, DropBear-gated, page-2-only experimental native-`U16` TS transport seam**
- it is isolated behind an alternate signature / alternate INI and does not replace the production byte contract
- it is validated as a **transport/editing/CRC/SPI-flash seam**, not as end-to-end high-resolution fueling

## What Was Verified

- TunerStudio itself supports `U16` arrays/tables.
- This firmware currently exposes tune/config pages through a byte-oriented API:
  - [pages.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/pages.h)
  - [pages.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/pages.cpp)
- Normal comms streams pages byte-by-byte via `getPageValue()` / `setPageValue()`:
  - [comms.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/comms.cpp)
- Legacy comms does the same:
  - [comms_legacy.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/comms_legacy.cpp)
- CRCs are computed over the same TS-visible byte stream:
  - [page_crc.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/page_crc.cpp)
- SPI-flash table mirroring also iterates byte-by-byte through that same API:
  - [storage_spi.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/storage_spi.cpp)
- The packaged INI still advertises that byte contract:
  - [speeduino-dropbear-v2.0.1.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/release/speeduino-dropbear-v2.0.1.ini)
  - `pageSize = 128, 288, 288, 128, 288, 128, 240, 384, 192, 192, 288, 192, 128, 288, 256`
  - main 16x16 tables are still `U08` TS-visible arrays

## Important Distinction

- Internal Teensy table storage may be `uint16_t`:
  - [table3d_typedefs.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/table3d_typedefs.h)
- Non-TS persistence already writes full `table3d_value_t` rows:
  - [storage.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/storage.cpp)
- But the TS-facing page layer still exposes those tables as byte-serialized virtual pages.

That means:
- the blocker is **not** internal storage width
- the blocker **is** the TS-visible page transport contract

## Why An In-Place U16 Flip Is Unsafe

The same byte-oriented page contract is shared by:
- page size calculation
- offset-to-entity mapping
- `p` page reads
- `M` page writes
- `d` CRC calculation
- SPI-flash table page save/load
- config migration helpers that iterate `getPageValue()` / `setPageValue()`

There is currently **no isolated TS-only abstraction boundary** for page serialization.

There is also no focused automated test suite for:
- TS page read serialization
- TS page write deserialization
- TS CRC vs TS readback equivalence
- TS page contract vs SPI-flash page mirror equivalence

So changing the existing page API in place would risk breaking:
- modern comms
- legacy comms
- burn/readback CRC
- SPI-flash table restore
- migration behavior

## Minimum Safe Experimental Seam

If native `U16` TS pages are attempted, the narrowest safe path is:

1. Keep the existing byte-serialized page API unchanged and as the default production path.
2. Add a **parallel Teensy-only experimental TS serializer/deserializer/CRC path** for **one page only**.
3. Gate it behind an explicit alternate signature or capability so current projects stay on the working byte contract.

Minimum new API surface likely needed:
- `getTsPageSize(page, mode)` or equivalent experimental page-size helper
- `readTsPageToBuffer(page, offset, buffer, length, mode)`
- `writeTsPageFromBuffer(page, offset, buffer, length, mode)`
- `calculateTsPageCRC32(page, mode)`
- explicit experimental mode selection tied to Teensy-only signature/capability

Where `mode` is conceptually:
- current byte-serialized contract
- experimental native-`U16` contract

This should **not** start by widening `getPageValue()` / `setPageValue()` directly.

## One-Page Experimental Candidate

If a future session prototypes this, the best first candidate is probably:
- page `2` (`veTable`)

Reason:
- single mainstream 16x16 page
- directly exercises table values and axes
- easier to reason about than multi-table pages like page `8`

Do **not** start with:
- page `8` (`seqFuelPage`) due to multiple non-contiguous table blocks
- a whole-repo INI change
- all map pages at once

## Required Proof For A Real Prototype

Before claiming success for a one-page experimental `U16` path, prove:

1. TS page read bytes match the INI contract exactly.
2. TS page write bytes rehydrate the intended in-memory values exactly.
3. TS CRC is computed over the exact same serialized bytes TS reads.
4. SPI-flash mirror for that page either:
   - remains intentionally on the existing byte contract, or
   - is explicitly upgraded to the same experimental stream
5. The default production byte path remains unchanged and working.

Those transport proofs now exist for the experimental page-2 seam.

## What Is Still Blocking End-To-End U16 Fueling

The remaining blocker is no longer TunerStudio page transport. It is the runtime VE contract.

Primary bottlenecks that originally blocked end-to-end U16 fueling:
- [speeduino.ino](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/speeduino.ino)
  - `getVE1Runtime()` / `PW(..., table3d_value_t VE, ...)` now provide a high-resolution runtime fueling path in the experimental page-2 mode
  - `getVE1()` remains a byte-sized compatibility/display view
- [secondaryTables.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/secondaryTables.cpp)
  - `getVE2Runtime()` and `calculateSecondaryFuel(...)` now normalize the secondary-table runtime VE path to the same experimental runtime unit
  - `getVE2()` remains a byte-sized compatibility/display view
- [globals.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/globals.h)
  - `currentStatus.VE`, `currentStatus.VE1`, and `currentStatus.VE2` are all `byte`
- [logger.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/logger.cpp), [live_data_map.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/live_data_map.h), and [comms_legacy.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/comms_legacy.cpp)
  - output channels and legacy live-data layouts still expose VE fields as byte-sized values

That means the experimental page-2 native-`U16` path currently proves:
- TS can read/write page 2 as `U16`
- CRC matches the same serialized page-2 byte stream
- SPI-flash mirroring can follow the same serialized page-2 byte stream
- runtime fueling can consume the full high-resolution VE path for experimental page 2 while keeping byte-sized compatibility status fields

It does **not** yet prove:
- output channels / logs / legacy channels can represent widened VE values coherently without a separate packet contract

## Minimum Safe Design For Real U16 Fueling

If end-to-end high-resolution fueling is attempted, the narrowest defensible path is:

1. Keep the production byte TS contract unchanged.
2. Keep the current experimental page-2 TS seam as the transport boundary.
3. Use a separate runtime VE type/helper for Teensy high-resolution fueling instead of widening every status/log field first.
4. Keep `currentStatus.VE*` as byte-sized compatibility/display channels unless a separate experimental output-channel contract is deliberately added.
5. Only widen logger/live-data contracts if a concrete external consumer requires true runtime high-resolution VE visibility and an alternate signature/INI/ochBlockSize flow is introduced.

The risky version would be to widen `currentStatus.VE`, `VE1`, `VE2`, `PW(...)`, logger maps, legacy channels, and TS contracts all in one slice. That is not a narrow change.

## Telemetry Contract Decision

Do **not** add high-resolution VE telemetry to the existing output-channel packet in place.

Why:
- [logger.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/logger.h) locks the live-data packet size through `LOG_ENTRY_SIZE`
- [live_data_map.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/live_data_map.h) is the authoritative byte map for that packet
- [logger.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/logger.cpp) and [comms.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/comms.cpp) stream the same packet to TunerStudio
- [comms_legacy.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/comms_legacy.cpp) also has separate legacy packet builders and field ordering that still assume byte-sized VE exposure
- both the production INI and the experimental page-2 INI still bind `VE1`, `VE2`, and `VE` to the existing `148`-byte output-channel contract

So an actual high-resolution VE telemetry slice would need:
- a separate experimental output-channel packet layout
- a bumped `LOG_ENTRY_SIZE`
- a matching alternate `ochBlockSize`
- matching alternate output-channel field definitions in an experimental INI
- likely an alternate signature so projects do not attach the wrong logger contract

That is a distinct protocol feature, not a safe extension of the current page-2 TS transport and fueling experiment.

Recommended current state:
- keep byte-sized VE telemetry as the compatibility view
- treat the current experimental mode as proving TS page transport plus runtime fueling only
- only revisit high-resolution VE telemetry if a real TunerStudio/logging workflow requires it

## Current Test Gap

There are migration/config tests that use `getPageValue()` / `setPageValue()`, but there is no focused TS page contract test layer. Relevant references:
- [test_config_migrations.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_updates/test_config_migrations.cpp)
- [test_config_migrations_tail.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_updates_tail/test_config_migrations_tail.cpp)

These are useful as evidence that the byte page API is widely depended on, not as proof of TS `U16` readiness.

## Recommended Next Prompt

```text
Use FIRMWARE_ROADMAP.md and SESSION_HANDOFF_2026-03-26_TS_U16_PAGES.md as source of truth. The current Teensy/DropBear TunerStudio path is working because it preserves the byte-serialized TS page contract. Native 16-bit TS pages remain deferred, but the narrowest safe experimental seam is now defined.

Session goal: prototype a Teensy-only experimental native-16-bit TunerStudio path for page 2 only, without disturbing the current working byte-serialized production path.

Read first:
- speeduino/pages.h
- speeduino/pages.cpp
- speeduino/comms.cpp
- speeduino/comms_legacy.cpp
- speeduino/page_crc.cpp
- speeduino/storage.cpp
- speeduino/storage_spi.cpp
- release/speeduino-dropbear-v2.0.1.ini
- SESSION_HANDOFF_2026-03-26_TS_U16_PAGES.md

Working rules:
1. Do not change the existing byte page contract in place.
2. Keep the default production path unchanged and working.
3. Gate any native-16-bit experiment behind an explicit Teensy-only alternate signature or capability.
4. Start with page 2 only.
5. Add a focused check/test around TS-visible page bytes vs CRC bytes if possible.

Deliverables:
- Add the smallest possible parallel TS page read/write/CRC seam for an experimental mode.
- Prototype it for page 2 only, or stop and document the precise blocker if even that is unsafe.
- Keep SPI-flash behavior explicit: either unchanged on byte mode or deliberately matched to the experimental stream.
- Leave the packaged production INI and current project flow working by default.

Verification:
- pio run -e teensy41
- any focused tests added

When done, report:
- whether the one-page experimental path worked
- the exact seam used or blocker found
- files changed
- compatibility risks
- recommended next prompt
```
