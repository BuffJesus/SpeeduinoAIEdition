# Session Handoff: Missing-Tooth 36-1 State Model

Date: 2026-03-23

Focus: Formalize the current `Missing-Tooth 36-1` sync / half-sync / sync-loss contract as part of the maintained Phase 2 decoder baseline

## Current decoder contract

The current [triggerSetup_missingTooth()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_missingTooth()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSec_missingTooth()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior on the `36-1` crank-speed path is:

- a full primary cycle with the expected long missing-tooth gap establishes sync in wasted / semisequential operation and restarts on tooth `1`
- sequential operation without cam activity stays in half-sync, leaves `hasSync` false, and still restarts on tooth `1`
- sequential cam activity promotes that half-sync state into full sync on the next qualifying cycle
- an unexpectedly early long gap after sync is treated as a sync-loss event and increments `syncLossCounter`
- the current maintained slice is about the in-tree `36-1` state machine contract, not about broader missing-tooth families or `60-2` timing assumptions

## Implemented in-tree

- Formalized the existing direct AVR state coverage in [missing_tooth.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/missing_tooth/missing_tooth.cpp)
- Linked it explicitly to the replay-backed `36-1` traces already present in [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp)

The maintained direct/replay coverage now locks:

- clean gap-based sync onto tooth `1`
- primary-only sequential half-sync retention
- sequential cam promotion to full sync
- noisy but valid gap sync retention
- early-gap sync loss

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing with the `Missing-Tooth 36-1` state model formalized in the full decoder entrypoint

## Notes

- The repo already had both direct AVR state coverage and replay-backed `36-1` coverage; this handoff makes that maintained contract explicit alongside the newer per-decoder state notes
- No Rover replay work was changed here
