# Session Handoff: Suzuki K6A State Model

Date: 2026-03-23

Focus: Formalize the current `Suzuki K6A` descending-gap sync / wrap / filter contract as part of the maintained Phase 2 decoder baseline

## Current decoder contract

The current [triggerSetup_SuzukiK6A()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_SuzukiK6A()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [getCrankAngle_SuzukiK6A()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSetEndTeeth_SuzukiK6A()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- the distinctive descending-gap primary sequence establishes sync on the current K6A sync tooth and lands on tooth `6`
- a synced follow-up cycle wraps back to tooth `1` and advances `startRevolutions` to `2`
- a very short post-sync primary pulse is filtered and does not disturb the synced tooth state
- the replay-backed noise slice preserves sync across a noisy primary-only acquisition without changing the current in-tree tooth progression
- this maintained slice is about the current in-tree K6A primary-only ISR contract, not about claiming external wheel truth beyond what the code and fixtures already encode

## Implemented in-tree

- Formalized the existing direct AVR state coverage in [test_runtimeState.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/SuzukiK6A/test_runtimeState.cpp)
- Linked it explicitly to the replay-backed K6A traces already present in [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp)

The maintained direct/replay coverage now locks:

- clean descending-gap sync onto tooth `6`
- synced wrap back to tooth `1` with two-revolution counting
- post-sync short-gap filtering
- noisy primary-only sync retention

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing with the `Suzuki K6A` state model formalized in the full decoder entrypoint

## Notes

- The repo already had direct runtime-state coverage, crank-angle coverage, set-end-tooth coverage, and replay-backed K6A traces; this handoff makes the maintained state-machine contract explicit alongside the newer per-decoder state notes
- No Rover replay work was changed here
