#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
import re
import sys
import time
import xml.etree.ElementTree as ET
from collections import defaultdict, deque
from dataclasses import asdict, dataclass, field
from html import unescape
from pathlib import Path
from typing import Optional
from urllib.parse import parse_qs, urlencode, urljoin, urlparse, urlunparse

import requests
from bs4 import BeautifulSoup


USER_AGENT = "Mozilla/5.0 (compatible; SpeeduinoEvidenceCollector/3.0)"
TIMEOUT = 30
DEFAULT_DELAY = 1.0
DEFAULT_SEARCH_ENGINE = "forum"
SCRIPT_DIR = Path(__file__).resolve().parent
SEARCH_ENGINES = ("forum", "bing", "duckduckgo")
DEFAULT_ENGINE_FAILURE_THRESHOLD = 3

MAINTAINER_NAMES = {
    "noisymime",
    "dazq",
    "lsdlsd88",
    "mck1117",
    "srh550",
    "nismo",
    "speeduino",
}

EVIDENCE_TERMS = {
    "tooth logger",
    "composite logger",
    "scope",
    "oscilloscope",
    "sync loss",
    "trigger trace",
    "trigger wheel",
    "tooth log",
    "trace",
    "capture",
    "msq",
    "base tune",
    "base map",
    "signature mismatch",
    "vss",
    "flat shift",
    "rolling cut",
    "idle advance",
    "idle taper",
    "aircon",
    "air con",
    "knock input",
    "knock sensor",
    "knock_pin",
    "dropbear",
    "teensy 4.1",
    "decoder",
}

ROADMAP_AREAS = [
    {
        "name": "Knock and pin/default policy",
        "search_terms": [
            "knock_pin",
            "knock input",
            "knock sensor",
            "external knock controller",
            "A8 A10 knock",
            "Teensy 4.1 knock",
            "DropBear knock",
        ],
        "focus_terms": [
            "recommended knock input pins",
            "analog vs digital",
            "A8",
            "A10",
            "default",
            "packaged tune",
            "board specific",
            "DropBear",
            "Teensy 4.1",
            "caution",
        ],
    },
    {
        "name": "Tune compatibility, base tune, and INI/default drift",
        "search_terms": [
            "base tune",
            "base map",
            "msq",
            "ini mismatch",
            "signature",
            "speeduino 202501",
            "tune migration",
            "tunerstudio defaults",
        ],
        "focus_terms": [
            "signature mismatch",
            "speeduino.ini",
            "version",
            "migrate",
            "rebuild",
            "default",
            "base tune",
            "base map",
            "202501",
            "202402",
            "tunerstudio",
        ],
    },
    {
        "name": "VSS, launch, rolling cut, and related protection behavior",
        "search_terms": [
            "VSS input mode",
            "vss pulses per km",
            "launch control VSS",
            "rolling cut",
            "launch speed",
            "flat shift",
        ],
        "focus_terms": [
            "VSS disabled",
            "launch",
            "rolling cut",
            "flat shift",
            "zero",
            "off",
            "disable",
            "sensor mode",
            "pulses per km",
        ],
    },
    {
        "name": "Idle advance, idle taper, and air-con defaults",
        "search_terms": [
            "idle advance",
            "delay before idle control",
            "idle taper",
            "crank to run taper",
            "air con polarity",
            "airConReqPol",
            "airConCompPol",
            "idle up",
        ],
        "focus_terms": [
            "idle advance",
            "delay",
            "taper",
            "crank to run",
            "aircon",
            "air con",
            "polarity",
            "idle up",
            "default",
            "wiring",
        ],
    },
    {
        "name": "Decoder and trigger-pattern evidence",
        "search_terms": [
            "trigger decoder",
            "tooth logger",
            "composite logger",
            "sync loss",
            "Honda J32",
            "36-2-1",
            "Harley decoder",
            "Subaru 6/7",
            "trigger trace",
        ],
        "focus_terms": [
            "tooth logger",
            "composite logger",
            "sync",
            "sync loss",
            "trigger",
            "tooth",
            "cam",
            "crank",
            "scope",
            "trace",
            "decoder",
        ],
    },
]

DECODER_FAMILIES = [
    "Honda J32",
    "36-2-1",
    "Harley",
    "Subaru 6/7",
    "Mazda AU",
    "Daihatsu",
    "Rover MEMS",
    "36-2-2-2",
    "420a",
    "Vmax",
    "Honda D17",
    "Basic distributor",
]

DECODER_EVIDENCE_TERMS = [
    "trigger decoder",
    "tooth logger",
    "composite logger",
    "sync loss",
    "trigger trace",
    "crank signal",
    "cam signal",
    "missing tooth",
    "second trigger",
    "poll level",
    "trigger pattern",
    "decoder",
    "scope",
    "oscilloscope",
    "sync",
    "no sync",
    "half sync",
    "crank",
    "cam",
    "trigger wheel",
    "CAS",
]

KNOWN_PATTERN_REGEXES = [
    r"\b\d{1,2}-\d(?:-\d(?:-\d)?)?\b",
    r"\bsubaru\s*6/7\b",
    r"\brover\s*mems\b",
    r"\bbasic distributor\b",
    r"\bhonda\s*j32\b",
    r"\bhonda\s*d17\b",
    r"\bmazda\s*au\b",
    r"\b420a\b",
    r"\bvmax\b",
    r"\bharley\b",
    r"\bdaihatsu\b",
]

DATE_RE = re.compile(
    r"(Mon|Tue|Wed|Thu|Fri|Sat|Sun)\s+[A-Z][a-z]{2}\s+\d{1,2},\s+\d{4}\s+\d{1,2}:\d{2}\s*(?:am|pm)?",
    flags=re.I,
)


@dataclass
class SearchHit:
    query: str
    title: str
    url: str
    snippet: str = ""
    label: str = ""


@dataclass
class PostEvidence:
    author: str
    date: str
    post_anchor: str
    content_text: str
    content_markdown: str
    is_maintainer: bool
    evidence_terms_found: list[str] = field(default_factory=list)
    attachments: list[str] = field(default_factory=list)
    image_links: list[str] = field(default_factory=list)
    score: float = 0.0
    evidence_type: list[str] = field(default_factory=list)


