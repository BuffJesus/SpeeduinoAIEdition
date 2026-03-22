import unittest

from tools.index_pdf_images import normalize_text
from tools.index_pdf_images import score_ocr_text


class IndexPdfImagesTests(unittest.TestCase):
    def test_normalize_text_collapses_whitespace_and_lowercases(self) -> None:
        self.assertEqual(normalize_text(" Crank \n SENSOR\tCam "), "crank sensor cam")

    def test_score_ocr_text_counts_keywords(self) -> None:
        hits, score = score_ocr_text("Crank cam flywheel tooth teeth trigger sensor cam")
        self.assertEqual(hits["crank"], 1)
        self.assertEqual(hits["cam"], 2)
        self.assertEqual(hits["flywheel"], 1)
        self.assertEqual(hits["tooth"], 1)
        self.assertEqual(hits["teeth"], 1)
        self.assertEqual(hits["trigger"], 1)
        self.assertEqual(hits["sensor"], 1)
        self.assertGreater(score, 0)


if __name__ == "__main__":
    unittest.main()
