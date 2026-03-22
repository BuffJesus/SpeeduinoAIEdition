from pathlib import Path
import json
import tempfile
import textwrap
import unittest

from tools.parse_speeduino_composite_csv import parse_composite_csv
from tools.parse_speeduino_composite_csv import parse_logger_fields
from tools.parse_speeduino_composite_csv import summarize_rows


class ParseSpeeduinoCompositeCsvTests(unittest.TestCase):
    def test_parse_logger_fields_2021_style_composite_logger(self) -> None:
        ini = textwrap.dedent(
            """
            loggerDef = toothLogger, "Tooth Logger", tooth
                recordField = toothTime, "ToothTime", 0, 32, 1.0, "uS"
            loggerDef = compositeLogger, "Composite Logger", composite
                recordField = priLevel, "PriLevel", 0, 1, 1.0, "Flag"
                recordField = secLevel, "SecLevel", 1, 1, 1.0, "Flag"
                recordField = trigger, "Trigger", 2, 1, 1.0, "Flag"
                recordField = sync, "Sync", 3, 1, 1.0, "Flag"
                recordField = refTime, "RefTime", 8, 32, 0.001, "ms"
            loggerDef = compositeLogger2, "Composite Logger 2nd Cam", composite
                recordField = priLevel, "PriLevel", 0, 1, 1.0, "Flag"
            """
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "mainController.ini"
            path.write_text(ini, encoding="utf-8")
            self.assertEqual(parse_logger_fields(path), ["priLevel", "secLevel", "trigger", "sync"])

    def test_parse_composite_csv_skips_comments_and_marks(self) -> None:
        content = textwrap.dedent(
            """
            #Firmware: Speeduino 2021.04-dev
            MARK 000
            1.0,0.0,1.0,0.0,10.0,10.0,NaN,10.0
            1.0,1.0,1.0,0.0,12.0,12.0,2.0,12.0
            """
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "sample.csv"
            path.write_text(content, encoding="utf-8")
            rows = parse_composite_csv(path, 4)
            self.assertEqual(len(rows), 2)
            self.assertEqual(rows[0].states, (1, 0, 1, 0))
            self.assertEqual(rows[1].tooth_time_ms, 2.0)

    def test_summarize_rows_counts_toggles_and_long_gaps(self) -> None:
        content = textwrap.dedent(
            """
            1.0,0.0,1.0,0.0,10.0,10.0,NaN,10.0
            1.0,1.0,1.0,0.0,12.0,12.0,2.0,12.0
            0.0,1.0,1.0,1.0,22.5,22.5,10.5,22.5
            """
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "sample.csv"
            path.write_text(content, encoding="utf-8")
            rows = parse_composite_csv(path, 4)
            summary = summarize_rows(rows, ["priLevel", "secLevel", "trigger", "sync"])
            self.assertEqual(summary["rows"], 3)
            self.assertEqual(summary["toggle_counts"]["priLevel"], 1)
            self.assertEqual(summary["toggle_counts"]["secLevel"], 1)
            self.assertEqual(summary["toggle_counts"]["trigger"], 0)
            self.assertEqual(summary["toggle_counts"]["sync"], 1)
            self.assertEqual(summary["long_gap_count_ge_8ms"], 1)

    def test_summary_is_json_serializable(self) -> None:
        content = "1.0,0.0,1.0,0.0,10.0,10.0,NaN,10.0\n"
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "sample.csv"
            path.write_text(content, encoding="utf-8")
            rows = parse_composite_csv(path, 4)
            summary = summarize_rows(rows, ["priLevel", "secLevel", "trigger", "sync"])
            json.dumps(summary)


if __name__ == "__main__":
    unittest.main()
