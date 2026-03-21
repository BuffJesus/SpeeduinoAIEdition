#!/usr/bin/env python3
from __future__ import annotations

import argparse
import re
import sys
import xml.etree.ElementTree as ET
from dataclasses import dataclass
from pathlib import Path


MSQ_NS = {"msq": "http://www.msefi.com/:msq"}
DEFAULT_MSQ_PATH = Path("Resources") / "Speeduino base tune.msq"
DEFAULT_INI_PATH = Path("speeduino.ini")

# These fields are where this fork has already touched defaults, migrations, or UI exposure.
HIGH_RISK_CONSTANTS = (
    "knock_mode",
    "knock_pin",
    "knock_trigger",
    "knock_pullup",
    "knock_limiterDisable",
    "knock_count",
    "knock_threshold",
    "knock_maxMAP",
    "knock_maxRPM",
    "knock_maxRetard",
    "knock_firstStep",
    "knock_stepSize",
    "knock_stepTime",
    "knock_duration",
    "knock_recoveryStepTime",
    "knock_recoveryStep",
    "launchEnable",
    "launchPin",
    "launchHiLo",
    "lnchCtrlVss",
    "dfcoEnabled",
    "dfcoRPM",
    "dfcoHyster",
    "dfcoTPSThresh",
    "dfcoDelay",
    "dfcoTaperEnable",
    "dfcoTaperTime",
    "dfcoTaperFuel",
    "dfcoTaperAdvance",
    "vssMode",
    "vssPin",
    "vssPulsesPerKm",
    "vssAuxCh",
    "wmiEnabled",
    "wmiMode",
    "wmiTable",
    "rollingProtRPMDelta",
    "rollingProtCutPercent",
    "oilPressureEnable",
    "oilPressureProtEnbl",
    "airConEnable",
)


@dataclass(frozen=True)
class MsqAudit:
    signature: str
    n_pages: int
    numbered_page_count: int
    total_page_nodes: int
    constants: set[str]
    pc_variables: set[str]


@dataclass(frozen=True)
class IniAudit:
    signature: str
    constants: set[str]


def parse_msq(msq_path: Path) -> MsqAudit:
    root = ET.parse(msq_path).getroot()
    version_info = root.find("msq:versionInfo", MSQ_NS)
    if version_info is None:
        raise ValueError(f"{msq_path} is missing <versionInfo>")

    signature = version_info.attrib.get("signature", "").strip()
    n_pages = int(version_info.attrib.get("nPages", "0"))
    page_nodes = root.findall("msq:page", MSQ_NS)
    numbered_page_count = sum(1 for page in page_nodes if "number" in page.attrib)
    constants = {
        element.attrib["name"]
        for element in root.findall(".//msq:constant", MSQ_NS)
        if "name" in element.attrib
    }
    pc_variables = {
        element.attrib["name"]
        for element in root.findall(".//msq:pcVariable", MSQ_NS)
        if "name" in element.attrib
    }
    return MsqAudit(
        signature=signature,
        n_pages=n_pages,
        numbered_page_count=numbered_page_count,
        total_page_nodes=len(page_nodes),
        constants=constants,
        pc_variables=pc_variables,
    )


def parse_ini(ini_path: Path) -> IniAudit:
    text = ini_path.read_text(encoding="utf-8", errors="replace")
    signature_match = re.search(r'^\s*signature\s*=\s*"([^"]+)"', text, re.MULTILINE)
    if signature_match is None:
        raise ValueError(f"{ini_path} is missing a MegaTune signature")

    constants_block = _extract_constants_block(text)
    constant_names = {
        match.group(1)
        for match in re.finditer(r"^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=", constants_block, re.MULTILINE)
    }
    return IniAudit(signature=signature_match.group(1).strip(), constants=constant_names)


def _extract_constants_block(text: str) -> str:
    start = text.find("[Constants]")
    if start < 0:
        raise ValueError("INI is missing [Constants]")

    next_section = text.find("\n[", start + 1)
    if next_section < 0:
        return text[start:]
    return text[start:next_section]


def evaluate_compatibility(msq: MsqAudit, ini: IniAudit) -> list[str]:
    failures: list[str] = []
    if msq.signature != ini.signature:
        failures.append(
            f"Signature mismatch: msq={msq.signature!r}, ini={ini.signature!r}"
        )
    if msq.n_pages != msq.numbered_page_count:
        failures.append(
            f"MSQ page count mismatch: versionInfo declares {msq.n_pages}, file contains {msq.numbered_page_count} numbered tune pages"
        )

    missing_in_msq = sorted(name for name in HIGH_RISK_CONSTANTS if name not in msq.constants)
    if missing_in_msq:
        failures.append(
            "MSQ is missing high-risk constants: " + ", ".join(missing_in_msq)
        )

    missing_in_ini = sorted(name for name in HIGH_RISK_CONSTANTS if name not in ini.constants)
    if missing_in_ini:
        failures.append(
            "INI is missing high-risk constants: " + ", ".join(missing_in_ini)
        )

    return failures


def build_summary(msq: MsqAudit, ini: IniAudit) -> str:
    return (
        f"MSQ signature: {msq.signature}\n"
        f"INI signature: {ini.signature}\n"
        f"MSQ numbered tune pages: {msq.numbered_page_count}/{msq.n_pages}\n"
        f"MSQ total page nodes: {msq.total_page_nodes}\n"
        f"MSQ constants: {len(msq.constants)}\n"
        f"MSQ pcVariables: {len(msq.pc_variables)}\n"
        f"High-risk constants audited: {len(HIGH_RISK_CONSTANTS)}"
    )


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Audit stock Speeduino base tune compatibility against the current firmware INI."
    )
    parser.add_argument("--msq", type=Path, default=DEFAULT_MSQ_PATH, help="Path to the stock .msq file")
    parser.add_argument("--ini", type=Path, default=DEFAULT_INI_PATH, help="Path to speeduino.ini")
    args = parser.parse_args(argv)

    try:
        msq = parse_msq(args.msq)
        ini = parse_ini(args.ini)
    except (OSError, ET.ParseError, ValueError) as exc:
        print(f"compatibility audit failed: {exc}", file=sys.stderr)
        return 2

    print(build_summary(msq, ini))
    failures = evaluate_compatibility(msq, ini)
    if failures:
        print("\nCompatibility failures:", file=sys.stderr)
        for failure in failures:
            print(f"- {failure}", file=sys.stderr)
        return 1

    print("\nCompatibility audit passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
