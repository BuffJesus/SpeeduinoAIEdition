# Session Handoff: Basic Distributor State Model

Date: 2026-03-23
Focus: Add direct AVR state coverage for the current `Basic Distributor` sync, wrap, and filter contract

## Speeduino Decoder Contract

The current [triggerSetup_BasicDistributor()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) and [triggerPri_BasicDistributor()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- setup uses `triggerActualTeeth = nCylinders`, with a fallback to `1` if cylinders are unset
- setup uses evenly spaced teeth on the distributor: `720 / nCylinders` degrees for `FOUR_STROKE`, `360 / nCylinders` for `TWO_STROKE`
- the first accepted primary pulse immediately establishes sync, sets tooth `1`, and increments `startRevolutions`
- once synced, the decoder advances one tooth per accepted pulse and wraps from tooth `triggerActualTeeth` back to tooth `1`
- when the lite trigger filter is enabled, each valid synced pulse sets `triggerFilterTime` to `25%` of the previous accepted gap, so shorter post-sync gaps are rejected

This slice is about current decoder behavior, not about claiming any new external waveform truth.

## What Changed

- Added [BasicDistributor.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/BasicDistributor/BasicDistributor.cpp)
- Added [BasicDistributor.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/BasicDistributor/BasicDistributor.h)
- Wired `testBasicDistributor()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The tests cover:

- stroke-dependent setup angles for `4` cylinders in `FOUR_STROKE` and `TWO_STROKE`
- first-pulse sync and the next valid pulse advancing to tooth `2`
- wrap from tooth `4` back to tooth `1` on the fifth accepted pulse for a `4`-cylinder `4`-stroke setup
- lite-filter rejection of a short post-sync gap without losing sync

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing with the Basic Distributor state suite in the full decoder entrypoint at `225/225`

## Remaining Limits

- The repo already had replay-backed Basic Distributor coverage; this slice complements it with direct ISR state assertions
- No new external waveform evidence was added here
