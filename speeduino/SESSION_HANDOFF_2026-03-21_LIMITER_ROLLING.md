# Session Handoff: Limiter Rolling-Cut State Coverage

## Workspace

- Repo root: [speeduino-202501.6](C:/Users/Cornelio/Desktop/speeduino-202501.6)
- Focus area: Phase 2 regression harness - launch / flat-shift limiter state interactions under rolling hard-cut mode

## What Changed This Session

- Extended [test_limiter_state_machine.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_ign/test_limiter_state_machine.cpp) with two new state-machine tests:
  - `test_launch_state_machine_rolling_cut_uses_delta_threshold_but_keeps_base_limit_source`
  - `test_flat_shift_state_machine_rolling_cut_uses_delta_threshold_but_keeps_latched_limit_source`

## Coverage Added

- Launch control in `HARD_CUT_ROLLING` mode now has explicit host-side verification that:
  - activation uses `rollingProtRPMDelta[0]` to lower the trigger threshold
  - `calculateMaxAllowedRPM()` still reports the base launch limiter source (`lnchHardLim`)
- Flat shift in `HARD_CUT_ROLLING` mode now has explicit host-side verification that:
  - activation uses the negative rolling delta relative to the latched `clutchEngagedRPM`
  - `calculateMaxAllowedRPM()` still reports the latched clutch RPM limit source rather than the lowered rolling threshold

## Verified Results

- `pio test -e megaatmega2560_sim_unittest --filter test_ign`
  - passed, `141/141`
- Prior verified related suites remain:
  - `test_updates` + `test_updates_tail`: `43/43`
  - `test_decoders`: `182/182`

## Notes

- No production firmware logic changed in this slice; changes are host-side tests only
- This closes an interaction gap where rolling-cut behavior affected limiter flag transitions, but existing tests only covered base launch / flat-shift thresholds

## Safe Next Slice

1. Add another ignition/protection interaction test:
   - AFR protection in target-table mode (`afrProtectEnabled = 2`)
   - boost-cut behavior with invalid / edge protection modes
   - launch / flat-shift interaction with VSS gating under rolling cut
2. Or switch to another Phase 2 host-side subsystem:
   - sensor filtering edge cases
   - logger/status-channel regression checks
   - init-time config/runtime consistency tests

## Recommended Prompt For Next Session

`Continue Phase 2 regression harness from SESSION_HANDOFF_2026-03-21_LIMITER_ROLLING.md. Rolling-cut launch and flat-shift limiter state tests were added in test/test_ign/test_limiter_state_machine.cpp, verifying that activation thresholds honor rollingProtRPMDelta[0] while calculateMaxAllowedRPM() still reports the base limiter source. test_ign now passes 141/141, with test_updates + test_updates_tail at 43/43 and test_decoders at 182/182. Next recommended slice: add another ignition/protection interaction test such as AFR target-table protection mode or VSS-gated rolling launch behavior.` 
