from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.check_stock_base_tune_compat import (
    HIGH_RISK_CONSTANTS,
    evaluate_compatibility,
    parse_ini,
    parse_msq,
)


class StockBaseTuneCompatTests(unittest.TestCase):
    def test_real_stock_tune_flags_known_missing_knock_limiter_disable(self) -> None:
        repo_root = Path(__file__).resolve().parents[2]
        msq = parse_msq(repo_root / "Resources" / "Speeduino base tune.msq")
        ini = parse_ini(repo_root / "speeduino.ini")

        failures = evaluate_compatibility(msq, ini)
        self.assertEqual(
            ["MSQ is missing high-risk constants: knock_limiterDisable"],
            failures,
        )
        self.assertEqual("speeduino 202501", msq.signature)
        self.assertEqual(msq.n_pages, msq.numbered_page_count)
        self.assertEqual(16, msq.total_page_nodes)

    def test_fork_base_tune_matches_current_ini(self) -> None:
        repo_root = Path(__file__).resolve().parents[2]
        msq = parse_msq(repo_root / "Resources" / "Speeduino AI base tune.msq")
        ini = parse_ini(repo_root / "speeduino.ini")

        self.assertEqual([], evaluate_compatibility(msq, ini))
        self.assertEqual("speeduino 202501", msq.signature)
        self.assertEqual(msq.n_pages, msq.numbered_page_count)
        self.assertEqual(16, msq.total_page_nodes)

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
