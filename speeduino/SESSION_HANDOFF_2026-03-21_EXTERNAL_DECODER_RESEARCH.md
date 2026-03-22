# Session Handoff: External Decoder Research

Date: 2026-03-21
Focus: External evidence for blocked decoder replay work

## What Was Reviewed

- rusEFI supported-trigger wiki
- local rusEFI source tree in [Resources/rusefi-2026-03-17](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17)
- local MS3 source tree in [Resources/ms3-source-master/ms3](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master/ms3)
- prior Speeduino forum evidence already collected in [Resources/speeduino_forum_evidence.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/speeduino_forum_evidence.md)

## High-Value Findings

### 36-2-1

- The best external reference is rusEFI's real-trigger test in [test_real_4b11.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/unit_tests/tests/trigger/test_real_4b11.cpp).
- That file explicitly documents:
  - `Crank: 36-2-1`
  - `Cam: Single tooth (half moon)`
- More importantly, it validates against real CSV captures instead of guessed waveforms. That makes it the best model for how this decoder should be researched here.
- rusEFI changelog entries in [CHANGELOG.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/firmware/CHANGELOG.md) also show active maintenance on `36-2-1`, including `Fix 36-2-1 trigger (Mitsubishi 4B11, etc)`.

Practical takeaway:
- Do not resume Speeduino `36-2-1` replay work from synthetic gap guesses.
- First obtain or derive real capture data, ideally equivalent to the rusEFI `4b11-running.csv` style of evidence.

### Honda J32

- rusEFI has a named Honda decoder implementation in [trigger_honda.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_honda.cpp): `configureHondaJ30A2_24_1_1()`.
- rusEFI also exposes this as a real supported trigger type in generated enums:
  - `TT_HONDA_J30A2_24_1_1`
- Prior Speeduino forum evidence already pointed at a `J35a4 Triggers` thread saying the engine uses crank `12` and cam `6-2`, and that the existing Speeduino `Honda J30/J32` decoder appears to fit that setup.

Practical takeaway:
- rusEFI's Honda J30 decoder is useful as topology evidence for the blocked Speeduino `Honda J32` slice.
- It is not a drop-in waveform, but it is strong enough to justify a focused design note comparing:
  - Speeduino `J30/J32` decoder expectations
  - forum-described `12 / 6-2` behavior
  - rusEFI `24_1_1` sync-gap expectations

### Rover MEMS / Rover-family

- rusEFI includes a Rover-family decoder in [trigger_rover.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_rover.cpp): `initializeRoverK()`.
- MS3 has stronger direct clues on Rover phase behavior:
  - [core.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master/ms3/core.ini) documents `Rover#1`, `Rover#2`, and `Rover#3`
  - the same file says `poll_level_tooth` defaults to `1` and explicitly notes `Rover 4.6 try 17`
  - [ms3_ign_in.c](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master/ms3/ms3_ign_in.c) contains dedicated Rover modes:
    - `Rover 36-1-1 mode`
    - `Rover 36-1-1-1-1 mode2 (EU3)`
    - `Rover 36-1-1-1-1 mode3`
  - [ms3_ign_wheel.c](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/ms3-source-master/ms3/ms3_ign_wheel.c) contains the corresponding wheel setup sections

Practical takeaway:
- The MS3 `poll_level_tooth = 17` note is the strongest external clue yet for Rover polling and phase semantics.
- If Rover replay work resumes, start by writing an explicit Rover phase note from MS3 plus forum evidence before attempting new trace synthesis.

## Source Ranking

1. Local rusEFI source: most actionable for blocked decoder work because it includes real-trigger tests and decoder implementations.
2. Local MS3 source: most useful for Rover phase and polling semantics.
3. rusEFI trigger wiki: useful to confirm supported families, but weaker than code/tests.
4. rusEFI forum: potentially useful for captures, but only after targeted searching.

## Recommended Next Decoder Research Order

1. Rover MEMS
2. Honda J32
3. 36-2-1 only after real-capture evidence is available

## Safe Next Step

Before any new replay implementation:

1. Convert the surviving forum plus external source evidence into a one-page decoder note for the target family.
2. Explicitly write down the expected tooth/gap layout or poll-phase rule.
3. Only then synthesize or encode replay traces.

This avoids repeating the earlier failed `Honda J32`, `36-2-1`, and Rover attempts that were based on plausible but insufficiently grounded synthetic sequences.
