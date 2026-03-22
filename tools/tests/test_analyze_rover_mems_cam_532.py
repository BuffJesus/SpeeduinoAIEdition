from pathlib import Path
import tempfile
import unittest

from tools.analyze_rover_mems_cam_532 import extract_relevant_tune_constants
from tools.analyze_rover_mems_cam_532 import rover_mems_532_gap_actions
from tools.analyze_rover_mems_cam_532 import summarize_post_sync_secondary_bins
from tools.analyze_rover_mems_cam_532 import summarize_post_sync_secondary_bins_by_level
from tools.analyze_rover_mems_cam_532 import summarize_secondary_transitions
from tools.parse_speeduino_composite_csv import CompositeRow


class AnalyzeRoverMemsCam532Tests(unittest.TestCase):
    def test_gap_actions_match_decoder_cases(self) -> None:
        actions = rover_mems_532_gap_actions()
        self.assertEqual((6, 4, 3), tuple(action.secondary_tooth_count for action in actions))
        self.assertEqual((5, 3, 2), tuple(action.teeth_before_gap for action in actions))
        self.assertEqual((False, True, True), tuple(action.revolution_one for action in actions))

    def test_secondary_transition_summary_counts_primary_toggles(self) -> None:
        rows = [
            CompositeRow((0, 0, 0, 0), 0.0, None, (0.0, 0.0, 0.0, 0.0)),
            CompositeRow((1, 0, 0, 0), 1.0, 1.0, (1.0, 0.0, 0.0, 0.0)),
            CompositeRow((0, 0, 0, 0), 2.0, 1.0, (0.0, 0.0, 0.0, 0.0)),
            CompositeRow((0, 1, 1, 1), 3.0, 1.0, (0.0, 1.0, 1.0, 1.0)),
            CompositeRow((1, 1, 0, 1), 4.0, 1.0, (1.0, 1.0, 0.0, 1.0)),
            CompositeRow((0, 1, 0, 1), 5.0, 1.0, (0.0, 1.0, 0.0, 1.0)),
            CompositeRow((0, 0, 1, 1), 6.0, 1.0, (0.0, 0.0, 1.0, 1.0)),
        ]

        transitions = summarize_secondary_transitions(rows)
        self.assertEqual(2, len(transitions))
        self.assertEqual(2, transitions[0].primary_toggles_since_last_secondary)
        self.assertEqual(2, transitions[1].primary_toggles_since_last_secondary)

    def test_post_sync_bins_ignore_pre_sync_transitions(self) -> None:
        rows = [
            CompositeRow((0, 0, 0, 0), 0.0, None, (0.0, 0.0, 0.0, 0.0)),
            CompositeRow((1, 0, 0, 0), 1.0, 1.0, (1.0, 0.0, 0.0, 0.0)),
            CompositeRow((0, 1, 1, 0), 2.0, 1.0, (0.0, 1.0, 1.0, 0.0)),
            CompositeRow((1, 1, 0, 1), 3.0, 1.0, (1.0, 1.0, 0.0, 1.0)),
            CompositeRow((0, 1, 0, 1), 4.0, 1.0, (0.0, 1.0, 0.0, 1.0)),
            CompositeRow((0, 0, 1, 1), 5.0, 1.0, (0.0, 0.0, 1.0, 1.0)),
        ]

        self.assertEqual({2: 1}, summarize_post_sync_secondary_bins(rows))

    def test_post_sync_bins_split_by_level(self) -> None:
        rows = [
            CompositeRow((0, 0, 0, 0), 0.0, None, (0.0, 0.0, 0.0, 0.0)),
            CompositeRow((1, 0, 0, 1), 1.0, 1.0, (1.0, 0.0, 0.0, 1.0)),
            CompositeRow((0, 1, 1, 1), 2.0, 1.0, (0.0, 1.0, 1.0, 1.0)),
            CompositeRow((1, 1, 0, 1), 3.0, 1.0, (1.0, 1.0, 0.0, 1.0)),
            CompositeRow((0, 1, 0, 1), 4.0, 1.0, (0.0, 1.0, 0.0, 1.0)),
            CompositeRow((0, 0, 1, 1), 5.0, 1.0, (0.0, 0.0, 1.0, 1.0)),
        ]

        self.assertEqual({"rising": {2: 1}, "falling": {2: 1}}, summarize_post_sync_secondary_bins_by_level(rows))

    def test_extract_relevant_tune_constants(self) -> None:
        content = """<?xml version='1.0'?><msq>
<constant name="TrigEdge">"RISING"</constant>
<constant name="TrigEdgeSec">"FALLING"</constant>
<constant name="trigPatternSec">"5-3-2 cam"</constant>
</msq>"""
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "sample.msq"
            path.write_text(content, encoding="iso-8859-1")
            self.assertEqual(
                {
                    "TrigEdge": "RISING",
                    "TrigEdgeSec": "FALLING",
                    "trigPatternSec": "5-3-2 cam",
                },
                extract_relevant_tune_constants(path),
            )


if __name__ == "__main__":
    unittest.main()
