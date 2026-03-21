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
INI_CONSTANT_TYPES = ("scalar", "array", "bits", "string")

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

# These still exist in the current INI / firmware surface but are not exported by the stock base tune.
# Keep this explicit so any new drift has to be acknowledged in code review instead of being silently ignored.
KNOWN_STOCK_BASE_TUNE_GAPS = (
    "knock_limiterDisable",
)

# Known oddities observed in current tune exports.
KNOWN_EXTRA_MSQ_CONSTANTS = (
    "UNALLOCATED_TOP_11",
    "knock_unused1",
)

KNOWN_ARRAY_ALIAS_PARENTS = {
    "firstDataIn0": "firstDataIn",
    "firstDataIn1": "firstDataIn",
    "firstDataIn2": "firstDataIn",
    "firstDataIn3": "firstDataIn",
    "firstDataIn4": "firstDataIn",
    "firstDataIn5": "firstDataIn",
    "firstDataIn6": "firstDataIn",
    "firstDataIn7": "firstDataIn",
    "secondDataIn0": "secondDataIn",
    "secondDataIn1": "secondDataIn",
    "secondDataIn2": "secondDataIn",
    "secondDataIn3": "secondDataIn",
    "secondDataIn4": "secondDataIn",
    "secondDataIn5": "secondDataIn",
    "secondDataIn6": "secondDataIn",
    "secondDataIn7": "secondDataIn",
}

CRITICAL_VALUE_EXPECTATIONS = {
    "launchPin": "Board Default",
    "launchEnable": "No",
    "launchHiLo": "LOW",
    "idleAdvEnabled": "Off",
    "idleAdvAlgorithm": "TPS",
    "idleAdvDelay": "2.0",
    "idleAdvRPM": "2000.0",
    "idleAdvTPS": "4.0",
    "idleTaperTime": "5.0",
    "idleAdvStartDelay": "0.7",
    "idleAdvVss": "255.0",
    "idleAdvBins": "-200.0 -100.0 -50.0 50.0 100.0 200.0",
    "idleAdvValues": "0.0 0.0 0.0 0.0 0.0 0.0",
    "idleUpPin": "Board Default",
    "idleUpPolarity": "Normal",
    "idleUpEnabled": "Off",
    "idleUpAdder": "15.0",
    "idleUpOutputEnabled": "Off",
    "idleUpOutputInv": "No",
    "idleUpOutputPin": "Board Default",
    "vssMode": "Off",
    "vssPin": "Board Default",
    "vssPulsesPerKm": "0.0",
    "vssAuxCh": "Aux0",
    "knock_pin": "A8",
    "knock_trigger": "HIGH",
    "knock_pullup": "Internal pullup",
    "knock_limiterDisable": "No",
    "knock_count": "3",
    "knock_threshold": "4.0",
    "knock_maxMAP": "150.0",
    "knock_maxRPM": "5000.0",
    "knock_maxRetard": "20.0",
    "knock_firstStep": "10.0",
    "knock_stepSize": "1.0",
    "knock_stepTime": "5.0",
    "knock_duration": "2.5",
    "knock_recoveryStepTime": "1.0",
    "knock_recoveryStep": "1.0",
    "rollingProtRPMDelta": "-300.0 -200.0 -100.0 -50.0",
    "rollingProtCutPercent": "50.0 65.0 80.0 95.0",
    "dfcoDelay": "0.1",
    "dfcoRPM": "1500.0",
    "dfcoHyster": "200.0",
    "dfcoMinCLT": "70.0",
    "lnchCtrlVss": "255.0",
    "wmiEnabled": "Off",
    "wmiMode": "Simple",
    "wmiTPS": "0.0",
    "wmiRPM": "2000.0",
    "wmiMAP": "120.0",
    "wmiMAP2": "0.0",
    "wmiIAT": "75.0",
    "wmiOffset": "0.0",
    "wmiAdvEnabled": "Off",
    "wmiEmptyEnabled": "Off",
    "wmiIndicatorEnabled": "Off",
    "oilPressureEnable": "Off",
    "oilPressureProtEnbl": "Off",
    "oilPressurePin": "A15",
    "oilPressureMin": "-18.0",
    "oilPressureMax": "168.0",
    "oilPressureProtMins": "0.0 30.0 40.0 40.0",
    "oilPressureProtRPM": "500.0 1500.0 3500.0 6000.0",
    "fanWhenOff": "Yes",
    "fanWhenCranking": "No",
    "fanInv": "No",
    "fanEnable": "On/Off",
    "fanPin": "Board Default",
    "fanSP": "90.0",
    "fanHyster": "5.0",
    "fanFreq": "0.0",
    "airConEnable": "Off",
    "airConCompPol": "Inverted",
    "airConReqPol": "Inverted",
    "airConTurnsFanOn": "Yes",
    "airConCompPin": "Unused",
    "airConReqPin": "Unused",
    "airConFanPin": "Unused",
    "airConFanPol": "Normal",
    "airConFanEnabled": "Disabled",
    "airConTPSCut": "65.0",
    "airConMinRPM": "500.0",
    "airConMaxRPM": "4700.0",
    "airConClTempCut": "120.0",
    "airConIdleSteps": "0.0",
    "airConTPSCutTime": "5.0",
    "airConCompOnDelay": "0.4",
    "airConAfterStartDelay": "5.0",
    "airConRPMCutTime": "5.0",
    "airConIdleUpRPMAdder": "100.0",
    "airConPwmFanMinDuty": "80.0",
}


