# Session Handoff: Rover MEMS 5-3-2 Cam Phase

Date: 2026-03-22
Focus: Encode the Rover `5-3-2` cam ISR semantics and check whether the archived composite logs are precise enough for safe full-sync replay

## What Changed

- Added [tools/analyze_rover_mems_cam_532.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_rover_mems_cam_532.py)
- Added [tools/tests/test_analyze_rover_mems_cam_532.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/tests/test_analyze_rover_mems_cam_532.py)

The tool has two jobs:

1. encode the exact `triggerSec_RoverMEMS()` gap-handling rules for `SEC_TRIGGER_5_3_2`
2. summarize secondary transitions in the recovered Rover composite CSVs without pretending they already give exact tooth identity

## Exact `5-3-2` Cam Rules From `triggerSec_RoverMEMS()`

The Rover cam ISR only does phase work when it sees a secondary gap larger than the running `1.5x` threshold. At that point it interprets the tooth after the gap using `secondaryToothCount`:

- `secondaryToothCount == 6`
  - gap after `5` teeth
  - cycle `360-720`
  - tooth window `18-36`
  - `revolutionOne = false`
  - if `toothCurrentCount < 19`, add `18`
- `secondaryToothCount == 4`
  - gap after `3` teeth
  - cycle `0-360`
  - tooth window `1-18`
  - `revolutionOne = true`
  - if `toothCurrentCount > 17`, subtract `18`
- `secondaryToothCount == 3`
  - gap after `2` teeth
  - cycle `0-360`
  - tooth window `18-36`
  - `revolutionOne = true`
  - if `toothCurrentCount < 19`, add `18`

After any such gap, the ISR resets `secondaryToothCount = 1`, because the current tooth becomes the first tooth after the gap.

Safe conclusion:

- the decoder's `5-3-2` logic is not ambiguous in code anymore
- the remaining uncertainty is not the ISR semantics
- the remaining uncertainty is which logged secondary transitions correspond to those gap teeth in the archived composite export

## What The Archived Rover CSVs Show

The new tool was run against:

- [2021-06-23_01.12.43-cranking_risingoncam_crank.csv](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting/DataLogs/2021-06-23_01.12.43-cranking_risingoncam_crank.csv)
- [2021-06-23_01.59.58_rising_ne.csv](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting/DataLogs/2021-06-23_01.59.58_rising_ne.csv)

Post-sync secondary-transition primary-toggle bins:

### `2021-06-23_01.12.43-cranking_risingoncam_crank.csv`

- `{2: 1, 4: 3, 5: 4, 12: 1, 22: 1}`

### `2021-06-23_01.59.58_rising_ne.csv`

- `{2: 1, 4: 4, 5: 5, 10: 1, 11: 1, 23: 1}`

Practical meaning:

- once sync is up, the dominant observed secondary spacing bins are `4`, `5`, and `2` primary toggles between secondary state transitions
- that is qualitatively compatible with a `5-3-2` family signal being exported as level transitions rather than already-decoded tooth groups
- but the exported composite rows still do not safely tell us which specific transition is the tooth-after-gap event that the ISR keys off

An additional local pass also confirmed:

- [CurrentTune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting/CurrentTune.msq) and every saved restore-point `.msq` in [restorePoints](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting/restorePoints) use:
  - `TrigEdge = RISING`
  - `TrigEdgeSec = RISING`
  - `TrigPattern = Rover MEMS`
  - `trigPatternSec = 5-3-2 cam`
  - `TrigSpeed = Crank Speed`
  - `useResync = No`
- splitting the post-sync composite transitions by `secLevel` still does not resolve the ambiguity:
  - `2021-06-23_01.59.58_rising_ne.csv`
    - rising transitions: `{4: 4, 5: 1, 11: 1}`
    - falling transitions: `{2: 1, 5: 4, 10: 1, 23: 1}`
  - `2021-06-23_01.12.43-cranking_risingoncam_crank.csv`
    - rising transitions: `{4: 3, 5: 1, 12: 1}`
    - falling transitions: `{2: 1, 5: 3, 22: 1}`

Safe conclusion:

- even knowing the tune used `TrigEdgeSec = RISING`, the composite export still does not map cleanly onto the decoder's internal `5-3-2` tooth-group state without another unstated assumption about how `secLevel` and `priLevel` are sampled

## Additional External Forum Evidence

The original Speeduino Rover thread adds two concrete details that were not recoverable from the extracted local archive alone:

