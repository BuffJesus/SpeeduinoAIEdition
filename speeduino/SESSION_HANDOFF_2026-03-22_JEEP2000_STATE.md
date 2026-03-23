# Session Handoff: Jeep 2000 State Model

Date: 2026-03-22
Focus: Add direct AVR state coverage for the current `Jeep 2000` cam-reset sync contract

## Speeduino Decoder Contract

The current [triggerSetup_Jeep2000()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_Jeep2000()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSec_Jeep2000()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- startup uses `toothCurrentCount = 13` as the explicit unsynced sentinel
- while that sentinel is active, primary teeth do not advance state or establish sync
- each cam pulse resets `toothCurrentCount = 0`
- the next accepted primary tooth after that cam pulse becomes tooth `1`, sets `hasSync = true`, increments `startRevolutions`, and uses a `60` degree tooth angle
- within a revolution, the grouped primary spacing alternates as `20 / 20 / 60` degrees across the `12` effective teeth
- there is no primary-only wrap; the next revolution begins when the cam pulse resets the counter again

This note is about current decoder behavior, not about adding new external waveform claims.

## What Changed

- Added [Jeep2000.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Jeep2000/Jeep2000.cpp)
- Added [Jeep2000.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Jeep2000/Jeep2000.h)
- Wired `testJeep2000()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The tests cover:

- the startup sentinel blocking primary-only sync
- a cam pulse priming the next primary tooth to become synced tooth `1`
- grouped primary stepping through `20 / 20 / 60` degree gaps
- a later cam reset starting the next revolution cleanly

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing with the Jeep 2000 state suite in the full decoder entrypoint

## Remaining Limits

- The repo already had replay-backed Jeep 2000 coverage; this slice complements it with direct ISR state tests
- No new external waveform or scope evidence was added here
