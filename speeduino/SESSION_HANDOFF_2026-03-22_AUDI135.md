# Session Handoff: Audi 135 State Model

Date: 2026-03-22
Focus: Add direct AVR state coverage for the current `Audi 135` sync and decimation contract

## Speeduino Decoder Contract

The current [triggerPri_Audi135()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) and [triggerSec_Audi135()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- the crank wheel has `135` physical teeth, but only every third tooth becomes an effective decoder tooth
- a cam edge establishes sync by setting `toothCurrentCount = 0` and priming the next primary tooth to count immediately
- the first counted effective tooth becomes tooth `1` and increments `startRevolutions`
- the decoder wraps after effective tooth `45`
- when `useResync = 1`, a later cam edge resets the effective counter back to `0` until the next counted primary tooth

This slice is intentionally about current code behavior, not external tooth-geometry claims.

## What Changed

- Added [Audi135.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Audi135/Audi135.cpp)
- Added [Audi135.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Audi135/Audi135.h)
- Wired `testAudi135()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The tests cover:

- cam-established sync and the first counted effective tooth
- every-third-tooth decimation across the first two effective teeth
- wrap from effective tooth `45` back to tooth `1`
- `useResync` resetting the effective counter until the next counted primary tooth

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing with the new Audi 135 state suite in the full entrypoint

## Remaining Limits

- The repo already had replay-backed Audi 135 coverage; this new suite complements it with direct ISR state checks
- No new external waveform evidence was introduced in this slice
