# Session Handoff: Fork-Owned Compatible Base Tune

## Workspace

- Repo root: [speeduino-202501.6](C:/Users/Cornelio/Desktop/speeduino-202501.6)
- Focus area: Phase 5 configuration compatibility - preserve the stock base tune as a drift detector and add a fork-owned compatible tune

## What Changed This Session

- Added a fork-owned base tune:
  - [Speeduino AI base tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino%20AI%20base%20tune.msq)
- The fork tune is derived from the stock base tune but adds the missing:
  - `knock_limiterDisable`
- Extended the compatibility tests in:
  - [test_stock_base_tune_compat.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/tests/test_stock_base_tune_compat.py)
- Updated [README.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/README.md) with both audit commands and the new base-tune status

## Compatibility State After This Slice

- [Speeduino base tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino%20base%20tune.msq)
  - still useful as the stock drift detector
  - still fails the compatibility audit because it is missing `knock_limiterDisable`
- [Speeduino AI base tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino%20AI%20base%20tune.msq)
  - passes the compatibility audit against the current [speeduino.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino.ini)
  - preserves the stock `speeduino 202501` signature while matching the current fork tuning surface

## Verification

- `python -m unittest tools.tests.test_stock_base_tune_compat`
  - passed
- `python tools/check_stock_base_tune_compat.py`
  - fails as expected on the stock tune with:
    - missing `knock_limiterDisable`
- `python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq"`
  - passed

## Why This Slice Matters

- This keeps the upstream stock tune intact for evidence of future drift
- It also gives the fork a concrete compatible tune artifact instead of relying on undocumented manual edits after import
- The repo now has both:
  - a regression detector for stock compatibility
  - a known-good tune file for the current fork surface

## Safe Next Slice

1. Decide whether to keep the stock `speeduino 202501` signature long term or formalize divergence with a fork signature
2. If keeping the stock signature, expand the audit from the high-risk field subset toward broader INI-vs-MSQ constant coverage
3. If shipping releases from this fork, include the fork-owned base tune alongside the release INI/firmware bundle

## Recommended Prompt For Next Session

`Continue from SESSION_HANDOFF_2026-03-21_FORK_BASE_TUNE.md. The repo now has a stock compatibility audit, the stock base tune still fails on missing knock_limiterDisable, and Resources/Speeduino AI base tune.msq passes the audit. Next slice: decide whether to keep the stock signature or bump it, then either broaden the compatibility audit or wire the fork tune into release packaging.` 