@dataclass
class ThreadData:
    title: str
    url: str
    breadcrumb: list[str]
    posts: list[PostEvidence]
    source_queries: list[str] = field(default_factory=list)
    source_labels: list[str] = field(default_factory=list)


@dataclass
class EvidenceRecord:
    roadmap_area: Optional[str] = None
    decoder_pattern: Optional[str] = None
    thread_title: str = ""
    url: str = ""
    date: str = ""
    posted_by: str = ""
    evidence_type: str = ""
    summary: str = ""
    why_it_matters: str = ""
    specific_test_details: str = ""
    confidence: str = "medium"
    score: float = 0.0
    source_queries: list[str] = field(default_factory=list)


def log(message: str) -> None:
    print(message, file=sys.stderr)


def canonicalize_url(url: str) -> str:
    parsed = urlparse(url)
    qs = parse_qs(parsed.query, keep_blank_values=True)
    allowed = {"f", "t", "p", "start"}
    clean_qs = {k: v for k, v in qs.items() if k in allowed}
    if clean_qs.get("start") == ["0"]:
        clean_qs.pop("start")
    new_query = urlencode(clean_qs, doseq=True)
    return urlunparse((parsed.scheme, parsed.netloc, parsed.path, "", new_query, ""))


def text_or_empty(node) -> str:
    if not node:
        return ""
    return " ".join(node.get_text(" ", strip=True).split())


def normalize_ws(text: str) -> str:
    text = unescape(text or "")
    text = text.replace("\r\n", "\n").replace("\r", "\n")
    text = re.sub(r"[ \t]+", " ", text)
    text = re.sub(r"\n{3,}", "\n\n", text)
    return text.strip()


def normalize_thread_title(title: str) -> str:
    return re.sub(r"\s*-\s*Page\s+\d+\s*$", "", title or "", flags=re.I).strip()


def fetch_soup(session: requests.Session, url: str) -> BeautifulSoup:
    response = session.get(url, timeout=TIMEOUT)
    response.raise_for_status()
    return BeautifulSoup(response.text, "html.parser")


def dedupe_hits(hits: list[SearchHit]) -> list[SearchHit]:
    out: list[SearchHit] = []
    seen: set[str] = set()
    for hit in hits:
        key = canonicalize_url(hit.url)
        if key in seen:
            continue
        seen.add(key)
        out.append(hit)
    return out


def make_session() -> requests.Session:
    session = requests.Session()
    session.headers.update({"User-Agent": USER_AGENT})
    return session


def resolve_output_path(path_str: str) -> Path:
    path = Path(path_str)
    if path.is_absolute():
        return path
    return SCRIPT_DIR / path


def search_duckduckgo_site(session: requests.Session, query: str, delay: float, label: str) -> list[SearchHit]:
    params = {"q": f"site:speeduino.com/forum {query}"}
    log(f"[search:duckduckgo] {params['q']}")
    response = session.get("https://html.duckduckgo.com/html/", params=params, timeout=TIMEOUT)
    response.raise_for_status()
    soup = BeautifulSoup(response.text, "html.parser")
    hits: list[SearchHit] = []

    for result in soup.select(".result"):
        anchor = result.select_one(".result__title a") or result.select_one("a.result__a") or result.find("a", href=True)
        if not anchor:
            continue
        href = anchor.get("href", "").strip()
        if not href:
            continue
        if "uddg=" in href:
            href = parse_qs(urlparse(href).query).get("uddg", [href])[0]
        href = canonicalize_url(href)
        if "speeduino.com/forum/viewtopic.php" not in href:
            continue
        hits.append(
            SearchHit(
                query=query,
                title=text_or_empty(anchor),
                url=href,
                snippet=text_or_empty(result.select_one(".result__snippet")) or text_or_empty(result),
                label=label,
            )
        )

    time.sleep(delay)
    return dedupe_hits(hits)


def search_bing_site(session: requests.Session, query: str, delay: float, label: str) -> list[SearchHit]:
    params = {"q": f"site:speeduino.com/forum {query}"}
    log(f"[search:bing] {params['q']}")
    response = session.get(
        "https://www.bing.com/search",
        params={**params, "format": "rss"},
        timeout=TIMEOUT,
    )
    response.raise_for_status()
    hits: list[SearchHit] = []
    root = ET.fromstring(response.text)

    for result in root.findall(".//item"):
        link_text = (result.findtext("link") or "").strip()
        if not link_text:
            continue
        href = canonicalize_url(link_text)
        if "speeduino.com/forum/viewtopic.php" not in href:
            continue
        hits.append(
            SearchHit(
                query=query,
                title=normalize_ws(result.findtext("title") or href),
                url=href,
                snippet=normalize_ws(result.findtext("description") or ""),
                label=label,
            )
        )

    time.sleep(delay)
    return dedupe_hits(hits)


def search_forum_site(session: requests.Session, query: str, delay: float, label: str) -> list[SearchHit]:
    params = {
        "keywords": query,
        "terms": "all",
        "sf": "all",
        "sr": "posts",
    }
    log(f"[search:forum] {query}")
    response = session.get(
        "https://speeduino.com/forum/search.php",
        params=params,
        timeout=TIMEOUT,
    )
    response.raise_for_status()
    soup = BeautifulSoup(response.text, "html.parser")
    hits: list[SearchHit] = []

    for result in soup.select("div.postrow_container div.well.well-sm"):
        heading = result.select_one("h4 a[href*='viewtopic.php']")
        topic_link = result.select_one("a[href*='viewtopic.php?t=']")
        snippet_node = result.select_one("div.content")
        selected_link = topic_link or heading
        if not selected_link:
            continue
        href = urljoin("https://speeduino.com/forum/", selected_link.get("href", "").strip())
        href = canonicalize_url(href)
        if "speeduino.com/forum/viewtopic.php" not in href:
            continue
        hits.append(
            SearchHit(
                query=query,
                title=normalize_thread_title(text_or_empty(topic_link or heading)),
                url=href,
                snippet=text_or_empty(snippet_node) or text_or_empty(result),
                label=label,
            )
        )

    time.sleep(delay)
    return dedupe_hits(hits)


