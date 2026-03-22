from __future__ import annotations

import argparse
import json
from dataclasses import dataclass


PATTERN_IDS = {
    "17-17": 1,
    "11-5-12-4": 2,
    "2-14-3-13": 3,
    "3-14-2-13": 4,
    "9-7-10-6": 5,
}

DECODER_WINDOWS = {
    "17-17": 0b11111111111101111111111111111101,
    "11-5-12-4": 0b11111101111101111111111110111101,
    "2-14-3-13": 0b11011011111111111111011101111111,
    "3-14-2-13": 0b11011101111111111111101101111111,
    "9-7-10-6": 0b11111101111111011111111110111111,
}


@dataclass(frozen=True)
class RoverPatternSummary:
    layout: str
    pattern_id: int
    wheel_bits: str
    decoder_window_bits: str
    missing_slots_1_based: tuple[int, ...]
    omitted_slots_1_based: tuple[int, ...]
    canonical_start_slot_0_based: int
    actual_teeth_to_match: int
    theoretical_tooth_count_at_match: int
    match_ending_slot_0_based: int


def parse_layout(layout: str) -> tuple[int, ...]:
    values = tuple(int(part) for part in layout.split("-") if part)
    if not values:
        raise ValueError("layout must contain one or more tooth runs")
    if sum(values) + len(values) != 36:
        raise ValueError("Rover MEMS layouts must describe 32 teeth plus missing slots on a 36-slot wheel")
    return values


def build_wheel_bits(layout: str) -> str:
    runs = parse_layout(layout)
    return "".join(("1" * run) + "0" for run in runs)


def decoder_window_bits(layout: str) -> str:
    return f"{DECODER_WINDOWS[layout]:032b}"


def omitted_slots_1_based(layout: str) -> tuple[int, ...]:
    wheel = build_wheel_bits(layout)
    window = decoder_window_bits(layout)
    doubled = wheel + wheel
    for start in range(len(wheel)):
        if doubled[start : start + 32] == window:
            omitted = tuple((((start - offset - 1) % len(wheel)) + 1) for offset in range(4))
            return tuple(sorted(omitted))
    raise ValueError(f"decoder window for {layout} is not a 32-bit slice of the 36-slot wheel")


def find_first_decoder_match(layout: str) -> tuple[int, int, int, int]:
    wheel = build_wheel_bits(layout)
    expected = DECODER_WINDOWS[layout]
    best_match: tuple[int, int, int, int] | None = None
    for start_slot in range(len(wheel)):
        tooth_last = 0
        tooth_last_minus_one = 0
        theoretical_count = 0
        actual_teeth = 0
        window = 0
        for step in range(len(wheel) * 3):
            slot = (start_slot + step) % len(wheel)
            if wheel[slot] != "1":
                continue
            previous_slot = (slot - 1) % len(wheel)
            gap = 2000 if wheel[previous_slot] == "0" else 1000
            actual_teeth += 1
            if tooth_last > 0 and tooth_last_minus_one > 0:
                previous_gap = tooth_last - tooth_last_minus_one
                target_gap = (3 * previous_gap) >> 1
                if gap > target_gap:
                    window = ((window << 2) + 1) & 0xFFFFFFFF
                    theoretical_count += 2
                else:
                    window = ((window << 1) + 1) & 0xFFFFFFFF
                    theoretical_count += 1
                if theoretical_count >= 36 and window == expected:
                    candidate = (start_slot, actual_teeth, theoretical_count, slot)
                    if best_match is None or candidate[2:] < best_match[2:]:
                        best_match = candidate
                    break
            tooth_last_minus_one = tooth_last
            tooth_last = tooth_last + gap if tooth_last else gap
    if best_match is None:
        raise ValueError(f"no decoder match found for {layout}")
    return best_match


def summarize_layout(layout: str) -> RoverPatternSummary:
    start_slot, actual_teeth, theoretical_count, end_slot = find_first_decoder_match(layout)
    wheel = build_wheel_bits(layout)
    missing = tuple(index + 1 for index, bit in enumerate(wheel) if bit == "0")
    return RoverPatternSummary(
        layout=layout,
        pattern_id=PATTERN_IDS[layout],
        wheel_bits=wheel,
        decoder_window_bits=decoder_window_bits(layout),
        missing_slots_1_based=missing,
        omitted_slots_1_based=omitted_slots_1_based(layout),
        canonical_start_slot_0_based=start_slot,
        actual_teeth_to_match=actual_teeth,
        theoretical_tooth_count_at_match=theoretical_count,
        match_ending_slot_0_based=end_slot,
    )


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Derive the Rover MEMS rolling 32-bit decoder window from documented 36-slot wheel layouts."
    )
    parser.add_argument(
        "layouts",
        nargs="*",
        default=list(PATTERN_IDS.keys()),
        help="Layouts to summarize, for example 2-14-3-13 or 11-5-12-4",
    )
    parser.add_argument("--json", action="store_true", help="Emit machine-readable JSON")
    args = parser.parse_args()

    summaries = [summarize_layout(layout) for layout in args.layouts]
    if args.json:
        print(json.dumps([summary.__dict__ for summary in summaries], indent=2))
        return 0

    for summary in summaries:
        print(f"{summary.layout} (pattern #{summary.pattern_id})")
        print(f"  wheel bits:      {summary.wheel_bits}")
        print(f"  decoder window:  {summary.decoder_window_bits}")
        print(f"  missing slots:   {summary.missing_slots_1_based}")
        print(f"  omitted slots:   {summary.omitted_slots_1_based}")
        print(f"  canonical start: slot {summary.canonical_start_slot_0_based}")
        print(
            "  first match:     "
            f"{summary.actual_teeth_to_match} actual teeth, "
            f"{summary.theoretical_tooth_count_at_match} theoretical teeth, "
            f"ending on slot {summary.match_ending_slot_0_based}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
