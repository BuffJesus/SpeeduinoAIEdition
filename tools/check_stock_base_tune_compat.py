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

EXPECTED_CONTRACT_CONFLICT_CLASSIFICATIONS = {
    "knock_pin": "fork_and_stock_both_differ_from_ini_default",
}

CONTEXTUAL_CONTRACT_DEFAULT_EXEMPTIONS = {
    "airConCompPol": (
        "A/C compressor output polarity is wiring-dependent; the INI says Normal is most common, "
        "but stock and fork tunes both ship Inverted and runtime macros flip output state directly "
        "from this field."
    ),
    "airConReqPol": (
        "A/C request input polarity is wiring-dependent; the INI says Normal is most common, "
        "but stock and fork tunes both ship Inverted and runtime request sampling branches "
        "directly on this field."
    ),
    "idleAdvStartDelay": (
        "The manual describes this as a settle delay before idle advance control begins, so it is "
        "a tuning choice rather than a universal default; stock and fork tunes both use 0.7."
    ),
    "idleTaperTime": (
        "The manual and runtime code treat this as the crank-to-run idle taper duration, so it is "
        "a tuning choice rather than a universal default; stock and fork tunes both use 5.0."
    ),
    "vssPulsesPerKm": (
        "With VSS input mode Off, the manual says VSS is unused and runtime code treats "
        "0 as no dividing/disabled for aux-channel speed input."
    ),
}

POLICY_EVIDENCE_NOTES = {
    "airConCompPol": (
        "INI help text says Normal should be used in most cases, but stock and fork tunes both "
        "ship Inverted; compressor output macros in auxiliaries.h flip active state on airConCompPol."
    ),
    "airConReqPol": (
        "INI help text says Normal should be used in most cases, but stock and fork tunes both "
        "ship Inverted; READ_AIRCON_REQUEST() in auxiliaries.cpp branches directly on airConReqPol."
    ),
    "idleAdvStartDelay": (
        "Manual page 86 describes Delay before idle control as the settle time before ignition advance "
        "changes; stock and fork tunes both use 0.7 while INI defaultValue remains 0.2."
    ),
    "idleTaperTime": (
        "idle.cpp uses idleTaperTime as the crank-to-run taper duration for idle load transition; "
        "stock and fork tunes both use 5.0 while INI defaultValue remains 1.0."
    ),
    "knock_pin": (
        "Stock tune uses pin 3, INI defaultValue points to A10, and the fork contract intentionally "
        "pins knock input to A8 for the current knock baseline; init.cpp and sensors.cpp consume "
        "knock_pin directly for digital interrupt and analog translation setup."
    ),
    "vssPulsesPerKm": (
        "Manual pages 97-98 say VSS should be set Off when unused and pulses-per-km is only for active "
        "VSS calibration; sensors.cpp treats 0 as no dividing/disabled for aux-channel VSS mode."
    ),
}

PACKAGED_PROFILE_OVERRIDES = {
    "knock_pin": "A8",
}

PACKAGED_PROFILE_OVERRIDE_NOTES = {
    "knock_pin": (
        "The generic INI default is intentionally A10 to avoid defaulting onto DropBear Teensy 4.1 "
        "crank/cam or MAP/baro pins, but the packaged fork tunes target DropBear hardware and pin "
        "knock input to A8 as their shipped board profile."
    ),
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
            or _is_contextually_exempt_contract_conflict(name)
            or not explicit_variants
            or any(_values_equivalent(expected, explicit_default) for explicit_default in explicit_variants)
        ):
            continue
        conflicts.append(
            f"{name}: fork_contract={expected!r}, ini_defaultValue={_format_default_variants(explicit_variants)!r}"
        )
    return conflicts


