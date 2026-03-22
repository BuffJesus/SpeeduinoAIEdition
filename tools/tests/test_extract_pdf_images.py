from pathlib import Path
import tempfile
import unittest

from tools.extract_pdf_images import build_manifest
from tools.extract_pdf_images import sanitize_stem


REPO_ROOT = Path(__file__).resolve().parents[2]


class ExtractPdfImagesTests(unittest.TestCase):
    def test_sanitize_stem(self) -> None:
        self.assertEqual(sanitize_stem("mems 2 crank n cam"), "mems_2_crank_n_cam")
        self.assertEqual(sanitize_stem("..weird///name.."), "weird_name")

    def test_build_manifest_on_rover_pdfs(self) -> None:
        pdf_dir = REPO_ROOT / "Resources" / "rover_mems_evidence"
        self.assertTrue((pdf_dir / "mems19_trigger_description.pdf").exists())

        with tempfile.TemporaryDirectory() as tmp:
            manifest = build_manifest(pdf_dir, Path(tmp))

            pdf_entries = {entry["pdf_name"]: entry for entry in manifest["pdfs"]}
            self.assertIn("mems19_trigger_description.pdf", pdf_entries)
            self.assertIn("mems3_crank_and_cam.pdf", pdf_entries)
            self.assertIn("mems_2_crank_n_cam.pdf", pdf_entries)
            self.assertGreater(pdf_entries["mems19_trigger_description.pdf"]["image_count"], 0)
            self.assertGreater(pdf_entries["mems3_crank_and_cam.pdf"]["image_count"], 0)
            self.assertGreater(pdf_entries["mems_2_crank_n_cam.pdf"]["image_count"], 0)


if __name__ == "__main__":
    unittest.main()
