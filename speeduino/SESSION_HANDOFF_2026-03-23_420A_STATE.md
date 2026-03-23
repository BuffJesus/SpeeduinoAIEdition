# Session Handoff: 420a State Model

Date: 2026-03-23
Focus: Add direct AVR state coverage for the current `420a` secondary-falling sync and wrap contract

## Speeduino Decoder Contract

The current [triggerSetup_420a()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_420a()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSec_420a()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- setup starts on tooth `1` with `triggerToothAngle = 20`, no sync, and the secondary path enabled
- a secondary falling edge while the primary input is high establishes sync on tooth `13`
- a secondary falling edge while the primary input is low establishes sync on tooth `5`
- if already synced, a secondary falling edge on the wrong tooth increments `syncLossCounter` and realigns the tooth count back to the expected sync tooth
- once synced, the primary side wraps from tooth `16` back to tooth `1`, captures `toothOneTime`, and increments `startRevolutions`

This slice is about the current ISR contract, not about claiming external 420a wheel geometry.

## What Changed

- Added [FourTwentyA.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/FourTwentyA/FourTwentyA.cpp)
- Added [FourTwentyA.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/FourTwentyA/FourTwentyA.h)
- Wired `testFourTwentyA()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The tests cover:

- setup seeding the current tooth-one start state
- sync on a secondary falling edge with primary high
- sync on a secondary falling edge with primary low
- sync-loss counting and tooth realignment when a synced secondary event arrives on the wrong tooth
- synced wrap after tooth `16`

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing with the 420a state suite in the full decoder entrypoint at `250/250`

## Remaining Limits

- This is a direct state-machine slice only; no new replay trace or external waveform evidence was added here
- The tests intentionally lock current code behavior, including the current tooth-`13` / tooth-`5` sync entry assumptions
