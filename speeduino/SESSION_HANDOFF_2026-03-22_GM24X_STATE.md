# Session Handoff: GM 24X State Model

Date: 2026-03-22
Focus: Add direct AVR state coverage for the current `GM 24X` cam-reset sync contract

## Speeduino Decoder Contract

The current [triggerSetup_24X()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_24X()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSec_24X()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- startup uses `toothCurrentCount = 25` as the unsynced sentinel
- while that sentinel is active, primary teeth do not establish sync
- each cam pulse resets `toothCurrentCount = 0` and sets `revolutionOne = 1`
- the next primary tooth becomes tooth `1`, establishes sync, flips `revolutionOne`, and increments `startRevolutions`
- later primary teeth advance by the `24X` lookup-table deltas, such as `6` and `15` degrees at the start of the sequence
- any later cam pulse restarts the sequence again from tooth `0`

This slice is about current decoder behavior, not new external waveform claims.

## What Changed

- Added [Gm24X.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Gm24X/Gm24X.cpp)
- Added [Gm24X.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Gm24X/Gm24X.h)
- Wired `testGm24X()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The tests cover:

- the startup sentinel blocking primary-only sync
- a cam pulse priming the next primary tooth to become synced tooth `1`
- early lookup-table stepping through the current `6` and `15` degree deltas
- a later cam reset restarting the next revolution cleanly

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing with the GM 24X state suite in the full decoder entrypoint

## Remaining Limits

- The repo already had replay-backed GM 24X coverage; this slice complements it with direct ISR state assertions
- No new external capture evidence was added here
