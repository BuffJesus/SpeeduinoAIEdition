# Session Handoff: 36-2-1 State Model

Date: 2026-03-23

Focus: Formalize the current `36-2-1` single-gap / double-gap current-code contract as part of the maintained Phase 2 decoder baseline

## Current decoder contract

The current [triggerSetup_ThirtySixMinus21()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_ThirtySixMinus21()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSetEndTeeth_ThirtySixMinus21()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- a large gap above the current `1.5x` threshold establishes sync on the `36-2-1` primary path
- a current-code single-gap event lands on `toothCurrentCount = 20`
- a current-code double-gap event lands on `toothCurrentCount = 1`
- the maintained narrow replay/state slice proves the current decoder can advance from the single-gap path through the long run of normal teeth and resync on the double-gap path
- this maintained slice is explicitly about current in-tree ISR behavior, not about claiming the single-gap tooth numbering is physically correct

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
- The remaining blocker is unchanged: the single-gap tooth-number assignment is still not safe to promote from current-code behavior into asserted physical truth
- No Rover replay work was changed here
