# Session Handoff: Updates v10 to v13 Migration Coverage

## Completed

- Added direct host-side migration tests for:
  - `v10 -> v11`
  - `v11 -> v12`
  - `v12 -> v13`

## Coverage Added

- `v10 -> v11`
  - priming pulse table seeding from legacy scalar value
  - ASE amount/count/bin defaults
  - coolant advance defaults
  - tacho duration clamp
  - MAP AE default seeding
  - secondary fuel table disable default
- `v11 -> v12`
  - battery voltage correction default
  - legacy MAP mode default
  - secondary fuel switch defaults
- `v12 -> v13`
  - battery correction mode default
  - baro fuel curve defaults
  - idle advance enable/TPS/RPM defaults
  - idle advance bins/values defaults

## Validation

- `pio test -e megaatmega2560_sim_unittest --filter test_updates`
- Result: `32/32` passing

## Notes

- Migration tests now cover a contiguous block from `v10 -> v15`, plus the later `v18 -> v24` path and new/future EEPROM guard behavior.

## Next Roadmap Step

- Continue update coverage for the remaining earliest migrations (`v2 -> v10`) if you want the migration chain nearly complete, or
- switch back to trigger replay/noise work for more Phase 2 decoder coverage.
