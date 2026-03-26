# Session Handoff: 36-2-1 State Model

Date: 2026-03-23

Focus: Formalize the current `36-2-1` single-gap / double-gap current-code contract as part of the maintained Phase 2 decoder baseline

## Current decoder contract

The current [triggerSetup_ThirtySixMinus21()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_ThirtySixMinus21()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSetEndTeeth_ThirtySixMinus21()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- a large gap above the current `1.5x` threshold establishes sync on the `36-2-1` primary path
- a single-gap event (1.5x < gap < 3x) lands on `toothCurrentCount = 19` (**evidence-backed, Phase 9**)
- a double-gap event (gap >= 3x) lands on `toothCurrentCount = 1`
- the maintained narrow replay/state slice proves the decoder advances from the single-gap path through the 16-tooth run and resyncs on the double-gap path

## Resolved: single-gap tooth-number assignment (Phase 9)

The prior `toothCurrentCount = 20` with the comment `// it's either 19 or 20, need to clarify engine direction!` has been resolved by evidence:

- **rusEFI 4B11 real capture** (`4b11-running.csv`) and `analyze_36_2_1.py` confirm: 17 physical teeth from double gap to single gap, 16 physical teeth from single gap to double gap
- **Nominal-position accounting**: count=1 (double gap) through count=17 (last actual tooth before single gap) → single gap occupies nominal position 18 → first tooth after single gap = position **19**
- **Total-position check**: 17 actual + 1 single-gap + 16 actual + 2 double-gap = **36 total** ✓ (with 19; count=20 breaks this: only 1 position remains for the 2-tooth double gap)
- **Changed in**: `decoders.cpp` `triggerPri_ThirtySixMinus21` (assignment + comment) and `getRPM_ThirtySixMinus21` (`!= 19` guard)
- **Test updated**: `ThirtySixMinus21.cpp` assertion changed to `19U`, loop changed from 17 to 15 teeth (16-tooth run), intermediate assertion changed to `34U`
- **Replay trace updated**: `3621_resync_trace.h` changed from 17 to 15 teeth to represent the real 16-tooth run

## Implemented in-tree

- Formalized the existing direct AVR state coverage in [ThirtySixMinus21.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/ThirtySixMinus21/ThirtySixMinus21.cpp)
- Kept the earlier evidence-backed topology note in [SESSION_HANDOFF_2026-03-22_36-2-1.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_36-2-1.md) as the source for the proven physical `17 -> single gap -> 16 -> double gap` wheel layout
- Linked the maintained state contract to the existing narrow replay slice in [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp)

The maintained direct/replay coverage now locks:

- current-code sync on the single-gap path
- current-code resync on the double-gap path
- tooth-count progression between those two gap classes
- preservation of the intentional limit around unresolved single-gap tooth numbering

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing with the `36-2-1` current-code state model formalized in the full decoder entrypoint

## Notes

- The repo already had the physical wheel-evidence note plus a narrow AVR/replay current-code slice; this handoff separates the stable in-tree state contract from the external-evidence narrative
- **The single-gap tooth-number blocker is resolved** (Phase 9): `toothCurrentCount = 19` is now evidence-backed physical truth, not just current-code behavior
- No Rover replay work was changed here
