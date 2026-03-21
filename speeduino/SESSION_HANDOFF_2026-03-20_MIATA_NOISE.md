# Session Handoff: Miata 99-05 Cam Noise Replay Coverage

## Completed

- Added a noisy Miata 99-05 replay fixture in `test/test_decoders/traces/miata9905_cam_noise_trace.h`.
- Extended `test/test_decoders/trace_replay.cpp` with a new replay-backed assertion for Miata cam-edge noise handling.

## Coverage Added

- New case: extra narrow cam pulse adds extra secondary edges and must defer sync rather than create a false one.
- The trace then proves the decoder can still establish sync on the next clean 2-edge cam pulse.

## Validation

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
- Result: `182/182` passing

## Notes

- This is a good fit for the roadmap’s “noisy and borderline trigger recordings” goal because Miata 99-05 counts cam edges, not physical pulses.
- The new replay locks down a subtle failure mode: secondary-edge noise should delay sync, not accidentally satisfy it.

## Next Roadmap Step

- Continue the noisy/borderline trace library with another mixed secondary-pattern decoder such as Audi 135 or Jeep 2000, or
- add a second Miata noise case that proves an extra edge after sync does not cause an incorrect wrap or sync-loss transition.
