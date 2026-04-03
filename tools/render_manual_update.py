from __future__ import annotations

import argparse
import html
from pathlib import Path

from reportlab.lib import colors
from reportlab.lib.enums import TA_LEFT
from reportlab.lib.pagesizes import letter
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import inch
from reportlab.platypus import ListFlowable, ListItem, Paragraph, SimpleDocTemplate, Spacer


def build_styles():
    styles = getSampleStyleSheet()
    styles.add(
        ParagraphStyle(
            name="ManualTitle",
            parent=styles["Title"],
            fontName="Helvetica-Bold",
            fontSize=20,
            leading=24,
            spaceAfter=12,
            alignment=TA_LEFT,
            textColor=colors.HexColor("#1F2937"),
        )
    )
    styles.add(
        ParagraphStyle(
            name="ManualHeading1",
            parent=styles["Heading1"],
            fontName="Helvetica-Bold",
            fontSize=15,
            leading=19,
            spaceBefore=10,
            spaceAfter=6,
            textColor=colors.HexColor("#111827"),
        )
    )
    styles.add(
        ParagraphStyle(
            name="ManualHeading2",
            parent=styles["Heading2"],
            fontName="Helvetica-Bold",
            fontSize=12,
            leading=15,
            spaceBefore=8,
            spaceAfter=4,
            textColor=colors.HexColor("#1F2937"),
        )
    )
    styles.add(
        ParagraphStyle(
            name="ManualBody",
            parent=styles["BodyText"],
            fontName="Helvetica",
            fontSize=10,
            leading=14,
            spaceAfter=6,
        )
    )
    styles.add(
        ParagraphStyle(
            name="ManualBullet",
            parent=styles["BodyText"],
            fontName="Helvetica",
            fontSize=10,
            leading=13,
            leftIndent=12,
            spaceAfter=2,
        )
    )
    return styles


def inline_markup(text: str) -> str:
    escaped = html.escape(text)
    parts = escaped.split("`")
    for i in range(1, len(parts), 2):
        parts[i] = f"<font name='Courier'>{parts[i]}</font>"
    return "".join(parts)


def flush_bullets(story, bullets, styles):
    if not bullets:
        return
    items = [
        ListItem(Paragraph(inline_markup(item), styles["ManualBullet"]))
        for item in bullets
    ]
    story.append(
        ListFlowable(
            items,
            bulletType="bullet",
            start="circle",
            leftIndent=18,
        )
    )
    story.append(Spacer(1, 4))
    bullets.clear()


def render_markdown_like(source: Path, output: Path) -> None:
    styles = build_styles()
    story = []
    bullets: list[str] = []

    for raw_line in source.read_text(encoding="utf-8").splitlines():
        line = raw_line.rstrip()
        stripped = line.strip()

        if not stripped:
            flush_bullets(story, bullets, styles)
            story.append(Spacer(1, 6))
            continue

        if stripped.startswith("# "):
            flush_bullets(story, bullets, styles)
            story.append(Paragraph(inline_markup(stripped[2:]), styles["ManualTitle"]))
            continue

        if stripped.startswith("## "):
            flush_bullets(story, bullets, styles)
            story.append(Paragraph(inline_markup(stripped[3:]), styles["ManualHeading1"]))
            continue

        if stripped.startswith("### "):
            flush_bullets(story, bullets, styles)
            story.append(Paragraph(inline_markup(stripped[4:]), styles["ManualHeading2"]))
            continue

        if stripped.startswith("- "):
            bullets.append(stripped[2:])
            continue

        flush_bullets(story, bullets, styles)
        story.append(Paragraph(inline_markup(stripped), styles["ManualBody"]))

    flush_bullets(story, bullets, styles)

    doc = SimpleDocTemplate(
        str(output),
        pagesize=letter,
        leftMargin=0.7 * inch,
        rightMargin=0.7 * inch,
        topMargin=0.7 * inch,
        bottomMargin=0.7 * inch,
        title="Speeduino Manual Update",
        author="Codex",
    )
    doc.build(story)


def main():
    parser = argparse.ArgumentParser(description="Render the Speeduino manual update PDF.")
    parser.add_argument("source", type=Path, help="Markdown-like source file")
    parser.add_argument("output", type=Path, help="Output PDF path")
    args = parser.parse_args()

    render_markdown_like(args.source, args.output)


if __name__ == "__main__":
    main()
