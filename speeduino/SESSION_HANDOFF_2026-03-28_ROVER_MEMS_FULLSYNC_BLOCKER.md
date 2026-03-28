# Session Handoff: Rover MEMS Full-Sync Blocker

Date: 2026-03-28

Focus: Consolidate the surviving Rover MEMS evidence into one maintained note that explains exactly what is solved, what remains blocked, and what the next safe step is.

## Solved Pieces

### Primary-wheel model is evidence-backed

The Rover primary decoder model is no longer the blocker.

- The rolling `roverMEMSTeethSeen` window is now understood as a `32`-bit sliding view over the theoretical `36`-slot wheel.
- [tools/derive_rover_mems_windows.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/derive_rover_mems_windows.py) reproduces all five hard-coded Rover pattern constants in [decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp).
- The maintained decoder state coverage is in [SESSION_HANDOFF_2026-03-23_ROVER_MEMS_PRIMARY_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_ROVER_MEMS_PRIMARY_STATE.md).

Derived canonical primary layouts:

- pattern `#1` `17-17`
  - decoder window: `11111111111101111111111111111101`
  - canonical start: slot `34`
  - first match: `36` actual teeth / `36` theoretical teeth
- pattern `#2` `11-5-12-4`
  - decoder window: `11111101111101111111111110111101`
  - canonical start: slot `34`
  - first match: `34` actual teeth / `36` theoretical teeth
- pattern `#3` `2-14-3-13`
  - decoder window: `11011011111111111111011101111111`
  - canonical start: slot `27`
  - first match: `34` actual teeth / `36` theoretical teeth

The current checked-in replay coverage already proves the manual-backed primary layouts `11-5-12-4` and `2-14-3-13`.

### Post-fix tooth logger proves pattern `#1`

The June 25, 2021 post-VR-fix Rover tooth logger is now safe evidence for the `17-17` path.

- [tools/analyze_rover_mems_tooth_log.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_rover_mems_tooth_log.py) mirrors the primary ISR bit-window rule.
- Applied to [2021-06-25_00.54.13_tooth_logger_crank_new_battery.csv](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/forum_downloads/2021-06-25_00.54.13_tooth_logger_crank_new_battery.csv), every match lands on Rover pattern `#1`.
- No matches land on patterns `#2` to `#5`.

Safe conclusion:

- the polarity-fixed June 25 setup reached the decoder's `17-17` primary path
- the remaining Rover blocker is not primary-wheel identification

### `5-3-2` cam ISR semantics are solved

The exact `SEC_TRIGGER_5_3_2` gap rules in [triggerSec_RoverMEMS()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) are encoded in [tools/analyze_rover_mems_cam_532.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_rover_mems_cam_532.py).

The tooth-after-gap actions are:

- `secondaryToothCount == 6`
  - gap after `5`
  - `revolutionOne = false`
  - if `toothCurrentCount < 19`, add `18`
- `secondaryToothCount == 4`
  - gap after `3`
  - `revolutionOne = true`
  - if `toothCurrentCount > 17`, subtract `18`
- `secondaryToothCount == 3`
  - gap after `2`
  - `revolutionOne = true`
  - if `toothCurrentCount < 19`, add `18`

After any recognized gap, the ISR resets `secondaryToothCount = 1`.

Safe conclusion:

- the cam-phase logic is not ambiguous in code
- the blocker is not decoder semantics

## Remaining Blocker

The repo still lacks a safe tooth-numbered mapping from a logged cam transition to the decoder's internal `secondaryToothCount == 6 / 4 / 3` event.

What is known:

- the archived Rover tune/config uses `TrigPattern = Rover MEMS`, `trigPatternSec = 5-3-2 cam`, `TrigEdge = RISING`, `TrigEdgeSec = RISING`, `TrigSpeed = Crank Speed`, `useResync = No`
- forum follow-up evidence says the cam polarity was initially wrong and later corrected
- the corrected path was considered running in-thread, with timing convergence later reported around `trigger angle = 12 degrees`
- composite exports show structured post-sync secondary spacing bins compatible with a `5-3-2` family signal

What is still not known safely:

- which logged `secLevel` transition is the decoder interrupt edge that should be treated as the tooth after the `5`, `3`, or `2` gap
- how the composite export's `secLevel` and `priLevel` sampling aligns to the decoder's internal gap counters without another assumption
- a tooth-numbered crank/cam overlay that would let a replay trace claim exact full-sync phase correctness

That means a full Rover `Crank Speed + 5-3-2 cam` replay trace is still unsafe to land.

## Maintained Safe Contract

Keep the current Rover scope limited to:

- evidence-backed primary-wheel identification
- maintained primary-only decoder state coverage
- explicit documentation that full replay is blocked on cam-edge-to-gap mapping, not on the primary wheel or the `5-3-2` ISR rules

Do not add a synthetic full-sync replay trace unless a stronger source resolves that exact cam-edge mapping.

## Next Safe Step

Only one software-side step remains safe here:

1. find stronger evidence that identifies which logged cam transition is the decoder's actual interrupt/gap event
2. align that transition to one proven primary layout, likely the now-proven June 25 `17-17` path
3. then add one full-sync replay trace that asserts the real `secondaryToothCount == 6 / 4 / 3` promotion behavior

If that evidence does not exist, leave Rover full replay deferred and keep the current primary-only contract green.

## Verification

Verified during consolidation with:

- `python tools/derive_rover_mems_windows.py`
- review of:
  - [SESSION_HANDOFF_2026-03-23_ROVER_MEMS_PRIMARY_STATE.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-23_ROVER_MEMS_PRIMARY_STATE.md)
  - [Resources/rover_mems_evidence/forum_downloads/2021-06-25_00.54.13_tooth_logger_crank_new_battery.csv](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rover_mems_evidence/forum_downloads/2021-06-25_00.54.13_tooth_logger_crank_new_battery.csv)
  - [tools/analyze_rover_mems_cam_532.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/tools/analyze_rover_mems_cam_532.py)
