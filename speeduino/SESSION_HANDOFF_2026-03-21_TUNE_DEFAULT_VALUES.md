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
  - idle-advance defaults
  - idle-up defaults
  - VSS defaults
  - WMI defaults
  - oil-pressure defaults
  - fan defaults
  - air-con defaults
- Added normalized `.msq` constant-value extraction so scalar and array values can be compared in a stable way
- Updated [test_stock_base_tune_compat.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/tests/test_stock_base_tune_compat.py) so the fork-owned tune must satisfy every checked critical value
- Added a release-path unit test so the shipped [speeduino-dropbear-v2.0.1-base-tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/release/speeduino-dropbear-v2.0.1-base-tune.msq) must satisfy the same value contract against [speeduino-dropbear-v2.0.1.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/release/speeduino-dropbear-v2.0.1.ini)
- The fork-owned and release tunes already matched the new idle-advance / idle-up / VSS / WMI / oil-pressure / fan defaults, so no `.msq` value changes were needed for these audit-only slices
- Added explicit `defaultValue` parsing to [check_stock_base_tune_compat.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/check_stock_base_tune_compat.py) so tune-vs-INI-default mismatches can be reported separately from the fork-owned contract
- Improved that report by decoding bitfield `defaultValue` entries to their display values, which removes false mismatches like `boostPin=0` vs `Board Default`
- Added a second report that compares the enforced fork contract itself against explicit INI `defaultValue` entries, after numeric and bitfield normalization
- Improved explicit-default parsing again to preserve multiple unit-specific `defaultValue` variants per field, so comparisons now accept any normalized variant instead of only the last one seen
- Added a stock-origin classification report that compares the enforced fork contract against both the stock base tune and the INI defaults
- Added filtering and count summaries to the stock-origin report so fork-specific conflicts can be isolated directly
- Added an expected-classification verifier so the current `5 inherited / 1 fork-specific` policy surface is machine-checked for both the fork-owned and release tunes
- Added a manual-backed contextual exemption for `vssPulsesPerKm`, since the manual says VSS should be `Off` when unused and runtime code treats `0` as disabled/no dividing in that mode
- Added a policy-evidence report that prints each active conflict or exemption together with the specific manual / INI-help / runtime-code rationale backing it
- Evaluated idle-advance / boost / VVT against explicit `defaultValue` entries in [speeduino.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino.ini) and confirmed a real ambiguity:
  - `idleAdvStartDelay`: tune `0.7` vs INI `0.2`
  - `idleTaperTime`: tune `5.0` vs INI `1.0`
  - `boostCutEnabled`: tune `Off` vs INI raw default `1`
  - `boostMinDuty`: tune `12.0` vs INI `0`
  - `boostMaxDuty`: tune `80.0` vs INI `100`
  - `vvtCL0DutyAng`: tune `240.0` vs INI `0`
  - `vvtMinClt`: tune `-40.0` vs INI `70 / 160`
  - `vvtDelay`: tune `0.0` vs INI `60`
- Because of that ambiguity, boost/VVT defaults were still not added to the enforced fork contract in this slice
- The contract-vs-default report narrows the enforced-contract conflicts to five active fields:
  - `airConCompPol`: fork contract `Inverted` vs INI `Normal`
  - `airConReqPol`: fork contract `Inverted` vs INI `Normal`
  - `idleAdvStartDelay`: fork contract `0.7` vs INI `0.2`
  - `idleTaperTime`: fork contract `5.0` vs INI `1.0`
  - `knock_pin`: fork contract `A8` vs INI `A10`
- Contextual exemption:
  - `vssPulsesPerKm`: contract `0.0` vs INI `3000`, but exempt because:
    - the manual says set VSS mode to `Off` if VSS is not used
    - runtime code uses `0` as no dividing / disabled in aux-input VSS mode
- Stock-origin classification of the five active conflicts:
  - inherited from the unchanged stock base tune:
    - `airConCompPol`
    - `airConReqPol`
    - `idleAdvStartDelay`
    - `idleTaperTime`
  - fork-specific divergence from both stock and INI:
    - `knock_pin` (`stock = 3`, `fork contract = A8`, `INI default = A10`)
  - current origin counts:
    - `inherited_from_stock_tune = 4`
    - `fork_and_stock_both_differ_from_ini_default = 1`

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
- Idle advance:
  - `idleAdvEnabled = Off`
  - `idleAdvAlgorithm = TPS`
  - `idleAdvDelay = 2.0`
  - `idleAdvRPM = 2000.0`
  - `idleAdvTPS = 4.0`
  - `idleTaperTime = 5.0`
  - `idleAdvStartDelay = 0.7`
  - `idleAdvVss = 255.0`
  - `idleAdvBins = -200 -100 -50 50 100 200`
  - `idleAdvValues = 0 0 0 0 0 0`