@dataclass(frozen=True)
class MsqAudit:
    signature: str
    n_pages: int
    numbered_page_count: int
    total_page_nodes: int
    constants: set[str]
    pc_variables: set[str]
    constant_values: dict[str, str]


@dataclass(frozen=True)
class IniAudit:
    signature: str
    constants: set[str]
    explicit_defaults: dict[str, str]
    explicit_default_variants: dict[str, tuple[str, ...]]
    bit_options: dict[str, tuple[str, ...]]


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
    constant_values = {
        element.attrib["name"]: _normalize_msq_constant_value(element)
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
        constant_values=constant_values,
    )


def parse_ini(ini_path: Path) -> IniAudit:
    text = ini_path.read_text(encoding="utf-8", errors="replace")
    signature_match = re.search(r'^\s*signature\s*=\s*"([^"]+)"', text, re.MULTILINE)
    if signature_match is None:
        raise ValueError(f"{ini_path} is missing a MegaTune signature")

    constants_block = _extract_constants_block(text)
    constant_names = _extract_ini_constant_names(constants_block)
    bit_options = _extract_ini_bit_options(constants_block)
    explicit_default_variants = _extract_ini_default_values(text, bit_options)
    explicit_defaults = {
        name: values[0] for name, values in explicit_default_variants.items() if values
    }
    return IniAudit(
        signature=signature_match.group(1).strip(),
        constants=constant_names,
        explicit_defaults=explicit_defaults,
        explicit_default_variants=explicit_default_variants,
        bit_options=bit_options,
    )


def _extract_constants_block(text: str) -> str:
    start = text.find("[Constants]")
    if start < 0:
        raise ValueError("INI is missing [Constants]")

    next_section = text.find("\n[", start + 1)
    if next_section < 0:
        return text[start:]
    return text[start:next_section]


def _extract_ini_constant_names(constants_block: str) -> set[str]:
    constant_names: set[str] = set()
    for raw_line in constants_block.splitlines():
        line = raw_line.split(";", 1)[0].strip()
        if not line or line.startswith("#") or line.startswith("["):
            continue

        match = re.match(
            r"^([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(" + "|".join(INI_CONSTANT_TYPES) + r")\b",
            line,
        )
        if match is not None:
            constant_names.add(match.group(1))
    return constant_names


def _extract_ini_bit_options(constants_block: str) -> dict[str, tuple[str, ...]]:
    bit_options: dict[str, tuple[str, ...]] = {}
    for raw_line in constants_block.splitlines():
        line = raw_line.split(";", 1)[0].strip()
        if not line or line.startswith("#") or line.startswith("["):
            continue

        match = re.match(r"^([A-Za-z_][A-Za-z0-9_]*)\s*=\s*bits\b", line)
        if match is None:
            continue

        options = tuple(re.findall(r'"([^"]*)"', line))
        if options:
            bit_options[match.group(1)] = options
    return bit_options


