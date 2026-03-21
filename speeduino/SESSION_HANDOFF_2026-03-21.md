# Session Handoff - 2026-03-21

## Workspace

- Repo root: [speeduino](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino)
- Prior handoff: [SESSION_HANDOFF_2026-03-20.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-20.md)
- Focus area: Phase 2 decoder regression harness - Miata 99-05 replay coverage

## What Changed This Session

### Miata 99-05 decoder replay-backed trace coverage added

- Added trace headers in:
  - [miata9905_single_cam_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/miata9905_single_cam_trace.h)
  - [miata9905_double_cam_sync_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/miata9905_double_cam_sync_trace.h)
- Extended [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp) with Miata 99-05 test coverage
- Coverage added for:
  - sync establishment when secondaryToothCount reaches 2 (one complete cam pulse = 2 edges)
  - full revolution wrap after sync with startRevolutions increment
- Key insight discovered: The Miata decoder counts **cam edges**, not cam pulses
  - One physical cam pulse (rising + falling) = 2 edges = secondaryToothCount of 2 = sync established
  - The decoder checks `secondaryToothCount == 2` during primary tooth processing to establish sync
  - After sync check, `secondaryToothCount` is reset to 0 at the end of each primary handler
  - When sync is established on a primary tooth, `toothCurrentCount` is set to 6 (not incremented normally)

### Trace replay infrastructure usage

- Reused existing callback helper pattern `makeMiata9905Callbacks()`
- Both TRACE_SECONDARY_RISING and TRACE_SECONDARY_FALLING map to `triggerSec_Miata9905()` which increments the edge counter
- Used testGetSecondaryToothCount() accessor function rather than direct access to static secondaryToothCount variable
- Setup function configures Miata as CAM_SPEED with sequential injection, useResync enabled

## Verified Results

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passed, `164/164` (up from 162/162)
- New Miata 99-05 trace replay tests:
  - `test_trace_replay_miata9905_single_cam_pulse_establishes_sync` - verifies sync after 2 cam edges
  - `test_trace_replay_miata9905_sync_then_wrap_counts_revolution` - verifies revolution counter increment at wrap

## Notes

- No firmware logic changed in this slice; changes are host-side tests only
- The Miata 99-05 decoder is unique in that it:
  - Runs at cam speed (4 crank teeth per revolution)
  - Has alternating 70°/110° tooth spacing
  - Uses a cam signal with varying pulse patterns for sequential sync
  - Resets the cam edge counter after every primary tooth, requiring cam activity to complete before the next primary arrives for sync to be detected
- This decoder adds coverage for edge-counting secondary synchronization patterns, distinct from the window-based (Nissan360), grouping-based (Ford ST170), and missing-tooth-based (NGC, missing-tooth 36-1) patterns already covered
- The decoder comments referring to "SINGLE" and "DOUBLE" cam pulses describe the physical Miata trigger wheel pattern where different pulse groupings occur at different crank positions for sequential identification

## Safe Next Slice

1. Add replay coverage for another decoder with mixed secondary behavior not yet covered, such as:
   - 4G63 (Mitsubishi) - 12-tooth missing-1 primary with complex cam phase detection
   - Jeep 2000 - 12-tooth primary with 4-tooth cam signal
   - Audi 135 - 135-tooth primary with specific cam window patterns
2. All of these have complex primary/secondary interactions that would benefit from trace-backed validation
3. Continue keeping changes host-side only and verify with `pio test`

## Recommended Prompt For Next Session

`Continue Phase 2 regression harness from SESSION_HANDOFF_2026-03-21.md. Miata 99-05 now has trace-backed replay coverage showing edge-counted cam synchronization with sync establishment after 2 cam edges and revolution wrap behavior. The trace fixture supports both simple TraceEvent arrays and compressed TraceRepeatEvent formats. Existing replay coverage now includes dual-wheel, Ford ST170, K6A, missing-tooth 36-1, NGC4, Nissan360, Renix, and Miata 99-05. Next recommended slice: add replay-backed coverage for another decoder with interesting secondary patterns such as 4G63, Jeep 2000, or Audi 135, keeping changes host-side only and verifying with pio test.`
