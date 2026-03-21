# Session Handoff: Trigger-State Override Seam And Harley Replay Coverage

## Workspace

- Repo root: [speeduino-202501.6](C:/Users/Cornelio/Desktop/speeduino-202501.6)
- Focus area: Phase 2 regression harness - unblock decoders that sample live trigger input state during replay

## What Changed This Session

- Added a unit-test-only trigger input seam in:
  - [decoders.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.h)
  - [decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp)
- The replay harness can now override primary / secondary / tertiary trigger readback through:
  - `testSetPrimaryTriggerState()`
  - `testSetSecondaryTriggerState()`
  - `testSetThirdTriggerState()`
  - `testClearTriggerStateOverrides()`
- `READ_PRI_TRIGGER()`, `READ_SEC_TRIGGER()`, and `READ_THIRD_TRIGGER()` now route through testable read helpers under `UNIT_TEST`, while still using the normal hardware path outside tests
- Added Harley replay coverage in [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp):
  - `setup_trace_harley()`
  - `test_trace_replay_harley_first_long_gap_establishes_sync()`
  - `test_trace_replay_harley_short_then_long_gap_toggles_between_teeth_two_and_one()`
  - `test_trace_replay_harley_low_primary_state_drops_sync()`
- Added trace headers:
  - [harley_sync_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/harley_sync_trace.h)
  - [harley_alternating_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/harley_alternating_trace.h)
  - [harley_low_state_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/harley_low_state_trace.h)
- Updated [README.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/README.md) with the new decoder total and seam summary

## Why This Slice Matters

- Some decoders do not rely on timing deltas alone; they also sample the live input pin state during the ISR path
- The previous replay approach could call decoder functions, but it could not deterministically control what `READ_PRI_TRIGGER()` returned
- This seam fixes that class of testability problem without changing runtime firmware behavior

## Coverage Added

- Harley decoder replay coverage now verifies:
  - a long high primary pulse establishes sync on tooth 1
  - a shorter post-sync gap maps to tooth 2 and a later longer gap returns to tooth 1
  - a low primary state on a later valid event forces sync loss and clears the tooth state

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passed, `191/191`

## Notes

- No runtime firmware behavior changed in this slice; the decoder seam is unit-test-only
- The seam is intentionally generic so it can be reused for other input-state-driven decoders, not just Harley
- This directly addresses the blocker from the previous failed replay attempts where `digitalWrite()` was not a reliable substitute for what `READ_PRI_TRIGGER()` actually sampled

## Related Verified State

- `test_decoders`: `191/191`
- `test_updates` + `test_updates_tail`: `43/43`
- `test_ign`: `146/146`
- `test_sensors`: `50/50`

## Safe Next Slice

1. Revisit previously blocked decoder families now that trigger-state overrides exist:
   - `HondaJ32`
   - `36-2-1`
   - other decoders that sample live input levels during decode
2. Or continue with another uncovered replay family that is timing-driven:
   - `Subaru67`
   - `MazdaAU`
   - `Daihatsu`

## Recommended Prompt For Next Session

`Continue Phase 2 regression harness from SESSION_HANDOFF_2026-03-21_HARLEY_REPLAY.md. A unit-test trigger-state override seam now exists in decoders.{h,cpp}, and Harley replay coverage is passing. test_decoders is now 191/191. Next recommended slice: revisit HondaJ32 or 36-2-1 using the new trigger-state seam, or add replay coverage for Subaru67 / MazdaAU.` 
