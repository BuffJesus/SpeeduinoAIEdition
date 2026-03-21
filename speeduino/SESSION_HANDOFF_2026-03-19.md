# Session Handoff - 2026-03-19

## Workspace

- Repo: [speeduino](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino)
- Roadmap: [FIRMWARE_ROADMAP.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/FIRMWARE_ROADMAP.md)
- Focus area: Phase 2 regression harness, especially decoder state transitions and host-side table coverage

## What Changed This Session

### Limiter / protection state-machine coverage

- Extracted `calculateMaxAllowedRPM()` into:
  - [speeduino.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/speeduino.h)
  - [speeduino.ino](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/speeduino.ino)
- Added named state-machine tests in:
  - [test_limiter_state_machine.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_ign/test_limiter_state_machine.cpp)
  - registered in [test/test_ign/main.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_ign/main.cpp)
- Coverage includes launch limiter, flat-shift limiter, hard limiter / AFR protection priority, and AFR reactivation latch behavior.

### Config migration coverage split and expanded

- Created dedicated migration suite:
  - [test/test_updates/main.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_updates/main.cpp)
  - [test/test_updates/test_config_migrations.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_updates/test_config_migrations.cpp)
- Removed old migration hook from:
  - [test/test_init/main.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_init/main.cpp)
- Added coverage for:
  - `multiplyTableLoad` / `divideTableLoad`
  - `multiplyTableValue` / `divideTableValue`
- These tests lock down axis-only scaling vs whole-page byte rewrites in [updates.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/updates.cpp).

### Decoder helper seams and shared timing coverage

- Added unit-test-only wrapper exports in:
  - [decoders.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.h)
  - [decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp)
- Added shared decoder timing tests in:
  - [test/test_decoders/general.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/general.cpp)
- Coverage includes:
  - `SetRevolutionTime()`
  - `UpdateRevolutionTimeFromTeeth()`
  - `RpmFromRevolutionTimeUs()`
  - `crankingGetRPM()`
  - `setFilter()`

### Decoder state-machine slices completed

- Added missing-tooth runtime sync tests in:
  - [missing_tooth.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/missing_tooth/missing_tooth.cpp)
- Added dual-wheel runtime sync / resync tests in:
  - [dual_wheel.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/dual_wheel/dual_wheel.cpp)
- Added Nissan360 runtime sync-window tests in:
  - [Nissan360.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/Nissan360/Nissan360.cpp)
- Added NGC 4-cylinder half-sync / full-sync / invalid-cam-phase tests in:
  - [test_ngc.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/NGC/test_ngc.cpp)
- Added Ford ST170 runtime cam/crank interaction tests in:
  - [ForsdST170.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/FordST170/ForsdST170.cpp)

### Host-side `table3d` harness repaired and expanded

- Added exact interpolation and cache-behavior tests in:
  - [tests_tables.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_tables/tests_tables.cpp)
  - declarations in [tests_tables.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_tables/tests_tables.h)
- Installed WinLibs `gcc` / `g++` and added the toolchain bin directory to the user `Path`.
- Repaired the PlatformIO `native` harness for `table3d` by:
  - extracting narrow division helpers into [maths_division.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/maths_division.h)
  - switching [table3d_interpolate.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/table3d_interpolate.cpp) to use that header
  - updating [maths.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/maths.h) to include the new helper header
  - fixing stale includes and Unity setup in [test/test_table3d_native/test_main.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_table3d_native/test_main.cpp)
  - adding a native-only shim [test/test_table3d_native/Arduino.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_table3d_native/Arduino.h)
  - guarding `table2d`-only helpers in [test/test_utils.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_utils.h)
  - updating [platformio.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/platformio.ini) native env flags

## Verified Results

### Passed this session

- `pio test -e megaatmega2560_sim_unittest --filter test_ign`
- `pio test -e megaatmega2560_sim_unittest --filter test_updates`
- `pio test -e megaatmega2560_sim_unittest --filter test_init --without-uploading --without-testing`
- `pio test -e megaatmega2560_sim_unittest --filter test_tables`
- `pio test -e native --filter test_table3d_native`
- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`

### Latest known counts

- `test_updates`: 18/18
- `test_tables`: 15/15
- `test_table3d_native`: 11/11
- `test_decoders`: 126/126

## Important Notes

### Native toolchain

- `gcc` / `g++` were not initially available on this machine.
- WinLibs was installed through `winget`.
- The toolchain path now resolves from:
  - `C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin`
- `platformio.ini` native env now includes:
  - `-DTABLE3D_NATIVE_TEST`
  - `-I test/test_table3d_native`

### PlatformIO build-tree caveat

- Cross-env `.pio/build` contamination still happens in this tree.
- Symptoms include spurious delete warnings and stale-object confusion between AVR, Teensy, and native envs.
- If builds start behaving inconsistently, the safest recovery remains:
  1. remove `C:\Users\Cornelio\Desktop\speeduino-202501.6\.pio\build`
  2. rerun only the desired env

### Decoder testing pattern that worked

- For AVR sim tests, direct pin-state emulation is unreliable for some decoder macros that read port registers directly.
- The reliable pattern was:
  - use `delayMicroseconds()` to drive timing
  - call decoder handlers directly
  - where necessary, seed the exact timing state that the decoder uses internally, rather than relying on simulated pin edges

## Safe Next Slice

The current substantial vertical slice is complete. The next safe roadmap work is still inside Phase 2.

### Recommended next options

1. Continue decoder state-transition coverage for one more common pattern not yet covered beyond end-tooth geometry.
   - Most likely candidates: Renault 44/66 or Suzuki K6A runtime paths.
2. Expand host-side table coverage further.
   - Good target: more `table3d` interpolation edge cases or start a similarly focused native harness for another pure math/table module.
3. Start the roadmap item for replayable noisy trigger traces.
   - This would require building a small trace fixture library and choosing a trace file format.
   - It is a larger slice than the current synthetic edge tests, but aligns directly with [FIRMWARE_ROADMAP.md](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/FIRMWARE_ROADMAP.md).

### Recommended prompt for next session

`Continue Phase 2 regression harness from SESSION_HANDOFF_2026-03-19.md. Decoder state-machine slices now cover missing-tooth, dual-wheel, Nissan360, NGC 4-cylinder, and Ford ST170, and the native table3d harness is working again. Next recommended slice: add runtime state-machine tests for another common decoder with only geometry coverage, or start the noisy trigger trace harness if you can define a safe fixture format. Verify with pio test and keep changes host-side only.`
