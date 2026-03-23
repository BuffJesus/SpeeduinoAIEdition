# Session Handoff: Dual-Wheel State Model

Date: 2026-03-23

Focus: Formalize the current `Dual-Wheel` sync / wrap / filtering / resync contract as part of the maintained Phase 2 decoder baseline

## Current decoder contract

The current [triggerSetup_DualWheel()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_DualWheel()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSec_DualWheel()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior on the maintained `12 + single secondary` crank-speed path is:

- a clean secondary pulse establishes sync immediately and primes the next primary edge from the configured tooth count
- the first primary edge after that sync wraps the counter to tooth `1`, clears `revolutionOne`, and increments `startRevolutions`
- a very short follow-up secondary pulse is filtered and does not disturb the synced tooth state
- with `useResync` enabled, a later secondary resync pulse counts as sync loss and realigns the tooth counter back to the configured primary count
- this maintained slice is about the in-tree `12-tooth + single secondary` state machine contract, not about broader dual-wheel variants that are not explicitly encoded in the current fixture

## Implemented in-tree

- Formalized the existing direct AVR state coverage in [dual_wheel.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/dual_wheel/dual_wheel.cpp)
- Linked it explicitly to the replay-backed dual-wheel traces already present in [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp)

The maintained direct/replay coverage now locks:

- clean secondary-pulse sync establishment
- first-primary wrap and revolution counting
- isolated short-secondary filtering
- synced secondary-noise survival
- `useResync` realignment with sync-loss accounting

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing with the `Dual-Wheel` state model formalized in the full decoder entrypoint

## Notes

- The repo already had both direct AVR state coverage and replay-backed dual-wheel coverage; this handoff makes that maintained contract explicit alongside the newer per-decoder state notes
- No Rover replay work was changed here
