# Session Handoff: Nissan360 State Model

Date: 2026-03-23

Focus: Formalize the current `Nissan360` window-mapping state contract as part of the maintained Phase 2 decoder baseline

## Current decoder contract

The current [triggerSetup_Nissan360()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_Nissan360()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSec_Nissan360()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- setup seeds a `2` degree primary tooth angle, starts on tooth `1`, and uses the secondary optical window as the sync source
- on the 4-cylinder path, valid window durations map to fixed sync positions at teeth `16`, `102`, `188`, and `274`
- on the supported 6-cylinder and 8-cylinder paths, the current code syncs to tooth `124` and tooth `56`
- invalid unsynced windows do not establish sync and increment `syncLossCounter`
- once already synced, the current maintained direct suite only locks the positive `useResync` case: a valid 4-cylinder long window realigns `toothCurrentCount` to tooth `16`
- a fully synced primary run wraps from tooth `360` back to tooth `1` and increments `startRevolutions`

This slice is about the current in-tree window-duration mapping and kept resync behavior, not about proving external Nissan optical wheel truth.

## Implemented in-tree

- Formalized the existing direct AVR state coverage in [Nissan360.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Nissan360/Nissan360.cpp)
- Linked it explicitly to the replay-backed Nissan360 traces already present in [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp)

The maintained direct/replay coverage now locks:

- valid 4-cylinder window sync positions
- supported 6-cylinder and 8-cylinder sync positions
- invalid-window rejection
- synced wrap and revolution counting
- the current positive `useResync` realignment path

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing with the Nissan360 state model formalized in the full decoder entrypoint

## Notes

- A broader Nissan360 direct assertion for the `useResync == false` case was attempted earlier and backed out after it triggered an order-sensitive interaction with later Harley replay coverage in the full entrypoint
- This handoff intentionally documents only the currently verified and kept Nissan360 contract
- No Rover replay work was changed here