def build_contract_conflict_origin_entries(
    ini: IniAudit, stock_msq: MsqAudit, names: list[str] | None = None
) -> list[tuple[str, str, str, str | None, str]]:
    if names is None:
        candidate_names = sorted(
            name
            for name in CRITICAL_VALUE_EXPECTATIONS
            if name in ini.explicit_default_variants
        )
    else:
        candidate_names = names

    entries = []
    for name in candidate_names:
        expected = CRITICAL_VALUE_EXPECTATIONS.get(name)
        explicit_variants = ini.explicit_default_variants.get(name, ())
        stock_value = stock_msq.constant_values.get(name)
        if (
            expected is None
            or _is_contextually_exempt_contract_conflict(name)
            or not explicit_variants
            or any(_values_equivalent(expected, explicit_default) for explicit_default in explicit_variants)
        ):
            continue

        if stock_value is None:
            classification = "stock_missing"
        elif _values_equivalent(stock_value, expected):
            classification = "inherited_from_stock_tune"
        elif any(_values_equivalent(stock_value, explicit_default) for explicit_default in explicit_variants):
            classification = "fork_diverged_from_stock_and_ini_default"
        else:
            classification = "fork_and_stock_both_differ_from_ini_default"

        entries.append(
            (
                name,
                classification,
                expected,
                stock_value,
                _format_default_variants(explicit_variants),
            )
        )
    return entries


def build_contract_conflict_origin_report(
    ini: IniAudit,
    stock_msq: MsqAudit,
    names: list[str] | None = None,
    classification_filter: str | None = None,
) -> list[str]:
    entries = build_contract_conflict_origin_entries(ini, stock_msq, names)
    if classification_filter is not None:
        entries = [entry for entry in entries if entry[1] == classification_filter]
    return [
        f"{name}: {classification}; fork_contract={expected!r}; "
        f"stock_tune={stock_value!r}; ini_defaultValue={ini_default!r}"
        for name, classification, expected, stock_value, ini_default in entries
    ]


def summarize_contract_conflict_origins(
    ini: IniAudit, stock_msq: MsqAudit, names: list[str] | None = None
) -> dict[str, int]:
    counts: dict[str, int] = {}
    for _, classification, _, _, _ in build_contract_conflict_origin_entries(
        ini, stock_msq, names
    ):
        counts[classification] = counts.get(classification, 0) + 1
    return counts


def build_contextual_contract_exemption_report(
    ini: IniAudit, names: list[str] | None = None
) -> list[str]:
    if names is None:
        candidate_names = sorted(
            name
            for name in CONTEXTUAL_CONTRACT_DEFAULT_EXEMPTIONS
            if name in ini.explicit_default_variants
        )
    else:
        candidate_names = names

    exemptions = []
    for name in candidate_names:
        if not _is_contextually_exempt_contract_conflict(name):
            continue
        expected = CRITICAL_VALUE_EXPECTATIONS.get(name)
        explicit_variants = ini.explicit_default_variants.get(name, ())
        if expected is None or not explicit_variants:
            continue
        exemptions.append(
            f"{name}: fork_contract={expected!r}; "
            f"ini_defaultValue={_format_default_variants(explicit_variants)!r}; "
            f"reason={CONTEXTUAL_CONTRACT_DEFAULT_EXEMPTIONS[name]!r}"
        )
    return exemptions


def build_policy_evidence_report(
    ini: IniAudit, stock_msq: MsqAudit, names: list[str] | None = None
) -> list[str]:
    if names is None:
        candidate_names = sorted(
            set(EXPECTED_CONTRACT_CONFLICT_CLASSIFICATIONS)
            | set(CONTEXTUAL_CONTRACT_DEFAULT_EXEMPTIONS)
        )
    else:
        candidate_names = names

    origin_map = {
        entry.split(":", 1)[0]: entry
        for entry in build_contract_conflict_origin_report(ini, stock_msq)
    }
    exemption_map = {
        entry.split(":", 1)[0]: entry
        for entry in build_contextual_contract_exemption_report(ini)
    }

    report = []
    for name in candidate_names:
        note = POLICY_EVIDENCE_NOTES.get(name)
        if note is None:
            continue
        if name in origin_map:
            report.append(f"{origin_map[name]}; evidence={note!r}")
        elif name in exemption_map:
            report.append(f"{exemption_map[name]}; evidence={note!r}")
    return report


