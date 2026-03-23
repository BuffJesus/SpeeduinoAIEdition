# Session Handoff: 36-2-2-2 H4 State Model

Focus: Land a safe `36-2-2-2` vertical slice from the local forum evidence without inventing a real-capture replay trace that the repo does not yet have.

## What Was Added

- Added [ThirtySixMinus222.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/ThirtySixMinus222/ThirtySixMinus222.cpp)
- Added [ThirtySixMinus222.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/ThirtySixMinus222/ThirtySixMinus222.h)
- Wired the new suite into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)
- Staged the strongest currently identified forum artifacts under [Resources/36_2_2_2_evidence](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/36_2_2_2_evidence)
- Added [analyze_36_2_2_2_evidence.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_36_2_2_2_evidence.py)
- Added [test_analyze_36_2_2_2_evidence.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/tests/test_analyze_36_2_2_2_evidence.py)

## Evidence Basis

The local curated forum evidence is good enough to justify narrow H4-focused coverage for Speeduino's existing `36-2-2-2` decoder:

- [Fresh build, no sync. Subaru EJ20.](https://speeduino.com/forum/viewtopic.php?t=6013#p64751)
- [Subaru EJ25 DOHC](https://speeduino.com/forum/viewtopic.php?t=6731#p70668)
- [Subaru EJ25 DOHC](https://speeduino.com/forum/viewtopic.php?t=6731#p70688)
- [Issues trying to start an EJ16 with Speeduino v0.4](https://speeduino.com/forum/viewtopic.php?t=6880#p71709)

These thread summaries consistently support:

- Subaru H4 use of the `36-2-2-2` decoder
- primary crank-only sync behavior as the first safe thing to lock
- real wheel/log discussion around missing-group placement and tooth counting

That is enough for a current-code state-machine slice, but not enough for a faithful replay trace from staged local captures because those captures are not yet extracted and normalized into this repo the way Rover evidence was.

## What The Staged Local Files Now Prove

The staged files now normalize the strongest currently identified local evidence from those threads:

- [2023-09-04_18.08.38.csv](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/36_2_2_2_evidence/2023-09-04_18.08.38.csv)
- [CurrentTune_6013.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/36_2_2_2_evidence/CurrentTune_6013.msq)
- [2025-02-01_13.01.19.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/36_2_2_2_evidence/2025-02-01_13.01.19.msq)
- [cranking_6880.csv](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/36_2_2_2_evidence/cranking_6880.csv)
- [subarugc4_6880.msq](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/36_2_2_2_evidence/subarugc4_6880.msq)

The host-side analyzer safely proves:

- all three `.msq` files use:
  - `TrigPattern = 36-2-2-2`
  - `TrigEdge = FALLING`
  - `TrigEdgeSec = RISING`
  - `TrigSpeed = Crank Speed`
  - `trigPatternSec = Single tooth cam`
  - `sparkMode = Wasted Spark`
  - `nCylinders = 4`
- the 2023 composite CSV is a real composite-style capture with:
  - `PriLevel` toggles: `329`
  - `SecLevel` toggles: `10`
  - `Sync` toggles: `0`
- the 2023 composite CSV also separates into distinct primary edge families:
  - tuned `FALLING` edge bins cluster mainly around `2.4` to `3.2 ms`
  - opposite `RISING` edge bins cluster mainly around `3.4` to `4.5 ms`
- the 2025 `cranking_6880.csv` file is a plain tooth log, not a composite log

That moves the blocker from "forum links exist" to a narrower problem:

- the repo now has staged local `36-2-2-2` files and machine-readable summaries
- the tuned edge polarity is now explicit from both `.msq` settings and composite-edge bins
- but the composite capture still never reaches `Sync`, so it does not yet provide a safe tooth-numbered replay truth source
- therefore the repo still does not have a safe tooth-numbered mapping from those captures to a replay trace

## Current-Code Contract Now Locked

For `configPage2.nCylinders = 4` the new tests lock the exact existing ISR behavior in [triggerPri_ThirtySixMinus222()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp):

1. `regular -> regular -> long gap -> long gap` promotes to sync on tooth `19`
2. `regular -> regular -> long gap -> regular` promotes to sync on tooth `35`
3. after the `35` path, two more regular teeth wrap to tooth `1` and increment `startRevolutions`

This is intentionally framed as current Speeduino behavior, not as a claim that the physical Subaru wheel numbering has now been fully proven tooth-by-tooth.

## Verification

- `python -m unittest tools.tests.test_analyze_36_2_2_2_evidence`
- `python tools\analyze_36_2_2_2_evidence.py --json Resources\36_2_2_2_evidence\CurrentTune_6013.msq Resources\36_2_2_2_evidence\2025-02-01_13.01.19.msq Resources\36_2_2_2_evidence\subarugc4_6880.msq Resources\36_2_2_2_evidence\2023-09-04_18.08.38.csv Resources\36_2_2_2_evidence\cranking_6880.csv`
- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
- Result: `203/203` passing

## Remaining Blocker

The remaining blocker for a stronger `36-2-2-2` slice is the same type of gap between topology evidence and replay evidence that existed earlier for `36-2-1`:

- the repo now has safe direct AVR-side coverage for the existing H4 sync paths
- but it does not yet have a staged, explicit real-capture trace spec in-repo that can safely assert physical tooth numbering or edge polarity from real Subaru logs

So the next safe step is:

1. stage the strongest surviving `36-2-2-2` forum attachments locally
2. convert them into an explicit tooth/gap note
3. only then add replay coverage if the capture is precise enough
