# TunerStudio Native U16 Page Investigation

## Outcome

Native `U16` TunerStudio tune pages were **not** implemented in this session.

The current Teensy 4.1 / DropBear TunerStudio path is working because it preserves the existing **byte-serialized virtual page contract**. A safe native-`U16` experiment would require a **parallel Teensy-only TS page path**, not an in-place conversion of the existing page API.

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
