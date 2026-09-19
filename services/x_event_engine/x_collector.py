#!/usr/bin/env python3
"""X/Twitter real-time event collector for the HFT event pipeline.

Uses the official X API v2 Filtered Stream. No credentials are stored in the repo.
Set X_BEARER_TOKEN in the environment before running.
"""
from __future__ import annotations
import json, os, sys, time, urllib.parse, urllib.request, urllib.error
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Any, List

API = "https://api.x.com/2/tweets/search/stream"
RULES = API + "/rules"

@dataclass
class Config:
    rules_file: str = "configs/x_rules.json"
    output_file: str = "data/x_events.jsonl"
    raw_file: str = "data/x_raw.jsonl"
    backfill_minutes: int = 5
    reconnect_initial: float = 1.0
    reconnect_max: float = 30.0
    request_timeout: int = 90


def load_config() -> Config:
    return Config(
        rules_file=os.getenv("X_RULES_FILE", "configs/x_rules.json"),
        output_file=os.getenv("X_EVENT_OUTPUT", "data/x_events.jsonl"),
        raw_file=os.getenv("X_RAW_OUTPUT", "data/x_raw.jsonl"),
        backfill_minutes=min(5, max(0, int(os.getenv("X_BACKFILL_MINUTES", "5")))),
    )


def token() -> str:
    v = os.getenv("X_BEARER_TOKEN", "").strip()
    if not v:
        raise SystemExit("X_BEARER_TOKEN is not set")
    return v


def request_json(url: str, method: str = "GET", payload: Any = None) -> Dict[str, Any]:
    body = None if payload is None else json.dumps(payload).encode()
    req = urllib.request.Request(url, data=body, method=method, headers={
        "Authorization": f"Bearer {token()}",
        "Content-Type": "application/json",
        "Accept": "application/json",
    })
    with urllib.request.urlopen(req, timeout=30) as r:
        return json.loads(r.read().decode())


def sync_rules(path: str) -> None:
    p = Path(path)
    rules = json.loads(p.read_text(encoding="utf-8"))
    desired = [{"value": x["value"], "tag": x.get("tag", "")}
               for x in rules.get("rules", []) if x.get("value")]
    current = request_json(RULES).get("data", [])
    current_ids = [x["id"] for x in current]
    if current_ids:
        request_json(RULES, "POST", {"delete": {"ids": current_ids}})
    if desired:
        request_json(RULES, "POST", {"add": desired})
    print(f"X rules synced: {len(desired)}", flush=True)


def classify(text: str, author: Dict[str, Any], matched: List[Dict[str, Any]]) -> Dict[str, Any]:
    t = text.lower()
    asset = "UNKNOWN"
    if any(k in t for k in ("bitcoin", "$btc", " btc ", "#btc")): asset = "BTC"
    elif any(k in t for k in ("ethereum", "$eth", " eth ", "#eth")): asset = "ETH"
    elif any(k in t for k in ("solana", "$sol", " sol ", "#sol")): asset = "SOL"

    positive = ("approved", "approval", "launch", "listed", "listing", "partnership", "adopt", "buy", "bullish", "breakout", "etf inflow")
    negative = ("hack", "exploit", "halt", "delist", "lawsuit", "ban", "sanction", "liquidation", "attack", "sell", "bearish")
    pos = sum(1 for k in positive if k in t)
    neg = sum(1 for k in negative if k in t)
    direction = 1 if pos > neg else -1 if neg > pos else 0
    sentiment = max(-1.0, min(1.0, (pos-neg)/3.0))
    novelty = 1.0 if len(matched) == 1 else 0.7
    relevance = 0.85 if asset != "UNKNOWN" else 0.35
    followers = author.get("public_metrics", {}).get("followers_count", 0) or 0
    credibility = min(1.0, 0.35 + (followers / 1_000_000.0) ** 0.5 * 0.65) if followers else 0.35
    impact = max(0.0, min(1.0, 0.45*relevance + 0.25*novelty + 0.30*credibility))
    return {
        "asset": asset, "direction": direction, "sentiment": sentiment,
        "relevance": relevance, "novelty": novelty, "credibility": credibility,
        "impact": impact, "matched_rules": matched,
    }


def normalize(payload: Dict[str, Any]) -> Dict[str, Any]:
    data = payload.get("data", {})
    includes = payload.get("includes", {})
    users = {u.get("id"): u for u in includes.get("users", [])}
    author = users.get(data.get("author_id"), {})
    rules = payload.get("matching_rules", [])
    cls = classify(data.get("text", ""), author, rules)
    return {
        "event_id": data.get("id"),
        "source": "x",
        "created_at": data.get("created_at"),
        "received_at_ns": time.time_ns(),
        "author_id": data.get("author_id"),
        "username": author.get("username"),
        "author": author,
        "text": data.get("text", ""),
        "event": cls,
        "raw_matching_rules": rules,
    }


def stream(cfg: Config) -> None:
    params = urllib.parse.urlencode({
        "tweet.fields": "id,author_id,created_at,text,public_metrics,entities,referenced_tweets",
        "expansions": "author_id",
        "user.fields": "id,username,name,verified,public_metrics,description",
        "backfill_minutes": str(cfg.backfill_minutes),
    })
    url = API + "?" + params
    Path(cfg.output_file).parent.mkdir(parents=True, exist_ok=True)
    Path(cfg.raw_file).parent.mkdir(parents=True, exist_ok=True)
    delay = cfg.reconnect_initial
    while True:
        try:
            req = urllib.request.Request(url, headers={"Authorization": f"Bearer {token()}", "Accept": "application/json"})
            print("connecting X filtered stream...", flush=True)
            with urllib.request.urlopen(req, timeout=cfg.request_timeout) as r, \
                 open(cfg.raw_file, "a", encoding="utf-8") as raw, \
                 open(cfg.output_file, "a", encoding="utf-8") as out:
                delay = cfg.reconnect_initial
                while True:
                    line = r.readline()
                    if not line:
                        raise ConnectionError("X stream closed")
                    if not line.strip():
                        continue
                    payload = json.loads(line.decode("utf-8"))
                    raw.write(json.dumps(payload, ensure_ascii=False) + "\n"); raw.flush()
                    if "data" in payload:
                        event = normalize(payload)
                        out.write(json.dumps(event, ensure_ascii=False) + "\n"); out.flush()
                        print(json.dumps({"id": event["event_id"], "asset": event["event"]["asset"], "impact": round(event["event"]["impact"],3)}, ensure_ascii=False), flush=True)
        except KeyboardInterrupt:
            print("stopped", flush=True); return
        except Exception as e:
            print(f"stream error: {e}; reconnect in {delay:.1f}s", file=sys.stderr, flush=True)
            time.sleep(delay); delay = min(cfg.reconnect_max, delay * 2)


def main() -> None:
    cfg = load_config()
    if len(sys.argv) > 1 and sys.argv[1] == "sync-rules":
        sync_rules(cfg.rules_file); return
    if len(sys.argv) > 1 and sys.argv[1] == "stream":
        stream(cfg); return
    print("Usage: python3 services/x_event_engine/x_collector.py sync-rules|stream")

if __name__ == "__main__": main()
