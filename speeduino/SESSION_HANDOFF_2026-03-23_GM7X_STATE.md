# Session Handoff: GM 7X State Model

Date: 2026-03-23
Focus: Add direct AVR state coverage for the current `GM 7X` short-gap sync contract

## Speeduino Decoder Contract

The current [triggerSetup_GM7X()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) and [triggerPri_GM7X()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- setup uses fixed `60` degree teeth
- the primary ISR increments the tooth counter before checking for sync
- once two prior tooth timestamps exist, the decoder computes `targetGap` as half of the previous gap
- if the current gap is shorter than that half-gap threshold, the decoder forces sync on logical tooth `3`
- uniform gaps do not establish sync
- when the counter advances past tooth `7`, the decoder wraps back to tooth `1` without incrementing `startRevolutions`

This slice is about current decoder behavior, not about adding new external waveform claims.

## What Changed

- Added [Gm7X.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Gm7X/Gm7X.cpp)
- Added [Gm7X.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Gm7X/Gm7X.h)
- Wired `testGm7X()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The tests cover:

- fixed `60` degree setup
- short-gap sync on logical tooth `3`
- uniform-gap no-sync behavior with the same half-gap threshold prepared
- wrap from tooth `7` back to tooth `1`

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing with the GM 7X state suite in the full decoder entrypoint at `229/229`

## Remaining Limits

- The repo already had replay-backed GM 7X coverage; this slice complements it with direct ISR state assertions
- No new external waveform evidence was added here
