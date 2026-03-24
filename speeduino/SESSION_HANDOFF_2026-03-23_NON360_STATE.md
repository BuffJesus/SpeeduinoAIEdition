# Session Handoff: Non-360 State Model

Date: 2026-03-23

Focus: Add a narrow direct AVR state slice for `DECODER_NON360` and formalize the maintained current-code contract

## Current decoder contract

The current [triggerSetup_non360()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [getRPM_non360()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [getCrankAngle_non360()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- setup scales `triggerToothAngle` by `TrigAngMul` instead of using the plain dual-wheel tooth angle
- setup seeds the primary and secondary debounce filters for the `NON360` path and leaves the decoder unsynced on the `UINT8_MAX` sentinel
- sync establishment still uses the shared dual-wheel ISR path, so a valid secondary pulse primes `toothCurrentCount = triggerTeeth` and the next primary pulse wraps to tooth `1`
- `getRPM_non360()` is intentionally gated on both `hasSync` and `toothCurrentCount != 0`
- `getCrankAngle_non360()` treats `toothCurrentCount == 0` as "the secondary tooth was last", maps that back to the last primary tooth, divides by `TrigAngMul`, and then applies `triggerAngle`

## Implemented in-tree

- Added direct AVR state coverage in [Non360.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Non360/Non360.cpp)
- Wired the suite into the full decoder entrypoint in [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The maintained direct state coverage now locks:

- multiplier-scaled setup and filter seeding
- sync / wrap behavior through the shared dual-wheel ISR path under `DECODER_NON360` config
- RPM gating while unsynced or while the last observed tooth is the secondary sentinel
- crank-angle fallback when `toothCurrentCount == 0`

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing at `259/259` in the full decoder entrypoint

## Notes

- This is a compact current-code state slice, not a claim about broader waveform evidence beyond the maintained `NON360` setup / RPM / angle contract
- No Rover replay work changed here
