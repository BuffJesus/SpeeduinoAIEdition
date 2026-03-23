# Session Handoff: 36-2-2-2 State Model

Date: 2026-03-23

Focus: Formalize the current `36-2-2-2` H4 sync-path contract as part of the maintained Phase 2 decoder baseline

## Current decoder contract

The current [triggerSetup_ThirtySixMinus222()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_ThirtySixMinus222()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSetEndTeeth_ThirtySixMinus222()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior on the maintained `4`-cylinder path is:

- `regular -> regular -> long gap -> long gap` promotes to sync on tooth `19`
- `regular -> regular -> long gap -> regular` promotes to sync on tooth `35`
- after the tooth-`35` path, two more regular teeth wrap the counter back to tooth `1` and increment `startRevolutions`
- the maintained slice is about the current in-tree H4 ISR behavior, not about claiming real-capture tooth numbering beyond what the direct state fixture safely encodes

## Implemented in-tree

- Formalized the existing direct AVR state coverage in [ThirtySixMinus222.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/ThirtySixMinus222/ThirtySixMinus222.cpp)
- Kept the earlier evidence-backed H4 note in [SESSION_HANDOFF_2026-03-22_36-2-2-2.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_36-2-2-2.md) as the source for why this narrower state-model slice is the safe scope

The maintained direct state coverage now locks:

- the consecutive-long-gap sync path onto tooth `19`
- the long-gap-then-normal sync path onto tooth `35`
- synced wrap and revolution counting on the H4 path

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing with the `36-2-2-2` H4 state model formalized in the full decoder entrypoint

## Notes

- The repo already had the forum-evidence note plus maintained direct AVR coverage; this handoff separates the stable current-code state contract from the earlier staging/evidence narrative
- No Rover replay work was changed here
