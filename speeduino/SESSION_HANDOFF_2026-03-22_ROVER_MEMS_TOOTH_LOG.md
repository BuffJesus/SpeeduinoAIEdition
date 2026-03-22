# Session Handoff: Rover MEMS Post-Fix Tooth Logger

Date: 2026-03-22
Focus: Convert the June 25, 2021 post-VR-fix Rover tooth logger into explicit primary-wheel evidence and replay coverage

## What Changed

- Added [tools/analyze_rover_mems_tooth_log.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_rover_mems_tooth_log.py)
- Added [tools/tests/test_analyze_rover_mems_tooth_log.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/tests/test_analyze_rover_mems_tooth_log.py)

## New External Evidence Pulled Into The Workspace

Direct forum attachment downloads from the original Rover thread were added under [forum_downloads](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/forum_downloads):

- [2021-06-25_00.54.13_tooth_logger_crank_new_battery.csv](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/forum_downloads/2021-06-25_00.54.13_tooth_logger_crank_new_battery.csv)
- [2021-06-25_03.16.31 - Copy.mlg.txt](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/forum_downloads/2021-06-25_03.16.31%20-%20Copy.mlg.txt)
- the associated June 25 scope screenshots, including [crank_cam_zoomed_fixed_vr.png](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/forum_downloads/crank_cam_zoomed_fixed_vr.png)

Useful direct forum sources:

- [post `#51394`](https://speeduino.com/forum/viewtopic.php?t=1427&start=70#p51394)
- [attachment `8557`](https://speeduino.com/forum/download/file.php?id=8557)
- [attachment `8553`](https://speeduino.com/forum/download/file.php?id=8553)

## Safe Primary-Wheel Result

The new tooth-log helper mirrors the primary Rover ISR rule:

- compare each tooth time to `1.5x` the previous tooth time
- append `01` for a gap tooth
- append `1` for a normal tooth
- compare the rolling `32`-bit window to the hard-coded Rover constants

Applied to the post-fix tooth logger:

- the first match lands at tooth index `42`
- every match is Rover pattern `#1`
- no matches land on patterns `#2` to `#5`

That means the June 25 post-fix tooth logger is now safe evidence that the running setup reached the decoder's `17-17` primary pattern path.

## What Did Not Land Safely

A narrow primary-only replay attempt was tried against `triggerPri_RoverMEMS()` using the June 25 tooth-times directly, but it was intentionally not kept.

Reason:

- the host-side rolling-window helper consistently identifies Rover pattern `#1`
- but the unit-harness ISR replay did not safely reproduce that match without another assumption about startup/filter history

Safe conclusion:

- the tooth logger is now proven evidence for the primary bit-window model
- it is not yet safe to claim a checked-in firmware replay trace from that capture

## What The Scope Images Add

[crank_cam_zoomed_fixed_vr.png](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/forum_downloads/crank_cam_zoomed_fixed_vr.png) still helps qualitatively:

- the original image title includes `signal_cam_3gap_2gapgap5(zoomed)`
- that supports Trevor's forum description that the corrected cam signal cycles as `3 gap 2 gap gap 5 gap gap`

Safe conclusion:

- the polarity-fixed June 25 evidence is internally consistent on the primary side
- the cam grouping description is stronger now than it was from text alone
- but the screenshots still do not provide a tooth-numbered mapping from a specific logged cam edge to `secondaryToothCount == 6 / 4 / 3`

## Remaining Blocker

The remaining blocker is unchanged but narrower:

- primary-wheel evidence is now replay-backed for documented patterns `#2` and `#3`
- a real post-fix tooth logger now host-side-proves pattern `#1`
- the cam `5-3-2` ISR semantics are already encoded

But the repo still does not have safe evidence for which logged cam edge corresponds to the decoder's tooth-after-gap phase event.

So full Rover `Crank Speed + 5-3-2 cam` replay is still blocked on cam-edge-to-gap mapping, not on primary-wheel identification.
