# Session Handoff: Tune Default Value Contract

## Workspace

- Repo root: [speeduino-202501.6](C:/Users/Cornelio/Desktop/speeduino-202501.6)
- Focus area: Phase 5 configuration compatibility - extend the tune audit from surface compatibility to selected fork-critical default values

## What Changed This Session

- Extended [check_stock_base_tune_compat.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/check_stock_base_tune_compat.py) with explicit value checks for selected fork-critical defaults:
  - knock defaults
  - rolling-cut defaults
  - DFCO defaults
  - launch defaults
  - VSS defaults
  - WMI defaults
  - oil-pressure defaults
  - air-con defaults
- Added normalized `.msq` constant-value extraction so scalar and array values can be compared in a stable way
- Updated [test_stock_base_tune_compat.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/tests/test_stock_base_tune_compat.py) so the fork-owned tune must satisfy every checked critical value
- Added a release-path unit test so the shipped [speeduino-dropbear-v2.0.1-base-tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/release/speeduino-dropbear-v2.0.1-base-tune.msq) must satisfy the same value contract against [speeduino-dropbear-v2.0.1.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/release/speeduino-dropbear-v2.0.1.ini)
- The fork-owned and release tunes already matched the new VSS / WMI / oil-pressure defaults, so no `.msq` value changes were needed for this slice

## Critical Default Contract Now Enforced

- Knock:
  - `knock_pin = A8`
  - `knock_trigger = HIGH`
  - `knock_limiterDisable = No`
  - threshold / MAP / retard / step / recovery values match the current INI defaults
- Rolling cut:
  - `rollingProtRPMDelta = -300 -200 -100 -50`
  - `rollingProtCutPercent = 50 65 80 95`
- DFCO:
  - `dfcoDelay = 0.1`
  - `dfcoRPM = 1500`
  - `dfcoHyster = 200`
  - `dfcoMinCLT = 70`
- Launch:
  - `launchPin = Board Default`
  - `launchEnable = No`
  - `launchHiLo = LOW`
  - `lnchCtrlVss = 255`
- VSS:
  - `vssMode = Off`
  - `vssPin = Board Default`
  - `vssPulsesPerKm = 0.0`
  - `vssAuxCh = Aux0`
- WMI:
  - `wmiEnabled = Off`
  - `wmiMode = Simple`
  - `wmiTPS = 0.0`
  - `wmiRPM = 2000.0`
  - `wmiMAP = 120.0`
  - `wmiMAP2 = 0.0`
  - `wmiIAT = 75.0`
  - `wmiOffset = 0.0`
  - `wmiAdvEnabled = Off`
  - `wmiEmptyEnabled = Off`
  - `wmiIndicatorEnabled = Off`
- Oil pressure:
  - `oilPressureEnable = Off`
  - `oilPressureProtEnbl = Off`
  - `oilPressurePin = A15`
  - `oilPressureMin = -18.0`
  - `oilPressureMax = 168.0`
  - `oilPressureProtMins = 0.0 30.0 40.0 40.0`
  - `oilPressureProtRPM = 500.0 1500.0 3500.0 6000.0`
- Air-con:
  - `airConEnable = Off`
  - `airConCompPol = Inverted`
  - `airConReqPol = Inverted`
  - `airConTurnsFanOn = Yes`
  - `airConCompPin = Unused`
  - `airConReqPin = Unused`
  - `airConFanPin = Unused`
  - `airConFanPol = Normal`
  - `airConFanEnabled = Disabled`
  - `airConTPSCut = 65.0`
  - `airConMinRPM = 500.0`
  - `airConMaxRPM = 4700.0`
  - `airConClTempCut = 120.0`
  - `airConIdleSteps = 0.0`
  - `airConTPSCutTime = 5.0`
  - `airConCompOnDelay = 0.4`
  - `airConAfterStartDelay = 5.0`
  - `airConRPMCutTime = 5.0`
  - `airConIdleUpRPMAdder = 100.0`
  - `airConPwmFanMinDuty = 80.0`

## Verification

- `python -m unittest tools.tests.test_stock_base_tune_compat`
  - passed, `6/6`
- `python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq"`
  - passed
- `python tools/check_stock_base_tune_compat.py --msq "release/speeduino-dropbear-v2.0.1-base-tune.msq" --ini "release/speeduino-dropbear-v2.0.1.ini"`
  - passed
- `python tools/check_stock_base_tune_compat.py`
  - still fails on the unchanged stock tune, now for both:
    - missing `knock_limiterDisable`
    - drift from the fork default-value contract, now including air-con values

## Why This Slice Matters

- Surface compatibility alone was not enough; the fork-owned and release-packaged tunes could still have silently retained stock defaults in fork-touched areas
- The repo now enforces both:
  - field presence / round-trippable surface compatibility
  - 67 selected semantic defaults for the fields this fork has materially changed

## Current State

- Stock tune:
  - remains the unchanged control artifact
  - intentionally fails the audit
- Fork-owned tune:
  - passes the broadened surface audit
  - passes the selected default-value contract
- Release tune:
  - passes the broadened surface audit
  - passes the selected default-value contract

## Safe Next Slice

1. Decide whether to keep the stock `speeduino 202501` signature or formalize a fork signature
2. If retaining the stock signature, continue adding value-level checks for other fork-touched defaults with clear evidence
3. If formalizing divergence, bump the signature and update the audit/test expectations to separate stock compatibility from fork compatibility

## Recommended Prompt For Next Session

`Continue from SESSION_HANDOFF_2026-03-21_TUNE_DEFAULT_VALUES.md. The compatibility audit now enforces both the round-trippable tune surface and a 67-check fork-default contract across knock, rolling cut, DFCO, launch, VSS, WMI, oil pressure, and air-con. The fork-owned and release-packaged tunes pass; the unchanged stock tune remains the intentional failing control. Next slice: either keep growing the default contract into adjacent config clusters like fan/idle-up, or formalize fork divergence with a signature bump.` 
