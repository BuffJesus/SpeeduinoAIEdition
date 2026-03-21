# Session Handoff: MAP Switch-Point And EMAP Sentinel Coverage

## Workspace

- Repo root: [speeduino-202501.6](C:/Users/Cornelio/Desktop/speeduino-202501.6)
- Focus area: Phase 2 regression harness - MAP sampling threshold and EMAP-disabled edge cases

## What Changed This Session

- Extended [test_map_sampling.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_sensors/test_map_sampling.cpp) with four more host-side MAP sampling tests:
  - `test_cycleAverageMAPReading_switch_point_fallback_resets_partial_average`
  - `test_cycleAverageMAPReading_emap_disabled_keeps_sentinel_on_cycle_end`
  - `test_cycleMinimumMAPReading_switch_point_fallback_resets_minimum`
  - `test_eventAverageMAPReading_switch_point_fallback_resets_partial_average`
- Updated [README.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/README.md) to reflect the new sensor coverage and the current `test_sensors` total

## Coverage Added

- Cycle-average MAP:
  - verifies dropping to exactly `mapSwitchPoint` exits averaged mode immediately and reseeds the accumulator from the latest instantaneous reading
  - verifies EMAP-disabled operation preserves the `UINT16_MAX` sentinel on cycle completion instead of producing a fake averaged EMAP value
- Cycle-minimum MAP:
  - verifies dropping to exactly `mapSwitchPoint` discards the in-progress minimum and reseeds from the latest instantaneous MAP reading
- Event-average MAP:
  - verifies dropping to exactly `mapSwitchPoint` exits event averaging immediately and reseeds from the latest instantaneous reading

## Verified Results

- `pio test -e megaatmega2560_sim_unittest --filter test_sensors`
  - passed, `44/44`

## Related Verified State

- `test_ign`: `146/146`
- `test_updates` + `test_updates_tail`: `43/43`
- `test_decoders`: `182/182`

## Notes

- No production firmware logic changed in this slice; changes are host-side tests and README documentation only
- The EMAP sentinel test matches the current helper contract in [sensors.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/sensors.cpp):
  - `cycleAverageEndCycle()` only derives averaged EMAP when `sensorReadings.emapADC != UINT16_MAX`

## Safe Next Slice

1. Continue sensor state-transition coverage:
   - `readMAP()` integration behavior for `configPage2.mapSample` mode switching
   - `setMAPValuesFromReadings()` behavior with EMAP enabled vs disabled
   - baro fallback transitions between stored baro, external sensor, and MAP-derived baro
2. Or move to another Phase 2 area:
   - logger/status-channel regression checks
   - init-time runtime/config consistency tests

## Recommended Prompt For Next Session

`Continue Phase 2 regression harness from SESSION_HANDOFF_2026-03-21_MAP_SWITCHPOINT_EMAP.md. test/test_sensors/test_map_sampling.cpp now covers mapSwitchPoint boundary fallback resets for cycle-average, cycle-minimum, and event-average MAP, plus EMAP-disabled sentinel preservation at cycle end. test_sensors passes 44/44, with test_ign at 146/146, test_updates + test_updates_tail at 43/43, and test_decoders at 182/182. Next recommended slice: readMAP integration mode switching or baro fallback transitions.` 
