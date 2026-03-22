# Session Handoff: Forum Evidence Curation

## What Changed

- Tightened [Resources/speeduino_evidence_collector_stable.py](C:/Users/Cornelio/Desktop/speeduino-202501.6/Resources/speeduino_evidence_collector_stable.py) so forum evidence extraction is usable for roadmap and decoder work instead of producing raw noisy dumps.
- The collector now:
  - prefers canonical topic URLs from phpBB search results instead of post-anchor links
  - strips thread title suffixes like `- Page N`
  - drops `f=` query noise during URL canonicalization so one topic is not treated as multiple identities
  - requires explicit thread/post evidence before a decoder family match is accepted
  - bounds thread crawling with `--max-thread-pages` to prevent very long topics from dominating a run

## Verification

- `python -m py_compile Resources\speeduino_evidence_collector_stable.py`
- `python -m unittest tools.tests.test_speeduino_evidence_collector` -> `13/13`

Targeted live runs that completed successfully:

- `python Resources\speeduino_evidence_collector_stable.py --mode roadmap --search-engine forum --roadmap-area "Knock and pin/default policy" --per-query-limit 2 --limit-results 12 --max-thread-pages 12`
  - `Roadmap records: 7`
- `python Resources\speeduino_evidence_collector_stable.py --mode decoder --search-engine forum --decoder-family "Honda J32" --decoder-family "36-2-1" --decoder-family "Rover MEMS" --per-query-limit 2 --limit-results 18 --max-thread-pages 12`
  - `Decoder records: 7`

The broad unbounded `--mode both` scrape still exceeds practical runtime in this environment. Use targeted runs plus `--max-thread-pages` for now.

## Evidence Quality Outcome

The cleaned targeted decoder output narrowed down to:

- `Honda J32` -> `3` records, centered on [J35a4 Triggers](https://speeduino.com/forum/viewtopic.php?t=6027)
- `Rover MEMS` -> `4` records, centered on [rover MEMs decoder](https://speeduino.com/forum/viewtopic.php?t=1427)
- `36-2-1` -> `0` surviving curated records in the targeted run

This is a better result than the earlier raw scrape because it removed generic trigger-discussion false positives.

## Roadmap Impact

- Best next decoder candidate from forum evidence is `Rover MEMS`
  - there is direct composite-log/sync-loss evidence plus attachments from the long-running decoder thread
- Second-best candidate is `Honda J32`
  - the `J35a4 Triggers` thread contains the clearest surviving J30/J32-specific pattern discussion found so far
- `36-2-1` remains blocked on cleaner pattern-specific evidence

## Next Recommended Slice

1. Mine the `Rover MEMS` thread attachments and maintainer explanations into replay-test requirements.
2. Revisit `Honda J32` using the `J35a4 Triggers` thread as the evidence source, not synthetic gap guessing.
3. Keep broad collector runs bounded with `--max-thread-pages` unless further crawl heuristics are added.