def _extract_ini_default_values(
    text: str, bit_options: dict[str, tuple[str, ...]]
) -> dict[str, tuple[str, ...]]:
    defaults: dict[str, list[str]] = {}
    for raw_line in text.splitlines():
        line = raw_line.split(";", 1)[0].strip()
        if not line or line.startswith("#") or not line.startswith("defaultValue"):
            continue

        match = re.match(r"^defaultValue\s*=\s*([A-Za-z_][A-Za-z0-9_]*)\s*,\s*(.+)$", line)
        if match is None:
            continue
        name = match.group(1)
        defaults.setdefault(name, []).append(
            _normalize_ini_default_value(match.group(2), bit_options.get(name))
        )
    return {name: tuple(values) for name, values in defaults.items()}


def _normalize_ini_default_value(raw: str, bit_labels: tuple[str, ...] | None = None) -> str:
    normalized = " ".join(raw.strip().strip('"').split())
    if bit_labels is not None:
        try:
            index = int(normalized, 10)
        except ValueError:
            return normalized
        if 0 <= index < len(bit_labels):
            return bit_labels[index]
    return normalized


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

    missing_roundtrip = sorted(
        name
        for name in (ini.constants - msq.constants)
        if name not in KNOWN_STOCK_BASE_TUNE_GAPS
        if not _is_satisfied_by_parent_array_alias(name, msq.constants)
    )
    if missing_roundtrip:
        failures.append(
            "MSQ is missing round-trippable INI constants: " + ", ".join(missing_roundtrip)
        )

    extra_in_msq = sorted(
        name
        for name in (msq.constants - ini.constants)
        if name not in KNOWN_EXTRA_MSQ_CONSTANTS
    )
    if extra_in_msq:
        failures.append(
            "MSQ contains constants not present in the INI: " + ", ".join(extra_in_msq)
        )

    value_failures = []
    for name, expected in CRITICAL_VALUE_EXPECTATIONS.items():
        actual = msq.constant_values.get(name)
        if actual != expected:
            value_failures.append(f"{name}={actual!r} (expected {expected!r})")
    if value_failures:
        failures.append(
            "MSQ critical values differ from the fork default contract: "
            + ", ".join(value_failures)
        )

    return failures


def build_summary(msq: MsqAudit, ini: IniAudit) -> str:
    return (
        f"MSQ signature: {msq.signature}\n"
        f"INI signature: {ini.signature}\n"
        f"MSQ numbered tune pages: {msq.numbered_page_count}/{msq.n_pages}\n"
        f"MSQ total page nodes: {msq.total_page_nodes}\n"
        f"MSQ constants: {len(msq.constants)}\n"
        f"INI constants: {len(ini.constants)}\n"
        f"MSQ pcVariables: {len(msq.pc_variables)}\n"
        f"High-risk constants audited: {len(HIGH_RISK_CONSTANTS)}\n"
        f"Known stock base tune gaps: {len(KNOWN_STOCK_BASE_TUNE_GAPS)}\n"
        f"Known extra MSQ constants: {len(KNOWN_EXTRA_MSQ_CONSTANTS)}\n"
        f"Known parent-array alias mappings: {len(KNOWN_ARRAY_ALIAS_PARENTS)}\n"
        f"Explicit INI defaultValue entries: {len(ini.explicit_defaults)}\n"
        f"Critical value checks: {len(CRITICAL_VALUE_EXPECTATIONS)}"
    )


def build_explicit_default_mismatch_report(
    msq: MsqAudit, ini: IniAudit, names: list[str] | None = None
) -> list[str]:
    if names is None:
        candidate_names = sorted(
            name for name in ini.explicit_defaults if name in msq.constant_values
        )
    else:
        candidate_names = names

    mismatches = []
    for name in candidate_names:
        explicit_variants = ini.explicit_default_variants.get(name, ())
        actual = msq.constant_values.get(name)
        if (
            not explicit_variants
            or actual is None
            or any(_values_equivalent(explicit_default, actual) for explicit_default in explicit_variants)
        ):
            continue
        mismatches.append(
            f"{name}: tune={actual!r}, ini_defaultValue={_format_default_variants(explicit_variants)!r}"
        )
    return mismatches


