# Session Handoff: Vmax State Model

Date: 2026-03-23
Focus: Add direct AVR state coverage for the current `Vmax` wide-lobe sync and wrap contract

## Speeduino Decoder Contract

The current [triggerSetup_Vmax()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) and [triggerPri_Vmax()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- setup seeds `triggerToothAngle = 0`, `triggerFilterTime = 1500`, and starts with a pre-set valid-trigger flag so lobe width can be measured
- on the configured leading edge, a valid lobe start records timing and, if already synced, publishes the current tooth angle and increments `startRevolutions` on tooth `1`
- on the opposite edge, a sufficiently wide lobe establishes or wraps sync onto tooth `1`
- a narrow lobe after sync advances the tooth counter normally
- a wide lobe at tooth `6` wraps back to tooth `1` without incrementing `syncLossCounter`

This slice is about the current ISR contract, not about claiming external Yamaha wheel geometry.

## What Changed

- Added [Vmax.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Vmax/Vmax.cpp)
- Added [Vmax.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Vmax/Vmax.h)
- Wired `testVmax()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The tests cover:

- setup seeding the filter and zero-degree starting state
- a first wide lobe establishing sync on tooth `1`
- a following small lobe advancing to tooth `2`
- a wide lobe at tooth `6` wrapping back to tooth `1`

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing with the Vmax state suite in the full decoder entrypoint at `254/254`

## Remaining Limits

- This is a direct state-machine slice only; no new replay trace or external waveform evidence was added here
- The assertions intentionally lock the current lobe-width-based ISR behavior, including its current `toothCurrentCount == 6` wrap rule
