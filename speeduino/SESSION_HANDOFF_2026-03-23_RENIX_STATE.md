# Session Handoff: Renix State Model

Date: 2026-03-23

Focus: Formalize the current `Renix` virtual-tooth state contract as part of the maintained Phase 2 decoder baseline

## Current decoder contract

The current [triggerSetup_Renix()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_Renix()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSetEndTeeth_Renix()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) behavior is:

- the decoder maps the physical `44`- or `66`-tooth wheel onto a smaller virtual tooth model used by the rest of the firmware
- on the `44`-tooth path, one valid `8 short + 1 long` group advances the virtual tooth model without yet declaring sync
- a very short follow-up primary pulse after a valid group is filtered and does not mutate the virtual tooth state
- the current `setEndTeeth` behavior depends on the virtual `4`-tooth or `6`-tooth model rather than the physical wheel tooth count

This slice is about the current in-tree virtual-tooth ISR contract, not about claiming external Renix wheel geometry beyond what the code already assumes.

## Implemented in-tree

- Formalized the existing direct AVR state coverage in [renix.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/renix/renix.cpp)

The maintained direct state coverage now locks:

- valid-group advancement without false sync loss
- short-gap filtering after a valid group

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing with the Renix state model formalized in the full decoder entrypoint

## Notes

- The repo already had replay-backed Renix coverage for valid-group advancement and short-gap filtering; this slice formalizes the direct AVR state contract alongside those traces
- A broader Renix wrap/sync assertion was attempted during this session and backed out because the current virtual-tooth progression was not yet narrow enough to encode safely from the existing test fixture
- No Rover replay work was changed here
