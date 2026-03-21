# Session Handoff: Basic Distributor Replay Coverage

## Workspace

- Repo root: [speeduino-202501.6](C:/Users/Cornelio/Desktop/speeduino-202501.6)
- Focus area: Phase 2 regression harness - next uncovered decoder replay slice

## What Changed This Session

- Added three new replay trace headers for the basic distributor decoder in [test/test_decoders/traces](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces):
  - [basic_distributor_sync_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/basic_distributor_sync_trace.h)
  - [basic_distributor_wrap_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/basic_distributor_wrap_trace.h)
  - [basic_distributor_short_gap_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/basic_distributor_short_gap_trace.h)
- Extended [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp) with:
  - `setup_trace_basic_distributor()`
  - `test_trace_replay_basic_distributor_first_pulse_establishes_sync()`
  - `test_trace_replay_basic_distributor_wraps_on_fifth_pulse_for_four_cylinder_four_stroke()`
  - `test_trace_replay_basic_distributor_short_gap_is_filtered_after_sync()`
- Updated [README.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/README.md) with the new decoder total and handoff link

## Coverage Added

- Basic distributor replay coverage now verifies:
  - the first valid tooth immediately establishes sync
  - a 4-cylinder 4-stroke distributor wraps back to tooth 1 on the fifth pulse
  - the even-tooth lite trigger filter rejects a very short post-sync gap

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passed, `188/188`

## Notes

- No production decoder logic changed in this slice; changes are replay tests only
- The short-gap test is based directly on the current `setFilter()` contract in [decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp):
  - `TRIGGER_FILTER_LITE` sets `triggerFilterTime` to 25% of the previous valid gap
- The setup seeds `toothLastToothTime = micros()` before replay so the first processed gap comes from the trace timing rather than simulator startup time

## Related Verified State

- `test_decoders`: `188/188`
- `test_updates` + `test_updates_tail`: `43/43`
- `test_ign`: `146/146`
- `test_sensors`: `50/50`

## Remaining Decoder Replay Gaps

- Replay coverage is still not complete across all supported trigger patterns
- Uncovered candidates still include:
  - `HondaJ32`
  - `MazdaAU`
  - `Subaru67`
  - `Daihatsu`
  - `Harley`
  - `36-2-2-2`
  - `36-2-1`
  - `420a`
  - `Vmax`
  - `RoverMEMS`

## Safe Next Slice

1. Continue decoder replay coverage with another common uncovered family:
   - `HondaJ32`
   - `Subaru67`
   - `36-2-1` or `36-2-2-2`
2. Or return to sensor integration coverage with explicit source-read seams for `readMAP()` and `readBaro()`

## Recommended Prompt For Next Session

`Continue Phase 2 regression harness from SESSION_HANDOFF_2026-03-21_BASIC_DISTRIBUTOR.md. Replay-backed decoder coverage now includes BasicDistributor first-pulse sync, four-cylinder wrap, and post-sync short-gap filtering. test_decoders passes 188/188, but replay coverage is still incomplete across many supported trigger families. Next recommended decoder slice: HondaJ32, Subaru67, or a 36-2-x family.` 