def build_packaged_profile_override_report(
    msq: MsqAudit, ini: IniAudit, names: list[str] | None = None
) -> list[str]:
    if names is None:
        candidate_names = sorted(PACKAGED_PROFILE_OVERRIDES)
    else:
        candidate_names = names

    report = []
    for name in candidate_names:
        packaged_value = PACKAGED_PROFILE_OVERRIDES.get(name)
        tune_value = msq.constant_values.get(name)
        explicit_variants = ini.explicit_default_variants.get(name, ())
        note = PACKAGED_PROFILE_OVERRIDE_NOTES.get(name)
        if (
            packaged_value is None
            or tune_value is None
            or note is None
            or not explicit_variants
            or not _values_equivalent(tune_value, packaged_value)
            or any(_values_equivalent(packaged_value, explicit_default) for explicit_default in explicit_variants)
        ):
            continue
        report.append(
            f"{name}: packaged_profile_value={packaged_value!r}; tune={tune_value!r}; "
            f"ini_defaultValue={_format_default_variants(explicit_variants)!r}; "
            f"reason={note!r}"
        )
    return report


def verify_expected_packaged_profile_overrides(
    msq: MsqAudit, ini: IniAudit
) -> list[str]:
    actual_entries = {
        line.split(":", 1)[0] for line in build_packaged_profile_override_report(msq, ini)
    }
    expected_entries = set(PACKAGED_PROFILE_OVERRIDES)
    failures: list[str] = []

    missing = sorted(name for name in expected_entries if name not in actual_entries)
    if missing:
        failures.append(
            "Expected packaged profile overrides are missing: " + ", ".join(missing)
        )

    unexpected = sorted(name for name in actual_entries if name not in expected_entries)
    if unexpected:
        failures.append(
            "Unexpected packaged profile overrides are present: " + ", ".join(unexpected)
        )

    return failures


def verify_expected_contract_conflict_classifications(
    ini: IniAudit, stock_msq: MsqAudit
) -> list[str]:
    actual_entries = {
        name: classification
        for name, classification, _, _, _ in build_contract_conflict_origin_entries(
            ini, stock_msq
        )
    }
    failures: list[str] = []

    missing = sorted(
        name
        for name in EXPECTED_CONTRACT_CONFLICT_CLASSIFICATIONS
        if name not in actual_entries
    )
    if missing:
        failures.append(
            "Expected classified contract conflicts are missing: " + ", ".join(missing)
        )

    unexpected = sorted(
        name
        for name in actual_entries
        if name not in EXPECTED_CONTRACT_CONFLICT_CLASSIFICATIONS
    )
    if unexpected:
        failures.append(
            "Unclassified contract conflicts are present: " + ", ".join(unexpected)
        )

    mismatched = sorted(
        (
            name,
            EXPECTED_CONTRACT_CONFLICT_CLASSIFICATIONS[name],
            actual_entries[name],
        )
        for name in EXPECTED_CONTRACT_CONFLICT_CLASSIFICATIONS
        if name in actual_entries
        if EXPECTED_CONTRACT_CONFLICT_CLASSIFICATIONS[name] != actual_entries[name]
    )
    if mismatched:
        failures.append(
            "Contract conflict classifications differ: "
            + ", ".join(
                f"{name}={actual!r} (expected {expected!r})"
                for name, expected, actual in mismatched
            )
        )

    return failures


def _is_satisfied_by_parent_array_alias(name: str, msq_constants: set[str]) -> bool:
    parent_name = KNOWN_ARRAY_ALIAS_PARENTS.get(name)
    return parent_name is not None and parent_name in msq_constants


