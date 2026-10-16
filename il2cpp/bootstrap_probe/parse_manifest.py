# Parse SWGoH RawAssetManifest (protobuf) — minimal wire parser.
import struct, sys

def read_varint(b, i):
    val = 0; shift = 0
    while True:
        c = b[i]; i += 1
        val |= (c & 0x7F) << shift
        if not (c & 0x80):
            return val, i
        shift += 7

def parse_fields(b):
    i = 0; out = []
    while i < len(b):
        key, i = read_varint(b, i)
        fn, wt = key >> 3, key & 7
        if wt == 0:
            v, i = read_varint(b, i)
        elif wt == 2:
            ln, i = read_varint(b, i)
            v = b[i:i+ln]; i += ln
        elif wt == 5:
            v = b[i:i+4]; i += 4
        elif wt == 1:
            v = b[i:i+8]; i += 8
        else:
            raise ValueError(f"wt {wt} at {i}")
        out.append((fn, wt, v))
    return out

data = open(sys.argv[1], "rb").read()
top = parse_fields(data)
hdr = {}
records = []
for fn, wt, v in top:
    if wt == 0:
        hdr.setdefault(fn, v)
    elif wt == 2 and fn == 2:
        r = {}
        for f2, w2, v2 in parse_fields(v):
            if w2 == 0:
                r.setdefault(f2, v2)
            elif w2 == 2:
                r.setdefault(f2, []).append(v2)
        records.append(r)

print("header fields:", {k: v for k, v in hdr.items()})
print("record count:", len(records))
for r in records[:3]:
    print({k: (v if not isinstance(v, bytes) else v[:20]) for k, v in list(r.items())[:8]})

# aggregate stats + find interesting packages
import re
sizes = {}
names = []
for r in records:
    names.append(r.get(1, b"").decode("utf-8", "replace"))
print("first names:", names[:10])
print("total bytes:", sum(r.get(3, 0) for r in records))
picks = [n for n in names if re.search(r"font", n, re.I)]
print("font-ish:", picks[:20])
ui = [n for n in names if n.startswith(("commanderselectui", "ui.", "uiglobals", "splash"))]
print("ui-ish sample:", ui[:20])
