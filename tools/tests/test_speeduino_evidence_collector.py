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
    def test_resolve_output_path_uses_script_directory_for_relative_paths(self) -> None:
        resolved = collector.resolve_output_path("report.md")
        self.assertEqual(MODULE_PATH.parent / "report.md", resolved)

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
        self.assertEqual("https://speeduino.com/forum/viewtopic.php?f=1&t=10", args[1])
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
        bing.assert_called_once()
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
        self.assertEqual(3, bing.call_count)
        self.assertEqual(1, len(hits))


if __name__ == "__main__":
    unittest.main()
