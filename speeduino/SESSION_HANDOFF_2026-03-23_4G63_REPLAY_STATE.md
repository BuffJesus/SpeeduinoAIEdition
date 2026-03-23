# Session Handoff: 4G63 Replay State Model

Date: 2026-03-23

Focus: Formalize the maintained replay-only `4G63` contract under the current host harness without reintroducing the unsafe direct AVR slice

## Current decoder contract

The maintained [triggerSetup_4G63()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), [triggerPri_4G63()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp), and [triggerSec_4G63()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) replay contract under the current host harness is:

- primary-only replay stays unsynced
- a replayed cam-phase candidate trace marks the current candidate tooth progression without falsely claiming sync
- an extra replayed cam edge does not falsely create sync under the current host harness
- this maintained slice is intentionally replay-only because the decoder relies on live pin-state sampling during edge processing and the current simulator harness does not yet isolate a safe direct AVR state suite

## Implemented in-tree

- Formalized the existing replay-backed coverage in [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp)
- Kept the earlier replay implementation note in [SESSION_HANDOFF_2026-03-20_4G63.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-20_4G63.md) as the source for why the safe scope stops short of asserting full sync

The maintained replay coverage now locks:

- no-sync behavior with no cam activity
- candidate-tooth phase marking without false sync
- extra-cam-edge rejection under the current harness

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing with the `4G63` replay-only state contract formalized in the full decoder entrypoint

## Notes

- The repo still does not have a safe direct AVR state suite for `4G63`; that remains a documented harness-isolation problem, not a reason to broaden the replay contract
- No Rover replay work was changed here
