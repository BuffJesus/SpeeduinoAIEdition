# Session Handoff: Stock Base Tune Compatibility Audit

## Workspace

- Repo root: [speeduino-202501.6](C:/Users/Cornelio/Desktop/speeduino-202501.6)
- Focus area: Phase 5 configuration compatibility - make stock Speeduino base-tune drift explicit and checkable

## What Changed This Session

- Added a host-side compatibility audit script:
  - [check_stock_base_tune_compat.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/check_stock_base_tune_compat.py)
- Added parser / audit unit tests:
  - [test_stock_base_tune_compat.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/tests/test_stock_base_tune_compat.py)
- Updated [README.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/README.md) with the audit command and current finding

## What The Audit Checks

- `versionInfo.signature` in the `.msq` against the current [speeduino.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino.ini) signature
- Declared firmware page count against numbered `<page number="...">` nodes in the `.msq`
- Presence of high-risk constants tied to fork-touched areas:
  - knock
  - launch / VSS
  - DFCO
  - WMI
  - rolling cut
  - oil pressure
  - air-con

## Current Finding

- The stock base tune at [Resources/Speeduino base tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino%20base%20tune.msq) still matches the stock `speeduino 202501` signature and has the expected 15 numbered tune pages
- It does **not** contain `knock_limiterDisable`
- That means the tune is near-compatible, but not fully compatible with the current fork’s exposed knock configuration surface

## Why This Matters

- The fork still advertises the stock `speeduino 202501` signature
- Without an explicit audit, it is easy to keep the stock signature while letting fork-only fields drift out of the stock base tune
- `knock_limiterDisable` is a concrete example: the firmware / INI expose it, but the imported stock `.msq` does not currently carry it

## Verification

- `python -m unittest tools.tests.test_stock_base_tune_compat`
  - passed
- `python tools/check_stock_base_tune_compat.py`
  - exits nonzero and reports the current real compatibility gap:
    - missing `knock_limiterDisable`

## Notes

- The `.msq` contains 16 total `<page>` nodes, but only 15 are numbered firmware tune pages; the extra node is the unnamed local page at the top of the file and should not be treated as a firmware page-count mismatch
- The audit intentionally treats missing high-risk fields as failures, not warnings

## Safe Next Slice

1. Decide the compatibility policy and encode it:
   - keep the stock signature and patch / regenerate a fork-owned base tune that includes `knock_limiterDisable`
   - or bump the signature and ship a clearly fork-specific base tune
2. If keeping the stock signature, expand the audit to compare more of the INI constant surface instead of only the high-risk subset

## Recommended Prompt For Next Session

`Continue from SESSION_HANDOFF_2026-03-21_STOCK_TUNE_AUDIT.md. The repo now has a host-side stock base tune compatibility audit in tools/check_stock_base_tune_compat.py and parser tests in tools/tests/test_stock_base_tune_compat.py. The current real finding is that Resources/Speeduino base tune.msq matches signature/page-count expectations but is missing knock_limiterDisable, so decide whether to patch a fork-owned base tune while keeping the stock signature or to bump the signature and formalize divergence.` 
