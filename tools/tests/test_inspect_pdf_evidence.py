import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from pypdf import PdfWriter

from tools.inspect_pdf_evidence import find_renderer_path
from tools.inspect_pdf_evidence import find_renderers
from tools.inspect_pdf_evidence import get_pdf_text_char_count
from tools.inspect_pdf_evidence import summarize_pdf_dir


class InspectPdfEvidenceTests(unittest.TestCase):
    def test_find_renderers_reports_missing_entries(self) -> None:
        with patch("tools.inspect_pdf_evidence.shutil.which", return_value=None):
            with patch("tools.inspect_pdf_evidence.Path.home", return_value=Path("C:/Users/test")):
                with patch.object(Path, "glob", return_value=[]):
                    self.assertEqual(
                        find_renderers(),
                        {"magick": None, "pdftoppm": None, "gswin64c": None},
                    )

    def test_find_renderer_path_falls_back_to_winget_pdftoppm(self) -> None:
        fake_root = Path("C:/Users/test/AppData/Local/Microsoft/WinGet/Packages")
        with patch("tools.inspect_pdf_evidence.shutil.which", return_value=None):
            with patch("tools.inspect_pdf_evidence.Path.home", return_value=Path("C:/Users/test")):
                with patch.object(Path, "glob", return_value=[fake_root / "pkg" / "pdftoppm.exe"]):
                    self.assertEqual(
                        find_renderer_path("pdftoppm"),
                        str(fake_root / "pkg" / "pdftoppm.exe"),
                    )

    def test_find_renderer_path_does_not_glob_for_other_tools(self) -> None:
        with patch("tools.inspect_pdf_evidence.shutil.which", return_value=None):
            with patch("tools.inspect_pdf_evidence.Path.home", return_value=Path("C:/Users/test")):
                with patch.object(Path, "glob", return_value=[Path("C:/unexpected/magick.exe")]):
                    self.assertIsNone(find_renderer_path("magick"))

    def test_get_pdf_text_char_count_handles_blank_pdf(self) -> None:
        with tempfile.TemporaryDirectory() as temp_dir:
            pdf_path = Path(temp_dir) / "blank.pdf"
            writer = PdfWriter()
            writer.add_blank_page(width=200, height=200)
            with pdf_path.open("wb") as handle:
                writer.write(handle)

            self.assertEqual(get_pdf_text_char_count(pdf_path), 0)

    def test_summarize_pdf_dir_reports_renderer_and_pdf_metadata(self) -> None:
        with tempfile.TemporaryDirectory() as temp_dir:
            pdf_dir = Path(temp_dir)
            pdf_path = pdf_dir / "blank.pdf"
            writer = PdfWriter()
            writer.add_blank_page(width=200, height=200)
            with pdf_path.open("wb") as handle:
                writer.write(handle)

            with patch(
                "tools.inspect_pdf_evidence.find_renderers",
                return_value={"magick": None, "pdftoppm": "C:/tools/pdftoppm.exe", "gswin64c": None},
            ):
                summary = summarize_pdf_dir(pdf_dir)

            self.assertEqual(summary["pdf_dir"], str(pdf_dir))
            self.assertEqual(summary["renderers"]["pdftoppm"], "C:/tools/pdftoppm.exe")
            self.assertEqual(len(summary["pdfs"]), 1)
            self.assertEqual(summary["pdfs"][0]["pdf_name"], "blank.pdf")
            self.assertEqual(summary["pdfs"][0]["page_count"], 1)
            self.assertEqual(summary["pdfs"][0]["text_chars"], 0)


if __name__ == "__main__":
    unittest.main()
