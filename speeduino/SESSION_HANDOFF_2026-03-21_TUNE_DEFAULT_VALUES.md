# Session Handoff: Tune Default Value Contract

## Workspace

- Repo root: [speeduino-202501.6](C:/Users/Cornelio/Desktop/speeduino-202501.6)
- Focus area: Phase 5 configuration compatibility - extend the tune audit from surface compatibility to selected fork-critical default values

## What Changed This Session

- Extended [check_stock_base_tune_compat.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/check_stock_base_tune_compat.py) with explicit value checks for selected fork-critical defaults:
  - knock defaults
  - rolling-cut defaults
  - DFCO defaults
  - launch VSS limit
- Added normalized `.msq` constant-value extraction so scalar and array values can be compared in a stable way
- Updated [test_stock_base_tune_compat.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/tests/test_stock_base_tune_compat.py) so the fork-owned tune must satisfy every checked critical value
- Updated:
  - [Speeduino AI base tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino%20AI%20base%20tune.msq)
  - [speeduino-dropbear-v2.0.1-base-tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/release/speeduino-dropbear-v2.0.1-base-tune.msq)
  to match the current fork default contract for those selected fields

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
  - `lnchCtrlVss = 255`

## Verification

- `python -m unittest tools.tests.test_stock_base_tune_compat`
  - passed, `5/5`
- `python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq"`
  - passed
- `python tools/check_stock_base_tune_compat.py --msq "release/speeduino-dropbear-v2.0.1-base-tune.msq" --ini "release/speeduino-dropbear-v2.0.1.ini"`
  - passed
- `python tools/check_stock_base_tune_compat.py`
  - still fails on the unchanged stock tune, now for both:
    - missing `knock_limiterDisable`
    - drift from the fork default-value contract

## Why This Slice Matters

- Surface compatibility alone was not enough; the fork-owned and release-packaged tunes could still have silently retained stock defaults in fork-touched areas
- The repo now enforces both:
  - field presence / round-trippable surface compatibility
  - selected semantic defaults for the fields this fork has materially changed

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

`Continue from SESSION_HANDOFF_2026-03-21_TUNE_DEFAULT_VALUES.md. The compatibility audit now enforces both the round-trippable tune surface and selected fork-critical default values. The fork-owned and release-packaged tunes pass; the unchanged stock tune remains the intentional failing control. Next slice: decide whether to retain the stock signature and grow the default-value contract, or bump the signature and formalize divergence.` 
