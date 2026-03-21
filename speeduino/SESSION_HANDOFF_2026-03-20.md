# Session Handoff - 2026-03-20

## Workspace

- Repo root: [speeduino](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino)
- Prior handoff: [SESSION_HANDOFF_2026-03-19.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/SESSION_HANDOFF_2026-03-19.md)
- Focus area: Phase 2 decoder regression harness

## What Changed This Session

### Suzuki K6A runtime state-machine coverage

- Added runtime-path tests in:
  - [test_runtimeState.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/SuzukiK6A/test_runtimeState.cpp)
- Registered the new suite through:
  - [SuzukiK6A.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/SuzukiK6A/SuzukiK6A.h)
  - [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)
- Coverage added for:
  - sync acquisition on the distinctive descending-gap sequence that identifies the K6A sync tooth
  - tooth-counter rollover and `startRevolutions` increment after the next valid two teeth
  - trigger filtering of a too-short pulse after sync has been established

### Renix runtime coverage expanded

- Extended [renix.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/renix/renix.cpp) with runtime-path assertions for the 44-tooth Renix decoder.
- Coverage added for:
  - a valid long-gap sequence advancing the virtual tooth counter without a sync-loss increment
  - a too-short post-gap pulse being filtered without advancing state
- Attempted broader wrap/sync-loss state tests were not retained because the full physical wheel semantics were not yet modeled safely enough in sim. The kept tests are the subset that matched observed decoder behavior reliably.

### Replayable trigger-trace harness started

- Added a reusable header-only trace fixture in:
  - [trace_fixture.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_fixture.h)
- Added first replay-backed decoder tests in:
  - [trace_replay.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_replay.cpp)
  - registered in [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)
- Current fixture format:
  - `TraceEvent { deltaUs, kind }`
  - kinds are `TRACE_PRIMARY`, `TRACE_PRIMARY_WIDE`, `TRACE_SECONDARY_RISING`, and `TRACE_SECONDARY_FALLING`
  - replay uses `delayMicroseconds(deltaUs)` then dispatches to the matching decoder handler
  - a second compressed trace format now exists for repeated identical pulses: `TraceRepeatEvent { deltaUs, kind, repeatCount }`
- Trace definitions are now externalized into reusable headers under:
  - [test/test_decoders/traces](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces)
- Current external traces:
  - [dual_wheel_resync_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/dual_wheel_resync_trace.h)
  - [dual_wheel_short_secondary_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/dual_wheel_short_secondary_trace.h)
  - [dual_wheel_secondary_noise_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/dual_wheel_secondary_noise_trace.h)
  - [dual_wheel_sync_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/dual_wheel_sync_trace.h)
  - [dual_wheel_wrap_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/dual_wheel_wrap_trace.h)
  - [fordst170_cam_regroup_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/fordst170_cam_regroup_trace.h)
  - [fordst170_cam_noise_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/fordst170_cam_noise_trace.h)
  - [fordst170_cam_sync_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/fordst170_cam_sync_trace.h)
  - [fordst170_primary_only_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/fordst170_primary_only_trace.h)
  - [k6a_post_sync_short_gap_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/k6a_post_sync_short_gap_trace.h)
  - [k6a_sync_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/k6a_sync_trace.h)
  - [k6a_sync_noise_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/k6a_sync_noise_trace.h)
  - [k6a_wrap_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/k6a_wrap_trace.h)
  - [missing_tooth_36_1_early_gap_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/missing_tooth_36_1_early_gap_trace.h)
  - [missing_tooth_36_1_half_sync_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/missing_tooth_36_1_half_sync_trace.h)
  - [missing_tooth_36_1_noise_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/missing_tooth_36_1_noise_trace.h)
  - [missing_tooth_36_1_sequential_cam_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/missing_tooth_36_1_sequential_cam_trace.h)
  - [ngc4_primary_missing_tooth_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/ngc4_primary_missing_tooth_trace.h)
  - [nissan360_invalid_window_6_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/nissan360_invalid_window_6_trace.h)
  - [nissan360_6cyl_window_4_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/nissan360_6cyl_window_4_trace.h)
  - [nissan360_8cyl_window_7_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/nissan360_8cyl_window_7_trace.h)
  - [nissan360_window_16_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/nissan360_window_16_trace.h)
  - [nissan360_window_12_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/nissan360_window_12_trace.h)
  - [nissan360_window_8_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/nissan360_window_8_trace.h)
  - [nissan360_window_4_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/nissan360_window_4_trace.h)
  - [nissan360_wrap_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/nissan360_wrap_trace.h)
  - [renix_short_gap_after_group_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/renix_short_gap_after_group_trace.h)
  - [renix_valid_group_trace.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/traces/renix_valid_group_trace.h)
