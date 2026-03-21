# Session Handoff: Updates v2 to v10 Migration Coverage

## Completed

- Added direct host-side migration coverage for:
  - `v2 -> v3`
  - `v3 -> v4`
  - `v4 -> v5`
  - `v7 -> v8`
  - `v8 -> v9`
  - `v9 -> v10`

## Coverage Added

- `v2 -> v3`
  - ignition table +40 offset migration
- `v3 -> v4`
  - CAN addressing defaults
  - invalid spark-duration repair
- `v4 -> v5`
  - cranking enrichment scalar-to-curve conversion
- `v7 -> v8`
  - flex boost/fuel/advance table generation from legacy scalar settings
- `v8 -> v9`
  - legacy load-source copy into split fuel / ignition algorithms
  - closed-loop boost default
- `v9 -> v10`
  - aux input disable defaults
  - ADC filter defaults

## Validation

- `pio test -e megaatmega2560_sim_unittest --filter test_updates`
- Result: `38/38` passing

## Notes

- Migration coverage is now broad across `v2 -> v24`, plus new/future EEPROM guard behavior.
- The notable remaining untested update steps are the EEPROM relocation migrations:
  - `v5 -> v6`
  - `v6 -> v7`
- Those are still host-testable, but they are more about raw EEPROM block movement and `loadConfig()` handoff than config-field translation.

## Next Roadmap Step

- Finish the migration chain with relocation tests for `v5 -> v7`, or
- pivot back to Phase 2 trigger replay/noise coverage.
