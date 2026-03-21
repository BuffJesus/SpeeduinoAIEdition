from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.check_stock_base_tune_compat import (
    CRITICAL_VALUE_EXPECTATIONS,
    HIGH_RISK_CONSTANTS,
    KNOWN_EXTRA_MSQ_CONSTANTS,
    KNOWN_STOCK_BASE_TUNE_GAPS,
    build_contract_conflict_origin_report,
    build_contract_default_conflict_report,
    build_explicit_default_mismatch_report,
    evaluate_compatibility,
    parse_ini,
    parse_msq,
)


class StockBaseTuneCompatTests(unittest.TestCase):
    def test_ini_parser_only_collects_real_constant_definitions(self) -> None:
        with tempfile.TemporaryDirectory() as temp_dir_name:
            temp_dir = Path(temp_dir_name)
            ini_path = temp_dir / "test.ini"
            ini_path.write_text(
                """signature = "speeduino 202501"

[Constants]
page = 1
pageSize = 128
blockingFactor = 121
knock_mode = bits, U08, 92, [0:1], "Off", "Digital", "Analog", "INVALID"
fuelLoadMax = scalar, U08, 0, "", 1, 0, 0, 511, 0
veTable = array, U08, 0, [16x16], "%", 1.0, 0.0, 0.0, 255.0, 0
testName = string, ASCII, 20
defaultValue = knock_mode, 0
requiresPowerCycle = knock_mode
""",
                encoding="utf-8",
            )

            ini = parse_ini(ini_path)
            self.assertEqual(
                {"knock_mode", "fuelLoadMax", "veTable", "testName"},
                ini.constants,
            )
            self.assertEqual(
                {"knock_mode": "Off"},
                ini.explicit_defaults,
            )
            self.assertEqual(
                {"knock_mode": ("Off",)},
                ini.explicit_default_variants,
            )
            self.assertEqual(
                {"knock_mode": ("Off", "Digital", "Analog", "INVALID")},
                ini.bit_options,
            )

    def test_explicit_default_mismatch_report_uses_ini_defaults(self) -> None:
        with tempfile.TemporaryDirectory() as temp_dir_name:
            temp_dir = Path(temp_dir_name)
            msq_path = temp_dir / "test.msq"
            ini_path = temp_dir / "test.ini"

            msq_path.write_text(
                """<?xml version="1.0" encoding="ISO-8859-1"?>
<msq xmlns="http://www.msefi.com/:msq">
  <versionInfo fileFormat="5.0" firmwareInfo="X" nPages="1" signature="speeduino 202501"/>
  <page>
    <constant name="idleTaperTime">5.0</constant>
    <constant name="idleAdvStartDelay">0.7</constant>
    <constant name="launchEnable">"No"</constant>
  </page>
</msq>
""",
                encoding="utf-8",
            )
            ini_path.write_text(
                """signature = "speeduino 202501"

[Constants]
idleTaperTime = scalar, U08, 99, "S", 0.1, 0.0, 0.0, 25.5, 1
idleAdvStartDelay = scalar, U08, 155, "S", 0.1, 0.0, 0.0, 25.5, 1
launchEnable = bits, U08, 48, [6:6], "No", "Yes"

defaultValue = idleTaperTime, 1.0
defaultValue = idleAdvStartDelay, 0.2
defaultValue = launchEnable, 0
""",
                encoding="utf-8",
            )

            report = build_explicit_default_mismatch_report(
                parse_msq(msq_path),
                parse_ini(ini_path),
            )
            self.assertEqual(
                [
                    "idleAdvStartDelay: tune='0.7', ini_defaultValue='0.2'",
                    "idleTaperTime: tune='5.0', ini_defaultValue='1.0'",
                ],
                sorted(report),
            )

    def test_reports_normalize_equivalent_numeric_values(self) -> None:
        with tempfile.TemporaryDirectory() as temp_dir_name:
            temp_dir = Path(temp_dir_name)
            msq_path = temp_dir / "test.msq"
            ini_path = temp_dir / "test.ini"

            msq_path.write_text(
                """<?xml version="1.0" encoding="ISO-8859-1"?>
<msq xmlns="http://www.msefi.com/:msq">
  <versionInfo fileFormat="5.0" firmwareInfo="X" nPages="1" signature="speeduino 202501"/>
  <page>
    <constant name="dfcoRPM">1500.0</constant>
    <constant name="rollingProtRPMDelta"><value>-300.0</value><value>-200.0</value><value>-100.0</value><value>-50.0</value></constant>
  </page>
</msq>
""",
                encoding="utf-8",
            )
            ini_path.write_text(
                """signature = "speeduino 202501"

[Constants]
dfcoRPM = scalar, U08, 60, "RPM", 10.0, 0.0, 100, 2550, 0
rollingProtRPMDelta = array, S16, 0, [4], "RPM", 1.0, 0.0, -500, 0, 0

defaultValue = dfcoRPM, 1500
defaultValue = rollingProtRPMDelta, -300 -200 -100 -50
""",
                encoding="utf-8",
            )

            msq = parse_msq(msq_path)
            ini = parse_ini(ini_path)
            self.assertEqual([], build_explicit_default_mismatch_report(msq, ini))
            self.assertEqual([], build_contract_default_conflict_report(ini, ["dfcoRPM", "rollingProtRPMDelta"]))

    def test_reports_accept_any_explicit_default_variant(self) -> None:
        with tempfile.TemporaryDirectory() as temp_dir_name:
            temp_dir = Path(temp_dir_name)
            msq_path = temp_dir / "test.msq"
            ini_path = temp_dir / "test.ini"

            msq_path.write_text(
                """<?xml version="1.0" encoding="ISO-8859-1"?>
<msq xmlns="http://www.msefi.com/:msq">
  <versionInfo fileFormat="5.0" firmwareInfo="X" nPages="1" signature="speeduino 202501"/>
  <page>
    <constant name="dfcoMinCLT">70.0</constant>
  </page>
</msq>
""",
                encoding="utf-8",
            )
            ini_path.write_text(
                """signature = "speeduino 202501"

[Constants]
dfcoMinCLT = scalar, U08, 101, "C", 1.0, -40, -40, 215, 0

defaultValue = dfcoMinCLT, 70
defaultValue = dfcoMinCLT, 158
""",
                encoding="utf-8",
            )

            msq = parse_msq(msq_path)
            ini = parse_ini(ini_path)
            self.assertEqual(("70", "158"), ini.explicit_default_variants["dfcoMinCLT"])
            self.assertEqual([], build_explicit_default_mismatch_report(msq, ini, ["dfcoMinCLT"]))
            self.assertEqual([], build_contract_default_conflict_report(ini, ["dfcoMinCLT"]))

    def test_contract_default_conflict_report_finds_real_semantic_drift(self) -> None:
        with tempfile.TemporaryDirectory() as temp_dir_name:
            temp_dir = Path(temp_dir_name)
            ini_path = temp_dir / "test.ini"
            ini_path.write_text(
                """signature = "speeduino 202501"

[Constants]
idleAdvStartDelay = scalar, U08, 155, "S", 0.1, 0.0, 0.0, 25.5, 1
airConCompPol = bits, U08, 83, [1:1], "Normal", "Inverted"
dfcoMinCLT = scalar, U08, 101, "C", 1.0, -40, -40, 215, 0

defaultValue = idleAdvStartDelay, 0.2
defaultValue = airConCompPol, 0
defaultValue = dfcoMinCLT, 70
defaultValue = dfcoMinCLT, 158
""",
                encoding="utf-8",
            )

            self.assertEqual(
                [
                    "airConCompPol: fork_contract='Inverted', ini_defaultValue='Normal'",
                    "idleAdvStartDelay: fork_contract='0.7', ini_defaultValue='0.2'",
                ],
                sorted(build_contract_default_conflict_report(parse_ini(ini_path))),
            )

    def test_contract_conflict_origin_report_distinguishes_stock_inheritance(self) -> None:
        with tempfile.TemporaryDirectory() as temp_dir_name:
            temp_dir = Path(temp_dir_name)
            stock_msq_path = temp_dir / "stock.msq"
            ini_path = temp_dir / "test.ini"

            stock_msq_path.write_text(
                """<?xml version="1.0" encoding="ISO-8859-1"?>
<msq xmlns="http://www.msefi.com/:msq">
  <versionInfo fileFormat="5.0" firmwareInfo="X" nPages="1" signature="speeduino 202501"/>
  <page>
    <constant name="idleAdvStartDelay">0.7</constant>
    <constant name="knock_pin">3</constant>
  </page>
</msq>
""",
                encoding="utf-8",
            )
            ini_path.write_text(
                """signature = "speeduino 202501"

[Constants]
idleAdvStartDelay = scalar, U08, 155, "S", 0.1, 0.0, 0.0, 25.5, 1
knock_pin = bits, U08, 92, [2:7], "INVALID", "INVALID", "2", "3", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "18", "19", "20", "21", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "A10", "A11", "A12", "A13", "A14", "A15", "INVALID"

defaultValue = idleAdvStartDelay, 0.2
defaultValue = knock_pin, 57
""",
                encoding="utf-8",
            )

            self.assertEqual(
                [
                    "idleAdvStartDelay: inherited_from_stock_tune; fork_contract='0.7'; stock_tune='0.7'; ini_defaultValue='0.2'",
                    "knock_pin: fork_and_stock_both_differ_from_ini_default; fork_contract='A8'; stock_tune='3'; ini_defaultValue='A10'",
                ],
                sorted(
                    build_contract_conflict_origin_report(
                        parse_ini(ini_path),
                        parse_msq(stock_msq_path),
                        ["idleAdvStartDelay", "knock_pin"],
                    )
                ),
            )

    def test_real_stock_tune_flags_known_missing_knock_limiter_disable(self) -> None:
        repo_root = Path(__file__).resolve().parents[2]
        msq = parse_msq(repo_root / "Resources" / "Speeduino base tune.msq")
        ini = parse_ini(repo_root / "speeduino.ini")

        failures = evaluate_compatibility(msq, ini)
        self.assertTrue(
            any(
                failure == "MSQ is missing high-risk constants: knock_limiterDisable"
                for failure in failures
            )
        )
        self.assertTrue(
            any(
                failure.startswith("MSQ critical values differ from the fork default contract:")
                for failure in failures
            )
        )
        self.assertEqual("speeduino 202501", msq.signature)
        self.assertEqual(msq.n_pages, msq.numbered_page_count)
        self.assertEqual(16, msq.total_page_nodes)
        self.assertIn("knock_limiterDisable", KNOWN_STOCK_BASE_TUNE_GAPS)
        self.assertTrue(KNOWN_EXTRA_MSQ_CONSTANTS)

    def test_fork_base_tune_matches_current_ini(self) -> None:
        repo_root = Path(__file__).resolve().parents[2]
        msq = parse_msq(repo_root / "Resources" / "Speeduino AI base tune.msq")
        ini = parse_ini(repo_root / "speeduino.ini")

        self.assertEqual([], evaluate_compatibility(msq, ini))
        self.assertEqual("speeduino 202501", msq.signature)
        self.assertEqual(msq.n_pages, msq.numbered_page_count)
        self.assertEqual(16, msq.total_page_nodes)
        for name, expected in CRITICAL_VALUE_EXPECTATIONS.items():
            self.assertEqual(expected, msq.constant_values.get(name), name)

    def test_release_base_tune_matches_current_release_ini(self) -> None:
        repo_root = Path(__file__).resolve().parents[2]
        msq = parse_msq(
            repo_root / "release" / "speeduino-dropbear-v2.0.1-base-tune.msq"
        )
        ini = parse_ini(repo_root / "release" / "speeduino-dropbear-v2.0.1.ini")

        self.assertEqual([], evaluate_compatibility(msq, ini))
        self.assertEqual("speeduino 202501", msq.signature)
        self.assertEqual(msq.n_pages, msq.numbered_page_count)
        self.assertEqual(16, msq.total_page_nodes)
        for name, expected in CRITICAL_VALUE_EXPECTATIONS.items():
            self.assertEqual(expected, msq.constant_values.get(name), name)

    def test_signature_mismatch_is_reported(self) -> None:
        with tempfile.TemporaryDirectory() as temp_dir_name:
            temp_dir = Path(temp_dir_name)
            msq_path = temp_dir / "test.msq"
            ini_path = temp_dir / "test.ini"

            msq_path.write_text(
                """<?xml version="1.0" encoding="ISO-8859-1"?>
<msq xmlns="http://www.msefi.com/:msq">
  <versionInfo fileFormat="5.0" firmwareInfo="X" nPages="1" signature="speeduino 202501"/>
  <page>
    <constant name="knock_mode">"Off"</constant>
    <constant name="launchEnable">"No"</constant>
    <constant name="launchPin">"Board Default"</constant>
    <constant name="launchHiLo">"LOW"</constant>
    <constant name="lnchCtrlVss">255</constant>
    <constant name="dfcoEnabled">"Off"</constant>
    <constant name="dfcoRPM">1500</constant>
    <constant name="dfcoHyster">150</constant>
    <constant name="dfcoTPSThresh">1</constant>
    <constant name="dfcoDelay">1.0</constant>
    <constant name="dfcoTaperEnable">"Off"</constant>
    <constant name="dfcoTaperTime">0.0</constant>
    <constant name="dfcoTaperFuel">0</constant>
    <constant name="dfcoTaperAdvance">0</constant>
    <constant name="vssMode">"Off"</constant>
    <constant name="vssPin">"Board Default"</constant>
    <constant name="vssPulsesPerKm">0</constant>
    <constant name="vssAuxCh">"Aux0"</constant>
    <constant name="wmiEnabled">"Off"</constant>
    <constant name="wmiMode">"Simple"</constant>
    <constant name="wmiTable">0</constant>
    <constant name="rollingProtRPMDelta">0</constant>
    <constant name="rollingProtCutPercent">0</constant>
    <constant name="oilPressureEnable">"Off"</constant>
    <constant name="oilPressureProtEnbl">"Off"</constant>
    <constant name="airConEnable">"Off"</constant>
    <constant name="knock_pin">0</constant>
    <constant name="knock_trigger">0</constant>
    <constant name="knock_pullup">0</constant>
    <constant name="knock_limiterDisable">0</constant>
    <constant name="knock_count">0</constant>
    <constant name="knock_threshold">0</constant>
    <constant name="knock_maxMAP">0</constant>
    <constant name="knock_maxRPM">0</constant>
    <constant name="knock_maxRetard">0</constant>
    <constant name="knock_firstStep">0</constant>
    <constant name="knock_stepSize">0</constant>
    <constant name="knock_stepTime">0</constant>
    <constant name="knock_duration">0</constant>
    <constant name="knock_recoveryStepTime">0</constant>
    <constant name="knock_recoveryStep">0</constant>
  </page>
</msq>
""",
                encoding="utf-8",
            )
            ini_lines = ['signature = "forked signature"', "", "[Constants]"]
            ini_lines.extend(f"{name} = scalar, U08, 0" for name in HIGH_RISK_CONSTANTS)
            ini_path.write_text("\n".join(ini_lines), encoding="utf-8")

            failures = evaluate_compatibility(parse_msq(msq_path), parse_ini(ini_path))
            self.assertTrue(any("Signature mismatch" in failure for failure in failures))

    def test_missing_high_risk_constant_is_reported(self) -> None:
        with tempfile.TemporaryDirectory() as temp_dir_name:
            temp_dir = Path(temp_dir_name)
            msq_path = temp_dir / "test.msq"
            ini_path = temp_dir / "test.ini"

            msq_constants = [name for name in HIGH_RISK_CONSTANTS if name != "knock_mode"]
            msq_constant_xml = "\n".join(
                f'    <constant name="{name}">0</constant>' for name in msq_constants
            )
            msq_path.write_text(
                f"""<?xml version="1.0" encoding="ISO-8859-1"?>
<msq xmlns="http://www.msefi.com/:msq">
  <versionInfo fileFormat="5.0" firmwareInfo="X" nPages="1" signature="speeduino 202501"/>
  <page>
{msq_constant_xml}
  </page>
</msq>
""",
                encoding="utf-8",
            )
            ini_lines = ['signature = "speeduino 202501"', "", "[Constants]"]
            ini_lines.extend(f"{name} = scalar, U08, 0" for name in HIGH_RISK_CONSTANTS)
            ini_path.write_text("\n".join(ini_lines), encoding="utf-8")

            failures = evaluate_compatibility(parse_msq(msq_path), parse_ini(ini_path))
            self.assertTrue(any("MSQ is missing high-risk constants" in failure for failure in failures))


if __name__ == "__main__":
    unittest.main()
