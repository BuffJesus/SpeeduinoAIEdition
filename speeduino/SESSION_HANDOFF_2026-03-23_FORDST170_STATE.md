# Session Handoff: Ford ST170 State Model

Date: 2026-03-23

Focus: Formalize the current `Ford ST170` direct AVR state coverage as part of the Phase 2 maintained decoder baseline

## Current decoder contract

The current [triggerSetup_FordST170()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) and [triggerSec_FordST170()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- primary decoding stays on the existing missing-tooth path, with `36-1` setup and a `10` degree primary tooth angle
- primary-only operation reaches half-sync on the first missing-tooth gap, but does not establish full sequential sync
- cam activity before the first missing-tooth gap promotes the decoder to full sync and clears the half-sync bit
- short secondary noise pulses below the current secondary filter do not regroup the cam pattern or promote sync
- the sixth accepted cam tooth forces regrouping by resetting `secondaryToothCount` to `1`, setting `revolutionOne`, and clearing `triggerSecFilterTime`

This slice is about the current ISR contract and filter/grouping behavior, not about proving external ST170 wheel geometry.

## Implemented in-tree

- Kept the existing direct AVR state coverage in [ForsdST170.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/FordST170/ForsdST170.cpp)
- Added an explicit short-cam-noise assertion that matches the replay-backed `fordst170_cam_noise_trace` behavior

The direct state coverage now locks:

- primary-only half-sync on the first missing-tooth gap
- cam-before-gap promotion to full sync
- filtered short secondary noise without false regrouping
- sixth-cam-tooth regroup reset

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing with the Ford ST170 state model formalized in the full decoder entrypoint

## Notes

- The repo already had replay-backed Ford ST170 coverage for primary-only, clean cam sync, cam-noise filtering, and regroup behavior; this slice formalizes the direct AVR state contract alongside those traces
- No Rover replay work was changed here
