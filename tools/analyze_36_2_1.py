from __future__ import annotations

import argparse
import csv
import json
from collections import Counter
from dataclasses import asdict
from dataclasses import dataclass
from pathlib import Path


DOUBLE_GAP_RATIO = 2.2
LARGE_GAP_RATIO = 1.4


@dataclass(frozen=True)
class GapEvent:
    tooth_after_gap_index_1_based: int
    gap_us: float
    gap_ratio: float
    gap_kind: str
    normal_intervals_since_previous_gap: int
    teeth_since_previous_gap_tooth: int


def _find_channel_name(fieldnames: list[str], channel_index: int) -> str:
    normalized = {name.strip().lower(): name for name in fieldnames}
    target = f"channel {channel_index}".lower()
    if target not in normalized:
        raise ValueError(f"CSV does not contain Channel {channel_index}")
    return normalized[target]


def parse_scope_csv(csv_path: Path, channel_index: int = 0) -> list[tuple[float, int]]:
    with csv_path.open(newline="", encoding="utf-8", errors="replace") as handle:
        reader = csv.DictReader(handle)
        if reader.fieldnames is None:
            raise ValueError("CSV is missing a header row")

        time_name = next((name for name in reader.fieldnames if name.strip().lower() == "time[s]"), None)
        if time_name is None:
            raise ValueError("CSV does not contain Time[s]")

        channel_name = _find_channel_name(reader.fieldnames, channel_index)
        samples: list[tuple[float, int]] = []
        for row in reader:
            samples.append((float(row[time_name]), int(row[channel_name])))
        return samples


def extract_rising_edges(samples: list[tuple[float, int]]) -> list[float]:
    rising_edges: list[float] = []
    previous_level: int | None = None
    for timestamp_s, level in samples:
        if previous_level == 0 and level == 1:
            rising_edges.append(timestamp_s)
        previous_level = level
    return rising_edges


def detect_gap_events(edge_times_s: list[float]) -> tuple[float, list[GapEvent]]:
    if len(edge_times_s) < 3:
        raise ValueError("Need at least three rising edges to detect 36-2-1 gaps")

    deltas_us = [(edge_times_s[index] - edge_times_s[index - 1]) * 1_000_000.0 for index in range(1, len(edge_times_s))]
    reference_gap_us = sorted(deltas_us)[len(deltas_us) // 4]

    events: list[GapEvent] = []
    previous_gap_delta_index = 0
    for delta_index, gap_us in enumerate(deltas_us, start=1):
        gap_ratio = gap_us / reference_gap_us
        if gap_ratio <= LARGE_GAP_RATIO:
            continue

        events.append(
            GapEvent(
                tooth_after_gap_index_1_based=delta_index + 1,
                gap_us=gap_us,
                gap_ratio=gap_ratio,
                gap_kind="double" if gap_ratio >= DOUBLE_GAP_RATIO else "single",
                normal_intervals_since_previous_gap=delta_index - previous_gap_delta_index - 1,
                teeth_since_previous_gap_tooth=delta_index - previous_gap_delta_index,
            )
        )
        previous_gap_delta_index = delta_index

    return reference_gap_us, events


def summarize_gap_events(events: list[GapEvent]) -> dict[str, object]:
    if not events:
        return {"gap_events": 0, "dominant_pattern": [], "anomalies": []}

    steady_state = events[1:] if len(events) > 1 else events
    dominant_counter = Counter((event.teeth_since_previous_gap_tooth, event.gap_kind) for event in steady_state)
    dominant_pattern = [
        {"teeth_since_previous_gap_tooth": teeth, "gap_kind": gap_kind, "count": count}
        for (teeth, gap_kind), count in sorted(dominant_counter.items())
    ]

    expected_pairs = {(16, "double"), (17, "single")}
    anomalies = [
        asdict(event)
        for event in steady_state
        if (event.teeth_since_previous_gap_tooth, event.gap_kind) not in expected_pairs
    ]

    return {
        "gap_events": len(events),
        "dominant_pattern": dominant_pattern,
        "anomalies": anomalies,
    }


def analyze_scope_csv(csv_path: Path, channel_index: int = 0) -> dict[str, object]:
    samples = parse_scope_csv(csv_path, channel_index=channel_index)
    rising_edges = extract_rising_edges(samples)
    reference_gap_us, gap_events = detect_gap_events(rising_edges)
    return {
        "csv": str(csv_path),
        "channel": channel_index,
        "samples": len(samples),
        "rising_edges": len(rising_edges),
        "reference_gap_us": round(reference_gap_us, 3),
        "summary": summarize_gap_events(gap_events),
        "gap_events": [asdict(event) for event in gap_events],
    }


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Analyze a real 36-2-1 / 4B11 scope CSV into single-gap and double-gap events."
    )
    parser.add_argument("csv", type=Path, help="Path to a rusEFI-style scope CSV")
    parser.add_argument("--channel", type=int, default=0, help="Channel index to treat as the crank input")
    parser.add_argument("--json", action="store_true", help="Emit machine-readable JSON")
    args = parser.parse_args()

    analysis = analyze_scope_csv(args.csv, channel_index=args.channel)
    if args.json:
        print(json.dumps(analysis, indent=2))
        return 0

    print(f"csv: {analysis['csv']}")
    print(f"channel: {analysis['channel']}")
    print(f"samples: {analysis['samples']}")
    print(f"rising edges: {analysis['rising_edges']}")
    print(f"reference gap: {analysis['reference_gap_us']:.3f} us")
    print(f"gap events: {analysis['summary']['gap_events']}")
    for item in analysis["summary"]["dominant_pattern"]:
        print(
            "dominant:"
            f" {item['teeth_since_previous_gap_tooth']} teeth before {item['gap_kind']} gap"
            f" ({item['count']} events)"
        )
    if analysis["summary"]["anomalies"]:
        print(f"anomalies: {len(analysis['summary']['anomalies'])}")
    else:
        print("anomalies: none")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