def run_searches(
    session: requests.Session,
    query_specs: list[tuple[str, str, list[str]]],
    engine: str,
    delay: float,
    per_query_limit: int,
    failure_threshold: int = DEFAULT_ENGINE_FAILURE_THRESHOLD,
) -> list[SearchHit]:
    all_hits: list[SearchHit] = []
    seen: set[str] = set()
    failed_engines: set[str] = set()
    failure_counts: dict[str, int] = defaultdict(int)

    for label, query, _focus_terms in query_specs:
        hits: list[SearchHit] = []
        engines_to_try = build_search_engine_fallback_order(engine)
        for engine_name in engines_to_try:
            if engine_name in failed_engines:
                continue
            try:
                if engine_name == "forum":
                    hits = search_forum_site(session, query, delay, label)
                elif engine_name == "bing":
                    hits = search_bing_site(session, query, delay, label)
                else:
                    hits = search_duckduckgo_site(session, query, delay, label)
                failure_counts[engine_name] = 0
                if engine_name != engine:
                    log(f"[fallback] {query!r} succeeded via {engine_name}")
                break
            except Exception as exc:
                log(f"[warn] search failed for {query!r} via {engine_name}: {exc}")
                failure_counts[engine_name] += 1
                if failure_threshold > 0 and failure_counts[engine_name] >= failure_threshold:
                    failed_engines.add(engine_name)
                    log(
                        f"[circuit-breaker] disabling {engine_name} after "
                        f"{failure_counts[engine_name]} consecutive failures"
                    )
        else:
            hits = []

        if per_query_limit > 0:
            hits = hits[:per_query_limit]

        for hit in hits:
            key = canonicalize_url(hit.url)
            if key in seen:
                continue
            seen.add(key)
            all_hits.append(hit)

    return all_hits


def build_search_engine_fallback_order(primary_engine: str) -> list[str]:
    fallback_preference = {
        "forum": ["forum", "bing", "duckduckgo"],
        "bing": ["bing", "forum", "duckduckgo"],
        "duckduckgo": ["duckduckgo", "forum", "bing"],
    }
    return fallback_preference.get(primary_engine, [primary_engine, *SEARCH_ENGINES])


def html_fragment_to_markdown(node) -> str:
    if node is None:
        return ""

    for br in node.find_all("br"):
        br.replace_with("\n")

    for code in node.find_all(["code", "pre"]):
        code_text = code.get_text("\n", strip=False).strip()
        code.replace_with(f"\n```\n{code_text}\n```\n")

    for quote in node.find_all(class_=re.compile(r"\bquote\b", re.I)):
        q_text = normalize_ws(quote.get_text("\n", strip=True))
        quoted = "\n".join("> " + line if line else ">" for line in q_text.splitlines())
        quote.replace_with(f"\n{quoted}\n")

    for anchor in node.find_all("a"):
        href = anchor.get("href", "").strip()
        label = text_or_empty(anchor) or href
        anchor.replace_with(f"[{label}]({href})" if href else label)

    raw = node.get_text("\n", strip=False)
    lines = [line.rstrip() for line in raw.splitlines()]
    out: list[str] = []
    blank = False
    for line in lines:
        if line.strip():
            out.append(line)
            blank = False
        elif not blank:
            out.append("")
            blank = True

    return normalize_ws("\n".join(out))


def parse_breadcrumb(soup: BeautifulSoup) -> list[str]:
    crumbs: list[str] = []
    for anchor in soup.select('a[href*="index.php"], a[href*="viewforum.php"]'):
        label = text_or_empty(anchor)
        if label and label not in crumbs:
            crumbs.append(label)
    return crumbs[:10]


def extract_attachments(block, page_url: str) -> tuple[list[str], list[str]]:
    attachments: list[str] = []
    images: list[str] = []

    for anchor in block.find_all("a", href=True):
        href = urljoin(page_url, anchor["href"])
        label = text_or_empty(anchor)
        lower = href.lower()

        if "download/file.php" in lower or any(lower.endswith(ext) for ext in [".msq", ".csv", ".log", ".txt"]):
            attachments.append(f"{label} -> {href}" if label else href)

        if any(token in lower for token in ["download/file.php", "/files/"]) or any(
            lower.endswith(ext) for ext in [".png", ".jpg", ".jpeg", ".gif", ".webp"]
        ):
            if any(lower.endswith(ext) for ext in [".png", ".jpg", ".jpeg", ".gif", ".webp"]) or "download/file.php" in lower:
                images.append(href)

    return sorted(set(attachments)), sorted(set(images))


def classify_evidence_type(text: str, attachments: list[str], images: list[str], is_maintainer: bool) -> list[str]:
    lower = text.lower()
    kinds: list[str] = []

    if "tooth logger" in lower or "tooth log" in lower:
        kinds.append("tooth log")
    if "composite logger" in lower or re.search(r"\bcomposite\b", lower):
        kinds.append("composite log")
    if "scope" in lower or "oscilloscope" in lower:
        kinds.append("scope capture")
    if "msq" in lower or any(".msq" in item.lower() for item in attachments):
        kinds.append("tune file")
    if "trigger wheel" in lower or "pattern" in lower or any(re.search(rx, lower) for rx in KNOWN_PATTERN_REGEXES):
        kinds.append("wheel description")
    if is_maintainer:
        kinds.append("maintainer explanation")
    if images and not any(kind in kinds for kind in ["tooth log", "composite log", "scope capture"]):
        kinds.append("image attachment")

    if not kinds:
        kinds.append("technical discussion")

    return kinds


