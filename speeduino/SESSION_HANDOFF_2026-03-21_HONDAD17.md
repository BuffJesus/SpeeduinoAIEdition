# Session Handoff: Honda D17 Replay Coverage

## Workspace

- Repo root: [speeduino-202501.6](C:/Users/Cornelio/Desktop/speeduino-202501.6)
- Focus area: Phase 2 regression harness - next uncovered decoder replay slice

## Are All Trigger Patterns Covered?

- No.
- The replay-backed decoder harness now covers 13 families, but the firmware supports substantially more trigger patterns than that.
- Uncovered replay candidates still include examples such as:
  - `BasicDistributor`
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

## What Changed This Session

- Added three new replay trace headers for Honda D17 in [test/test_decoders/traces](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces):
  - [hondad17_sync_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/hondad17_sync_trace.h)
  - [hondad17_no_sync_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/hondad17_no_sync_trace.h)
  - [hondad17_wrap_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/hondad17_wrap_trace.h)
- Extended [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp) with:
  - `setup_trace_hondad17()`
  - `test_trace_replay_hondad17_short_gap_establishes_sync()`
  - `test_trace_replay_hondad17_uniform_gaps_do_not_sync()`
  - `test_trace_replay_hondad17_synced_cycle_wraps_on_next_reference_tooth()`
- Updated [README.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/README.md) to reflect the new decoder count and handoff

## Coverage Added

- Honda D17 decoder behavior now has replay-backed coverage for:
  - short-gap 13th-tooth sync establishment
  - uniform-gap rejection when the reference short gap never appears
  - synced-cycle wrap behavior across the next full tooth sequence

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passed, `185/185`

## Notes

- No production decoder logic changed in this slice; changes are replay tests plus a test-harness setup fix
- One harness issue surfaced during verification:
  - with `toothLastToothTime` left at `0`, Honda D17 could interpret the startup gap from boot-time `micros()` as a valid long baseline
  - `setup_trace_hondad17()` now seeds `toothLastToothTime = micros()` before replay so the first processed gap comes from the trace itself, not simulator startup time
- This was a deterministic harness correction, not a runtime decoder change

## Related Verified State

- `test_decoders`: `185/185`
- `test_updates` + `test_updates_tail`: `43/43`
- `test_ign`: `146/146`
- `test_sensors`: `50/50`

## Safe Next Slice

1. Continue decoder replay coverage for another uncovered common pattern:
   - `BasicDistributor`
   - `HondaJ32`
   - `Subaru67`
   - `36-2-1` or `36-2-2-2`
2. Or continue sensor integration coverage with explicit ADC/source seams for `readMAP()` and `readBaro()`

## Recommended Prompt For Next Session

`Continue Phase 2 regression harness from SESSION_HANDOFF_2026-03-21_HONDAD17.md. Replay-backed decoder coverage now includes Honda D17 short-gap sync, no-sync uniform gaps, and synced wrap behavior. test_decoders passes 185/185, but replay coverage is still not complete across all supported trigger patterns. Next recommended decoder slice: BasicDistributor, HondaJ32, Subaru67, or a 36-2-x family.` 
