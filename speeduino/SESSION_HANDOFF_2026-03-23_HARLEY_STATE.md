# Session Handoff: Harley State Model

Date: 2026-03-23
Focus: Add direct AVR state coverage for the current `Harley` high-state sync and sync-loss contract

## Speeduino Decoder Contract

The current [triggerSetup_Harley()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) and [triggerPri_Harley()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- setup seeds `triggerToothAngle = 0`, a fixed `triggerFilterTime = 1500`, and an initial `toothLastToothTime`
- on a filtered primary event with the input state high, the decoder compares the new gap against the previous gap
- if the new gap is longer than the previous one, the decoder declares tooth `1`, captures `toothOneTime`, and sets `hasSync = true`
- otherwise the decoder declares tooth `2` and sets `triggerToothAngle = 157`
- on a filtered primary event with the input state low, the decoder drops sync, increments `syncLossCounter` if it had sync, and clears `toothCurrentCount`

This slice is about the current ISR contract, not about claiming external Harley wheel geometry.

## What Changed

- Added [Harley.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Harley/Harley.cpp)
- Added [Harley.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Harley/Harley.h)
- Wired `testHarley()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The tests cover:

- setup seeding the fixed filter and zero-degree starting state
- a first long gap with the primary input high establishing sync on tooth `1`
- a shorter follow-up high-state gap classifying as tooth `2` with the current `157` degree angle
- a later low-state primary event dropping sync and clearing the active tooth

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing with the Harley state suite in the full decoder entrypoint at `233/233`

## Remaining Limits

- The repo already had replay-backed Harley coverage; this slice complements it with direct ISR state assertions
- No new external Harley waveform evidence was added here