def score_post(
    text: str,
    author: str,
    attachments: list[str],
    images: list[str],
    focus_terms: Optional[list[str]] = None,
) -> tuple[float, list[str]]:
    lower = text.lower()
    score = 0.0
    found: list[str] = []

    if author.lower() in MAINTAINER_NAMES:
        score += 3.0

    for term in EVIDENCE_TERMS:
        if term in lower:
            score += 0.7
            found.append(term)

    for term in focus_terms or []:
        if term.lower() in lower:
            score += 1.25
            found.append(term)

    if attachments:
        score += 1.5
    if images:
        score += 1.2

    for term, bonus in [
        ("sync loss", 1.0),
        ("tooth logger", 1.7),
        ("composite logger", 1.7),
        ("scope", 1.5),
        ("oscilloscope", 1.5),
        ("signature mismatch", 1.5),
        ("speeduino.ini", 1.5),
        ("default", 1.0),
        ("base tune", 1.0),
        ("base map", 1.0),
        ("a8", 1.0),
        ("a10", 1.0),
    ]:
        if term in lower:
            score += bonus

    word_count = len(lower.split())
    if word_count > 80:
        score += 0.8
    if word_count > 180:
        score += 0.8

    return score, sorted(set(found))


def extract_thread_posts_from_soup(
    soup: BeautifulSoup,
    page_url: str,
    focus_terms: Optional[list[str]] = None,
) -> list[PostEvidence]:
    posts: list[PostEvidence] = []
    seen_anchors: set[str] = set()
    post_blocks = soup.select(".post") or soup.select("[id^='p']") or soup.select(".topicpost")

    for block in post_blocks:
        anchor_id = ""
        for anchor in block.select("a[href*='#p'], a"):
            match = re.search(r"#(p\d+)", anchor.get("href", ""))
            if match:
                anchor_id = match.group(1)
                break
        if not anchor_id:
            block_id = block.get("id", "")
            if re.match(r"p\d+", block_id):
                anchor_id = block_id

        if anchor_id and anchor_id in seen_anchors:
            continue
        if anchor_id:
            seen_anchors.add(anchor_id)

        author = ""
        for candidate in [
            block.select_one(".author a"),
            block.select_one(".username"),
            block.select_one(".username-coloured"),
            block.select_one("a.username"),
        ]:
            author = text_or_empty(candidate)
            if author:
                break

        author_text = text_or_empty(block.select_one(".author")) or text_or_empty(block)
        date_match = DATE_RE.search(author_text)
        date = date_match.group(0) if date_match else ""

        content_node = block.select_one(".content") or block.select_one(".postbody") or block.select_one(".inner") or block
        content_md = html_fragment_to_markdown(content_node)
        content_text = normalize_ws(content_node.get_text("\n", strip=True)) if content_node else ""
        if not content_text:
            continue

        attachments, images = extract_attachments(block, page_url)
        is_maintainer = author.lower() in MAINTAINER_NAMES
        score, found = score_post(content_text, author, attachments, images, focus_terms)
        evidence_type = classify_evidence_type(content_text, attachments, images, is_maintainer)

        posts.append(
            PostEvidence(
                author=author or "Unknown",
                date=date,
                post_anchor=anchor_id,
                content_text=content_text,
                content_markdown=content_md,
                is_maintainer=is_maintainer,
                evidence_terms_found=found,
                attachments=attachments,
                image_links=images,
                score=score,
                evidence_type=evidence_type,
            )
        )

    return posts


def extract_thread_title(soup: BeautifulSoup) -> str:
    for candidate in [
        soup.select_one("h2.topic-title"),
        soup.select_one("h2 a"),
        soup.select_one("title"),
        soup.select_one("h1"),
        soup.select_one("h2"),
    ]:
        title = text_or_empty(candidate)
        if title:
            return re.sub(r"\s*-\s*Speeduino.*$", "", title, flags=re.I)
    return "Untitled Thread"


def topic_pagination_links(soup: BeautifulSoup, page_url: str, topic_id: Optional[str]) -> list[str]:
    links: set[str] = set()
    for anchor in soup.find_all("a", href=True):
        href = canonicalize_url(urljoin(page_url, anchor["href"]))
        parsed = urlparse(href)
        if "viewtopic.php" not in parsed.path:
            continue
        qs = parse_qs(parsed.query)
        if topic_id is not None and qs.get("t", [None])[0] != topic_id:
            continue
        if "start" in qs:
            links.add(href)
    return sorted(links)


def collect_thread(
    session: requests.Session,
    thread_url: str,
    delay: float,
    focus_terms: Optional[list[str]] = None,
    source_queries: Optional[list[str]] = None,
    source_labels: Optional[list[str]] = None,
) -> ThreadData:
    thread_url = canonicalize_url(thread_url)
    topic_id = parse_qs(urlparse(thread_url).query).get("t", [None])[0]

    queue: deque[str] = deque([thread_url])
    visited: set[str] = set()
    pages: list[tuple[str, BeautifulSoup]] = []

    while queue:
        url = queue.popleft()
        if url in visited:
            continue
        visited.add(url)
        log(f"[thread] {url}")
        soup = fetch_soup(session, url)
        pages.append((url, soup))

        for next_url in topic_pagination_links(soup, url, topic_id):
            if next_url not in visited:
                queue.append(next_url)

        time.sleep(delay)

    first_soup = pages[0][1]
    all_posts: list[PostEvidence] = []
    seen_keys: set[tuple[str, str, str, str]] = set()

    for page_url, soup in pages:
        for post in extract_thread_posts_from_soup(soup, page_url, focus_terms):
            key = (post.post_anchor, post.author, post.date, post.content_text[:120])
            if key in seen_keys:
                continue
            seen_keys.add(key)
            all_posts.append(post)

    return ThreadData(
        title=extract_thread_title(first_soup),
        url=thread_url,
        breadcrumb=parse_breadcrumb(first_soup),
        posts=all_posts,
        source_queries=sorted(set(source_queries or [])),
        source_labels=sorted(set(source_labels or [])),
    )


def summarize_sentences(text: str, max_sentences: int, fallback_chars: int) -> str:
    cleaned = " ".join(text.split())
    parts = re.split(r"(?<=[.!?])\s+", cleaned)
    summary = " ".join(parts[:max_sentences]).strip()
    if len(summary) < 80:
        summary = cleaned[:fallback_chars].strip()
    return summary


