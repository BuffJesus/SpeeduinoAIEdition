from __future__ import annotations

import argparse
import json
import re
from dataclasses import asdict
from dataclasses import dataclass
from pathlib import Path

import pytesseract
from PIL import Image


KEYWORDS = {
    "crank": 3.0,
    "cam": 3.0,
    "flywheel": 2.5,
    "tooth": 2.5,
    "teeth": 2.5,
    "trigger": 2.0,
    "sensor": 1.5,
    "position": 1.0,
    "phase": 1.0,
    "vvc": 0.5,
}


@dataclass(frozen=True)
class IndexedImage:
    filename: str
    width: int
    height: int
    size_bytes: int
    mean_brightness: float
    ocr_text: str
    keyword_hits: dict[str, int]
    score: float


def normalize_text(text: str) -> str:
    return re.sub(r"\s+", " ", text).strip().lower()


def score_ocr_text(text: str) -> tuple[dict[str, int], float]:
    normalized = normalize_text(text)
    hits: dict[str, int] = {}
    score = 0.0
    for keyword, weight in KEYWORDS.items():
        count = len(re.findall(rf"\b{re.escape(keyword)}\b", normalized))
        if count:
            hits[keyword] = count
            score += count * weight
    return hits, score


def index_image(path: Path) -> IndexedImage:
    image = Image.open(path).convert("L")
    text = pytesseract.image_to_string(image)
    hits, score = score_ocr_text(text)
    histogram = image.histogram()
    total = sum(histogram)
    mean = sum(idx * count for idx, count in enumerate(histogram)) / total if total else 0.0
    return IndexedImage(
        filename=path.name,
        width=image.width,
        height=image.height,
        size_bytes=path.stat().st_size,
        mean_brightness=round(mean, 2),
        ocr_text=text[:1200],
        keyword_hits=hits,
        score=round(score, 2),
    )


def build_index(image_dir: Path) -> dict:
    entries = [index_image(path) for path in sorted(image_dir.glob("*.jpg"))]
    ranked = sorted(entries, key=lambda entry: (entry.score, entry.size_bytes), reverse=True)
    return {
        "image_dir": str(image_dir),
        "images": [asdict(entry) for entry in ranked],
    }


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="OCR and keyword-rank extracted PDF images.")
    parser.add_argument("--image-dir", required=True, help="Directory containing extracted images")
    parser.add_argument("--output", help="Optional JSON output path")
    parser.add_argument("--top", type=int, default=12, help="Number of ranked images to print")
    return parser


def main() -> int:
    parser = build_arg_parser()
    args = parser.parse_args()

    image_dir = Path(args.image_dir)
    index = build_index(image_dir)

    if args.output:
        Path(args.output).write_text(json.dumps(index, indent=2), encoding="utf-8")

    for entry in index["images"][: args.top]:
        hits = ", ".join(f"{key}={value}" for key, value in entry["keyword_hits"].items()) or "none"
        print(f"{entry['filename']}: score={entry['score']} hits={hits}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
