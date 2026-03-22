from __future__ import annotations

import argparse
import json
from dataclasses import asdict
from dataclasses import dataclass
from pathlib import Path
import sys
import re

if __package__ is None or __package__ == "":
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools.parse_speeduino_composite_csv import CompositeRow
from tools.parse_speeduino_composite_csv import parse_composite_csv
from tools.parse_speeduino_composite_csv import parse_logger_fields


@dataclass(frozen=True)
class CamGapAction:
    secondary_tooth_count: int
    teeth_before_gap: int
    cycle_window: str
    tooth_window: str
    revolution_one: bool
    tooth_adjustment: int
    tooth_adjustment_condition: str


@dataclass(frozen=True)
class SecondaryTransition:
    row_index: int
    sec_level: int
    trigger_level: int
    sync_level: int
    primary_toggles_since_last_secondary: int
    tooth_time_ms: float | None


CONSTANT_RE = re.compile(r'<constant name=\"([^\"]+)\">\"?([^<\"]+)\"?</constant>')


def rover_mems_532_gap_actions() -> tuple[CamGapAction, ...]:
    return (
        CamGapAction(
            secondary_tooth_count=6,
            teeth_before_gap=5,
            cycle_window="360-720",
            tooth_window="18-36",
            revolution_one=False,
            tooth_adjustment=18,
            tooth_adjustment_condition="add 18 if toothCurrentCount < 19",
        ),
        CamGapAction(
            secondary_tooth_count=4,
            teeth_before_gap=3,
            cycle_window="0-360",
            tooth_window="1-18",
            revolution_one=True,
            tooth_adjustment=-18,
            tooth_adjustment_condition="subtract 18 if toothCurrentCount > 17",
        ),
        CamGapAction(
            secondary_tooth_count=3,
            teeth_before_gap=2,
            cycle_window="0-360",
            tooth_window="18-36",
            revolution_one=True,
            tooth_adjustment=18,
            tooth_adjustment_condition="add 18 if toothCurrentCount < 19",
        ),
    )


def summarize_secondary_transitions(rows: list[CompositeRow]) -> list[SecondaryTransition]:
    if not rows:
        return []

    transitions: list[SecondaryTransition] = []
    previous = rows[0].states
    primary_toggles = 0
    for index, row in enumerate(rows[1:], start=1):
        if row.states[0] != previous[0]:
            primary_toggles += 1
        if row.states[1] != previous[1]:
            transitions.append(
                SecondaryTransition(
                    row_index=index,
                    sec_level=row.states[1],
                    trigger_level=row.states[2],
                    sync_level=row.states[3],
                    primary_toggles_since_last_secondary=primary_toggles,
                    tooth_time_ms=row.tooth_time_ms,
                )
            )
            primary_toggles = 0
        previous = row.states
    return transitions


def summarize_post_sync_secondary_bins(rows: list[CompositeRow]) -> dict[int, int]:
    bins: dict[int, int] = {}
    for transition in summarize_secondary_transitions(rows):
        if transition.sync_level != 1:
            continue
        count = transition.primary_toggles_since_last_secondary
        bins[count] = bins.get(count, 0) + 1
    return dict(sorted(bins.items()))


def summarize_post_sync_secondary_bins_by_level(rows: list[CompositeRow]) -> dict[str, dict[int, int]]:
    rising: dict[int, int] = {}
    falling: dict[int, int] = {}
    for transition in summarize_secondary_transitions(rows):
        if transition.sync_level != 1:
            continue
        target = rising if transition.sec_level == 1 else falling
        count = transition.primary_toggles_since_last_secondary
        target[count] = target.get(count, 0) + 1
    return {
        "rising": dict(sorted(rising.items())),
        "falling": dict(sorted(falling.items())),
    }


def extract_relevant_tune_constants(msq_path: Path) -> dict[str, str]:
    keys = {
        "TrigEdge",
        "TrigEdgeSec",
        "TrigPattern",
        "trigPatternSec",
        "TrigSpeed",
        "useResync",
        "sparkMode",
        "injLayout",
    }
    values: dict[str, str] = {}
    text = msq_path.read_text(encoding="iso-8859-1", errors="ignore")
    for name, value in CONSTANT_RE.findall(text):
        if name in keys:
            values[name] = value
    return values


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Summarize the Rover MEMS 5-3-2 cam phase rules and optional composite-log secondary spacing."
    )
    parser.add_argument("--ini", help="Path to the project INI used to parse the composite log")
    parser.add_argument("--csv", help="Path to a Rover composite CSV")
    parser.add_argument("--msq", help="Path to a Rover tune file to report the trigger-edge settings")
    parser.add_argument("--json", action="store_true", help="Emit JSON")
    return parser


def main() -> int:
    parser = build_arg_parser()
    args = parser.parse_args()

    payload: dict[str, object] = {
        "gap_actions": [asdict(action) for action in rover_mems_532_gap_actions()],
    }

    if args.msq:
        payload["msq"] = str(Path(args.msq))
        payload["tune_constants"] = extract_relevant_tune_constants(Path(args.msq))

    if args.ini and args.csv:
        ini_path = Path(args.ini)
        csv_path = Path(args.csv)
        fields = parse_logger_fields(ini_path)
        rows = parse_composite_csv(csv_path, len(fields))
        transitions = summarize_secondary_transitions(rows)
        payload["csv"] = str(csv_path)
        payload["secondary_transition_count"] = len(transitions)
        payload["post_sync_primary_toggle_bins"] = summarize_post_sync_secondary_bins(rows)
        payload["post_sync_primary_toggle_bins_by_level"] = summarize_post_sync_secondary_bins_by_level(rows)
        payload["first_transitions"] = [asdict(item) for item in transitions[:16]]

    if args.json:
        print(json.dumps(payload, indent=2))
        return 0

    print("Rover MEMS 5-3-2 cam gap actions:")
    for action in rover_mems_532_gap_actions():
        print(
            f"  secondaryToothCount={action.secondary_tooth_count}: "
            f"gap after {action.teeth_before_gap} teeth, cycle {action.cycle_window}, "
            f"teeth {action.tooth_window}, revolutionOne={int(action.revolution_one)}, "
            f"{action.tooth_adjustment_condition}"
        )

    if args.ini and args.csv:
        print(f"\nCSV: {payload['csv']}")
        print(f"secondary transitions: {payload['secondary_transition_count']}")
        print(f"post-sync primary-toggle bins: {payload['post_sync_primary_toggle_bins']}")
        print(f"post-sync bins by sec level: {payload['post_sync_primary_toggle_bins_by_level']}")
        print("first transitions:")
        for item in payload["first_transitions"]:
            print(
                "  "
                f"row={item['row_index']} sec={item['sec_level']} trig={item['trigger_level']} "
                f"sync={item['sync_level']} pri_between={item['primary_toggles_since_last_secondary']} "
                f"tooth_ms={item['tooth_time_ms']}"
            )

    if args.msq:
        print(f"\nMSQ: {payload['msq']}")
        for key, value in payload["tune_constants"].items():
            print(f"  {key}={value}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
