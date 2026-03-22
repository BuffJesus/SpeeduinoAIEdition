from __future__ import annotations

import argparse
import json
import re
from dataclasses import asdict
from dataclasses import dataclass
from pathlib import Path

from pypdf import PdfReader


def sanitize_stem(name: str) -> str:
    return re.sub(r"[^A-Za-z0-9._-]+", "_", name).strip("._-") or "document"


@dataclass(frozen=True)
class ExtractedImage:
    pdf_name: str
    page_index: int
    image_index: int
    image_name: str
    output_name: str
    size_bytes: int


def extract_pdf_images(pdf_path: Path, output_dir: Path) -> list[ExtractedImage]:
    reader = PdfReader(str(pdf_path))
    output_dir.mkdir(parents=True, exist_ok=True)
    pdf_stem = sanitize_stem(pdf_path.stem)
    extracted: list[ExtractedImage] = []

    for page_index, page in enumerate(reader.pages):
        for image_index, image_file in enumerate(page.images):
            suffix = Path(image_file.name).suffix or ".bin"
            output_name = f"{pdf_stem}_p{page_index + 1:02d}_img{image_index + 1:02d}{suffix}"
            output_path = output_dir / output_name
            output_path.write_bytes(image_file.data)
            extracted.append(
                ExtractedImage(
                    pdf_name=pdf_path.name,
                    page_index=page_index,
                    image_index=image_index,
                    image_name=image_file.name,
                    output_name=output_name,
                    size_bytes=len(image_file.data),
                )
            )

    return extracted


def build_manifest(pdf_dir: Path, output_dir: Path) -> dict:
    manifest: dict[str, object] = {"pdfs": []}

    for pdf_path in sorted(pdf_dir.glob("*.pdf")):
        images = extract_pdf_images(pdf_path, output_dir)
        manifest["pdfs"].append(
            {
                "pdf_name": pdf_path.name,
                "image_count": len(images),
                "images": [asdict(image) for image in images],
            }
        )

    return manifest


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Extract embedded images from PDF files.")
    parser.add_argument("--pdf-dir", required=True, help="Directory containing PDF files")
    parser.add_argument("--output-dir", required=True, help="Directory to write extracted image files")
    parser.add_argument("--manifest", help="Optional manifest JSON output path")
    return parser


def main() -> int:
    parser = build_arg_parser()
    args = parser.parse_args()

    pdf_dir = Path(args.pdf_dir)
    output_dir = Path(args.output_dir)
    manifest = build_manifest(pdf_dir, output_dir)

    if args.manifest:
        Path(args.manifest).write_text(json.dumps(manifest, indent=2), encoding="utf-8")

    total_images = sum(entry["image_count"] for entry in manifest["pdfs"])
    print(f"pdfs: {len(manifest['pdfs'])}")
    print(f"images: {total_images}")
    for entry in manifest["pdfs"]:
        print(f"{entry['pdf_name']}: {entry['image_count']}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
