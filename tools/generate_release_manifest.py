"""FW-001: Release manifest generator.

Scans the `release/` folder and emits `release/release_manifest.json` with one
entry per `.hex` artifact.  The manifest is deterministic: entries are sorted by
kind priority then by filename so the output is stable across re-runs.

Each entry carries:
  hex                 - path relative to the release folder
  board_family        - e.g. "teensy41-dropbear"
  firmware_signature  - text string the ECU returns for the 'Q' command
  artifact_kind       - "production" | "experimental" | "diagnostic"
  paired_ini          - relative path, or null if not applicable
  paired_msq          - relative path, or null if not applicable
  preferred           - true for the single recommended build of each kind

Classification rules (applied in order):
  1. Any hex whose filename contains "diagnostic" -> kind=diagnostic
  2. Any hex whose filename contains "experimental" (and not "diagnostic") -> kind=experimental
  3. Everything else -> kind=production

Pairing rules:
  - For non-diagnostic hexes, find an INI and MSQ in the same directory whose
    names share the longest common filename prefix with the hex.
  - INI/MSQ pairing is exact: each non-diagnostic hex should have exactly one
    INI and one MSQ partner; a warning is emitted otherwise.
  - Diagnostic hexes get null paired_ini / paired_msq.

Signature extraction:
  - Read the paired INI and extract the `signature = "..."` line.
  - If no paired INI, signature is null.

Preferred logic:
  - Exactly one production hex is marked preferred=true (the one without
    "experimental" or "diagnostic" in its name, if unambiguous).
  - Exactly one experimental hex is marked preferred=true (the one without
    "diagnostic" in its name, if unambiguous).
  - Diagnostic hexes: preferred=false.

Usage:
    python tools/generate_release_manifest.py [--release-dir PATH]
"""
from __future__ import annotations

import argparse
import json
import os
import re
import sys
from datetime import datetime, timezone
from pathlib import Path


_SIGNATURE_RE = re.compile(r'^\s*signature\s*=\s*"([^"]+)"', re.MULTILINE)

_KIND_ORDER = {"production": 0, "experimental": 1, "diagnostic": 2}


def classify_hex(name: str) -> str:
    """Return artifact_kind based on filename conventions."""
    low = name.lower()
    if "diagnostic" in low:
        return "diagnostic"
    if "experimental" in low:
        return "experimental"
    return "production"


def classify_ini_or_msq(name: str) -> str:
    """Return artifact_kind for an INI or MSQ file."""
    low = name.lower()
    if "experimental" in low:
        return "experimental"
    return "production"


def longest_common_prefix_length(a: str, b: str) -> int:
    length = 0
    for ca, cb in zip(a, b):
        if ca != cb:
            break
        length += 1
    return length


def find_paired_file(hex_stem: str, candidates: list[Path]) -> Path | None:
    """Return the candidate that shares the longest prefix with *hex_stem*."""
    best: Path | None = None
    best_score = 0
    for candidate in candidates:
        score = longest_common_prefix_length(hex_stem, candidate.stem)
        if score > best_score:
            best_score = score
            best = candidate
    return best if best_score > 0 else None


def extract_signature(ini_path: Path | None) -> str | None:
    if ini_path is None or not ini_path.exists():
        return None
    text = ini_path.read_text(encoding="utf-8", errors="ignore")
    match = _SIGNATURE_RE.search(text)
    return match.group(1) if match else None


def scan_release_dir(release_dir: Path) -> list[dict]:
    hex_files = sorted(
        p for p in release_dir.rglob("*.hex")
        # Skip build-output blobs (compiler outputs in subdirs alongside .ino)
        if not any(part.endswith("_build") for part in p.parts)
    )
    ini_files = sorted(release_dir.glob("*.ini"))
    msq_files = sorted(
        p for p in release_dir.glob("*.msq")
        if not p.name.endswith(".backup")
    )

    # Only MSQs that look like base tunes (not user-specific project files)
    base_msq_files = [
        p for p in msq_files
        if "base" in p.name.lower() or "base-tune" in p.name.lower()
    ]

    entries: list[dict] = []

    for hex_path in hex_files:
        rel_hex = hex_path.relative_to(release_dir).as_posix()
        name = hex_path.name
        kind = classify_hex(name)

        paired_ini: Path | None = None
        paired_msq: Path | None = None
        signature: str | None = None

        if kind != "diagnostic":
            # Only match INIs/MSQs of the same kind (production ↔ production,
            # experimental ↔ experimental) so that prefix comparison cannot
            # accidentally cross-pair them.
            same_kind_inis = [p for p in ini_files if classify_ini_or_msq(p.name) == kind]
            same_kind_msqs = [p for p in base_msq_files if classify_ini_or_msq(p.name) == kind]
            paired_ini = find_paired_file(hex_path.stem, same_kind_inis)
            paired_msq = find_paired_file(hex_path.stem, same_kind_msqs)

            if paired_ini is None:
                print(f"WARNING: no paired INI found for {name}", file=sys.stderr)
            if paired_msq is None:
                print(f"WARNING: no paired MSQ found for {name}", file=sys.stderr)

            signature = extract_signature(paired_ini)

        entries.append(
            {
                "hex": rel_hex,
                "board_family": "teensy41-dropbear",
                "firmware_signature": signature,
                "artifact_kind": kind,
                "paired_ini": paired_ini.name if paired_ini else None,
                "paired_msq": paired_msq.name if paired_msq else None,
                "preferred": False,  # set below
            }
        )

    # Mark preferred: one per kind (production, experimental); prefer shortest
    # non-diagnostic name within kind (simplest / most generic filename).
    for kind in ("production", "experimental"):
        group = [e for e in entries if e["artifact_kind"] == kind]
        if not group:
            continue
        # Prefer the entry without extra suffixes — shortest hex basename.
        preferred = min(group, key=lambda e: len(e["hex"]))
        preferred["preferred"] = True

    # Sort: kind order, then hex path for determinism
    entries.sort(key=lambda e: (_KIND_ORDER[e["artifact_kind"]], e["hex"]))

    return entries


def build_manifest(release_dir: Path) -> dict:
    entries = scan_release_dir(release_dir)
    return {
        "schema_version": "1.0",
        "generated_at": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "release_dir": release_dir.name,
        "artifacts": entries,
    }


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="FW-001: Generate release/release_manifest.json from the release folder."
    )
    parser.add_argument(
        "--release-dir",
        default=None,
        help="Path to the release folder. Defaults to <repo-root>/release.",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Print the manifest to stdout instead of writing the file.",
    )
    return parser


def main() -> int:
    parser = build_arg_parser()
    args = parser.parse_args()

    if args.release_dir:
        release_dir = Path(args.release_dir).resolve()
    else:
        # Default: tools/ lives one level below repo root, release/ is a sibling of speeduino/
        script_dir = Path(__file__).resolve().parent
        repo_root = script_dir.parent
        release_dir = repo_root / "release"

    if not release_dir.is_dir():
        print(f"ERROR: release directory not found: {release_dir}", file=sys.stderr)
        return 1

    manifest = build_manifest(release_dir)
    output = json.dumps(manifest, indent=2) + "\n"

    if args.dry_run:
        print(output, end="")
    else:
        out_path = release_dir / "release_manifest.json"
        out_path.write_text(output, encoding="utf-8")
        print(f"Wrote {out_path}")
        print(f"  {len(manifest['artifacts'])} artifacts")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