def summarize_post_for_area(post: PostEvidence) -> tuple[str, str, str]:
    lower = post.content_text.lower()
    summary = summarize_sentences(post.content_text, 3, 450)

    why: list[str] = []
    details: list[str] = []

    if post.is_maintainer:
        why.append("Includes maintainer guidance, which is more reliable for roadmap decisions.")
    if "signature mismatch" in lower or "speeduino.ini" in lower:
        why.append("Directly informs tune and INI compatibility handling plus migration expectations.")
    if "tooth logger" in lower or "composite logger" in lower or "scope" in lower or "oscilloscope" in lower:
        why.append("Contains concrete trace-oriented evidence that can support replay tests and decoder validation.")
    if "default" in lower or "base tune" in lower or "base map" in lower:
        why.append("Helps separate universal defaults from context-specific tune choices.")
    if "a8" in lower or "a10" in lower or "knock" in lower:
        why.append("Relevant to pin-selection policy and board-specific packaged settings.")
    if "launch" in lower or "vss" in lower or "rolling cut" in lower:
        why.append("Useful for protection-mode expectations and disabled or off-state behavior.")
    if "idle" in lower or "air con" in lower or "aircon" in lower:
        why.append("Touches wiring-dependent versus universal idle and A/C behavior.")
    if not why:
        why.append("Contains technical evidence that may affect firmware behavior, tests, or documentation.")

    patterns: list[str] = []
    for rx in KNOWN_PATTERN_REGEXES:
        patterns.extend(re.findall(rx, lower, flags=re.I))
    if patterns:
        details.append("Pattern mentions: " + ", ".join(sorted(set(patterns))))

    edge_terms = [t for t in ["rising", "falling", "change", "poll level"] if t in lower]
    if edge_terms:
        details.append("Edge or poll details: " + ", ".join(edge_terms))

    signal_terms = []
    if "cam" in lower:
        signal_terms.append("cam mentioned")
    if "crank" in lower:
        signal_terms.append("crank mentioned")
    if signal_terms:
        details.append("Signal roles: " + ", ".join(signal_terms))

    if "sync loss" in lower:
        details.append("Explicit sync-loss discussion present.")
    if post.attachments:
        details.append("Attachments: " + "; ".join(post.attachments[:3]))
    if post.image_links:
        details.append(f"Image or capture links found: {len(post.image_links)}")

    return summary, " ".join(why), " ".join(details).strip()


def summarize_post_for_decoder(post: PostEvidence, decoder_name: str) -> tuple[str, str, str]:
    lower = post.content_text.lower()
    summary = summarize_sentences(post.content_text, 4, 550)

    why: list[str] = []
    details: list[str] = []

    if post.is_maintainer:
        why.append("Maintainer explanation increases confidence in expected decoder behavior.")
    if "tooth logger" in lower or "composite logger" in lower:
        why.append("Provides replay-test-relevant trigger traces.")
    if "scope" in lower or "oscilloscope" in lower:
        why.append("Includes hardware-signal evidence, useful for edge and polarity interpretation.")
    if "sync loss" in lower:
        why.append("Discusses sync-loss conditions that can inform negative test cases.")
    if not why:
        why.append("Contains decoder-specific technical details that may support test coverage.")

    patterns: list[str] = []
    for rx in KNOWN_PATTERN_REGEXES:
        patterns.extend(re.findall(rx, post.content_text, flags=re.I))
    if patterns:
        details.append("Pattern references: " + ", ".join(sorted(set(patterns))))

    for phrase in [
        "missing tooth",
        "second trigger",
        "poll level",
        "rising edge",
        "falling edge",
        "change",
        "cam",
        "crank",
        "sync",
        "sync loss",
        "half sync",
        "cas",
    ]:
        if phrase in lower:
            details.append(f"Includes: {phrase}")

    if decoder_name.lower() not in lower:
        details.append(f"Matched decoder family: {decoder_name}")
    if post.image_links:
        details.append(f"Image or capture links: {len(post.image_links)}")
    if post.attachments:
        details.append("Attachments: " + "; ".join(post.attachments[:3]))

    return summary, " ".join(why), " ".join(details).strip()


def confidence_from_post(post: PostEvidence) -> str:
    lower = post.content_text.lower()
    if post.is_maintainer and any(t in lower for t in ["tooth logger", "composite logger", "scope", "oscilloscope"]):
        return "high"
    if post.score >= 8:
        return "high"
    if post.score >= 4.5:
        return "medium"
    return "low"


def infer_decoder_from_text(thread: ThreadData, post: PostEvidence) -> str:
    combined = (thread.title + "\n" + post.content_text).lower()
    for decoder in DECODER_FAMILIES:
        if decoder.lower() in combined:
            return decoder
    for rx in KNOWN_PATTERN_REGEXES:
        match = re.search(rx, combined, flags=re.I)
        if match:
            return match.group(0)
    return "Unclassified decoder"


def thread_explicitly_matches_decoder(thread: ThreadData, requested_decoder: str) -> bool:
    combined = "\n".join(
        [normalize_thread_title(thread.title)] + [post.content_text for post in thread.posts[:6]]
    ).lower()
    requested = requested_decoder.lower()
    if requested in combined:
        return True

    normalized_patterns = {
        "36-2-1": r"\b36\s*-\s*2\s*-\s*1\b",
        "36-2-2-2": r"\b36\s*-\s*2\s*-\s*2\s*-\s*2\b",
        "420a": r"\b420a\b",
        "vmax": r"\bvmax\b",
        "harley": r"\bharley\b",
        "rover mems": r"\brover\s+mems\b",
        "subaru 6/7": r"\bsubaru\s*6\s*/\s*7\b",
        "honda d17": r"\bhonda\s*d17\b",
        "honda j32": r"\bhonda\s*j(?:30|32)\b|\bj3[02]\b",
        "mazda au": r"\bmazda\s*au\b",
        "basic distributor": r"\bbasic\s+distributor\b|\bdistributor\b",
        "daihatsu": r"\bdaihatsu\b",
    }
    pattern = normalized_patterns.get(requested)
    if pattern and re.search(pattern, combined, flags=re.I):
        return True
    return False


