# Session Handoff: Subaru 6/7 State Model

Date: 2026-03-23
Focus: Add direct AVR state coverage for the current `Subaru 6/7` cam-count sync and wrap contract

## Speeduino Decoder Contract

The current [triggerSetup_Subaru67()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_Subaru67()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSec_Subaru67()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- setup starts at tooth `1` with `secondaryToothCount = 0`, `triggerToothAngle = 2`, and the secondary path enabled
- cam pulses are counted between primary teeth, and the next primary decides sync from that queued cam count
- `secondaryToothCount == 3` on the next primary establishes sync on tooth `2`
- `secondaryToothCount == 1` at the wrong primary position does not sync; instead it increments `syncLossCounter`, clears the queued cam count, and realigns to tooth `5`
- once synced, the decoder wraps from tooth `12` to tooth `1`, captures `toothOneTime`, and increments `startRevolutions`

This slice is about the current ISR contract, not about claiming external Subaru wheel geometry.

## What Changed

- Added [Subaru67.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Subaru67/Subaru67.cpp)
- Added [Subaru67.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Subaru67/Subaru67.h)
- Wired `testSubaru67()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The tests cover:

- setup seeding the current tooth-one and secondary-enabled starting state
- three queued cam pulses making the next primary become synced tooth `2`
- a single queued cam pulse at the wrong primary position realigning to tooth `5` without sync
- synced wrap after the `12`-tooth cycle

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing with the Subaru 6/7 state suite in the full decoder entrypoint at `241/241`

## Remaining Limits

- The repo already had replay-backed Subaru 6/7 coverage; this slice complements it with direct ISR state assertions
- No new external waveform evidence was added here
