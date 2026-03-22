# Session Handoff: 36-2-2-2 H4 State Model

Focus: Land a safe `36-2-2-2` vertical slice from the local forum evidence without inventing a real-capture replay trace that the repo does not yet have.

## What Was Added

- Added [ThirtySixMinus222.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/ThirtySixMinus222/ThirtySixMinus222.cpp)
- Added [ThirtySixMinus222.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/ThirtySixMinus222/ThirtySixMinus222.h)
- Wired the new suite into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

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

## Current-Code Contract Now Locked

For `configPage2.nCylinders = 4` the new tests lock the exact existing ISR behavior in [triggerPri_ThirtySixMinus222()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp):

1. `regular -> regular -> long gap -> long gap` promotes to sync on tooth `19`
2. `regular -> regular -> long gap -> regular` promotes to sync on tooth `35`
3. after the `35` path, two more regular teeth wrap to tooth `1` and increment `startRevolutions`

This is intentionally framed as current Speeduino behavior, not as a claim that the physical Subaru wheel numbering has now been fully proven tooth-by-tooth.

## Verification

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
