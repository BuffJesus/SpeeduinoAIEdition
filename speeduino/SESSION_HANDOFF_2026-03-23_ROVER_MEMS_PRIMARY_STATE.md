# Session Handoff: Rover MEMS Primary-Only State Model

Date: 2026-03-23

Focus: Formalize the maintained primary-only `Rover MEMS` contract without advancing the still-blocked full `Crank Speed + 5-3-2 cam` replay slice

## Current decoder contract

The maintained [triggerSetup_RoverMEMS()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) and [triggerPri_RoverMEMS()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) primary-only contract is:

- the decoder recognizes documented Rover primary-wheel layouts through the rolling `roverMEMSTeethSeen` bit window
- on the maintained primary-only sequential path, recognized Rover patterns land in half-sync rather than full sync:
  - `hasSync == false`
  - `BIT_STATUS3_HALFSYNC == true`
  - `toothCurrentCount == 1`
- the replay-backed OEM-backed layouts `2-14-3-13` and `11-5-12-4` populate the expected skip-tooth maps and pattern identifiers without speculative cam timing assumptions
- a separate host-side tooth-log analysis now safely proves that the real June 25 post-fix running setup reached Rover pattern `#1` (`17-17`) on the primary side
- this maintained slice is explicitly limited to primary-wheel identification and half-sync behavior, not full-sync `5-3-2 cam` promotion

## Implemented in-tree

- Formalized the existing replay-backed primary-only coverage in [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp)
- Kept the earlier Rover evidence notes in:
  - [SESSION_HANDOFF_2026-03-22_ROVER_MEMS_BIT_WINDOW.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_ROVER_MEMS_BIT_WINDOW.md)
  - [SESSION_HANDOFF_2026-03-22_ROVER_MEMS_TOOTH_LOG.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-22_ROVER_MEMS_TOOTH_LOG.md)

The maintained replay/evidence coverage now locks:

- primary-only identification of Rover pattern `#3` from the OEM-backed `2-14-3-13` layout
- primary-only identification of Rover pattern `#2` from the OEM-backed `11-5-12-4` layout
- half-sync landing and skip-tooth population for those documented layouts
- host-side proof that a real post-fix tooth log reaches Rover pattern `#1`

## Verification

Verified with:

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

Expected maintained state after this slice:

- passing with the `Rover MEMS` primary-only state model formalized in the full decoder entrypoint

## Notes

- This does not move full Rover replay forward; it only separates the already-safe primary-only contract from the broader Rover evidence trail
- The remaining blocker is unchanged: there is still no safe tooth-numbered mapping from a logged cam transition to the decoder's `secondaryToothCount == 6 / 4 / 3` events needed for full `Crank Speed + 5-3-2 cam` replay
