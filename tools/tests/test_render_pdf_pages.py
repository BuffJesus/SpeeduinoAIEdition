import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from pypdf import PdfWriter

from tools.render_pdf_pages import find_pdftoppm
from tools.render_pdf_pages import render_pdf_dir


class RenderPdfPagesTests(unittest.TestCase):
    def test_find_pdftoppm_uses_path_when_available(self) -> None:
        with patch("tools.render_pdf_pages.shutil.which", return_value="C:/tools/pdftoppm.exe"):
            self.assertEqual(find_pdftoppm(), "C:/tools/pdftoppm.exe")

    def test_find_pdftoppm_falls_back_to_winget_package_dir(self) -> None:
        fake_root = Path("C:/Users/test/AppData/Local/Microsoft/WinGet/Packages")
        with patch("tools.render_pdf_pages.shutil.which", return_value=None):
            with patch("tools.render_pdf_pages.Path.home", return_value=Path("C:/Users/test")):
                with patch.object(Path, "glob", return_value=[fake_root / "pkg" / "pdftoppm.exe"]):
                    self.assertEqual(
                        find_pdftoppm(),
                        str(fake_root / "pkg" / "pdftoppm.exe"),
                    )

    def test_render_pdf_dir_uses_renderer_and_collects_output(self) -> None:
        with tempfile.TemporaryDirectory() as temp_dir:
            pdf_dir = Path(temp_dir) / "pdfs"
            output_dir = Path(temp_dir) / "out"
            pdf_dir.mkdir()
            pdf_path = pdf_dir / "sample.pdf"
            writer = PdfWriter()
            writer.add_blank_page(width=200, height=200)
            with pdf_path.open("wb") as handle:
                writer.write(handle)

            def fake_render(pdf_path: Path, output_dir: Path, pdftoppm_path: str, dpi: int):
                page = output_dir / f"{pdf_path.stem}-1.png"
                page.write_bytes(b"png")
                return [page]

            with patch("tools.render_pdf_pages.render_pdf", side_effect=fake_render):
                manifest = render_pdf_dir(pdf_dir, output_dir, 150, "C:/tools/pdftoppm.exe")

            self.assertEqual(manifest["pdftoppm_path"], "C:/tools/pdftoppm.exe")
            self.assertEqual(manifest["dpi"], 150)
            self.assertEqual(manifest["pdfs"][0]["pdf_name"], "sample.pdf")
            self.assertEqual(manifest["pdfs"][0]["page_count"], 1)
            self.assertEqual(manifest["pdfs"][0]["pages"], ["sample-1.png"])


if __name__ == "__main__":
    unittest.main()
