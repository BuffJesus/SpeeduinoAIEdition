# Session Handoff: 36-2-1 Evidence Model

Date: 2026-03-22
Focus: Convert the local 4B11 / `36-2-1` evidence into an explicit gap model before any new replay traces

## Evidence Base

Primary sources used:

- [decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp)
- [test_real_4b11.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/unit_tests/tests/trigger/test_real_4b11.cpp)
- [4b11-running.csv](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/unit_tests/tests/trigger/resources/4b11-running.csv)
- [4b11-running-doubled-edge.csv](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/unit_tests/tests/trigger/resources/4b11-running-doubled-edge.csv)
- [analyze_36_2_1.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_36_2_1.py)

## Safe Conclusions

The local rusEFI `4b11` captures safely prove the crank-side wheel topology that Speeduino's `36-2-1` decoder is trying to detect:

- rising-edge spacing contains two large-gap families
  - about `1.8x` the normal tooth period
  - about `2.9x` the normal tooth period
- after startup transients, the gap sequence settles into:
  - `16` teeth before the `double` gap
  - `17` teeth before the `single` gap
- those two runs add to `33` physical teeth, which matches a `36` nominal wheel with one double-missing and one single-missing section

That is strong enough to state the real-capture crank pattern explicitly as:

- `17 present teeth -> single missing gap -> 16 present teeth -> double missing gap`

Equivalent rotationally shifted descriptions are the same wheel.

## Speeduino Decoder Contract

The current Speeduino ISR in [decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp):

- detects any large gap above `1.5x` the previous tooth period
- treats gaps below `3.0x` as the tooth after the `single` gap
- treats gaps at or above `3.0x` as the tooth after the `double` gap
- assigns:
  - `toothCurrentCount = 20` after the single gap
  - `toothCurrentCount = 1` after the double gap

Important limit:

- the source still contains the comment `it's either 19 or 20, need to clarify engine direction!`
- so the real-capture gap model is now clear, but the exact in-decoder tooth numbering around the single-gap sync point is still not fully resolved from evidence

## What Changed

- Added [analyze_36_2_1.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_36_2_1.py)
- Added [test_analyze_36_2_1.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/tests/test_analyze_36_2_1.py)
- Added [ThirtySixMinus21.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/ThirtySixMinus21/ThirtySixMinus21.cpp)
- Added [ThirtySixMinus21.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/ThirtySixMinus21/ThirtySixMinus21.h)

The helper:

- parses rusEFI-style scope CSVs
- extracts crank rising edges
- classifies large gaps as `single` vs `double`
- reports the dominant steady-state tooth runs between those gaps
- surfaces anomalies, such as the hand-added doubled-edge capture

The AVR decoder test intentionally validates current ISR behavior, not physical truth:

- a `~2x` large gap after steady normal teeth sets `toothCurrentCount = 20`
- steady normal teeth can then carry the current code to `37` before the next explicit large-gap resync
- a `~3.5x` gap sets `toothCurrentCount = 1`

## Verification

- `python -m unittest tools.tests.test_analyze_36_2_1`
- temporary focused `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Verified results:

- `4b11-running.csv` settles into exactly:
  - `16` teeth before `double` gaps, `30` times
  - `17` teeth before `single` gaps, `30` times
- `4b11-running-doubled-edge.csv` preserves the same dominant pattern but surfaces one anomaly:
  - a `17 teeth before double gap` event caused by the injected duplicate edge
- the focused `36-2-1` decoder suite passes with the restored current-code contract above

## Remaining Blocker

Do not add a new Speeduino `36-2-1` replay trace yet unless the replay is anchored to the decoder's exact tooth numbering, not just the physical wheel topology.

The exact blocker is:

- the wheel layout is now evidence-backed
- but the Speeduino ISR still has an unresolved tooth-number comment at the single-gap sync point
- so a replay that asserts specific `toothCurrentCount` values across that transition would still require an unsafe assumption unless that numbering is derived or intentionally tested as current-code behavior rather than physical truth
