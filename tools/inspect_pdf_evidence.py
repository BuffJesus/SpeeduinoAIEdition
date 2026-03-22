from __future__ import annotations

import argparse
import json
import shutil
from pathlib import Path

from pypdf import PdfReader


RENDERERS = ("magick", "pdftoppm", "gswin64c")


def find_renderer_path(name: str) -> str | None:
    if resolved := shutil.which(name):
        return resolved

    if name == "pdftoppm":
        winget_root = Path.home() / "AppData" / "Local" / "Microsoft" / "WinGet" / "Packages"
        matches = sorted(winget_root.glob("**/pdftoppm.exe"))
        if matches:
            return str(matches[0])

    return None


def find_renderers() -> dict[str, str | None]:
    return {name: find_renderer_path(name) for name in RENDERERS}


def get_pdf_text_char_count(path: Path) -> int:
    reader = PdfReader(str(path))
    return sum(len(page.extract_text() or "") for page in reader.pages)


def summarize_pdf(path: Path) -> dict[str, object]:
    reader = PdfReader(str(path))
    return {
        "pdf_name": path.name,
        "page_count": len(reader.pages),
        "size_bytes": path.stat().st_size,
        "text_chars": get_pdf_text_char_count(path),
    }


def summarize_pdf_dir(pdf_dir: Path) -> dict[str, object]:
    pdfs = [summarize_pdf(path) for path in sorted(pdf_dir.glob("*.pdf"))]
    return {
        "pdf_dir": str(pdf_dir),
        "renderers": find_renderers(),
        "pdfs": pdfs,
    }


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Inspect local PDF evidence readiness.")
    parser.add_argument("--pdf-dir", required=True, help="Directory containing PDF files")
    parser.add_argument("--output", help="Optional JSON output path")
    return parser


def main() -> int:
    parser = build_arg_parser()
    args = parser.parse_args()

    summary = summarize_pdf_dir(Path(args.pdf_dir))

    if args.output:
        Path(args.output).write_text(json.dumps(summary, indent=2), encoding="utf-8")

    for name, resolved in summary["renderers"].items():
        print(f"renderer {name}: {'available' if resolved else 'missing'}")
    for pdf in summary["pdfs"]:
        print(
            f"{pdf['pdf_name']}: pages={pdf['page_count']} size={pdf['size_bytes']} text_chars={pdf['text_chars']}"
        )

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
