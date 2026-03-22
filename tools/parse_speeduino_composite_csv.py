from __future__ import annotations

import argparse
import csv
import json
from collections import Counter
from dataclasses import dataclass
from pathlib import Path
import re
from typing import Iterable


LOGGER_DEF_RE = re.compile(r"^\s*loggerDef\s*=\s*([^,]+),")
RECORD_FIELD_RE = re.compile(r'^\s*recordField\s*=\s*([A-Za-z0-9_]+),')


@dataclass(frozen=True)
class CompositeRow:
    states: tuple[int, ...]
    ref_time_ms: float
    tooth_time_ms: float | None
    raw_values: tuple[float, ...]


def parse_logger_fields(ini_path: Path, logger_name: str = "compositeLogger") -> list[str]:
    active_logger: str | None = None
    fields: list[str] = []

    for raw_line in ini_path.read_text(encoding="utf-8", errors="ignore").splitlines():
        logger_match = LOGGER_DEF_RE.match(raw_line)
        if logger_match:
            current = logger_match.group(1).strip()
            if active_logger == logger_name and current != logger_name:
                break
            active_logger = current
            continue

        if active_logger != logger_name:
            continue

        field_match = RECORD_FIELD_RE.match(raw_line)
        if field_match:
            name = field_match.group(1).strip()
            if name != "refTime":
                fields.append(name)

    return fields


def parse_composite_csv(csv_path: Path, state_field_count: int) -> list[CompositeRow]:
    rows: list[CompositeRow] = []

    with csv_path.open(encoding="utf-8", errors="ignore", newline="") as handle:
        reader = csv.reader(handle)
        for raw in reader:
            if not raw:
                continue
            first = raw[0].strip()
            if first.startswith("#") or first.startswith("MARK"):
                continue
            try:
                values = tuple(float(part) for part in raw)
            except ValueError:
                continue
            if len(values) < state_field_count + 1:
                continue

            states = tuple(int(values[idx]) for idx in range(state_field_count))
            ref_time_ms = values[state_field_count]
            tooth_time_ms = values[state_field_count + 2] if len(values) > state_field_count + 2 else None
            rows.append(
                CompositeRow(
                    states=states,
                    ref_time_ms=ref_time_ms,
                    tooth_time_ms=tooth_time_ms,
                    raw_values=values,
                )
            )

    return rows


def summarize_rows(rows: Iterable[CompositeRow], field_names: list[str]) -> dict:
    row_list = list(rows)
    if not row_list:
        return {
            "rows": 0,
            "field_names": field_names,
            "toggle_counts": {},
            "state_counts": {},
            "max_tooth_time_ms": None,
            "min_tooth_time_ms": None,
            "long_gap_count_ge_8ms": 0,
        }

    toggle_counts = Counter()
    state_counts = Counter(row.states for row in row_list)
    prev = row_list[0].states
    for row in row_list[1:]:
        for idx, field in enumerate(field_names):
            if row.states[idx] != prev[idx]:
                toggle_counts[field] += 1
        prev = row.states

    tooth_times = [row.tooth_time_ms for row in row_list if row.tooth_time_ms is not None and row.tooth_time_ms == row.tooth_time_ms]

    return {
        "rows": len(row_list),
        "field_names": field_names,
        "toggle_counts": {field: toggle_counts.get(field, 0) for field in field_names},
        "state_counts": {"".join(str(bit) for bit in state): count for state, count in state_counts.most_common()},
        "max_tooth_time_ms": max(tooth_times) if tooth_times else None,
        "min_tooth_time_ms": min(tooth_times) if tooth_times else None,
        "long_gap_count_ge_8ms": sum(1 for value in tooth_times if value >= 8.0),
    }


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Parse a Speeduino composite logger CSV using the matching TunerStudio INI.")
    parser.add_argument("--ini", required=True, help="Path to mainController.ini or speeduino.ini")
    parser.add_argument("--csv", required=True, help="Path to a composite logger CSV")
    parser.add_argument("--logger-name", default="compositeLogger", help="loggerDef name to parse, default: compositeLogger")
    parser.add_argument("--json", action="store_true", help="Emit JSON summary")
    return parser


def main() -> int:
    parser = build_arg_parser()
    args = parser.parse_args()

    ini_path = Path(args.ini)
    csv_path = Path(args.csv)

    field_names = parse_logger_fields(ini_path, args.logger_name)
    if not field_names:
        parser.error(f"Could not find logger fields for {args.logger_name!r} in {ini_path}")

    rows = parse_composite_csv(csv_path, len(field_names))
    summary = summarize_rows(rows, field_names)

    if args.json:
        print(json.dumps(summary, indent=2))
    else:
        print(f"csv: {csv_path}")
        print(f"logger: {args.logger_name}")
        print(f"fields: {', '.join(field_names)}")
        print(f"rows: {summary['rows']}")
        print(f"toggle_counts: {summary['toggle_counts']}")
        print(f"long_gap_count_ge_8ms: {summary['long_gap_count_ge_8ms']}")
        print(f"max_tooth_time_ms: {summary['max_tooth_time_ms']}")
        print(f"min_tooth_time_ms: {summary['min_tooth_time_ms']}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
