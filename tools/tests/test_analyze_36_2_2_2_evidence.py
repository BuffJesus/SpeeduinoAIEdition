from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.analyze_36_2_2_2_evidence import (
    analyze_path,
    detect_csv_kind,
    extract_trigger_settings,
)


class Analyze36222EvidenceTests(unittest.TestCase):
    def test_extract_trigger_settings_from_msq(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            msq_path = Path(tmpdir) / "test.msq"
            msq_path.write_text(
                """<?xml version="1.0"?>
<msq>
  <constants>
    <constant name="TrigPattern">"36-2-2-2"</constant>
    <constant name="TrigEdge">"FALLING"</constant>
    <constant name="TrigSpeed">"Crank Speed"</constant>
    <constant name="nCylinders">"4"</constant>
  </constants>
</msq>
""",
                encoding="utf-8",
            )

            self.assertEqual(
                extract_trigger_settings(msq_path),
                {
                    "TrigPattern": "36-2-2-2",
                    "TrigEdge": "FALLING",
                    "TrigSpeed": "Crank Speed",
                    "nCylinders": "4",
                },
            )

    def test_detect_and_summarize_composite_csv(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            csv_path = Path(tmpdir) / "composite.csv"
            csv_path.write_text(
                """#Firmware: Speeduino 2023.05
PriLevel,SecLevel,ThirdLevel,Trigger,Sync,Cycle,RefTime,ToothTime,Time
Flag,Flag,Flag,Flag,Flag,Flag,ms,ms,ms
1,0,0,0,0,0,1,3,1
0,0,0,0,0,0,2,3.1,2
1,0,0,1,0,0,3,6.2,3
""",
                encoding="utf-8",
            )

            self.assertEqual(detect_csv_kind(csv_path), "composite")
            summary = analyze_path(csv_path)
            self.assertEqual(summary["kind"], "composite")
            self.assertEqual(summary["rows"], 3)
            self.assertEqual(summary["toggle_counts"]["PriLevel"], 2)
            self.assertEqual(summary["toggle_counts"]["Trigger"], 1)
            self.assertEqual(summary["positive_tooth_rows"], 3)
            self.assertEqual(summary["primary_falling_count"], 1)
            self.assertEqual(summary["primary_rising_count"], 1)

    def test_detect_and_summarize_tooth_log_csv(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            csv_path = Path(tmpdir) / "tooth.csv"
            csv_path.write_text(
                """#Firmware: Speeduino 2025.01.2
ToothTime,Time
uS,uS
0,0
20000,20000
32000,52000
""",
                encoding="utf-8",
            )

            self.assertEqual(detect_csv_kind(csv_path), "tooth_log")
            summary = analyze_path(csv_path)
            self.assertEqual(summary["kind"], "tooth_log")
            self.assertEqual(summary["rows"], 3)
            self.assertEqual(summary["positive_rows"], 2)
            self.assertEqual(summary["min_us"], 20000.0)
            self.assertEqual(summary["max_us"], 32000.0)


if __name__ == "__main__":
    unittest.main()
