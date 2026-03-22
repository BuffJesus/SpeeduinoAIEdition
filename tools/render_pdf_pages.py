from __future__ import annotations

import argparse
import json
import shutil
import subprocess
from pathlib import Path


def find_pdftoppm() -> str | None:
    if resolved := shutil.which("pdftoppm"):
        return resolved

    winget_root = Path.home() / "AppData" / "Local" / "Microsoft" / "WinGet" / "Packages"
    matches = sorted(winget_root.glob("**/pdftoppm.exe"))
    if matches:
        return str(matches[0])

    return None


def render_pdf(
    pdf_path: Path,
    output_dir: Path,
    pdftoppm_path: str,
    dpi: int,
) -> list[Path]:
    output_dir.mkdir(parents=True, exist_ok=True)
    prefix = output_dir / pdf_path.stem
    subprocess.run(
        [pdftoppm_path, "-png", "-r", str(dpi), str(pdf_path), str(prefix)],
        check=True,
        capture_output=True,
        text=True,
    )
    return sorted(output_dir.glob(f"{pdf_path.stem}-*.png"))


def render_pdf_dir(pdf_dir: Path, output_dir: Path, dpi: int, pdftoppm_path: str | None = None) -> dict[str, object]:
    resolved = pdftoppm_path or find_pdftoppm()
    if not resolved:
        raise FileNotFoundError("pdftoppm executable not found")

    output_dir.mkdir(parents=True, exist_ok=True)
    rendered = []
    for pdf_path in sorted(pdf_dir.glob("*.pdf")):
        pages = render_pdf(pdf_path, output_dir, resolved, dpi)
        rendered.append(
            {
                "pdf_name": pdf_path.name,
                "page_count": len(pages),
                "pages": [page.name for page in pages],
            }
        )

    return {
        "pdf_dir": str(pdf_dir),
        "output_dir": str(output_dir),
        "pdftoppm_path": resolved,
        "dpi": dpi,
        "pdfs": rendered,
    }


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Render PDF pages to PNGs via pdftoppm.")
    parser.add_argument("--pdf-dir", required=True, help="Directory containing PDF files")
    parser.add_argument("--output-dir", required=True, help="Directory for rendered PNG pages")
    parser.add_argument("--dpi", type=int, default=200, help="Render DPI")
    parser.add_argument("--pdftoppm", help="Optional explicit pdftoppm path")
    parser.add_argument("--manifest", help="Optional JSON manifest path")
    return parser


def main() -> int:
    parser = build_arg_parser()
    args = parser.parse_args()

    manifest = render_pdf_dir(
        Path(args.pdf_dir),
        Path(args.output_dir),
        args.dpi,
        args.pdftoppm,
    )

    if args.manifest:
        Path(args.manifest).write_text(json.dumps(manifest, indent=2), encoding="utf-8")

    for pdf in manifest["pdfs"]:
        print(f"{pdf['pdf_name']}: pages={pdf['page_count']}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
