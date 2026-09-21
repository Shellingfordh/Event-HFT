#!/usr/bin/env python3
import json, os, time, urllib.request, urllib.error
from pathlib import Path

TOKEN=os.getenv('X_BEARER_TOKEN','')
URL=os.getenv('X_STREAM_URL','https://api.x.com/2/tweets/search/stream')
OUT=Path('data/x_raw.jsonl'); OUT.parent.mkdir(parents=True,exist_ok=True)
if not TOKEN: raise SystemExit('X_BEARER_TOKEN is required')

def headers(): return {'Authorization':f'Bearer {TOKEN}','User-Agent':'event-hft-v5'}

def stream():
    req=urllib.request.Request(URL+'?tweet.fields=id,text,author_id,created_at,public_metrics&expansions=author_id&user.fields=username,public_metrics,verified',headers=headers())
    while True:
        try:
            with urllib.request.urlopen(req, timeout=90) as r:
                for line in r:
                    if not line.strip(): continue
                    raw=json.loads(line)
                    raw['_received_ns']=time.time_ns()
                    with OUT.open('a') as f: f.write(json.dumps(raw,separators=(',',':'))+'\n')
                    print(json.dumps(raw,ensure_ascii=False))
        except (urllib.error.URLError, TimeoutError, ConnectionError) as ex:
            print('stream reconnect:',ex); time.sleep(2)

if __name__=='__main__': stream()
