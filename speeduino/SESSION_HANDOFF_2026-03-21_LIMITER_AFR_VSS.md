# Session Handoff: AFR Target-Table And VSS-Gated Limiter Coverage

## Workspace

- Repo root: [speeduino-202501.6](C:/Users/Cornelio/Desktop/speeduino-202501.6)
- Focus area: Phase 2 regression harness - ignition/protection interaction coverage

## What Changed This Session

- Extended [test_limiter_state_machine.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_ign/test_limiter_state_machine.cpp) with:
  - `test_launch_state_machine_rolling_cut_respects_vss_gate_until_speed_drops`
  - `test_afr_protection_target_table_mode_uses_target_plus_deviation_threshold`
  - `test_afr_protection_target_table_mode_stays_inactive_when_target_rises_above_current_o2`
- Performed a small test-only cleanup:
  - added `seed_active_afr_inputs()` so repeated AFR-condition setup stays explicit and avoids duplicated field writes

## Coverage Added

- Rolling launch with VSS gating:
  - verifies `HARD_CUT_ROLLING` launch will not arm when `vss >= lnchCtrlVss`
  - verifies the same RPM/TPS state does arm once speed drops below the configured gate
  - confirms `calculateMaxAllowedRPM()` switches from the base hard rev limit to the launch limit only after the gate clears
- AFR protection target-table mode (`afrProtectEnabled = 2`):
  - verifies activation uses `afrTarget + afrProtectDeviation` as the threshold, not the fixed-value mode semantics
  - verifies equality at the threshold is sufficient for activation after the configured delay
  - verifies rising `afrTarget` can keep protection inactive when the same O2 reading is no longer over the target-relative threshold

## Verified Results

- `pio test -e megaatmega2560_sim_unittest --filter test_ign`
  - passed, `144/144`

## Related Verified State

- `test_updates` + `test_updates_tail`: `43/43`
- `test_decoders`: `182/182`

## Notes

- No production firmware logic changed in this slice; changes are host-side tests only
- The brief regression during this session came from the new AFR setup helper incorrectly overwriting `RPMdiv100`; that was corrected before final verification

## Safe Next Slice

1. Continue ignition/protection interaction coverage:
   - flat-shift behavior under VSS-enabled launch configs
   - target-table AFR protection reactivation edge cases
   - boost-cut / AFR / RPM protection precedence in combined scenarios
2. Or move to another roadmap area:
   - sensor filtering edge cases
   - logger/status-channel regression checks
   - init-time config/runtime consistency

## Recommended Prompt For Next Session

`Continue Phase 2 regression harness from SESSION_HANDOFF_2026-03-21_LIMITER_AFR_VSS.md. test/test_ign/test_limiter_state_machine.cpp now includes rolling-launch VSS gating coverage plus AFR target-table protection mode coverage, and test_ign passes 144/144. Updates remain split and passing at 43/43, and decoder replay coverage remains at 182/182. Next recommended slice: add another ignition/protection interaction test such as combined boost/AFR/RPM precedence or AFR target-table reactivation edge cases.` 
