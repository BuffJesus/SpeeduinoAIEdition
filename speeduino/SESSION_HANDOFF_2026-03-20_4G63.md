# Session Handoff - 2026-03-20 (4G63 Replay Slice)

## Workspace

- Repo root: [speeduino](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino)
- Test root: [test](C:/Users/Cornelio/Desktop/speeduino-202501.6/test)
- Focus area: Phase 2 regression harness - 4G63 replay coverage and workflow breakage cleanup

## What Changed This Session

### 4G63 replay-backed decoder coverage added

- Added trace headers in:
  - [4g63_cam_sync_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/4g63_cam_sync_trace.h)
  - [4g63_no_cam_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/4g63_no_cam_trace.h)
  - [4g63_sync_loss_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/4g63_sync_loss_trace.h)
- Extended [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp) with:
  - `setup_trace_4g63()`
  - explicit rising/falling wrappers for primary and secondary edges
  - three 4G63 replay tests

### What the new 4G63 tests currently prove

- Primary-only replay stays unsynced
- A replayed cam-phase candidate trace does not falsely claim sync and advances the tooth counter to the currently observed candidate state
- An extra replayed cam edge does not falsely create sync under the current host harness

### Important harness note

- The 4G63 decoder relies heavily on live pin-state sampling during primary/secondary edge processing
- Under the current host replay harness, the modeled secondary-edge sequence does not reach full sync, even though the same decoder logic clearly contains sync paths in production code
- The tests added here therefore lock current observed host-harness behavior rather than asserting a fabricated full-sync state
- If higher-fidelity 4G63 replay coverage is needed later, the next refinement is likely improving the harness around live trigger pin-state / port behavior rather than changing decoder logic

### GitHub workflow breakage cleanup completed

- Fixed simulator-unit-test build issues by:
  - updating stale `SetRevolutionTime` declarations in:
    - [tests_crankmaths.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_math/tests_crankmaths.cpp)
    - [test_ign_calcs.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_schedule_calcs/test_ign_calcs.cpp)
  - moving fuel global definitions from [speeduino.ino](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/speeduino.ino) into [globals.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/globals.cpp) so non-main test links succeed
  - tightening `isLegacyCommandByte()` in [comms_legacy.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/comms_legacy.cpp) so punctuation between `Z` and `a` is no longer treated as a valid legacy command
- Fixed the STM32 Arduino-CLI memory-delta workflow by adding `elapsedMillis` to [.github/workflows/pr-memory-deltas-generate.yml](C:/Users/Cornelio/Desktop/speeduino-202501.6/.github/workflows/pr-memory-deltas-generate.yml)

## Verified Results

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passed, `178/178`
- `pio test -e megaatmega2560_sim_unittest --filter test_init`
  - passed
- `arduino-cli compile --fqbn STMicroelectronics:stm32:GenF4 ... speeduino/speeduino.ino`
  - passed locally after matching the workflow dependency set

## Notes

- Replay-backed decoder coverage now includes a tenth family in the harness, but 4G63 is presently covered as a no-false-sync / phase-tracking case rather than a full-sync replay case
- No production decoder logic was changed for 4G63; only tests and workflow/build fixes were made

## Safe Next Slice

Continue Phase 2 regression harness with one of:
1. Add replay coverage for `GM 7X`
2. Improve the replay harness so pin-state-dependent decoders like `4G63` can reach full sync in host tests
3. Shift to configuration migration coverage in [test_config_migrations.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_updates/test_config_migrations.cpp)

## Recommended Prompt For Next Session

`Continue Phase 2 regression harness from SESSION_HANDOFF_2026-03-20_4G63.md. 4G63 replay coverage now exists, but under the current host harness it only proves no-false-sync / candidate-tooth progression, not full sync. test_decoders now passes 178/178. Workflow fixes for simulator unit tests and STM32 Arduino CLI memory-delta builds were also applied. Next recommended slice: GM 7X replay coverage, or improve the replay harness so 4G63 full-sync behavior becomes testable.`