def build_roadmap_records(threads: list[tuple[str, ThreadData]]) -> list[EvidenceRecord]:
    out: list[EvidenceRecord] = []
    seen: set[tuple[str, str, str]] = set()
    for area_name, thread in threads:
        ranked = sorted(thread.posts, key=lambda item: item.score, reverse=True)
        for post in ranked[:3]:
            if post.score < 3.0:
                continue
            dedupe_key = (
                area_name,
                normalize_thread_title(thread.title).lower(),
                post.post_anchor,
            )
            if dedupe_key in seen:
                continue
            seen.add(dedupe_key)
            summary, why, details = summarize_post_for_area(post)
            out.append(
                EvidenceRecord(
                    roadmap_area=area_name,
                    thread_title=normalize_thread_title(thread.title),
                    url=thread.url + (f"#{post.post_anchor}" if post.post_anchor else ""),
                    date=post.date,
                    posted_by=post.author,
                    evidence_type=" / ".join(post.evidence_type),
                    summary=summary,
                    why_it_matters=why,
                    specific_test_details=details,
                    confidence=confidence_from_post(post),
                    score=post.score,
                    source_queries=list(thread.source_queries),
                )
            )
    return sorted(out, key=lambda item: (item.roadmap_area or "", -item.score, item.thread_title))


def build_decoder_records(threads: list[tuple[str, ThreadData]]) -> list[EvidenceRecord]:
    out: list[EvidenceRecord] = []
    seen: set[tuple[str, str, str]] = set()
    for requested_decoder, thread in threads:
        if not thread_explicitly_matches_decoder(thread, requested_decoder):
            continue
        ranked = sorted(thread.posts, key=lambda item: item.score, reverse=True)
        for post in ranked[:4]:
            lower = post.content_text.lower()
            if post.score < 3.2:
                continue
            if not any(token in lower for token in ["tooth logger", "composite logger", "scope", "oscilloscope", "sync", "trigger", "cam", "crank", "poll level", "missing tooth", "cas"]):
                continue
            decoder = infer_decoder_from_text(thread, post)
            if decoder == "Unclassified decoder":
                decoder = requested_decoder
            dedupe_key = (
                decoder.lower(),
                normalize_thread_title(thread.title).lower(),
                post.post_anchor,
            )
            if dedupe_key in seen:
                continue
            seen.add(dedupe_key)
            summary, why, details = summarize_post_for_decoder(post, decoder)
            out.append(
                EvidenceRecord(
                    decoder_pattern=decoder,
                    thread_title=normalize_thread_title(thread.title),
                    url=thread.url + (f"#{post.post_anchor}" if post.post_anchor else ""),
                    date=post.date,
                    posted_by=post.author,
                    evidence_type=" / ".join(post.evidence_type),
                    summary=summary,
                    why_it_matters=why,
                    specific_test_details=details,
                    confidence=confidence_from_post(post),
                    score=post.score,
                    source_queries=list(thread.source_queries),
                )
            )
    return sorted(out, key=lambda item: (item.decoder_pattern or "", -item.score, item.thread_title))


def strongest_by_key(records: list[EvidenceRecord], key_name: str) -> dict[str, EvidenceRecord]:
    best: dict[str, EvidenceRecord] = {}
    for record in records:
        key = getattr(record, key_name) or "Unclassified"
        if key not in best or record.score > best[key].score:
            best[key] = record
    return best


def collect_unresolved_contradictions(records: list[EvidenceRecord], key_name: str) -> list[str]:
    grouped: dict[str, list[EvidenceRecord]] = defaultdict(list)
    for record in records:
        grouped[getattr(record, key_name) or "Unclassified"].append(record)

    contradictions: list[str] = []
    for key, recs in grouped.items():
        blob = " ".join((r.summary + " " + r.why_it_matters + " " + r.specific_test_details).lower() for r in recs)
        if "default" in blob and "context-specific" in blob:
            contradictions.append(f"{key}: some posts imply a universal default while others frame it as context-specific.")
        if "rising" in blob and "falling" in blob:
            contradictions.append(f"{key}: conflicting edge or polarity references appear across posts.")
        if "migrate" in blob and "rebuild" in blob:
            contradictions.append(f"{key}: mixed guidance appears on tune migration versus rebuilding.")
    return contradictions


def roadmap_action_hints(records: list[EvidenceRecord]) -> list[str]:
    blob = "\n".join((r.summary + " " + r.why_it_matters + " " + r.specific_test_details).lower() for r in records)
    hints: list[str] = []

    if "signature mismatch" in blob or "speeduino.ini" in blob:
        hints.append("Documentation and test addition: add explicit firmware, INI, and base-tune compatibility matrix checks plus migration guidance.")
    if "tooth logger" in blob or "composite logger" in blob or "sync loss" in blob:
        hints.append("Test addition: build replay and regression coverage from real tooth/composite traces and sync-loss cases.")
    if "a8" in blob or "a10" in blob or "knock" in blob:
        hints.append("Code and documentation review: verify knock-pin enumerations, board pin exposure, and packaged tune defaults for board variants.")
    if "aircon" in blob or "air con" in blob:
        hints.append("Documentation change: clarify that A/C polarity is wiring-dependent, not a universal default.")
    if "launch" in blob or "vss" in blob or "rolling cut" in blob:
        hints.append("Test and documentation change: confirm disabled or off-state semantics for VSS, launch control, and rolling cut settings.")

    return hints