- In [post `#51343`](https://speeduino.com/forum/viewtopic.php?t=1427&start=60#p51343), Trevor Getty explicitly reviewed the scope trace and wrote that the blue cam signal cycles as:
  - `3 gap 2 gap gap 5 gap gap`
  - and that the section he was concerned about was the middle of the `5` tooth sequence
- In [post `#51394`](https://speeduino.com/forum/viewtopic.php?t=1427&start=70#p51394), Trevor then reported that the cam input polarity had in fact been wrong:
  - the cam sensor inputs were inverted
  - after swapping the cam leads into the correct `VR+ / VR-` orientation, the tooth logger started working
  - with that correction, the setup reached sync and injector dwell started appearing

The earlier context around that same phase problem is also useful:

- In [post `#51340`](https://speeduino.com/forum/viewtopic.php?t=1427&start=60#p51340), Trevor says the composite logger shows crank and cam pulses, but he cannot see the expected `5/3/2` cam signals and suspects the decoder is not seeing cam sync correctly.
- In [post `#51348`](https://speeduino.com/forum/viewtopic.php?t=1427&start=70#p51348), `theonewithin` reminds him that some conditioners invert the signal.
- In [post `#51354`](https://speeduino.com/forum/viewtopic.php?t=1427&start=70#p51354), `miker` says the data file does not show the same cam behavior seen in the images and calls out the MAX VR conditioner as a likely trigger-offset variable.

Safe conclusion from the external evidence:

- the live 2021 Rover test work did hit a real cam-polarity problem
- once the cam VR polarity was corrected, the Rover setup progressed from high sync loss / no injector activity to working tooth logging and sync
- the forum does support treating cam polarity as a first-order issue rather than a side note

An additional attachment pass against the June 25 follow-up files under [forum_downloads](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/forum_downloads) tightened the same conclusion rather than changing it:

- [crank_cam_spark1_at_360_degree_trigger.png](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/forum_downloads/crank_cam_spark1_at_360_degree_trigger.png) shows crank, cam, and ignition together, but without a tooth-number overlay or any marker that identifies which cam transition the decoder treated as the gap tooth
- [trigger_0_degree_full_rotation_wrong_at_crank.png](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/forum_downloads/trigger_0_degree_full_rotation_wrong_at_crank.png) and [trigger_360_nearly_perfectTDC.png](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/forum_downloads/trigger_360_nearly_perfectTDC.png) refine absolute ignition timing against the crank waveform, but they do not show a labeled cam channel, so they cannot disambiguate `secondaryToothCount == 6 / 4 / 3`
- [logging_fine.png](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/forum_downloads/logging_fine.png) confirms the setup was running with sync losses and dwell activity visible in TunerStudio after the polarity fix, but it is a standard runtime graph rather than a composite/tooth view
- [2021-06-25_03.16.31 - Copy.mlg.txt](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/forum_downloads/2021-06-25_03.16.31%20-%20Copy.mlg.txt) is also only a regular `MLVLG` datalog with channels such as `SecL`, `Advance`, `Dwell`, and `Sync Loss #`, not a trigger/composite stream

The later thread pages add one more configuration-level result, but still not the missing phase marker:

- In [post `#51428`](https://speeduino.com/forum/viewtopic.php?t=1427&start=80#p51428), Trevor reports the car as running on the tested T16 pattern and says the attached running log was taken using the `360` trigger with `5` cranking and `10` fixed running timing.
- In [post `#51543`](https://speeduino.com/forum/viewtopic.php?t=1427&start=90#p51543), he says that when fixed running ignition was set to `0` and `trigger angle` to `12 degrees`, the timing gun aligned perfectly at TDC and that this cam pattern was tested to completion.
- In [post `#51554`](https://speeduino.com/forum/viewtopic.php?t=1427&start=90#p51554), he clarifies that the setup was running with `trigger angle = 12 degrees` and `0` cranking / cam timing to fire at `0 TDC`, and that the earlier `360/369` discussion was a temporary firmware bug rather than the final configured trigger angle.

Safe conclusion from those follow-up posts:

- the tested T16 Rover path was considered working in-thread
- the practical timing calibration outcome for that path was `trigger angle = 12 degrees`
- this helps validate runtime ignition alignment for the already-working path
- it still does not identify which cam transition the decoder used as the `5`, `3`, or `2` gap tooth event

What it still does not safely provide:

- an exact mapping from a logged `secLevel` transition in the TunerStudio composite export to the decoder's internal "tooth after 5 / 3 / 2 gap" event
- a tooth-numbered crank/cam alignment precise enough to land a full-sync replay trace without another assumption

## Why Full-Sync Replay Is Still Not Safe

The current blocker is now precise:

- the primary-wheel model is solved and replay-backed
- the `5-3-2` cam ISR semantics are solved and encoded in a tool
- the archived CSVs do show structured post-sync secondary spacing

But the composite export still leaves at least one unsafe ambiguity:

- `secLevel` records state changes, not the decoder's internal `secondaryToothCount`
- `trigger` changes alongside both primary and secondary activity, so it is not yet a clean cam-gap marker
- the logs do not directly identify which secondary transition is the tooth after the `5`, `3`, or `2` gap

An additional local-evidence check was also completed on the extracted Rover project:

- [TuneView_000.tuneView](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting/TuneView/TuneView_000.tuneView) contains no `composite`, `priLevel`, `secLevel`, or trigger-channel bindings
- the extracted dashboard files under [dashboard](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting/dashboard) expose generic `sync` / `syncLossCounter` gauges, but no composite logger widget or direct crank/cam edge visualization

Safe conclusion:

- there is no surviving local project artifact in the extracted Rover archive that disambiguates the composite logger's secondary edge polarity or identifies the tooth-after-gap transition directly

Therefore it is still unsafe to add a full Rover `Crank Speed + 5-3-2 cam` replay trace that claims exact phase correctness.

## Safe Next Step

The next safe Rover step is one of:

1. derive the secondary ISR edge polarity and gap-to-transition mapping from stronger evidence than the current composite export
2. find a surviving capture or scope view that shows which `secLevel` transition corresponds to the actual decoder interrupt edge
3. only then align one of the now-proven primary layouts with the `5-3-2` gap cases above and add a full-sync replay trace

## Verification

- `python -m unittest tools.tests.test_analyze_rover_mems_cam_532 tools.tests.test_derive_rover_mems_windows tools.tests.test_render_pdf_pages tools.tests.test_inspect_pdf_evidence tools.tests.test_index_pdf_images tools.tests.test_extract_pdf_images tools.tests.test_parse_speeduino_composite_csv`
