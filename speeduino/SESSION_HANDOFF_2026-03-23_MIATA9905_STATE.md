# Session Handoff: Miata 99-05 State Model

Date: 2026-03-23
Focus: Add direct AVR state coverage for the current `Miata 99-05` cam-edge sync and wrap contract

## Speeduino Decoder Contract

The current [triggerSetup_Miata9905()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_Miata9905()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSec_Miata9905()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- setup starts unsynced with `toothCurrentCount = 99`, `secondaryToothCount = 0`, `triggerToothAngle = 90`, and a fixed `triggerFilterTime = 1500`
- while unsynced, cam edges are counted directly; when `secondaryToothCount == 2`, the next accepted primary tooth forces `toothCurrentCount = 6` and establishes sync
- extra cam edges do not falsely sync by themselves; they only defer sync until a later clean two-edge cam pair is followed by the next primary tooth
- once synced, the decoder uses the current alternating `70 / 110` degree tooth-angle model and wraps from tooth `8` back to tooth `1`, incrementing `startRevolutions`

This slice is about current ISR behavior, not about claiming ideal physical Miata wheel geometry.

## What Changed

- Added [Miata9905.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Miata9905/Miata9905.cpp)
- Added [Miata9905.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Miata9905/Miata9905.h)
- Wired `testMiata9905()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The tests cover:

- setup seeding the current unsynced sentinel and filter values
- a clean two-edge cam pair making the next primary tooth become synced tooth `6`
- a noisy four-edge cam group deferring sync until a later clean two-edge pair
- synced wrap from tooth `8` back to tooth `1`

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing with the Miata 99-05 state suite in the full decoder entrypoint at `237/237`

## Remaining Limits

- The repo already had replay-backed Miata 99-05 coverage; this slice complements it with direct ISR state assertions
- No new external waveform evidence was added here
