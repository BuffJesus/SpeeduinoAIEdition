# Tune Learn Validation Handoff

## Current State

The firmware-side tune-learning gate is implemented and exposed without changing the existing live-data packet size.

Completed pieces:
- `runtimeStatusA` bit packing in [logger.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/logger.cpp)
- bit definitions in [globals.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/globals.h)
- byte-map documentation in [live_data_map.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/live_data_map.h)
- regression coverage in [test_logger_byte_regression.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_ign/test_logger_byte_regression.cpp)
- named output-channel exposure in:
  - [speeduino.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino.ini)
  - [speeduino-u16p2-experimental.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino-u16p2-experimental.ini)
  - [speeduino-dropbear-v2.0.1.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/release/speeduino-dropbear-v2.0.1.ini)
  - [speeduino-dropbear-v2.0.1-u16p2-experimental.ini](C:/Users/Cornelio/Desktop/speeduino-202501.6/release/speeduino-dropbear-v2.0.1-u16p2-experimental.ini)
- front-page TunerStudio indicator for `rSA_tuneValid`

## Exposed Runtime Bits

`runtimeStatusA` byte at output-channel offset `147` now exports:
- `bit0`: `rSA_fuelPump`
- `bit1`: `rSA_launchHard`
- `bit2`: `rSA_flatShift`
- `bit3`: `rSA_idleUp`
- `bit4`: `rSA_fullSync`
- `bit5`: `rSA_transient`
- `bit6`: `rSA_warmupASE`
- `bit7`: `rSA_tuneValid`

Current firmware logic for `rSA_tuneValid` is:
- full sync required
- no TPS/MAP accel or decel transient
- no warmup
- no ASE
- no DFCO
- no engine protection active

## Why Validation Is Blocked

Real tune-learning behavior cannot be validated yet because the ECU hardware is not available.

That means:
- the firmware logic is testable
- the INI/TunerStudio surface is testable at config level
- actual runtime behavior under engine conditions is still unproven until bench or vehicle hardware is available

## Bench Validation Goals

When hardware is available, validate two things separately:

1. The raw runtime bits reflect firmware state correctly.
2. The `rSA_tuneValid` decision matches the intended “safe to learn” windows.

Do not treat “indicator toggled” alone as proof that autotune behavior improved. The useful proof is whether the state transitions line up with real engine conditions and whether the resulting logs make cell-hit acceptance easier to classify.

## Minimum Bench Checklist

Use a TunerStudio project that shows these channels at the same time:
- `rSA_fullSync`
- `rSA_transient`
- `rSA_warmupASE`
- `rSA_tuneValid`
- `syncLossCounter`
- `egoCorrection`
- `warmupEnrich`
- `ase_enrich`
- `TPSdot`
- `MAPdot`
- `engineProtectStatus`
- `DFCOOn`

Also keep the front-page `Tune Learn Valid` indicator visible.

## Expected Pass Conditions

### 1. Key-on, engine not synced

Expected:
- `rSA_fullSync = 0`
- `rSA_tuneValid = 0`

Reason:
- no sync means no valid learning window

### 2. Cranking before full sync

Expected:
- `rSA_fullSync = 0`
- `rSA_tuneValid = 0`

If half-sync exists internally, that should still remain blocked because the current gate requires full sync.

### 3. Stable running after full sync, no warmup/ASE/transient/protection/DFCO

Expected:
- `rSA_fullSync = 1`
- `rSA_transient = 0`
- `rSA_warmupASE = 0`
- `DFCOOn = 0`
- `engineProtectStatus = 0`
- `rSA_tuneValid = 1`

This is the primary positive case.

### 4. Warmup active

Expected:
- `rSA_warmupASE = 1`
- `rSA_tuneValid = 0`

`warmupEnrich` should be above baseline at the same time.

### 5. ASE active

Expected:
- `rSA_warmupASE = 1`
- `rSA_tuneValid = 0`

`ase_enrich` should be non-zero or active at the same time.

### 6. TPS acceleration event

Expected:
- `rSA_transient = 1`
- `rSA_tuneValid = 0`

`TPSdot` should show the corresponding transient.

### 7. TPS decel event

Expected:
- `rSA_transient = 1`
- `rSA_tuneValid = 0`

### 8. MAP accel/decel event

Expected:
- `rSA_transient = 1`
- `rSA_tuneValid = 0`

`MAPdot` should corroborate the event.

### 9. DFCO active

Expected:
- `DFCOOn = 1`
- `rSA_tuneValid = 0`

Even if `rSA_transient = 0`, DFCO should still block learning.

### 10. Engine protection active

Expected:
- `engineProtectStatus != 0`
- `rSA_tuneValid = 0`

This includes RPM, MAP, oil, AFR, or coolant protection paths.

### 11. Sync loss / resync event

Expected:
- loss of sync drops `rSA_fullSync` to `0`
- `rSA_tuneValid` drops to `0`
- `syncLossCounter` increments

### 12. Recovery back to stable running

Expected:
- `rSA_fullSync` returns to `1`
- blockers clear
- `rSA_tuneValid` returns to `1`

## Recommended Logging Method

Capture at least one short datalog for each case above. The priority cases are:
- stable post-sync idle/cruise
- warmup/ASE
- accel transient
- DFCO
- one forced sync-loss or induced unstable trigger case if safely bench-testable

For each log, verify:
- the bit channels toggle when expected
- the front-page indicator matches the bit channels
- the bit transitions line up with the legacy channels that represent the same underlying conditions

## Suggested First Bench Order

1. Confirm channel visibility and indicator visibility in TunerStudio.
2. Verify no-sync and full-sync transitions.
3. Verify warmup/ASE blocking.
4. Verify transient blocking with controlled throttle changes.
5. Verify DFCO blocking.
6. Verify protection blocking only if it can be tested safely.
7. Save logs for later offline comparison.

## Non-Goals

Do not treat these as part of this validation slice:
- proving TunerStudio’s private VE Analyze implementation changed
- widening the output-channel packet
- adding high-resolution VE telemetry
- broader native-`U16` page expansion beyond the already-documented page-2 experimental path

## Best Next Step Before Hardware

No additional firmware or INI protocol change is required just to support this validation.

The next real milestone is hardware-backed observation of:
- state correctness
- indicator correctness
- log usefulness for tune-learning decisions

If hardware remains unavailable for a while, the only worthwhile software-only extension would be a tiny offline helper that consumes logs and flags samples where `rSA_tuneValid == 1` versus blocked samples. That would validate downstream usability, not ECU behavior.
