# Session Handoff: DRZ400 State Model

Date: 2026-03-23

Focus: Add a narrow direct AVR state slice for `DECODER_DRZ400` and formalize the maintained current-code contract

## Current decoder contract

The current [triggerSetup_DRZ400()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) and [triggerSec_DRZ400()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- setup seeds a `6`-tooth dual-wheel style primary with `60` degree crank teeth and a dedicated secondary debounce filter
- the primary ISR path is still the shared dual-wheel decoder, so the custom DRZ behavior lives almost entirely in the secondary handler
- an unsynced valid secondary pulse forces sync, primes `toothCurrentCount = triggerTeeth`, and increments `syncLossCounter`
- a later synced secondary pulse realigns `toothCurrentCount = 6` without incrementing sync loss
- the next primary pulse after that realignment wraps to tooth `1` through the shared dual-wheel path and counts a revolution
- the secondary handler also recomputes `triggerSecFilterTime` from the last measured tooth-one period

## Implemented in-tree

- Added direct AVR state coverage in [DRZ400.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/DRZ400/DRZ400.cpp)
- Wired the suite into the full decoder entrypoint in [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The maintained direct state coverage now locks:

- setup-seeded primary and secondary filters
- unsynced secondary-pulse sync entry onto tooth `6`
- synced secondary-pulse realignment back to tooth `6` without extra sync-loss counting
- first-primary wrap to tooth `1` and revolution-count increment through the shared dual-wheel primary ISR

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing at `263/263` in the full decoder entrypoint

## Notes

- This is a compact current-code slice centered on the custom DRZ secondary ISR and its interaction with the shared dual-wheel primary path
- While landing this slice, the harness exposed that `Mazda AU`'s first cam-edge test is sensitive to stale global `curGap2`; the DRZ fixture now clears that shared scratch explicitly so the full decoder entrypoint stays order-safe
- No Rover replay work changed here