- First replay-backed traces now cover:
  - dual-wheel traces that cover clean valid secondary sync establishment, isolated short secondary filtering, clean first-primary rollover after sync, valid secondary sync with filtered short secondary noise plus first-primary rollover, and resync-driven sync-loss counting with tooth reset
  - Ford ST170 traces that cover primary-only half-sync, clean cam-before-gap full sync, cam-before-gap sync with filtered short-pulse noise, and the sixth cam tooth regroup reset that clears the secondary filter and restarts the cam grouping
  - Suzuki K6A traces that cover clean descending-gap sync acquisition, noisy sync acquisition, full wrap back to tooth one with revolution counting, and post-sync short-gap filtering
  - missing-tooth 36-1 traces that cover clean gap-based sync in wasted mode, noisy gap detection, explicit primary-only sequential half-sync retention, sequential promotion from half-sync to full sync after cam activity, and sync loss when a missing gap arrives too early
  - NGC4 primary-only missing-tooth progression into half-sync, plus both valid seeded cam phases that establish full sync on opposite revolutions
  - Nissan360 mixed primary/secondary traces that cover all four supported 4-cylinder valid windows (16, 12, 8, and 4 teeth), the supported 6-cylinder and 8-cylinder sync windows, invalid 6-tooth window rejection, resync realignment from an incorrect tooth counter, and full rollover back to tooth one with `startRevolutions` increment
  - Renix traces that separately cover valid long-gap virtual-tooth advancement and short-gap filtering after a valid group
- This fixture is still compile-time and in-memory, but traces are now externalized from the test logic and mixed-edge secondary events no longer require ad hoc replay code.
- Trace arrays now live in flash / program memory through [trace_fixture.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/trace_fixture.h), which avoided an AVR SRAM overflow after the trace library grew.
- `TriggerTrace` descriptors are now created at the callsite via `makeTriggerTrace(...)` instead of stored as static globals, and the replay test suite uses `RUN_TEST_P` so Unity test names stay in flash. This was necessary to stay under the AVR SRAM limit.
- `TRACE_PRIMARY_WIDE` was added so replay can model decoders like NGC that select different primary handlers or timing assumptions for missing-tooth edges without introducing decoder-specific branches into the replay core.
- Compressed repeated-run replay is now available via `TraceRepeatEvent` and `replayRepeatedTriggerTrace(...)`, which keeps long traces like Nissan360 rollover coverage compact in flash instead of expanding them into hundreds of explicit one-tooth events.
- The Nissan360 window traces are now on that compressed format as well, which made it practical to add the alternate 12-, 8-, and 4-tooth sync windows without growing the trace headers into repetitive event lists.
- The Nissan360 direct and replay tests were then regrouped so the added 4/6/8-cylinder window coverage stayed under the AVR SRAM limit instead of adding one Unity test symbol per supported window.
- The Ford ST170 long primary-run traces now use that compressed format as well, which reduced trace noise in the headers and eliminated the need to hand-maintain dozens of repeated crank events for each Ford replay case.
- The missing-tooth 36-1 replay traces now use that compressed format as well, including the new explicit half-sync sequential trace, which kept the added coverage under the AVR SRAM ceiling.
- Replay callback helpers now exist for decoder families with repeated channel wiring, which keeps trace-backed cases for dual-wheel, missing-tooth, Ford ST170, and Nissan360 aligned instead of duplicating callback initialization at each test site.
- Replay callback helpers now exist for decoder families with repeated channel wiring, which keeps trace-backed cases for dual-wheel, missing-tooth, Ford ST170, Nissan360, and Suzuki K6A aligned instead of duplicating callback initialization at each test site.
- A generic primary-only callback helper now covers simple one-channel replay cases, which let K6A and Renix share the same callback wiring instead of carrying duplicate helper code.
- `dispatchTraceEventKind(...)` is now forced out of line in the fixture to avoid an AVR codegen warning around repeated-run function-pointer dispatch on the Ford ST170 tests.

### Test seam cleanup for decoder state seeding

- Added unit-test-only secondary-tooth counter accessors in:
  - [decoders.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.h)
  - [decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp)
- Used that seam to stabilize NGC state-machine tests in:
  - [test_ngc.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/NGC/test_ngc.cpp)
- The NGC tests now seed exact cam-phase state directly instead of depending on fragile synthetic cam edge timings.

### Config migration coverage added

- Added a unit-test-only migration seam in:
  - [updates.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/updates.h)
  - [updates.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/updates.cpp)
- The new seam can:
  - seed a fake EEPROM data version for `doUpdates()`
  - intercept `writeAllConfig()`, `loadConfig()`, `writeCalibration()`, and `storeEEPROMVersion()` without touching real EEPROM-backed code paths
  - expose side-effect counters so migration tests can assert the exact update path taken
  - optionally stop the fake version flow after the first `storeEEPROMVersion()` so a single migration branch can be tested in isolation even though `doUpdates()` normally rolls forward through every remaining version
- Extended [test_config_migrations.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_updates/test_config_migrations.cpp) with end-to-end `doUpdates()` coverage for:
  - `19 -> 20`, including injector pairing selection for sequential 4-cylinders, boost lookup table default fill and axes, CAN broadcast defaulting, and AFR protection defaults
  - `20 -> 21`, including programmable output index remapping plus the new AE/decel/A/C/oil-pressure defaults
  - `21 -> 22`, including rolling cut defaults and DFCO hysteresis halving
  - `22 -> 23`, including WMI table scaling when `wmiMode >= WMI_MODE_OPENLOOP` plus the new DFCO taper, hardware-test, EGO MAP limit, and rusEFI CAN wideband defaults
  - `23 -> 24` migration, including CAN broadcast remapping, knock mode defaulting, GM flex frequency defaults, launch VSS defaulting, and the `configPage10` byte realignment
  - brand-new EEPROM `255 -> CURRENT_DATA_VERSION`, including `true_address`, programmable-output defaults, and `FILTER_FLEX_DEFAULT`
  - downgrade clamping when stored EEPROM version is newer than the current firmware