- Idle-up:
  - `idleUpPin = Board Default`
  - `idleUpPolarity = Normal`
  - `idleUpEnabled = Off`
  - `idleUpAdder = 15.0`
  - `idleUpOutputEnabled = Off`
  - `idleUpOutputInv = No`
  - `idleUpOutputPin = Board Default`
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
- Fan:
  - `fanWhenOff = Yes`
  - `fanWhenCranking = No`
  - `fanInv = No`
  - `fanEnable = On/Off`
  - `fanPin = Board Default`
  - `fanSP = 90.0`
  - `fanHyster = 5.0`
  - `fanFreq = 0.0`
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
  - passed, `13/13`
- `python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq"`
  - passed
- `python tools/check_stock_base_tune_compat.py --msq "release/speeduino-dropbear-v2.0.1-base-tune.msq" --ini "release/speeduino-dropbear-v2.0.1.ini"`
  - passed
- `python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --report-explicit-default-mismatches idleAdvStartDelay idleTaperTime boostCutEnabled boostMinDuty boostMaxDuty vvtCL0DutyAng vvtMinClt vvtDelay`
  - reported the same eight explicit-default mismatches listed above
- `python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --report-contract-default-conflicts`
  - reported the six contract-vs-INI-default conflicts listed above
- `python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --stock-msq "Resources/Speeduino base tune.msq" --report-contract-conflict-origins`
  - classified four active conflicts as inherited from stock and one (`knock_pin`) as fork-specific divergence
- `python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --stock-msq "Resources/Speeduino base tune.msq" --report-contract-conflict-origins --contract-origin-filter fork_and_stock_both_differ_from_ini_default`
  - isolates the one remaining fork-specific policy conflict: `knock_pin`
- `python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --stock-msq "Resources/Speeduino base tune.msq" --verify-expected-contract-conflicts`
  - passed
  - current expected classified baseline: `inherited_from_stock_tune = 4`, `fork_and_stock_both_differ_from_ini_default = 1`
- `python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --report-contextual-contract-exemptions`
  - reported the one documented contextual exemption: `vssPulsesPerKm`
- `python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq" --stock-msq "Resources/Speeduino base tune.msq" --report-policy-evidence`
  - reported all five active policy items plus the one contextual exemption with embedded evidence notes
- `python tools/check_stock_base_tune_compat.py`
  - still fails on the unchanged stock tune, now for both:
    - missing `knock_limiterDisable`
    - drift from the fork default-value contract, now including air-con values

## Why This Slice Matters

- Surface compatibility alone was not enough; the fork-owned and release-packaged tunes could still have silently retained stock defaults in fork-touched areas
- The repo now enforces both:
  - field presence / round-trippable surface compatibility
  - 92 selected semantic defaults for the fields this fork has materially changed
- The repo can now also report raw INI `defaultValue` mismatches separately, which makes it possible to investigate default-source ambiguity without pretending those fields are already settled
- The repo can now separately report where the enforced fork contract itself diverges from explicit INI defaults, which is the real remaining policy surface
- Unit-specific duplicate INI defaults are now preserved instead of overwritten, which removed `dfcoMinCLT` from the false-conflict set
- The remaining policy surface is now much smaller in practice:
  - 4 inherited stock-tune conflicts that argue for either accepting stock semantics or bumping the signature
  - 1 fork-specific conflict (`knock_pin`) that can be evaluated independently
- 1 context-dependent field (`vssPulsesPerKm`) is now explicitly exempted with manual + runtime evidence
- That policy surface is now encoded in the repo and test-covered, not just documented in prose
- The evidence for each policy item is now queryable directly from the audit tool, so future decisions do not depend on reconstructing rationale from handoff notes

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

`Continue from SESSION_HANDOFF_2026-03-21_TUNE_DEFAULT_VALUES.md. The compatibility audit now enforces both the round-trippable tune surface and a 92-check fork-default contract across knock, rolling cut, DFCO, launch, idle advance, idle-up, VSS, WMI, oil pressure, fan, and air-con. The tool parses 230 explicit INI defaultValue entries, preserves unit-specific default variants, machine-checks the classified policy surface, treats `vssPulsesPerKm` as a documented contextual exemption, and exposes a policy-evidence report for each active item. Current decision surface: eight tune-vs-default mismatches, five active contract-vs-default conflicts, four inherited-stock conflicts, one fork-specific conflict (`knock_pin`), and one contextual exemption (`vssPulsesPerKm`). The fork-owned and release-packaged tunes pass the enforced contract; the unchanged stock tune remains the intentional failing control. Next slice: decide whether to keep the four inherited stock semantics under the stock signature, resolve them toward INI defaults, or bump the signature; `knock_pin` can be decided separately.` 
