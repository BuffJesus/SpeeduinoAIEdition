# Session Handoff: Wiki Board Docs

## What Was Reviewed

- [Home | Speeduino Manual](https://wiki.speeduino.com/en/home)
- [Dropbear | Speeduino Manual](https://wiki.speeduino.com/en/boards/Dropbear)

## Useful Outcomes

The wiki did not replace the forum/manual pipeline for blocked decoder replay work, but it did add stronger primary-project documentation for board-scoped behavior:

- DropBear board docs explicitly map the board outputs used for:
  - `Boost` -> TS pin 24
  - `Fan` -> TS pin 25
  - `Idle` -> TS pin 27
  - `VVT` -> TS pin 28
  - `Idle 2` / spare output -> TS pin 29
- DropBear board docs also say spare digital inputs can be used for `VSS` and `Idle Up`
- The same page documents:
  - internal vs external MAP source switching
  - board-level crank hardware filtering guidance
  - Teensy/DropBear-specific board configuration expectations

## Why It Matters

This strengthens the roadmap direction of moving board-specific assumptions out of generic INI/default policy and into board-scoped capability/configuration layers.

In particular, future work on packaged tunes or board-profile audits should prefer the official board docs over generic INI assumptions when those disagree for:

- boost output defaults
- idle / idle-2 / VVT output expectations
- VSS / idle-up input usage
- MAP-source and crank-filter configuration behavior

## Limits

- The wiki did not provide enough new detail to resolve the current `knock_pin` policy question.
- It also did not provide enough decoder-specific trace detail to unblock `36-2-1` replay modeling.

## Roadmap Impact

- Keep using the forum and attachments as the primary source for replay-test trace construction.
- Use the wiki as the primary source for board-profile and packaged-tune behavior.
- Best next decoder candidate remains `Rover MEMS`, with `Honda J32` second.