def build_contract_default_conflict_report(
    ini: IniAudit, names: list[str] | None = None
) -> list[str]:
    if names is None:
        candidate_names = sorted(
            name
            for name in CRITICAL_VALUE_EXPECTATIONS
            if name in ini.explicit_defaults
        )
    else:
        candidate_names = names

    conflicts = []
    for name in candidate_names:
        expected = CRITICAL_VALUE_EXPECTATIONS.get(name)
        explicit_variants = ini.explicit_default_variants.get(name, ())
        if (
            expected is None
            or not explicit_variants
            or any(_values_equivalent(expected, explicit_default) for explicit_default in explicit_variants)
        ):
            continue
        conflicts.append(
            f"{name}: fork_contract={expected!r}, ini_defaultValue={_format_default_variants(explicit_variants)!r}"
        )
    return conflicts


def _is_satisfied_by_parent_array_alias(name: str, msq_constants: set[str]) -> bool:
    parent_name = KNOWN_ARRAY_ALIAS_PARENTS.get(name)
    return parent_name is not None and parent_name in msq_constants


def _values_equivalent(left: str, right: str) -> bool:
    left_tokens = left.split()
    right_tokens = right.split()
    if len(left_tokens) != len(right_tokens):
        return False

    for left_token, right_token in zip(left_tokens, right_tokens):
        if left_token == right_token:
            continue
        left_number = _try_parse_number(left_token)
        right_number = _try_parse_number(right_token)
        if left_number is None or right_number is None:
            return False
        if left_number != right_number:
            return False
    return True


def _try_parse_number(token: str) -> float | None:
    try:
        return float(token)
    except ValueError:
        return None


def _format_default_variants(values: tuple[str, ...]) -> str:
    if len(values) == 1:
        return values[0]
    return " / ".join(values)


def _normalize_msq_constant_value(element: ET.Element) -> str:
    if list(element):
        raw = " ".join((child.text or "").strip() for child in element)
    else:
        raw = (element.text or "").strip()
    return " ".join(raw.strip().strip('"').split())


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Audit stock Speeduino base tune compatibility against the current firmware INI."
    )
    parser.add_argument("--msq", type=Path, default=DEFAULT_MSQ_PATH, help="Path to the stock .msq file")
    parser.add_argument("--ini", type=Path, default=DEFAULT_INI_PATH, help="Path to speeduino.ini")
    parser.add_argument(
        "--report-explicit-default-mismatches",
        nargs="*",
        metavar="NAME",
        help=(
            "Print tune values that differ from explicit INI defaultValue entries. "
            "If names are omitted, all explicit-default mismatches are reported."
        ),
    )
    parser.add_argument(
        "--report-contract-default-conflicts",
        nargs="*",
        metavar="NAME",
        help=(
            "Print enforced fork-contract values that differ from explicit INI defaultValue "
            "entries. If names are omitted, all such conflicts are reported."
        ),
    )
    args = parser.parse_args(argv)

    try:
        msq = parse_msq(args.msq)
        ini = parse_ini(args.ini)
    except (OSError, ET.ParseError, ValueError) as exc:
        print(f"compatibility audit failed: {exc}", file=sys.stderr)
        return 2

    print(build_summary(msq, ini))
    if args.report_explicit_default_mismatches is not None:
        report = build_explicit_default_mismatch_report(
            msq,
            ini,
            None if not args.report_explicit_default_mismatches else args.report_explicit_default_mismatches,
        )
        print("\nExplicit defaultValue mismatches:")
        if report:
            for item in report:
                print(f"- {item}")
        else:
            print("- None")
    if args.report_contract_default_conflicts is not None:
        report = build_contract_default_conflict_report(
            ini,
            None if not args.report_contract_default_conflicts else args.report_contract_default_conflicts,
        )
        print("\nFork Contract vs INI defaultValue conflicts:")
        if report:
            for item in report:
                print(f"- {item}")
        else:
            print("- None")

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
