# Session Handoff: Release Bundle Base Tune

## Workspace

- Repo root: [speeduino-202501.6](C:/Users/Cornelio/Desktop/speeduino-202501.6)
- Focus area: Phase 5 configuration compatibility - make the fork-owned compatible tune part of the shipped release bundle

## What Changed This Session

- Added the fork-owned compatible tune to the release bundle:
  - [speeduino-dropbear-v2.0.1-base-tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/release/speeduino-dropbear-v2.0.1-base-tune.msq)
- Updated [README.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/README.md) so the release firmware, INI, and base tune are listed together
- Added a release-bundle audit command example using:
  - [check_stock_base_tune_compat.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/check_stock_base_tune_compat.py)
- Strengthened the audit so it now checks the full round-trippable INI constant surface, not just the original high-risk subset
- Added explicit handling for known `.msq` export shape quirks:
  - `firstDataIn0..7` / `secondDataIn0..7` are satisfied by the parent `firstDataIn` / `secondDataIn` array exports
  - `UNALLOCATED_TOP_11` and `knock_unused1` remain accepted extra `.msq` constants

## Verification

- `python tools/check_stock_base_tune_compat.py --msq "release/speeduino-dropbear-v2.0.1-base-tune.msq" --ini "release/speeduino-dropbear-v2.0.1.ini"`
  - passed
- `python tools/check_stock_base_tune_compat.py --msq "Resources/Speeduino AI base tune.msq"`
  - passed after the broadened round-trippable-surface audit

## Current Tune Policy State

- Stock upstream-style tune:
  - [Speeduino base tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino%20base%20tune.msq)
  - intentionally kept unchanged as a drift detector
  - still fails audit because it is missing `knock_limiterDisable`
- Fork-owned compatible tune:
  - [Speeduino AI base tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/Speeduino%20AI%20base%20tune.msq)
  - passes current audit against the root [speeduino.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino.ini)
- Release-packaged compatible tune:
  - [speeduino-dropbear-v2.0.1-base-tune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/release/speeduino-dropbear-v2.0.1-base-tune.msq)
  - passes current audit against the packaged release INI

## Notes

- Generated Python `__pycache__` files under `tools/` were not cleaned because the environment blocked deletion commands and `apply_patch` cannot delete binary `.pyc` files
- That cleanup issue is non-functional and separate from the release artifact work

## Safe Next Slice

1. Decide whether to keep the stock `speeduino 202501` signature or bump it for the fork
2. Expand the compatibility audit beyond the high-risk subset if the stock signature is retained
3. If release automation is added later, generate / copy the compatible base tune as part of the packaging step instead of maintaining it manually

## Recommended Prompt For Next Session

`Continue from SESSION_HANDOFF_2026-03-21_RELEASE_BASE_TUNE.md. The release bundle now includes a compatible base tune and it passes the audit against the packaged release INI. Remaining policy question: keep the stock signature with a stronger audit, or bump the signature and formalize fork divergence.` 
