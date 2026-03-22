# Session Handoff: Honda J32 Primary Model

Date: 2026-03-22
Focus: Convert the surviving `J35a4 Triggers` forum evidence into an explicit Honda J32 primary model and land narrow decoder coverage

## Evidence Base

Primary sources used:

- [J35a4 Triggers `#64958`](https://speeduino.com/forum/viewtopic.php?t=6027#p64958)
- [J35a4 Triggers `#64990`](https://speeduino.com/forum/viewtopic.php?t=6027#p64990)
- [J35a4 Triggers `#65009`](https://speeduino.com/forum/viewtopic.php?t=6027#p65009)
- [J35a4 Triggers `#65010`](https://speeduino.com/forum/viewtopic.php?t=6027#p65010)
- [J35a4 Triggers `#65017`](https://speeduino.com/forum/viewtopic.php?t=6027#p65017)
- [J35a4 Triggers `#65023`](https://speeduino.com/forum/viewtopic.php?t=6027&start=10#p65023)
- [signal.ods](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/honda_j32_evidence/signal.ods)
- [trigger_honda.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_honda.cpp)
- [decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp)

## Explicit Model

The forum thread starts with an ambiguous shorthand:

- crank `12+12+0`
- cam `6+4+2`

PSIG explicitly rejects that shorthand as not yet a valid trigger description and asks for a drawing or oscilloscope pattern. The follow-up evidence then converges on a safer interpretation:

- PSIG restates the combination as `12/6-2`
- PSIG says the existing Speeduino `Honda J30 / J32` decoder appears similar enough to try
- the poster says the first tooth after the missing teeth on the cam marks cylinder `1` TDC

Safe conclusion:

- the surviving forum evidence is strong enough to justify testing the existing Speeduino `Honda J32` decoder as the intended topology
- it is not strong enough to claim full cam-phase behavior beyond that

## Speeduino J32 Decoder Contract

The current Speeduino decoder is primary-only in practice:

- nominal crank wheel: `24` teeth
- physical teeth present: `22`
- structure: `15` teeth, missing tooth, `7` teeth, missing tooth
- sync point: first tooth after the second missing tooth, but only when the prior run length was `15`
- cam is ignored:
  - [triggerSec_HondaJ32()](C:/Users/Cornelio/Desktop/speeduino-202501.6/speeduino/decoders.cpp) returns immediately
  - setup clears `BIT_DECODER_HAS_SECONDARY`
  - setup clears `BIT_DECODER_IS_SEQUENTIAL`

Important implementation detail:

- teeth `14` and `22` are treated as slightly wider primary teeth in comments and crank-angle math
- but sync still depends on recognizing the two missing-tooth gaps in the `15/7` primary pattern

## External Cross-Check

rusEFI's [configureHondaJ30A2_24_1_1()](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/rusefi-2026-03-17/firmware/controllers/trigger/decoders/trigger_honda.cpp) is a useful topology cross-check:

- it models the Honda J30 family as a `24`-nominal crank pattern with two single missing teeth
- that supports Speeduino's current primary-wheel topology as a credible match for the forum-described J35/J30/J32 family

Safe conclusion:

- the repo now has enough evidence to add narrow primary-only Honda J32 coverage
- the safe scope is the `15 teeth -> gap -> 7 teeth -> gap` primary state machine
- it is still not appropriate to add any sequential or cam-phase claims

## What Changed

- Added [HondaJ32.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/HondaJ32/HondaJ32.cpp)
- Added [HondaJ32.h](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/HondaJ32/HondaJ32.h)
- Wired `testHondaJ32()` into [test_decoders.cpp](C:/Users/Cornelio/Desktop/speeduino-202501.6/test/test_decoders/test_decoders.cpp)

The new tests cover:

- sync only after a long gap following `15` teeth
- rejection of the same long gap after only `7` teeth
- sync loss if the expected gap after the short group is not actually long
- wrap and revolution counting across a full synced cycle

Implementation note:

- the Unity test names were intentionally compressed so the AVR simulator image stays under the `8 KiB` `data + bss` limit during `pio test`

## Verification

- `pio test -e megaatmega2560_sim_unittest --filter test_decoders`
  - the new Honda J32 tests pass in the restored full `test_decoders` entrypoint
  - the suite itself is not globally green in the current tree
  - current unrelated failures are in:
    - `missing_tooth` ignition-angle expectations
    - `dual_wheel` ignition-angle expectations
    - `trace_replay` Harley low-primary sync-drop expectation

## Remaining Limits

- No cam-phase or second-cam work was added
- No attempt was made to reinterpret the forum `6+4+2` cam beyond the current primary-only decoder contract
- If a future J35/J32 slice needs cam-aware behavior, resume from the forum thread and attached drawings rather than inferring from the shorthand alone
