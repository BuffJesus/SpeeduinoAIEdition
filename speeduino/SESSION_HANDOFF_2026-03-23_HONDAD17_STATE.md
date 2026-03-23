# Session Handoff: Honda D17 State Model

Date: 2026-03-23
Focus: Add direct AVR state coverage for the current `Honda D17` short-gap sync and wrap contract

## Speeduino Decoder Contract

The current [triggerSetup_HondaD17()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) and [triggerPri_HondaD17()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- setup seeds a `30` degree tooth angle and leaves the decoder unsynced with no secondary path
- every primary tooth increments the counter, but sync only establishes when the `13th` tooth arrives with a gap shorter than half of the previous gap
- uniform gaps therefore do not sync, because the short reference gap never appears
- once synced, the next `13th` short-gap reference resets the counter back to `0`, and the following tooth becomes tooth `1`, captures `toothOneTime`, and increments `startRevolutions`

This slice is about the current ISR contract, not about claiming external Honda wheel geometry.

## What Changed

- Added [HondaD17.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/HondaD17/HondaD17.cpp)
- Added [HondaD17.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/HondaD17/HondaD17.h)
- Wired `testHondaD17()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The tests cover:

- setup seeding the current `30` degree tooth angle
- short-gap sync on the `13th` tooth
- uniform-gap no-sync behavior
- synced wrap across the next reference tooth into revolution two

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing with the Honda D17 state suite in the full decoder entrypoint at `245/245`

## Remaining Limits

- The repo already had replay-backed Honda D17 coverage; this slice complements it with direct ISR state assertions
- No new external waveform evidence was added here
