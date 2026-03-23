# Session Handoff: NGC4 State Model

Date: 2026-03-23

Focus: Formalize the current `NGC4` half-sync and seeded cam-phase state contract as part of the maintained Phase 2 decoder baseline

## Current decoder contract

The current [triggerSetup_NGC()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) and [triggerPri_NGC()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior on the `4`-cylinder path is:

- the primary decoder treats the wide missing-tooth polarity as the crank-side position reference
- a primary-only missing-tooth sequence reaches half-sync, leaves `hasSync` false, and restarts on tooth `1`
- seeded cam phase `1` establishes full sync on the first revolution (`revolutionOne == false`)
- seeded cam phase `5` establishes full sync on the opposite revolution (`revolutionOne == true`)
- the current direct state suite intentionally seeds `secondaryToothCount` instead of inventing synthetic cam pulse timing that the simulator cannot defend safely

This slice is about the current in-tree 4-cylinder NGC sync contract, not about claiming broader 6- or 8-cylinder cam-group coverage from this specific state fixture.

## Implemented in-tree

- Formalized the existing direct AVR state coverage in [test_ngc.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/NGC/test_ngc.cpp)
- Linked it explicitly to the replay-backed primary-only and seeded-phase traces already present in [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp)

The maintained direct/replay coverage now locks:

- primary-only half-sync on the wide missing tooth
- full sync on the phase-`1` seeded cam state
- opposite-revolution full sync on the phase-`5` seeded cam state

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing with the NGC4 state model formalized in the full decoder entrypoint

## Notes

- The repo already had replay-backed NGC4 coverage for primary-only half-sync and both seeded full-sync phase outcomes; this handoff makes that maintained contract explicit
- No Rover replay work was changed here
