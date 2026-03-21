# Session Handoff: Updates v13 to v15 Migration Coverage

## Completed

- Added direct host-side migration coverage for `v13 -> v14` in `test/test_updates/test_config_migrations.cpp`.
- Added direct host-side migration coverage for `v14 -> v15`, including legacy calibration-table remap and `writeCalibration()` interception.

## Coverage Added

- `v13 -> v14`
  - cranking enrichment scaling
  - injector timing curve seeding
  - flex advance offset migration
  - PID resolution migration with saturation behavior
  - DFCO / AE / soft-limit / VSS defaults
- `v14 -> v15`
  - CLT / IAT / O2 legacy calibration extraction from EEPROM
  - generated calibration bins
  - `writeCalibration()` call path
  - oil / fuel pressure defaults
  - WMI defaults
  - programmable output default disable
  - MAP multiply / AE apply / priming delay / ASE taper defaults

## Validation

- `pio test -e megaatmega2560_sim_unittest --filter test_updates`
- Result: `29/29` passing

## Notes

- These tests use `updatesTestSetStopAfterStore(true)` so each migration step is asserted in isolation.
- The `v14 -> v15` test seeds the legacy EEPROM calibration slots through `EEPROMWriteRaw()` rather than touching the EEPROM object directly.

## Next Roadmap Step

- Continue migration coverage for earlier untested version steps such as `v10 -> v13`, or
- switch back to replay/noise harness work for another trigger-capture slice.