def decoder_action_hints(records: list[EvidenceRecord]) -> tuple[list[str], list[str], list[str]]:
    strongest = strongest_by_key(records, "decoder_pattern")

    safe: list[str] = []
    blocked: list[str] = []
    priorities: list[str] = []

    for decoder, record in strongest.items():
        blob = (record.summary + " " + record.specific_test_details + " " + record.evidence_type).lower()
        has_trace = any(token in blob for token in ["tooth log", "composite log", "scope capture", "image or capture links"])
        has_behavior = any(token in blob for token in ["sync", "missing tooth", "cam", "crank", "poll level", "rising", "falling"])
        if record.confidence == "high" and has_trace and has_behavior:
            safe.append(decoder)
        else:
            blocked.append(decoder)

    grouped: dict[str, list[EvidenceRecord]] = defaultdict(list)
    for record in records:
        grouped[record.decoder_pattern or "Unclassified"].append(record)

    for decoder, recs in grouped.items():
        blob = " ".join((r.summary + " " + r.specific_test_details + " " + r.evidence_type).lower() for r in recs)
        if "maintainer explanation" in blob and ("tooth log" in blob or "composite log" in blob):
            priorities.append(f"{decoder}: convert existing trace-backed maintainer guidance into replay tests first.")
        elif "scope capture" in blob:
            priorities.append(f"{decoder}: add edge and polarity-focused tests derived from scope evidence.")
        else:
            priorities.append(f"{decoder}: still needs stronger real-trace evidence before high-confidence replay coverage.")

    return sorted(set(safe)), sorted(set(blocked)), priorities[:12]


def render_markdown(
    roadmap_records: list[EvidenceRecord],
    decoder_records: list[EvidenceRecord],
    output_json_path: str,
) -> str:
    lines: list[str] = []
    lines.append("# Speeduino forum evidence report")
    lines.append("")
    lines.append("This report is generated from targeted public-forum searching and thread scraping.")
    lines.append("")

    if roadmap_records:
        lines.append("## Roadmap evidence")
        lines.append("")
        current_area = None
        for record in roadmap_records:
            if record.roadmap_area != current_area:
                current_area = record.roadmap_area
                lines.append(f"### {current_area}")
                lines.append("")
            lines.extend(
                [
                    f"#### {record.thread_title}",
                    f"- URL: {record.url}",
                    f"- Date: {record.date or 'Unknown'}",
                    f"- Who posted it: {record.posted_by}",
                    f"- Evidence type: {record.evidence_type}",
                    f"- Confidence: {record.confidence}",
                    f"- Summary: {record.summary}",
                    f"- Why it matters: {record.why_it_matters}",
                    f"- Specific details: {record.specific_test_details or 'None extracted'}",
                    (
                        "- Matched search terms: "
                        + ", ".join(record.source_queries)
                        if record.source_queries
                        else "- Matched search terms: None retained"
                    ),
                    "",
                ]
            )

        lines.append("## Strongest evidence by roadmap area")
        lines.append("")
        for area, record in strongest_by_key(roadmap_records, "roadmap_area").items():
            lines.append(f"- **{area}**: [{record.thread_title}]({record.url}) — {record.posted_by}, {record.date or 'Unknown'}, confidence {record.confidence}.")
        lines.append("")

        lines.append("## Unresolved contradictions")
        lines.append("")
        contradictions = collect_unresolved_contradictions(roadmap_records, "roadmap_area")
        if contradictions:
            for item in contradictions:
                lines.append(f"- {item}")
        else:
            lines.append("- No obvious contradictions were detected automatically; review medium-confidence entries manually.")
        lines.append("")

        lines.append("## Places the forum suggests code changes, tune changes, test additions, or documentation updates")
        lines.append("")
        for hint in roadmap_action_hints(roadmap_records):
            lines.append(f"- {hint}")
        lines.append("")

    if decoder_records:
        lines.append("## Trigger decoder and sync-behavior evidence")
        lines.append("")
        current_decoder = None
        for record in decoder_records:
            if record.decoder_pattern != current_decoder:
                current_decoder = record.decoder_pattern
                lines.append(f"### {current_decoder}")
                lines.append("")
            lines.extend(
                [
                    f"#### {record.thread_title}",
                    f"- URL: {record.url}",
                    f"- Date: {record.date or 'Unknown'}",
                    f"- Who posted it: {record.posted_by}",
                    f"- Evidence type: {record.evidence_type}",
                    f"- Confidence: {record.confidence}",
                    f"- Summary: {record.summary}",
                    f"- Why it matters: {record.why_it_matters}",
                    f"- Replay-test details: {record.specific_test_details or 'None extracted'}",
                    (
                        "- Matched search terms: "
                        + ", ".join(record.source_queries)
                        if record.source_queries
                        else "- Matched search terms: None retained"
                    ),
                    "",
                ]
            )

        lines.append("## Strongest evidence by decoder")
        lines.append("")
        for decoder, record in strongest_by_key(decoder_records, "decoder_pattern").items():
            lines.append(f"- **{decoder}**: [{record.thread_title}]({record.url}) — {record.posted_by}, {record.date or 'Unknown'}, confidence {record.confidence}.")
        lines.append("")

        safe, blocked, priorities = decoder_action_hints(decoder_records)

        lines.append("## Decoders with enough information to build replay tests safely")
        lines.append("")
        if safe:
            for decoder in safe:
                lines.append(f"- {decoder}")
        else:
            lines.append("- None reached the automatic safety threshold; inspect the highest-confidence trace-backed threads manually.")
        lines.append("")

        lines.append("## Decoders still blocked by missing real trace evidence")
        lines.append("")
        if blocked:
            for decoder in blocked:
                lines.append(f"- {decoder}")
        else:
            lines.append("- None identified as blocked by the heuristic.")
        lines.append("")

        lines.append("## Suggested next priorities for test coverage")
        lines.append("")
        for item in priorities:
            lines.append(f"- {item}")
        lines.append("")

    lines.append("## JSON export")
    lines.append("")
    lines.append(f"- Structured JSON dataset written to: `{output_json_path}`")
    lines.append("")

    return "\n".join(lines).strip() + "\n"


def save_json(path: str, roadmap_records: list[EvidenceRecord], decoder_records: list[EvidenceRecord]) -> None:
    payload = {
        "roadmap_records": [asdict(item) for item in roadmap_records],
        "decoder_records": [asdict(item) for item in decoder_records],
    }
    with open(path, "w", encoding="utf-8") as handle:
        json.dump(payload, handle, ensure_ascii=False, indent=2)


