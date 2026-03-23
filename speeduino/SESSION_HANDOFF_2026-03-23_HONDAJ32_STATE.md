# Session Handoff: Honda J32 State Model

Date: 2026-03-23

Focus: Formalize the current `Honda J32` primary-only sync / rejection / wrap contract as part of the maintained Phase 2 decoder baseline

## Current decoder contract

The current [triggerSetup_HondaJ32()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_HondaJ32()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSec_HondaJ32()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- the maintained slice is primary-only in practice; setup clears sequential behavior and the secondary handler returns immediately
- the decoder recognizes the current `15 teeth -> gap -> 7 teeth -> gap` primary pattern and only syncs on the first tooth after the second gap when the previous run length was `15`
- the same long-gap event after only `7` teeth does not establish sync and restarts from tooth `1`
- after sync, a full maintained cycle wraps back to tooth `1` and increments `startRevolutions`
- this maintained slice is about the current in-tree Honda J30/J32 primary-wheel state machine, not about broader cam-phase claims from the forum shorthand

## Implemented in-tree

- Formalized the existing direct AVR state coverage in [HondaJ32.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/HondaJ32/HondaJ32.cpp)
- Kept the existing evidence-backed topology note in [SESSION_HANDOFF_2026-03-22_HONDAJ32.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_HONDAJ32.md) as the source for why this primary-only slice is the safe scope

The maintained direct state coverage now locks:

- sync only after the `15`-tooth run
- rejection of the same gap after the `7`-tooth run
- full-cycle wrap back to tooth `1`
- revolution counting after a maintained synced cycle

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing with the `Honda J32` state model formalized in the full decoder entrypoint

## Notes

- The repo already had the evidence-backed Honda J32 topology note plus maintained direct AVR coverage; this handoff separates the stable state-machine contract from the earlier evidence-gathering narrative
- No Rover replay work was changed here
