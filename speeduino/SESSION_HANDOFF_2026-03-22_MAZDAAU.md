# Session Handoff: Mazda AU State Model

Date: 2026-03-22
Focus: Lock the current `Mazda AU` sync and wrap contract in direct AVR state tests

## Speeduino Decoder Contract

The current [triggerPri_MazdaAU()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) and [triggerSec_MazdaAU()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is narrower and stranger than the comments suggest:

- setup starts with `toothCurrentCount = 99` as the unsynced sentinel
- once synced, the primary side is a `4`-edge cycle that wraps on tooth `5`
- the primary filter and `triggerToothAngle` alternate between `108` and `72` degree intervals
- in the current unsynced cam path, the third cam tooth establishes sync when `secondaryToothCount == 2`
- a short second cam gap also forces `secondaryToothCount = 2`, then immediately increments it to `3`, which means that path does not directly sync on the next line

This note is about current implementation behavior only, not about what the physical Mazda AU wheel should ideally do.

## What Changed

- Added [MazdaAU.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/MazdaAU/MazdaAU.cpp)
- Added [MazdaAU.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/MazdaAU/MazdaAU.h)
- Wired `testMazdaAU()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)
- Added a tiny `UNIT_TEST` helper in [decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) / [decoders.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.h) to seed the last secondary-tooth timestamp deterministically for state tests

The tests cover:

- sync on the third cam tooth through the current `secondaryToothCount == 2` path
- the short-gap cam branch that mutates the secondary counter without actually syncing
- synced primary wrap after four edges with the current `108 / 72 / 108 / 72` tooth-angle alternation

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing with the Mazda AU suite in the full decoder entrypoint

## Remaining Limits

- No replay-backed Mazda AU trace was added in this slice
- The new tests intentionally pin the current code, including the odd unsynced cam-counter behavior, rather than claiming that behavior is physically ideal
