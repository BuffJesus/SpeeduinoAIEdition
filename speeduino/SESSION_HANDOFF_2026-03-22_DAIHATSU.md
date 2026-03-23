# Session Handoff: Daihatsu Plus-One State Model

Date: 2026-03-22
Focus: Lock the current `Daihatsu +1` primary sync contract in direct AVR state tests

## Speeduino Decoder Contract

The current [triggerPri_Daihatsu()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) implementation is a compact primary-only state machine:

- `toothSystemCount` must reach at least `3` before unsynced pattern detection starts
- sync is established by identifying the short extra tooth as tooth `2`
- on `4` cylinders the extra tooth is accepted when `curGap` is below roughly `3/8` of the prior long gap
- on `3` cylinders the extra tooth is accepted when `curGap` is below `1/4` of the prior long gap
- once synced, the decoder wraps when `toothCurrentCount == triggerActualTeeth`

Safe scope:

- test the current ISR contract for `3`- and `4`-cylinder configurations
- do not claim physical tooth-angle truth beyond the existing implementation

## What Changed

- Added [Daihatsu.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Daihatsu/Daihatsu.cpp)
- Added [Daihatsu.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Daihatsu/Daihatsu.h)
- Wired `testDaihatsu()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The tests cover:

- `4`-cylinder sync on the short extra tooth
- `4`-cylinder wrap and revolution counting after tooth `5`
- `3`-cylinder sync on the short extra tooth
- `3`-cylinder wrap and revolution counting after tooth `4`

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing after the Daihatsu suite landed

## Remaining Limits

- No real-capture Daihatsu replay source was added
- This slice only locks the existing primary state machine, not external waveform truth
