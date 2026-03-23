from __future__ import annotations

import argparse
import csv
import json
import math
import re
import xml.etree.ElementTree as ET
from collections import Counter
from pathlib import Path


INTERESTING_MSQ_CONSTANTS = (
    "TrigPattern",
    "TrigEdge",
    "TrigEdgeSec",
    "TrigSpeed",
    "trigPatternSec",
    "sparkMode",
    "nCylinders",
    "TrigFilter",
    "TrigAng",
    "crankRPM",
)

CONSTANT_RE_TEMPLATE = r'<constant[^>]*name="{name}"[^>]*>(.*?)</constant>'


def extract_trigger_settings(msq_path: Path) -> dict[str, str]:
    settings: dict[str, str] = {}
    try:
        root = ET.parse(msq_path).getroot()
    except ET.ParseError:
        root = None

    if root is not None:
        for constant in root.iter("constant"):
            name = constant.get("name")
            if name in INTERESTING_MSQ_CONSTANTS:
                settings[name] = (constant.text or "").strip().strip('"')

    if settings:
        return settings

    text = msq_path.read_text(encoding="utf-8", errors="ignore")
    for name in INTERESTING_MSQ_CONSTANTS:
        match = re.search(CONSTANT_RE_TEMPLATE.format(name=re.escape(name)), text, flags=re.IGNORECASE | re.DOTALL)
        if match:
            settings[name] = match.group(1).strip().strip('"')
    return settings


def detect_csv_kind(csv_path: Path) -> str:
    with csv_path.open(encoding="utf-8", errors="ignore", newline="") as handle:
        reader = csv.reader(handle)
        for row in reader:
            if not row:
                continue
            first = row[0].strip()
            if first.startswith("#") or first.startswith("MARK"):
                continue
            header = [item.strip() for item in row]
            if "PriLevel" in header and "ToothTime" in header:
                return "composite"
            if header[:2] == ["ToothTime", "Time"]:
                return "tooth_log"
            break
    return "unknown"


def summarize_numeric_bins(values: list[float], scale: float = 1.0, digits: int = 1, top: int = 12) -> list[list[float | int]]:
    counter = Counter(round(value / scale, digits) for value in values)
    return [[bucket, count] for bucket, count in counter.most_common(top)]


def summarize_tooth_log(csv_path: Path) -> dict[str, object]:
    tooth_times_us: list[float] = []

    with csv_path.open(encoding="utf-8", errors="ignore", newline="") as handle:
        reader = csv.reader(handle)
        for row in reader:
            if not row:
                continue
            first = row[0].strip()
            if first.startswith("#") or first.startswith("MARK") or first in {"ToothTime", "uS"}:
                continue
            try:
                tooth_time = float(row[0])
            except (ValueError, IndexError):
                continue
            tooth_times_us.append(tooth_time)

    positive = [value for value in tooth_times_us if value > 0.0]
    return {
        "kind": "tooth_log",
        "rows": len(tooth_times_us),
        "positive_rows": len(positive),
        "min_us": min(positive) if positive else None,
        "max_us": max(positive) if positive else None,
        "top_bins_ms": summarize_numeric_bins(positive, scale=1000.0) if positive else [],
    }


def summarize_composite(csv_path: Path) -> dict[str, object]:
    state_fields = ("PriLevel", "SecLevel", "ThirdLevel", "Trigger", "Sync", "Cycle")
    rows: list[list[float]] = []

    with csv_path.open(encoding="utf-8", errors="ignore", newline="") as handle:
        reader = csv.reader(handle)
        for row in reader:
            if not row:
                continue
            first = row[0].strip()
            if first.startswith("#") or first.startswith("MARK") or first in {"PriLevel", "Flag"}:
                continue
            try:
                rows.append([float(item) for item in row])
            except ValueError:
                continue

    toggle_counts = Counter()
    previous_state: tuple[int, ...] | None = None
    tooth_times_ms: list[float] = []
    primary_falling_ms: list[float] = []
    primary_rising_ms: list[float] = []

    for row in rows:
        state = tuple(int(row[index]) for index in range(len(state_fields)))
        if previous_state is not None:
            for index, name in enumerate(state_fields):
                if state[index] != previous_state[index]:
                    toggle_counts[name] += 1

            if state[0] != previous_state[0] and len(row) > 7 and not math.isnan(row[7]) and row[7] > 0.0:
                if state[0] == 0:
                    primary_falling_ms.append(row[7])
                else:
                    primary_rising_ms.append(row[7])
        previous_state = state

        if len(row) > 7 and not math.isnan(row[7]) and row[7] > 0.0:
            tooth_times_ms.append(row[7])

    return {
        "kind": "composite",
        "rows": len(rows),
        "toggle_counts": {name: toggle_counts.get(name, 0) for name in state_fields},
        "positive_tooth_rows": len(tooth_times_ms),
        "min_tooth_time_ms": min(tooth_times_ms) if tooth_times_ms else None,
        "max_tooth_time_ms": max(tooth_times_ms) if tooth_times_ms else None,
        "top_tooth_bins_ms": summarize_numeric_bins(tooth_times_ms) if tooth_times_ms else [],
        "primary_falling_count": len(primary_falling_ms),
        "primary_falling_bins_ms": summarize_numeric_bins(primary_falling_ms) if primary_falling_ms else [],
        "primary_rising_count": len(primary_rising_ms),
        "primary_rising_bins_ms": summarize_numeric_bins(primary_rising_ms) if primary_rising_ms else [],
    }


def analyze_path(path: Path) -> dict[str, object]:
    if path.suffix.lower() == ".msq":
        return {"kind": "msq", "settings": extract_trigger_settings(path)}

    if path.suffix.lower() != ".csv":
        return {"kind": "unknown"}

    kind = detect_csv_kind(path)
    if kind == "composite":
        return summarize_composite(path)
    if kind == "tooth_log":
        return summarize_tooth_log(path)
    return {"kind": "unknown"}


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Summarize local 36-2-2-2 evidence files without inferring unsupported tooth numbering.")
    parser.add_argument("paths", nargs="+", help="MSQ or CSV files to inspect")
    parser.add_argument("--json", action="store_true", help="Emit machine-readable JSON")
    return parser


def main() -> int:
    parser = build_arg_parser()
    args = parser.parse_args()

    report = {str(Path(item)): analyze_path(Path(item)) for item in args.paths}
    if args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    else:
        for name, summary in report.items():
            print(name)
            print(json.dumps(summary, indent=2, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
