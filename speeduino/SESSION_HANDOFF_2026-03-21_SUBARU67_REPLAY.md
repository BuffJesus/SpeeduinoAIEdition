# Session Handoff: Subaru 6/7 Replay Coverage

## Workspace

- Repo root: [speeduino-202501.6](C:/Users/Cornelio/Desktop/speeduino-202501.6)
- Focus area: Phase 2 regression harness - add replay-backed coverage for another uncovered timing-driven decoder family

## What Changed This Session

- Added Subaru 6/7 replay coverage in [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp):
  - `setup_trace_subaru67()`
  - `makeSubaru67Callbacks()`
  - `test_trace_replay_subaru67_three_cam_pulses_establish_sync_on_tooth_two()`
  - `test_trace_replay_subaru67_synced_cycle_wraps_after_twelve_primary_teeth()`
  - `test_trace_replay_subaru67_single_cam_pulse_at_wrong_tooth_realigns_tooth_five_without_sync()`
- Added trace headers:
  - [subaru67_sync_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/subaru67_sync_trace.h)
  - [subaru67_wrap_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/subaru67_wrap_trace.h)
  - [subaru67_single_cam_misaligned_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/subaru67_single_cam_misaligned_trace.h)
- Updated [README.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/README.md) with the new decoder total and Subaru 6/7 coverage

## Why This Slice Matters

- `triggerPri_Subaru67()` has a very explicit sync contract: the next primary decides sync purely from how many cam pulses were queued since the previous primary
- That made Subaru 6/7 a good next replay target after the Harley seam work, because it exercises multi-channel ordering without needing more harness changes
- The new tests pin down both the happy path and one important recovery path where a single cam pulse appears at the wrong crank position

## Coverage Added

- Subaru 6/7 replay coverage now verifies:
  - three queued cam pulses establish sync on tooth 2
  - a synced cycle wraps after twelve primary teeth and increments `startRevolutions`
  - a lone misaligned cam pulse forces tooth realignment to 5 while staying out of sync

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passed, `194/194`

## Notes

- No production decoder logic changed in this slice
- Like the recent Honda D17 and Basic Distributor slices, `setup_trace_subaru67()` seeds `toothLastToothTime = micros()` so the first primary gap comes from the trace rather than simulator startup time
- The Subaru 6/7 traces are intentionally minimal and map directly to the decoder’s observable counter logic, not inferred oscilloscope shapes

## Related Verified State

- `test_decoders`: `194/194`
- `test_updates` + `test_updates_tail`: `43/43`
- `test_ign`: `146/146`
- `test_sensors`: `50/50`

## Safe Next Slice

1. Revisit a previously blocked family using the trigger-state seam and the now broader replay baseline:
   - `HondaJ32`
   - `36-2-1`
2. Or continue with another uncovered timing-driven decoder:
   - `MazdaAU`
   - `Daihatsu`
   - `RoverMEMS`

## Recommended Prompt For Next Session

`Continue Phase 2 regression harness from SESSION_HANDOFF_2026-03-21_SUBARU67_REPLAY.md. Subaru 6/7 replay coverage is now passing and test_decoders is 194/194. Next recommended slice: revisit HondaJ32 or 36-2-1 with the trigger-state seam, or add another timing-driven decoder family such as MazdaAU or Daihatsu.` 