- This is the first non-pure-helper migration coverage in `test_updates`; it exercises real branch flow in `doUpdates()` rather than only standalone arithmetic helpers.

## Verified Results

- `pio run -e teensy41`
  - passed
  - produced `.pio/build/teensy41/firmware.hex`
- `pio test -e megaatmega2560_sim_unittest --filter test_updates`
  - passed, `25/25`
- Release artifacts refreshed from current sources:
  - [speeduino-dropbear-v2.0.1-teensy41.hex](C:/Users/Cornelio/Desktop/speeduino-202501.6/release/speeduino-dropbear-v2.0.1-teensy41.hex)
  - [speeduino-dropbear-v2.0.1.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/release/speeduino-dropbear-v2.0.1.ini)
- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - passed, `162/162`

## Notes

- One assertion needed tolerance instead of an exact microsecond value because `delayMicroseconds()` plus handler overhead produced a measured filter time of about `1024us` for a nominal `1000us` sync-gap case in sim.
- No firmware logic changed in this slice; changes are host-side tests only.
- The checked-in release INI had drifted from the current root [speeduino.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino.ini). A Teensy 4.1 build succeeded, and the release hex / INI pair under [release](C:/Users/Cornelio/Desktop/speeduino-202501.6/release) were refreshed so they now match the current source tree again.
- While adding the Ford ST170 regroup replay, one first-pass assertion turned out to be too broad because it was checking post-primary state that the direct state-machine test did not guarantee. The kept replay test now locks down the verified cam-only regroup boundary instead.
- Renix still needs a cleaner fixture if you want to lock down full sync acquisition and loss behavior rather than just validated gap/filter transitions.
- The new NGC replay tests rely on the existing unit-test seam for seeding `secondaryToothCount`, which keeps the traces focused on crank-side behavior and avoids unsafe assumptions about synthetic cam edge timing in sim.
- The new trace harness currently uses externalized compile-time trace headers, not file parsing at runtime. On the AVR sim target that is the safest way to keep tests host-side without adding unsupported filesystem assumptions inside the firmware test binary.
- The build-tree contamination caveat remains real in this workspace. For this slice, verification was run from the repo root and started with `pio run -e megaatmega2560_sim_unittest -t clean`, after which the suite passed normally.

## Safe Next Slice

1. Add another replay-backed trace derived from a real or semi-real captured waveform now that the harness can express both short mixed-edge traces and long repeated tooth runs compactly.
2. A good next target is a decoder whose secondary input has mixed pulse widths or alternating phase windows beyond Nissan360's fixed window sizes, so replay starts covering more than simple grouping, single-tooth cam promotion, resync boundaries, long primary rollover, Ford-style grouped cam resets, K6A-style post-sync filtering, Renix-style isolated primary filtering, and Nissan360-style discrete window mapping across cylinder variants.
3. If you want true file-backed traces later, do it via code generation or prebuild conversion into headers rather than runtime file IO inside the AVR test binary.

## Recommended Prompt For Next Session

`Continue Phase 2 regression harness from SESSION_HANDOFF_2026-03-20.md. Suzuki K6A has runtime state-machine tests, Renix has validated runtime gap/filter tests, and the trigger trace harness now supports externalized trace-header fixtures stored in flash, including TRACE_PRIMARY_WIDE for NGC-style missing-tooth replay and TraceRepeatEvent compression for long repeated runs. Replay-backed traces now cover dual-wheel clean sync establishment, dual-wheel isolated short-secondary filtering, dual-wheel clean rollover after sync, dual-wheel noisy sync/wrap plus dual-wheel resync sync-loss, Ford ST170 primary-only half-sync, Ford ST170 clean cam-before-gap full sync, Ford ST170 cam-noise sync, Ford ST170 sixth-tooth regroup reset, K6A clean descending-gap sync, K6A noisy sync, K6A wrap, K6A post-sync short-gap filtering, missing-tooth 36-1 clean gap sync, missing-tooth 36-1 noisy gap sync, missing-tooth 36-1 primary-only sequential half-sync, missing-tooth 36-1 sequential cam promotion, missing-tooth 36-1 early-gap sync loss, NGC4, Nissan360 4-cylinder 16/12/8/4-tooth valid windows, Nissan360 6-cylinder and 8-cylinder supported windows, Nissan360 invalid 6-tooth rejection, Nissan360 resync realignment, Nissan360 rollover wrap, Renix valid-group advancement, and Renix short-gap filtering. Next recommended slice: add another replay-backed noisy capture derived from a real or semi-real waveform, keeping changes host-side only and verifying with pio test.`
