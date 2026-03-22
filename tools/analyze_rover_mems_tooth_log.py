from __future__ import annotations

import argparse
import csv
import json
import sys
from dataclasses import asdict
from dataclasses import dataclass
from pathlib import Path

if __package__ in (None, ""):
    sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from tools.derive_rover_mems_windows import DECODER_WINDOWS
from tools.derive_rover_mems_windows import PATTERN_IDS


LAYOUT_BY_PATTERN_ID = {pattern_id: layout for layout, pattern_id in PATTERN_IDS.items()}


@dataclass(frozen=True)
class RoverToothLogMatch:
    tooth_index_0_based: int
    tooth_time_us: int
    previous_tooth_time_us: int
    target_gap_us: int
    tooth_current_count: int
    pattern_id: int
    layout: str
    window_bits: str


def parse_tooth_logger_csv(csv_path: Path) -> list[int]:
    tooth_times: list[int] = []
    with csv_path.open(newline="", encoding="utf-8", errors="replace") as handle:
        reader = csv.reader(handle)
        for row in reader:
            if not row or not row[0]:
                continue
            value = row[0].strip()
            if (
                value.startswith("#")
                or value == "ToothTime"
                or value == "uS"
                or value.startswith("MARK")
                or not value.isdigit()
            ):
                continue

            tooth_time = int(value)
            absolute_time = None
            if len(row) > 1 and row[1].strip().isdigit():
                absolute_time = int(row[1].strip())

            if not tooth_times and absolute_time is not None and tooth_time == absolute_time:
                continue

            tooth_times.append(tooth_time)
    return tooth_times


def find_rover_matches(tooth_times: list[int]) -> list[RoverToothLogMatch]:
    matches: list[RoverToothLogMatch] = []
    rover_teeth_seen = 0
    previous_tooth_time_us: int | None = None
    tooth_before_previous_us: int | None = None
    tooth_current_count = 0

    for tooth_index, tooth_time_us in enumerate(tooth_times):
        if previous_tooth_time_us is None or tooth_before_previous_us is None:
            previous_tooth_time_us, tooth_before_previous_us = tooth_time_us, previous_tooth_time_us
            continue

        target_gap_us = (3 * previous_tooth_time_us) >> 1
        if tooth_time_us > target_gap_us:
            rover_teeth_seen = ((rover_teeth_seen << 2) | 0x1) & 0xFFFFFFFF
            tooth_current_count += 2
        else:
            rover_teeth_seen = ((rover_teeth_seen << 1) | 0x1) & 0xFFFFFFFF
            tooth_current_count += 1

        if tooth_current_count >= 36:
            for layout, pattern_window in DECODER_WINDOWS.items():
                if rover_teeth_seen == pattern_window:
                    matches.append(
                        RoverToothLogMatch(
                            tooth_index_0_based=tooth_index,
                            tooth_time_us=tooth_time_us,
                            previous_tooth_time_us=previous_tooth_time_us,
                            target_gap_us=target_gap_us,
                            tooth_current_count=tooth_current_count,
                            pattern_id=PATTERN_IDS[layout],
                            layout=layout,
                            window_bits=f"{rover_teeth_seen:032b}",
                        )
                    )
                    break

        tooth_before_previous_us = previous_tooth_time_us
        previous_tooth_time_us = tooth_time_us

    return matches


def summarize_matches(matches: list[RoverToothLogMatch]) -> dict[str, object]:
    if not matches:
        return {"matches": 0, "patterns": []}

    seen_patterns: list[int] = []
    for match in matches:
        if match.pattern_id not in seen_patterns:
            seen_patterns.append(match.pattern_id)

    return {
        "matches": len(matches),
        "patterns": [
            {
                "pattern_id": pattern_id,
                "layout": LAYOUT_BY_PATTERN_ID[pattern_id],
                "first_match_tooth_index_0_based": next(
                    match.tooth_index_0_based for match in matches if match.pattern_id == pattern_id
                ),
            }
            for pattern_id in seen_patterns
        ],
    }


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Analyze a Rover MEMS tooth logger CSV with the decoder's rolling bit-window model."
    )
    parser.add_argument("csv", type=Path, help="Path to a TunerStudio tooth logger CSV")
    parser.add_argument("--json", action="store_true", help="Emit machine-readable JSON")
    args = parser.parse_args()

    tooth_times = parse_tooth_logger_csv(args.csv)
    matches = find_rover_matches(tooth_times)

    if args.json:
        print(
            json.dumps(
                {
                    "tooth_times": len(tooth_times),
                    "summary": summarize_matches(matches),
                    "matches": [asdict(match) for match in matches],
                },
                indent=2,
            )
        )
        return 0

    print(f"tooth times: {len(tooth_times)}")
    summary = summarize_matches(matches)
    print(f"matches: {summary['matches']}")
    if not matches:
        print("patterns: none")
        return 0

    for pattern in summary["patterns"]:
        print(
            f"pattern #{pattern['pattern_id']} ({pattern['layout']}), "
            f"first match at tooth index {pattern['first_match_tooth_index_0_based']}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
