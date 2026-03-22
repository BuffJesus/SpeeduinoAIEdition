from __future__ import annotations

import tempfile
import textwrap
import unittest
from pathlib import Path

from tools.analyze_rover_mems_tooth_log import find_rover_matches
from tools.analyze_rover_mems_tooth_log import parse_tooth_logger_csv


REAL_CAPTURE_PATTERN1_PREFIX = [
    28284,
    19788,
    16200,
    14604,
    13108,
    12308,
    11672,
    11036,
    21208,
    9844,
    9780,
    9224,
    9772,
    9584,
    9444,
    9400,
    9300,
    9216,
    8928,
    9020,
    8808,
    8456,
    8696,
    8488,
    8120,
    16556,
    7844,
    8040,
    7900,
    7936,
    7800,
    7928,
    7924,
    8192,
    7896,
    8216,
    8016,
    8160,
    7988,
    7992,
    8004,
    7868,
    15836,
]


class AnalyzeRoverMemsToothLogTests(unittest.TestCase):
    def test_parser_skips_headers_marks_and_initial_absolute_timestamp(self) -> None:
        payload = textwrap.dedent(
            """\
            #Firmware: Speeduino 2021.04-dev,
            ToothTime,Time
            uS,uS
            ,
            ,
            495767840,495767840
            28284,495796128
            19788,495815904
            MARK 003,
            16200,495832096
            """
        )
        with tempfile.TemporaryDirectory() as temp_dir:
            csv_path = Path(temp_dir) / "tooth_log.csv"
            csv_path.write_text(payload, encoding="utf-8")
            self.assertEqual([28284, 19788, 16200], parse_tooth_logger_csv(csv_path))

    def test_real_capture_prefix_matches_pattern_one_only(self) -> None:
        matches = find_rover_matches(REAL_CAPTURE_PATTERN1_PREFIX)
        self.assertGreaterEqual(len(matches), 1)
        self.assertEqual([1], sorted({match.pattern_id for match in matches}))
        self.assertEqual("17-17", matches[0].layout)
        self.assertEqual(42, matches[0].tooth_index_0_based)


if __name__ == "__main__":
    unittest.main()