def build_roadmap_query_specs() -> list[tuple[str, str, list[str]]]:
    specs: list[tuple[str, str, list[str]]] = []
    for area in ROADMAP_AREAS:
        for term in area["search_terms"]:
            specs.append((area["name"], term, area["focus_terms"]))
    return specs


def build_filtered_roadmap_query_specs(
    selected_areas: Optional[list[str]] = None,
) -> list[tuple[str, str, list[str]]]:
    if not selected_areas:
        return build_roadmap_query_specs()
    allowed = set(selected_areas)
    return [spec for spec in build_roadmap_query_specs() if spec[0] in allowed]


def build_decoder_query_specs() -> list[tuple[str, str, list[str]]]:
    specs: list[tuple[str, str, list[str]]] = []
    for decoder in DECODER_FAMILIES:
        specs.append((decoder, f"{decoder} Speeduino trigger", [decoder]))
        for evidence_term in ["tooth logger", "composite logger", "scope", "oscilloscope", "sync", "crank", "cam", "trigger wheel", "CAS"]:
            specs.append((decoder, f"{decoder} {evidence_term}", [decoder, evidence_term]))
    return specs


def build_filtered_decoder_query_specs(
    selected_decoders: Optional[list[str]] = None,
) -> list[tuple[str, str, list[str]]]:
    if not selected_decoders:
        return build_decoder_query_specs()
    allowed = set(selected_decoders)
    return [spec for spec in build_decoder_query_specs() if spec[0] in allowed]


def collect_threads_for_mode(
    session: requests.Session,
    query_specs: list[tuple[str, str, list[str]]],
    search_engine: str,
    delay: float,
    per_query_limit: int,
    limit_results: int,
    failure_threshold: int = DEFAULT_ENGINE_FAILURE_THRESHOLD,
) -> list[tuple[str, ThreadData]]:
    hits = run_searches(
        session,
        query_specs,
        search_engine,
        delay,
        per_query_limit,
        failure_threshold,
    )
    if limit_results > 0:
        hits = hits[:limit_results]

    focus_by_url: dict[str, set[str]] = defaultdict(set)
    labels_by_url: dict[str, set[str]] = defaultdict(set)
    queries_by_url: dict[str, set[str]] = defaultdict(set)
    for hit in hits:
        key = canonicalize_url(hit.url)
        focus_terms = next(
            (
                spec_focus_terms
                for label, query, spec_focus_terms in query_specs
                if label == hit.label and query == hit.query
            ),
            [],
        )
        focus_by_url[key].update(focus_terms)
        labels_by_url[key].add(hit.label)
        queries_by_url[key].add(hit.query)

    threads: list[tuple[str, ThreadData]] = []
    for thread_url in sorted(focus_by_url):
        try:
            labels = sorted(labels_by_url[thread_url])
            thread = collect_thread(
                session,
                thread_url,
                delay,
                sorted(focus_by_url[thread_url]),
                sorted(queries_by_url[thread_url]),
                labels,
            )
            for label in labels:
                threads.append((label, thread))
        except Exception as exc:
            log(f"[warn] failed collecting thread {thread_url}: {exc}")

    return threads


def main() -> int:
    parser = argparse.ArgumentParser(description="Collect high-signal Speeduino forum evidence into Markdown and JSON.")
    parser.add_argument("--mode", choices=["roadmap", "decoder", "both"], default="both")
    parser.add_argument("--output-markdown", default="speeduino_forum_evidence.md")
    parser.add_argument("--output-json", default="speeduino_forum_evidence.json")
    parser.add_argument("--delay", type=float, default=DEFAULT_DELAY)
    parser.add_argument("--search-engine", choices=list(SEARCH_ENGINES), default=DEFAULT_SEARCH_ENGINE)
    parser.add_argument("--per-query-limit", type=int, default=5)
    parser.add_argument("--limit-results", type=int, default=0)
    parser.add_argument(
        "--engine-failure-threshold",
        type=int,
        default=DEFAULT_ENGINE_FAILURE_THRESHOLD,
        help=(
            "Disable a search engine for the rest of the run after this many consecutive "
            "query failures. Set to 0 to keep retrying forever."
        ),
    )
    parser.add_argument(
        "--roadmap-area",
        action="append",
        choices=[area["name"] for area in ROADMAP_AREAS],
        help="Restrict roadmap-mode scraping to one or more roadmap areas.",
    )
    parser.add_argument(
        "--decoder-family",
        action="append",
        choices=DECODER_FAMILIES,
        help="Restrict decoder-mode scraping to one or more decoder families.",
    )
    args = parser.parse_args()

    session = make_session()

    roadmap_records: list[EvidenceRecord] = []
    decoder_records: list[EvidenceRecord] = []

    if args.mode in {"roadmap", "both"}:
        roadmap_threads = collect_threads_for_mode(
            session=session,
            query_specs=build_filtered_roadmap_query_specs(args.roadmap_area),
            search_engine=args.search_engine,
            delay=args.delay,
            per_query_limit=args.per_query_limit,
            limit_results=args.limit_results,
            failure_threshold=args.engine_failure_threshold,
        )
        roadmap_records = build_roadmap_records(roadmap_threads)

    if args.mode in {"decoder", "both"}:
        decoder_threads = collect_threads_for_mode(
            session=session,
            query_specs=build_filtered_decoder_query_specs(args.decoder_family),
            search_engine=args.search_engine,
            delay=args.delay,
            per_query_limit=args.per_query_limit,
            limit_results=args.limit_results,
            failure_threshold=args.engine_failure_threshold,
        )
        decoder_records = build_decoder_records(decoder_threads)

    output_json_path = resolve_output_path(args.output_json)
    output_markdown_path = resolve_output_path(args.output_markdown)
    save_json(str(output_json_path), roadmap_records, decoder_records)

    markdown = render_markdown(roadmap_records, decoder_records, str(output_json_path))
    with output_markdown_path.open("w", encoding="utf-8") as handle:
        handle.write(markdown)

    print(f"Saved Markdown to {output_markdown_path}")
    print(f"Saved JSON to {output_json_path}")
    print(f"Roadmap records: {len(roadmap_records)}")
    print(f"Decoder records: {len(decoder_records)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
