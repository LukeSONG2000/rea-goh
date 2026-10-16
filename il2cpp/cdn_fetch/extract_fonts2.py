# Extract real TTF/OTF binaries from Font objects (UnityPy 1.25.4 misparses the
# modern Font typetree, so scan the object's raw serialized bytes and validate
# candidates structurally: sfnt magic + table directory sanity, then trim to
# the true font size = max(table offset + table length)).
import hashlib, json, os, struct

import UnityPy

BASE = os.path.dirname(os.path.abspath(__file__))
BUNDLES = os.path.join(BASE, "bundles")
OUT = os.path.join(BASE, "..", "..", "catalog", "fonts_original")
os.makedirs(OUT, exist_ok=True)

MAGICS = (b"\x00\x01\x00\x00", b"OTTO", b"true", b"ttcf")
KNOWN_TAGS = {
    "cmap", "head", "hhea", "hmtx", "maxp", "name", "OS/2", "post", "cvt ",
    "fpgm", "glyf", "loca", "prep", "CFF ", "VORG", "EBDT", "EBLC", "gasp",
    "hdmx", "kern", "LTSH", "PCLT", "VDMX", "vhea", "vmtx", "BASE", "GDEF",
    "GPOS", "GSUB", "EBSC", "JSTF", "MATH", "CBDT", "CBLC", "COLR", "CPAL",
    "SVG ", "sbix", "acnt", "bdat", "bloc", "bsln", "cvar", "fdsc", "feat",
    "fmtx", "fvar", "gvar", "just", "lcar", "mort", "morx", "opbd", "prop",
    "trak", "Zapf", "Silf", "Glat", "Gloc", "Feat", "Sill", "woff", "DSIG",
}


def validate_and_trim(buf, pos):
    """buf[pos:] starts with an sfnt magic -> return trimmed font bytes or None."""
    if pos + 12 > len(buf):
        return None
    num_tables = struct.unpack_from(">H", buf, pos + 4)[0]
    if not (4 <= num_tables <= 500):
        return None
    end = pos + 12 + 16 * num_tables
    if end > len(buf):
        return None
    max_end = end
    tags = 0
    seen_head = False
    for i in range(num_tables):
        o = pos + 12 + 16 * i
        tag = buf[o:o + 4]
        off, ln = struct.unpack_from(">II", buf, o + 8)
        if tag.decode("latin-1", "replace") in KNOWN_TAGS:
            tags += 1
        if tag == b"head":
            seen_head = True
        if off > (1 << 28) or ln > (1 << 28):
            return None
        max_end = max(max_end, pos + off + ln)
    if not seen_head or tags < 4:
        return None
    if max_end > len(buf):
        max_end = len(buf)
    return bytes(buf[pos:max_end])


saved = {}
inventory = []
files = sorted(os.listdir(BUNDLES))
for fn in files:
    if not fn.endswith(".bundle"):
        continue
    pkg = fn[:-7]
    env = UnityPy.load(os.path.join(BUNDLES, fn))
    for o in env.objects:
        if o.type.name != "Font":
            continue
        try:
            f = o.read()
        except Exception as e:
            inventory.append({"bundle": pkg, "path_id": o.path_id, "error": str(e)})
            continue
        name = f.m_Name
        names = [x for x in (getattr(f, "m_FontNames", []) or [])]
        raw = o.get_raw_data() or b""
        found = []
        for pos in range(0, max(0, len(raw) - 12)):
            if raw[pos:pos + 4] in MAGICS:
                blob = validate_and_trim(raw, pos)
                if blob is not None and len(blob) >= 10240:
                    found.append(blob)
        entry = {"bundle": pkg, "path_id": o.path_id, "name": name,
                 "font_names": names, "fonts": []}
        for blob in found:
            ext = ".otf" if blob[:4] == b"OTTO" else ".ttf"
            digest = hashlib.md5(blob).hexdigest()[:8]
            out_name = f"{name}_{digest}{ext}"
            out_path = os.path.join(OUT, out_name)
            if not os.path.exists(out_path):
                with open(out_path, "wb") as fh:
                    fh.write(blob)
            saved[out_name] = len(blob)
            entry["fonts"].append({"file": out_name, "bytes": len(blob),
                                   "magic": blob[:4].hex(),
                                   "sha1_head": hashlib.sha1(blob).hexdigest()[:12]})
        inventory.append(entry)

with open(os.path.join(BASE, "font_extract_inventory.json"), "w", encoding="utf-8") as fh:
    json.dump(inventory, fh, indent=1)
print(json.dumps(saved, indent=1))
print("fonts saved:", len(saved))