def _is_contextually_exempt_contract_conflict(name: str) -> bool:
    return name in CONTEXTUAL_CONTRACT_DEFAULT_EXEMPTIONS


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
    parser.add_argument(
        "--report-contract-conflict-origins",
        nargs="*",
        metavar="NAME",
        help=(
            "Classify each fork-contract-vs-INI-default conflict by comparing it to the stock "
            "base tune. If names are omitted, all conflicts are classified."
        ),
    )
    parser.add_argument(
        "--stock-msq",
        type=Path,
        default=DEFAULT_MSQ_PATH,
        help="Path to the stock .msq file used for conflict-origin classification.",
    )
    parser.add_argument(
        "--contract-origin-filter",
        choices=(
            "inherited_from_stock_tune",
            "fork_diverged_from_stock_and_ini_default",
            "fork_and_stock_both_differ_from_ini_default",
            "stock_missing",
        ),
        help="When reporting contract-conflict origins, restrict output to one classification.",
    )
    parser.add_argument(
        "--verify-expected-contract-conflicts",
        action="store_true",
        help=(
            "Verify that the current contract-vs-default conflict set matches the "
            "repo's expected classified baseline."
        ),
    )
    parser.add_argument(
        "--report-contextual-contract-exemptions",
        nargs="*",
        metavar="NAME",
        help=(
            "Print documented contract-vs-default differences that are contextually exempt "
            "from the active conflict set."
        ),
    )
    parser.add_argument(
        "--report-policy-evidence",
        nargs="*",
        metavar="NAME",
        help=(
            "Print the current classified conflict/exemption policy items together with the "
            "embedded evidence notes used to justify them."
        ),
    )
    parser.add_argument(
        "--report-packaged-profile-overrides",
        nargs="*",
        metavar="NAME",
        help=(
            "Print documented packaged-tune hardware-profile overrides that intentionally differ "
            "from the generic INI defaults."
        ),
    )
    parser.add_argument(
        "--verify-expected-packaged-profile-overrides",
        action="store_true",
        help=(
            "Verify that the current tune matches the repo's expected packaged hardware-profile "
            "override set."
        ),
    )
    args = parser.parse_args(argv)

    try:
        msq = parse_msq(args.msq)
        ini = parse_ini(args.ini)
        stock_msq = (
            parse_msq(args.stock_msq)
            if args.report_contract_conflict_origins is not None
            else None
        )
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
    if args.report_contract_conflict_origins is not None:
        counts = summarize_contract_conflict_origins(
            ini,
            stock_msq,
            None if not args.report_contract_conflict_origins else args.report_contract_conflict_origins,
        )
        report = build_contract_conflict_origin_report(
            ini,
            stock_msq,
            None if not args.report_contract_conflict_origins else args.report_contract_conflict_origins,
            args.contract_origin_filter,
        )
        print("\nFork Contract Conflict Origins:")
        if counts:
            print(
                "- Counts: "
                + ", ".join(f"{name}={count}" for name, count in sorted(counts.items()))
            )
        if report:
            for item in report:
                print(f"- {item}")
        else:
            print("- None")
    if args.verify_expected_contract_conflicts:
        if stock_msq is None:
            stock_msq = parse_msq(args.stock_msq)
        failures = verify_expected_contract_conflict_classifications(ini, stock_msq)
        print("\nExpected Contract Conflict Classification Check:")
        if failures:
            for failure in failures:
                print(f"- {failure}")
        else:
            print("- Current contract conflict classification matches the expected baseline.")
            print(
                "- Expected counts: "
                + ", ".join(
                    f"{name}={count}"
                    for name, count in sorted(
                        summarize_contract_conflict_origins(ini, stock_msq).items()
                    )
                )
            )
        if failures:
            return 1
    if args.report_contextual_contract_exemptions is not None:
        report = build_contextual_contract_exemption_report(
            ini,
            None if not args.report_contextual_contract_exemptions else args.report_contextual_contract_exemptions,
        )
        print("\nContextual Contract Exemptions:")
        if report:
            for item in report:
                print(f"- {item}")
        else:
            print("- None")
    if args.report_policy_evidence is not None:
        if stock_msq is None:
            stock_msq = parse_msq(args.stock_msq)
        report = build_policy_evidence_report(
            ini,
            stock_msq,
            None if not args.report_policy_evidence else args.report_policy_evidence,
        )
        print("\nPolicy Evidence:")
        if report:
            for item in report:
                print(f"- {item}")
        else:
            print("- None")
    if args.report_packaged_profile_overrides is not None:
        report = build_packaged_profile_override_report(
            msq,
            ini,
            None
            if not args.report_packaged_profile_overrides
            else args.report_packaged_profile_overrides,
        )
        print("\nPackaged Profile Overrides:")
        if report:
            for item in report:
                print(f"- {item}")
        else:
            print("- None")
    if args.verify_expected_packaged_profile_overrides:
        failures = verify_expected_packaged_profile_overrides(msq, ini)
        print("\nExpected Packaged Profile Override Check:")
        if failures:
            for failure in failures:
                print(f"- {failure}")
            return 1
        print("- Current packaged profile override set matches the expected baseline.")
        print(
            "- Expected packaged overrides: "
            + ", ".join(sorted(PACKAGED_PROFILE_OVERRIDES))
        )

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
