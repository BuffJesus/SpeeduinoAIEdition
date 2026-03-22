import unittest

from tools.derive_rover_mems_windows import build_wheel_bits
from tools.derive_rover_mems_windows import decoder_window_bits
from tools.derive_rover_mems_windows import summarize_layout


class DeriveRoverMemsWindowsTests(unittest.TestCase):
    def test_documented_layouts_expand_to_36_slot_wheels(self) -> None:
        self.assertEqual("110111111111111110111011111111111110", build_wheel_bits("2-14-3-13"))
        self.assertEqual("111111111110111110111111111111011110", build_wheel_bits("11-5-12-4"))

    def test_all_supported_layouts_match_decoder_constants(self) -> None:
        expected = {
            "17-17": "11111111111101111111111111111101",
            "11-5-12-4": "11111101111101111111111110111101",
            "2-14-3-13": "11011011111111111111011101111111",
            "3-14-2-13": "11011101111111111111101101111111",
            "9-7-10-6": "11111101111111011111111110111111",
        }
        self.assertEqual(expected, {layout: decoder_window_bits(layout) for layout in expected})

    def test_helper_identifies_32_of_36_window_for_mems3_layout(self) -> None:
        summary = summarize_layout("2-14-3-13")
        self.assertEqual((3, 18, 22, 36), summary.missing_slots_1_based)
        self.assertEqual((30, 31, 32, 33), summary.omitted_slots_1_based)
        self.assertEqual(27, summary.canonical_start_slot_0_based)
        self.assertEqual(34, summary.actual_teeth_to_match)
        self.assertEqual(36, summary.theoretical_tooth_count_at_match)

    def test_helper_identifies_32_of_36_window_for_mems19_layout(self) -> None:
        summary = summarize_layout("11-5-12-4")
        self.assertEqual((12, 18, 31, 36), summary.missing_slots_1_based)
        self.assertEqual((2, 3, 4, 5), summary.omitted_slots_1_based)
        self.assertEqual(34, summary.canonical_start_slot_0_based)
        self.assertEqual(34, summary.actual_teeth_to_match)
        self.assertEqual(36, summary.theoretical_tooth_count_at_match)


if __name__ == "__main__":
    unittest.main()
