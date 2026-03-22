# Session Handoff: Rover MEMS Bit Window

Date: 2026-03-22
Focus: Derive the exact `roverMEMSTeethSeen` rolling window and land a safe primary-only replay slice

## What Changed

- Added [tools/derive_rover_mems_windows.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/derive_rover_mems_windows.py) to derive the Rover MEMS `32`-bit rolling decoder window directly from the documented `36`-slot wheel layouts.
- Added [tools/tests/test_derive_rover_mems_windows.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/tests/test_derive_rover_mems_windows.py) to lock the helper to all five hard-coded Rover pattern constants.
- Added two narrow primary-only Rover replay tests in [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp):
  - `MEMS 3` / `MEMS 2 VVC` layout `2-14-3-13`
  - `MEMS 1.9 MPi` layout `11-5-12-4`

## Exact Bit-Window Model

- each normal tooth appends one `1` bit
- each tooth after a missing slot appends `01`
- the decoder therefore tracks a rolling `32`-bit window of the theoretical `36`-slot wheel
- the hard-coded `roverMEMSTeethSeen` constants in [decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) are `32`-slot slices of the cyclic `36`-slot present/missing pattern, not a separate timing code

For the two manual-backed layouts now tested:

### `2-14-3-13`

- wheel bits: `110111111111111110111011111111111110`
- decoder window: `11011011111111111111011101111111`
- missing slots in the normalized `36`-slot wheel: `3, 18, 22, 36`
- omitted slots from the rolling `32`-bit detection window: `30, 31, 32, 33`
- earliest safe primary-only replay start found by the helper: slot `27`
- first decoder match reached after `34` actual tooth edges and `36` theoretical teeth

### `11-5-12-4`

- wheel bits: `111111111110111110111111111111011110`
- decoder window: `11111101111101111111111110111101`
- missing slots in the normalized `36`-slot wheel: `12, 18, 31, 36`
- omitted slots from the rolling `32`-bit detection window: `2, 3, 4, 5`
- earliest safe primary-only replay start found by the helper: slot `34`
- first decoder match reached after `34` actual tooth edges and `36` theoretical teeth

## What The New Replay Coverage Proves

- the current Rover decoder does recognize both manual-backed primary wheels when driven with a slot-accurate primary stream
- the earlier failed naive attempt was a window-alignment problem, not evidence that the current pattern constants were wrong
- on these primary-only sequential-fuel paths the decoder lands in half-sync exactly as expected:
  - `hasSync == false`
  - `BIT_STATUS3_HALFSYNC == true`
  - `toothCurrentCount == 1`
- the expected skip-tooth maps are populated for both patterns

## Verification

- `python -m unittest tools.tests.test_derive_rover_mems_windows tools.tests.test_render_pdf_pages tools.tests.test_inspect_pdf_evidence tools.tests.test_index_pdf_images tools.tests.test_extract_pdf_images tools.tests.test_parse_speeduino_composite_csv`
- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passing: `196/196`

## Rover Replay Status

Rover replay has moved forward materially:

- the primary-wheel rolling-window blocker is resolved for the documented `2-14-3-13` and `11-5-12-4` layouts
- there is now checked-in replay-backed decoder coverage for those two primary patterns

## Remaining Blocker

The next blocker is no longer primary-wheel identification.

The remaining blocker is exact phase alignment for promotion from half-sync to full sync on the real Rover path:

- the archived project uses `Crank Speed` plus `5-3-2 cam`
- the composite CSV signal names are known
- the manuals identify the primary wheel layouts
- but the repo still does not have a safe tooth-by-tooth alignment from those named crank/cam signal changes to the decoder's `secondaryToothCount` / phase transitions

That makes the next safe Rover slice:

1. derive the `5-3-2 cam` phase expectations tooth-by-tooth against one now-proven primary layout
2. align the recovered CSV edge stream to that phase model
3. then add a full-sync Rover replay trace
