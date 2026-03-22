from __future__ import annotations

import tempfile
import textwrap
import unittest
from pathlib import Path

from tools.analyze_36_2_1 import analyze_scope_csv
from tools.analyze_36_2_1 import extract_rising_edges
from tools.analyze_36_2_1 import parse_scope_csv


class Analyze3621Tests(unittest.TestCase):
    def test_parse_scope_csv_accepts_spaced_channel_headers(self) -> None:
        payload = textwrap.dedent(
            """\
            Time[s], Channel 0, Channel 1
            0.000000,0,1
            0.001000,1,1
            0.002000,0,1
            0.003000,1,1
            """
        )
        with tempfile.TemporaryDirectory() as temp_dir:
            csv_path = Path(temp_dir) / "scope.csv"
            csv_path.write_text(payload, encoding="utf-8")
            samples = parse_scope_csv(csv_path, channel_index=0)
            self.assertEqual([(0.0, 0), (0.001, 1), (0.002, 0), (0.003, 1)], samples)
            self.assertEqual([0.001, 0.003], extract_rising_edges(samples))

    def test_real_4b11_running_matches_steady_16_and_17_tooth_pattern(self) -> None:
        analysis = analyze_scope_csv(
            Path("Resources/rusefi-2026-03-17/unit_tests/tests/trigger/resources/4b11-running.csv")
        )
        dominant = analysis["summary"]["dominant_pattern"]
        self.assertEqual(
            [
                {"teeth_since_previous_gap_tooth": 16, "gap_kind": "double", "count": 30},
                {"teeth_since_previous_gap_tooth": 17, "gap_kind": "single", "count": 30},
            ],
            dominant,
        )
        self.assertEqual([], analysis["summary"]["anomalies"])

    def test_real_4b11_doubled_edge_surfaces_single_anomaly(self) -> None:
        analysis = analyze_scope_csv(
            Path("Resources/rusefi-2026-03-17/unit_tests/tests/trigger/resources/4b11-running-doubled-edge.csv")
        )
        dominant = analysis["summary"]["dominant_pattern"]
        self.assertEqual(
            [
                {"teeth_since_previous_gap_tooth": 16, "gap_kind": "double", "count": 29},
                {"teeth_since_previous_gap_tooth": 17, "gap_kind": "double", "count": 1},
                {"teeth_since_previous_gap_tooth": 17, "gap_kind": "single", "count": 30},
            ],
            dominant,
        )
        self.assertEqual(1, len(analysis["summary"]["anomalies"]))
        anomaly = analysis["summary"]["anomalies"][0]
        self.assertEqual("double", anomaly["gap_kind"])
        self.assertEqual(17, anomaly["teeth_since_previous_gap_tooth"])


if __name__ == "__main__":
    unittest.main()
