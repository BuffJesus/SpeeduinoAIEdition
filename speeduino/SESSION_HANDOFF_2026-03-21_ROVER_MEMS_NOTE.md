# Session Handoff: Rover MEMS Decoder Note

Date: 2026-03-21
Focus: Evidence-backed Rover MEMS decoder note before any new replay traces

## Why This Exists

Previous Rover replay attempts were backed out because the synthetic `17-17` traces did not match live decoder state under `simavr`.

This note is the safe intermediate step the roadmap now calls for:

1. gather the surviving Speeduino forum evidence
2. align it with the current Speeduino decoder logic
3. compare it with MS3 and rusEFI external references
4. write down only the parts that are supported strongly enough to guide the next implementation step

## Current Speeduino Decoder Contract

The Rover MEMS decoder is implemented in [decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) and explicitly supports these primary-wheel gap layouts:

- `3-14-2-13`
- `11-5-12-4`
- `2-14-3-13`
- `17-17`
- `9-7-10-6`

The relevant code is in:

- [triggerSetup_RoverMEMS()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp#L5342)
- [triggerPri_RoverMEMS()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp#L5366)
- [triggerRoverMEMSCommon()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp#L5498)
- [triggerSec_RoverMEMS()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp#L5558)

Important current behavior:

- The primary wheel is treated as a theoretical `36` tooth wheel at `10` crank degrees per tooth.
- The decoder identifies which Rover pattern is present by shifting a bit-pattern in `roverMEMSTeethSeen` and matching one of the five hard-coded binary layouts.
- Pattern `17-17` is special because it is not unique without phase information.
- Without cam sync, the code can only establish half-sync for that layout until enough information is available.
- Secondary trigger support covers:
  - no cam
  - single-tooth or half-moon cam
  - `5-3-2` multi-tooth cam

## High-Confidence Speeduino Forum Evidence

The strongest thread is [rover MEMs decoder](https://speeduino.com/forum/viewtopic.php?t=1427).

High-value posts already captured in [speeduino_forum_evidence.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/speeduino_forum_evidence.md):

- [p21484](https://speeduino.com/forum/viewtopic.php?t=1427#p21484) by `dazq`
  - maintainer explanation
  - says the MEMS pattern from manual is `32 poles spaced at 10 degree intervals, with 4 missing poles, at 30, 60, 210 and 250 degrees`
  - includes attached PDFs:
    - `mems 2 crank nn cam.pdf`
    - `mems1.9 trigger description.pdf`
    - `mems3 crank and cam.pdf`
- [p51307](https://speeduino.com/forum/viewtopic.php?t=1427#p51307) by `Trevor Getty`
  - says new VR setup shows both crank and cam in composite logger
  - says wasted spark works but injection is blocked by high sync loss
  - includes attached archive:
    - `T16-RoverMemsTesting.rar`
- [p51340](https://speeduino.com/forum/viewtopic.php?t=1427#p51340)
  - confirms the thread contains real composite/scope evidence and sync-loss behavior worth using for negative cases
- [p51343](https://speeduino.com/forum/viewtopic.php?t=1427#p51343)
  - explicitly raises VR polarity concerns, especially cam inversion versus crank

## Extracted Attachment Evidence

The forum attachments were downloaded into [Resources/rover_mems_evidence](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence) and the Rover test archive was extracted under [Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting).

### What Was Recovered

- PDFs:
  - [mems_2_crank_n_cam.pdf](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/mems_2_crank_n_cam.pdf)
  - [mems19_trigger_description.pdf](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/mems19_trigger_description.pdf)
  - [mems3_crank_and_cam.pdf](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/mems3_crank_and_cam.pdf)
- Test archive:
  - [T16-RoverMemsTesting.rar](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/T16-RoverMemsTesting.rar)

The PDFs are no longer opaque blobs. Embedded images were extracted into [pdf_images](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/pdf_images) with manifest [manifest.json](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/pdf_images/manifest.json) using [extract_pdf_images.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/extract_pdf_images.py).

Recovered image counts:

- `mems19_trigger_description.pdf`: `22`
- `mems3_crank_and_cam.pdf`: `21`
- `mems_2_crank_n_cam.pdf`: `22`
- total extracted Rover PDF images: `65`

Those images are now indexed with OCR and keyword ranking in [ocr_index.json](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/pdf_images/ocr_index.json) using [index_pdf_images.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/index_pdf_images.py).

Current highest-ranked candidate images for the Rover crank/cam diagrams are:

- [mems_2_crank_n_cam_p02_img10.jpg](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/pdf_images/mems_2_crank_n_cam_p02_img10.jpg)
  - OCR score `9.0`
  - contains `cam`, `teeth`, `sensor`, and `position`
  - OCR excerpt mentions the crankshaft position sensor and cam measurement in one image
- [mems3_crank_and_cam_p01_img02.jpg](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/pdf_images/mems3_crank_and_cam_p01_img02.jpg)
  - OCR score `8.0`
  - repeated `sensor` plus `position`
  - OCR excerpt explicitly names `CKP` and `CMP`
- [mems_2_crank_n_cam_p02_img11.jpg](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/pdf_images/mems_2_crank_n_cam_p02_img11.jpg)
  - OCR score `7.5`
  - contains `flywheel`, `sensor`, and `position`
  - OCR excerpt references the flywheel and crankshaft position sensor
- [mems3_crank_and_cam_p01_img03.jpg](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/pdf_images/mems3_crank_and_cam_p01_img03.jpg)
  - OCR score `5.5`
  - contains `flywheel`, `sensor`, `position`, and `vvc`
  - OCR excerpt references camshaft position relative to the crankshaft

Safe conclusion from the OCR ranking:

- the relevant Rover wheel/sensor images are now narrowed to a small candidate set
- the strongest candidates are concentrated in `mems_2_crank_n_cam` and `mems3_crank_and_cam`
- the blocker is no longer "which of the 65 images matter at all"
- the blocker is now manual visual alignment of those candidate images against the current Speeduino Rover layouts and the parsed composite-log signal changes

The environment-level PDF readiness is now also checked in a reproducible way with [inspect_pdf_evidence.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/inspect_pdf_evidence.py), which wrote [pdf_inspection.json](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/pdf_inspection.json).

Current inspection result:

- all three Rover PDFs have `0` extractable text-layer characters under `pypdf`
- no local page renderer is available in `PATH`:
  - `magick`: missing
  - `pdftoppm`: missing
  - `gswin64c`: missing

That means the Rover manuals are effectively image-only in this environment and cannot currently be turned into full-page diagrams without either:

- manual viewing outside the harness, or
- installing a page-rendering toolchain

### What The Extracted Rover Project Confirms

The recovered tune file [CurrentTune.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting/CurrentTune.msq) confirms this was not just a generic Rover setup:

- firmware: `Speeduino+2021.04-dev`
- cylinders: `4`
- ignition load algorithm: `MAP`
- trigger speed: `Crank Speed`
- secondary trigger pattern: `5-3-2 cam`
- spark mode: `Wasted Spark`
- injection layout: `Sequential`

That matters because it narrows the relevant Rover path inside the current decoder:

- primary wheel handling in `triggerPri_RoverMEMS()`
- secondary `5-3-2` logic in `triggerSec_RoverMEMS()`
- half-sync to full-sync transitions for crank-speed primary plus cam-derived phase

### What The Extracted CSV Logs Confirm

The extracted Rover project includes raw CSV logs in [DataLogs](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting/DataLogs), including:

- [2021-06-23_01.12.43-cranking_risingoncam_crank.csv](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting/DataLogs/2021-06-23_01.12.43-cranking_risingoncam_crank.csv)
- [2021-06-23_01.48.40_again.csv](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting/DataLogs/2021-06-23_01.48.40_again.csv)
- [2021-06-23_01.59.58_rising_ne.csv](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting/DataLogs/2021-06-23_01.59.58_rising_ne.csv)

The logger format is no longer opaque. The archived [mainController.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/extracted/T16-RoverMemsTesting/projectCfg/mainController.ini) defines the `compositeLogger` fields as:

- `priLevel`
- `secLevel`
- `trigger`
- `sync`
- `refTime`

That mapping is now captured in the host-side parser [parse_speeduino_composite_csv.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/parse_speeduino_composite_csv.py).

Safe conclusions from those CSVs after parsing with named fields:

- they preserve raw edge-state rows, not only a screenshot or summary
- the three key CSVs each contain `127` parsed edge rows
- each log includes repeated long-gap events consistent with a missing-tooth style primary pattern
- the extracted long-gap counts were stable across the three sampled logs at `19`
- `priLevel` is the dominant toggling channel, which is consistent with primary-wheel activity:
  - sample log `2021-06-23_01.12.43-cranking_risingoncam_crank.csv` produced `priLevel=103`, `secLevel=23`, `trigger=33`, `sync=2` toggles
  - sample log `2021-06-23_01.59.58_rising_ne.csv` produced `priLevel=104`, `secLevel=22`, `trigger=35`, `sync=3` toggles
- the logs are therefore strong enough to support future replay construction without guessing the CSV channel names

What they do not yet safely tell us:

- exact tooth numbers for each logged edge without first decoding the TunerStudio composite-log export format
- exact tooth numbers for each logged edge without first aligning the named signal changes to the Rover wheel drawings
- whether the `trigger` flag corresponds to the firmware's internal event marker in a way that can be used directly for replay expectations

## PDF Extraction Status

The Rover PDFs are now materially more usable:

- embedded JPEG assets are extracted and locally inspectable
- the extraction process is reproducible with a checked-in tool
- the image corpus is summarized by manifest rather than requiring repeated PDF parsing

What still remains unresolved:

- which of the OCR-ranked candidate images are the exact wheel drawings versus supporting sensor text
- which candidate images correspond to the exact `5-3-2 cam` plus crank-speed configuration used by the archived Rover test project
- full-page rendering of the Rover manuals in this environment, because the PDFs have no text layer and no renderer is currently installed

## What The Forum Evidence Safely Tells Us

These points are strong enough to treat as working assumptions:

- Rover MEMS is not just one wheel; the current Speeduino decoder is intentionally multi-pattern.
- The `32 poles / 4 missing` description is consistent with Speeduino's use of a theoretical `36` tooth base and pattern identification by missing-tooth placement.
- Cam polarity and real signal conditioning matter materially for Rover sync behavior.
- The thread contains real captures, not just verbal descriptions, and those captures are the right next input for replay work.
- The extracted Rover project confirms the relevant live configuration path is `Crank Speed` plus `5-3-2 cam`, not a no-cam or single-tooth-cam variant.

These points are not yet strong enough to encode as replay traces:

- exact edge order and timing for the `17-17` layout under the current decoder
- exact relationship between the composite logs and `toothCurrentCount` transitions
- exact secondary event timing needed to move from half-sync to full sync for the single-tooth or half-moon case
- exact tooth-position mapping for the recovered named Rover CSV signals

## External Cross-Checks

### MS3

MS3 is currently the strongest external clue for Rover phase behavior.

Useful references:

- [core.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master/ms3/core.ini)
  - documents `Rover#1`, `Rover#2`, and `Rover#3`
  - documents `poll_level_tooth`
  - help text says: `Rover 4.6 try 17`
- [ms3_ign_in.c](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master/ms3/ms3_ign_in.c)
  - contains dedicated Rover modes:
    - `Rover 36-1-1 mode`
    - `Rover 36-1-1-1-1 mode2 (EU3)`
    - `Rover 36-1-1-1-1 mode3`
- [ms3_ign_wheel.c](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master/ms3/ms3_ign_wheel.c)
  - contains corresponding Rover wheel setup code

Practical meaning:

- MS3 reinforces that Rover phase detection is not just a missing-tooth problem; poll-point choice can materially affect phase.
- The `try 17` note is consistent with Speeduino's own special treatment of the `17-17` layout.

### rusEFI

Useful reference:

- [trigger_rover.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_rover.cpp)

Practical meaning:

- rusEFI's `Rover K` decoder confirms that Rover-family support exists elsewhere and can be used as topology evidence.
- It is less directly useful than MS3 for phase/poll behavior, but still useful as a cross-check that Rover support is not a one-off Speeduino invention.

## Current Best Hypothesis

Safe hypothesis:

- The existing Speeduino Rover decoder is conceptually correct in treating the family as a `36` slot wheel with different missing-tooth layouts and optional phase information from cam.
- The `17-17` variant likely needs phase interpretation that is sensitive to where the cycle is sampled, which is why MS3 exposes `poll_level_tooth` and explicitly calls out `17`.
- The missing piece for replay is not general wheel topology anymore. It is exact event sequencing from the attached Rover captures.

## Next Safe Step

Before any new `test_decoders` replay work for Rover MEMS:

1. Inspect the image-only Rover PDFs manually or via a rendering path so their wheel drawings can be compared with the current decoder's five hard-coded layouts.
2. Convert one chosen Rover pattern into a short explicit note:
   - tooth order
   - missing-gap placement
   - expected cam relationship
   - expected half-sync and full-sync transitions
3. Align the named CSV signal changes with that wheel note.
4. Only then build replay traces for one pattern at a time.

## Not Safe Yet

It is still unsafe to:

- reintroduce the earlier guessed `17-17` replay traces
- assume the current forum summaries are enough to define exact timestamps
- assume a single Rover trace will validate all five supported layouts
- assume the named CSV signal changes already imply exact tooth positions without the wheel drawings
- assume the extracted PDF image set already identifies the right Rover wheel drawing without inspection

The blocker is now narrow and explicit: the attachment-level Rover captures are staged locally, the CSV channel mapping is solved, and the PDF image corpus is extracted, but the correct wheel drawings and the exact signal-to-tooth alignment have not yet been converted into a precise replay spec.
