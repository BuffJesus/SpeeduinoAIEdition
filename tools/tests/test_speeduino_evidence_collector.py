from __future__ import annotations

import importlib.util
import sys
import unittest
from pathlib import Path
from unittest.mock import patch


MODULE_PATH = (
    Path(__file__).resolve().parents[2]
    / "Resources"
    / "speeduino_evidence_collector_stable.py"
)
SPEC = importlib.util.spec_from_file_location(
    "speeduino_evidence_collector_stable", MODULE_PATH
)
if SPEC is None or SPEC.loader is None:
    raise RuntimeError(f"Unable to load module from {MODULE_PATH}")
collector = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = collector
SPEC.loader.exec_module(collector)


class SpeeduinoEvidenceCollectorTests(unittest.TestCase):
    class FakeResponse:
        def __init__(self, text: str) -> None:
            self.text = text

        def raise_for_status(self) -> None:
            return None

    def test_resolve_output_path_uses_script_directory_for_relative_paths(self) -> None:
        resolved = collector.resolve_output_path("report.md")
        self.assertEqual(MODULE_PATH.parent / "report.md", resolved)

    def test_normalize_thread_title_strips_page_suffix(self) -> None:
        self.assertEqual(
            "Messing with Subaru 6/7 trigger",
            collector.normalize_thread_title("Messing with Subaru 6/7 trigger - Page 5"),
        )

    def test_canonicalize_url_drops_forum_id_noise(self) -> None:
        self.assertEqual(
            "https://speeduino.com/forum/viewtopic.php?t=5544&start=280",
            collector.canonicalize_url(
                "https://speeduino.com/forum/viewtopic.php?f=19&t=5544&start=280"
            ),
        )

    def test_build_filtered_roadmap_query_specs_restricts_selected_area(self) -> None:
        specs = collector.build_filtered_roadmap_query_specs(
            ["Knock and pin/default policy"]
        )
        self.assertTrue(specs)
        self.assertEqual(
            {"Knock and pin/default policy"},
            {label for label, _query, _focus_terms in specs},
        )

    def test_build_filtered_decoder_query_specs_restricts_selected_decoder(self) -> None:
        specs = collector.build_filtered_decoder_query_specs(["Honda J32"])
        self.assertTrue(specs)
        self.assertEqual(
            {"Honda J32"},
            {label for label, _query, _focus_terms in specs},
        )

    def test_collect_threads_for_mode_dedupes_thread_fetches_and_merges_queries(self) -> None:
        query_specs = [
            ("Knock and pin/default policy", "knock_pin", ["knock_pin"]),
            ("Knock and pin/default policy", "A8 A10 knock", ["A8", "A10"]),
        ]
        hits = [
            collector.SearchHit(
                query="knock_pin",
                title="Thread one",
                url="https://speeduino.com/forum/viewtopic.php?f=1&t=10",
                label="Knock and pin/default policy",
            ),
            collector.SearchHit(
                query="A8 A10 knock",
                title="Thread one duplicate",
                url="https://speeduino.com/forum/viewtopic.php?f=1&t=10&start=0",
                label="Knock and pin/default policy",
            ),
        ]
        thread = collector.ThreadData(
            title="Thread one",
            url="https://speeduino.com/forum/viewtopic.php?f=1&t=10",
            breadcrumb=[],
            posts=[],
        )

        with patch.object(collector, "run_searches", return_value=hits), patch.object(
            collector, "collect_thread", return_value=thread
        ) as collect_thread:
            result = collector.collect_threads_for_mode(
                session=None,
                query_specs=query_specs,
                search_engine="duckduckgo",
                delay=0.0,
                per_query_limit=5,
                limit_results=0,
            )

        collect_thread.assert_called_once()
        args = collect_thread.call_args.args
        self.assertEqual("https://speeduino.com/forum/viewtopic.php?t=10", args[1])
        self.assertEqual(["A10", "A8", "knock_pin"], args[3])
        self.assertEqual(["A8 A10 knock", "knock_pin"], args[4])
        self.assertEqual(["Knock and pin/default policy"], args[5])
        self.assertEqual(
            [("Knock and pin/default policy", thread)],
            result,
        )

    def test_run_searches_falls_back_to_secondary_engine(self) -> None:
        query_specs = [("Knock and pin/default policy", "knock_pin", ["knock_pin"])]
        fallback_hit = collector.SearchHit(
            query="knock_pin",
            title="Thread one",
            url="https://speeduino.com/forum/viewtopic.php?t=10",
            label="Knock and pin/default policy",
        )

        with patch.object(
            collector,
            "search_duckduckgo_site",
            side_effect=RuntimeError("ddg blocked"),
        ) as ddg, patch.object(
            collector,
            "search_forum_site",
            return_value=[fallback_hit],
        ) as forum, patch.object(
            collector,
            "search_bing_site",
            return_value=[fallback_hit],
        ) as bing:
            hits = collector.run_searches(
                session=None,
                query_specs=query_specs,
                engine="duckduckgo",
                delay=0.0,
                per_query_limit=5,
            )

        ddg.assert_called_once()
        forum.assert_called_once()
        bing.assert_not_called()
        self.assertEqual([fallback_hit], hits)

    def test_run_searches_disables_failed_primary_engine_after_threshold(self) -> None:
        query_specs = [
            ("Knock and pin/default policy", "knock_pin", ["knock_pin"]),
            ("Knock and pin/default policy", "knock input", ["knock"]),
            ("Knock and pin/default policy", "knock sensor", ["sensor"]),
        ]
        fallback_hit = collector.SearchHit(
            query="knock_pin",
            title="Thread one",
            url="https://speeduino.com/forum/viewtopic.php?t=10",
            label="Knock and pin/default policy",
        )

        with patch.object(
            collector,
            "search_duckduckgo_site",
            side_effect=RuntimeError("ddg blocked"),
        ) as ddg, patch.object(
            collector,
            "search_forum_site",
            return_value=[fallback_hit],
        ) as forum, patch.object(
            collector,
            "search_bing_site",
            return_value=[fallback_hit],
        ) as bing:
            hits = collector.run_searches(
                session=None,
                query_specs=query_specs,
                engine="duckduckgo",
                delay=0.0,
                per_query_limit=5,
                failure_threshold=2,
            )

        self.assertEqual(2, ddg.call_count)
        self.assertEqual(3, forum.call_count)
        bing.assert_not_called()
        self.assertEqual(1, len(hits))

    def test_search_bing_site_parses_rss_results(self) -> None:
        rss = """<?xml version="1.0" encoding="utf-8" ?>
<rss version="2.0">
  <channel>
    <item>
      <title>Knock pin thread</title>
      <link>https://speeduino.com/forum/viewtopic.php?t=123&amp;start=0</link>
      <description>Discussion of knock_pin and A8.</description>
    </item>
    <item>
      <title>Off-site result</title>
      <link>https://example.com/post</link>
      <description>Ignore this.</description>
    </item>
  </channel>
</rss>
"""
        session = type("FakeSession", (), {})()
        session.get = lambda *args, **kwargs: self.FakeResponse(rss)

        hits = collector.search_bing_site(
            session=session,
            query="knock_pin",
            delay=0.0,
            label="Knock and pin/default policy",
        )

        self.assertEqual(1, len(hits))
        self.assertEqual(
            "https://speeduino.com/forum/viewtopic.php?t=123",
            hits[0].url,
        )
        self.assertEqual("Knock pin thread", hits[0].title)

    def test_search_forum_site_parses_phpbb_search_results(self) -> None:
        html = """
<html><body>
  <div class="postrow_container">
    <div class="well well-sm">
      <h4><a href="./viewtopic.php?p=73387&amp;hilit=knock_pin&amp;sid=abc#p73387">Re: Speeduino Dropbear V2 Knock sensor pin not available in Tunerstudio</a></h4>
      <span class="text-muted">Topic: <a href="./viewtopic.php?t=7211&amp;hilit=knock_pin&amp;sid=abc">Speeduino Dropbear V2 Knock sensor pin not available in Tunerstudio</a></span>
      <div class="content"><p>I now understand, thanks!</p></div>
    </div>
  </div>
</body></html>
"""
        session = type("FakeSession", (), {})()
        session.get = lambda *args, **kwargs: self.FakeResponse(html)

        hits = collector.search_forum_site(
            session=session,
            query="knock_pin",
            delay=0.0,
            label="Knock and pin/default policy",
        )

        self.assertEqual(1, len(hits))
        self.assertEqual(
            "https://speeduino.com/forum/viewtopic.php?t=7211",
            hits[0].url,
        )
        self.assertEqual(
            "Speeduino Dropbear V2 Knock sensor pin not available in Tunerstudio",
            hits[0].title,
        )
        self.assertIn("I now understand", hits[0].snippet)

    def test_thread_explicitly_matches_decoder_requires_thread_title_signal(self) -> None:
        supporting_post = collector.PostEvidence(
            author="PSIG",
            date="Mon Sep 25, 2023 9:39 pm",
            post_anchor="p65023",
            content_text="It appears the Speeduino Honda J30 / J32 decoder may be similar or same.",
            content_markdown="It appears the Speeduino Honda J30 / J32 decoder may be similar or same.",
            is_maintainer=True,
            evidence_terms_found=["trigger"],
            attachments=[],
            image_links=[],
            score=8.0,
            evidence_type=["maintainer explanation"],
        )
        matching = collector.ThreadData(
            title="J35a4 Triggers - Page 2",
            url="https://speeduino.com/forum/viewtopic.php?t=6027",
            breadcrumb=[],
            posts=[supporting_post],
        )
        generic = collector.ThreadData(
            title="Just another Speedy ECU - Page 3",
            url="https://speeduino.com/forum/viewtopic.php?t=74049&start=20",
            breadcrumb=[],
            posts=[],
        )

        self.assertTrue(collector.thread_explicitly_matches_decoder(matching, "Honda J32"))
        self.assertFalse(collector.thread_explicitly_matches_decoder(generic, "36-2-1"))

    def test_build_decoder_records_skips_threads_without_explicit_decoder_match(self) -> None:
        post = collector.PostEvidence(
            author="Black Knight",
            date="Sat Feb 28, 2026 1:05 am",
            post_anchor="p74172",
            content_text="Finalizing VR with tooth logger and scope capture on crank signal.",
            content_markdown="Finalizing VR with tooth logger and scope capture on crank signal.",
            is_maintainer=False,
            evidence_terms_found=["tooth logger", "scope"],
            attachments=[],
            image_links=[],
            score=8.5,
            evidence_type=["tooth log", "scope capture"],
        )
        thread = collector.ThreadData(
            title="Just another Speedy ECU - Page 3",
            url="https://speeduino.com/forum/viewtopic.php?t=74049&start=20",
            breadcrumb=[],
            posts=[post],
            source_queries=["36-2-1 tooth logger"],
            source_labels=["36-2-1"],
        )

        records = collector.build_decoder_records([("36-2-1", thread)])
        self.assertEqual([], records)

    def test_collect_thread_honors_max_page_limit(self) -> None:
        soups = {
            "https://speeduino.com/forum/viewtopic.php?t=1": object(),
            "https://speeduino.com/forum/viewtopic.php?t=1&start=10": object(),
            "https://speeduino.com/forum/viewtopic.php?t=1&start=20": object(),
        }
        post = collector.PostEvidence(
            author="PSIG",
            date="Mon Sep 25, 2023 9:39 pm",
            post_anchor="p1",
            content_text="Honda J32 trigger scope and sync details.",
            content_markdown="Honda J32 trigger scope and sync details.",
            is_maintainer=True,
            evidence_terms_found=["scope", "sync"],
            attachments=[],
            image_links=[],
            score=8.0,
            evidence_type=["maintainer explanation"],
        )

        with patch.object(collector, "fetch_soup", side_effect=lambda session, url: soups[url]) as fetch_soup, patch.object(
            collector,
            "topic_pagination_links",
            side_effect=lambda soup, url, topic_id: [
                "https://speeduino.com/forum/viewtopic.php?t=1&start=10",
                "https://speeduino.com/forum/viewtopic.php?t=1&start=20",
            ] if "start=20" not in url else [],
        ), patch.object(
            collector,
            "extract_thread_posts_from_soup",
            return_value=[post],
        ), patch.object(
            collector,
            "extract_thread_title",
            return_value="J35a4 Triggers - Page 2",
        ), patch.object(
            collector,
            "parse_breadcrumb",
            return_value=[],
        ):
            thread = collector.collect_thread(
                session=None,
                thread_url="https://speeduino.com/forum/viewtopic.php?t=1",
                delay=0.0,
                max_pages=2,
            )

        self.assertEqual(2, fetch_soup.call_count)
        self.assertEqual("J35a4 Triggers - Page 2", thread.title)


if __name__ == "__main__":
    unittest.main()
