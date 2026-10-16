# -*- coding: utf-8 -*-
"""read_game_constants.py -- pull the data-driven GameConstants values out of a
RUNNING SWGoH client (local read of the user's own game session; the tool never
writes to the target, only ReadProcessMemory).

Why a scanner instead of a class-pointer walk: GameConstants..ctor (VA
0x1811EBF50) writes nine float constants at fixed instance offsets, so any live
instance carries a unique byte signature -- a window W with

    f32[W+0x24B4] == 100.0   UnitSpeedFillMax            (ctor imm 0x42C80000)
    f32[W+0x2308] == 100.0   UnitAttackCritRatingMod
    f32[W+0x22F0] == 100.0   UnitAbilityCritRatingMod
    f32[W+0x22F4] == 100.0   UnitArmorRatingMod
    f32[W+0x24E0] == 100.0   UnitSuppressionRatingMod
    f32[W+0x22F8] ==   1.0   UnitAttackCritChanceLevelDeltaMax
    f32[W+0x22E0] ==   1.0   UnitAbilityCritChanceLevelDeltaMax
    f32[W+0x2340] ==   1.0   UnitDamageModLevelDeltaMax
    f32[W+0x2334] ==   1.0   UnitDamageModEraLevelDeltaMax

(field name/offset pairs from il2cpp/dump/cs/il2cpp.cs line 234952 ff.; the
immediates from the ctor machine-code pass, BATTLE_SIM_INVENTORY.md).  Nine
exact f32 matches at those strides occur only in the object itself (plus any
memcpy'd copies, e.g. the ConfigEntry cache source) -- so we scan all committed
PRIVATE memory of the game, collect candidate bases, decode every scalar
GameConstants field from each, cluster candidates that agree, and emit the
consensus to catalog/game_constants_runtime.json.

The prize fields (hydrated from the server-sent ConfigEntry list at login, NOT
present in binary or assets -- BATTLE_SIM_INVENTORY.md "Data-driven
GameConstants: NOT on disk"):

    +0x2300 UnitAttackCritChancePerLevelDelta   +0x22E8 UnitAbilityCritChancePerLevelDelta
    +0x22FC UnitAttackCritChanceLevelDeltaMin   +0x22E4 UnitAbilityCritChanceLevelDeltaMin
    +0x2314 UnitBaseAttackCritChance            +0x230C UnitBaseAbilityCritChance
    +0x231C UnitBaseCritDamage
    +0x2348 UnitDamageModPerLevelDelta          +0x2344 UnitDamageModLevelDeltaMin
    +0x233C UnitDamageModPerEraLevelDelta       +0x2338 UnitDamageModEraLevelDeltaMin
    +0x1C34 ShipMitigationAttackerLevelOffset   (int; shares duty as unit
                                                 mitigation level offset)

Usage (while the game runs):
    py il2cpp/tools/read_game_constants.py
    py il2cpp/tools/read_game_constants.py --process SWGoH.exe --out catalog/game_constants_runtime.json
    py il2cpp/tools/read_game_constants.py --all-memory          # also scan mapped/image regions
    py il2cpp/tools/read_game_constants.py --dll GameAssembly.dll  # re-derive anchors from the ctor
    py il2cpp/tools/read_game_constants.py --selftest            # dry-run: plant a synthetic
                                                                 # GameConstants in OUR process,
                                                                 # scan ourselves, verify round-trip

Exit codes: 0 ok | 2 game not running | 3 no candidates | 4 candidates but
data-driven fields still zero (pre-login hydration) | 5 selftest failed.
"""
from __future__ import annotations

import argparse
import ctypes
import json
import os
import re
import struct
import sys
import time
from dataclasses import dataclass
from datetime import datetime, timezone

TOOL_PATH = os.path.abspath(__file__)            # il2cpp/tools/read_game_constants.py
REPO_ROOT = os.path.dirname(os.path.dirname(os.path.dirname(TOOL_PATH)))
IL2CPP_CS = os.path.join(REPO_ROOT, "il2cpp", "dump", "cs", "il2cpp.cs")
DEFAULT_OUT = os.path.join(REPO_ROOT, "catalog", "game_constants_runtime.json")

import numpy as np  # noqa: E402  (above paths fixed first)

# ----------------------------------------------------------------- field table
FIELD_RE = re.compile(r"//\s*0x([0-9A-Fa-f]+)\s*$")
SCALAR_TYPES = {
    "bool":   (1, None),
    "byte":   (1, "<B"),
    "sbyte":  (1, "<b"),
    "short":  (2, "<h"),
    "ushort": (2, "<H"),
    "int":    (4, "<i"),
    "uint":   (4, "<I"),
    "float":  (4, "<f"),
    "long":   (8, "<q"),
    "ulong":  (8, "<Q"),
}


@dataclass
class GCField:
    name: str
    offset: int
    type: str
    is_static: bool
    is_readonly: bool


def parse_game_constants_fields(path: str) -> list[GCField]:
    """Extract the GameConstants field table (name, offset, type) from the
    Il2CppInspectorRedux dump -- the offsets in the `// 0x...` comments are
    metadata-derived and authoritative."""
    with open(path, "r", encoding="utf-8", errors="replace") as fh:
        lines = fh.read().splitlines()
    start = None
    for i, line in enumerate(lines):
        if re.match(r"\s*public class GameConstants\s*:", line):
            start = i
            break
    if start is None:
        raise RuntimeError("GameConstants class not found in " + path)
    fields: list[GCField] = []
    depth = 0
    started = False
    for line in lines[start:]:
        if not started:
            if "{" in line:
                started = True
                depth = line.count("{") - line.count("}")
            continue
        m = FIELD_RE.search(line)
        if m and ";" in line:
            offset = int(m.group(1), 16)
            decl = line.split(";")[0].strip()
            toks = decl.split()
            if len(toks) >= 2:
                name = toks[-1]
                type = toks[-2]
                is_static = "static" in toks
                is_readonly = "readonly" in toks
                fields.append(GCField(name, offset, type, is_static, is_readonly))
        depth += line.count("{") - line.count("}")
        if started and depth <= 0:
            break
    if not fields:
        raise RuntimeError("GameConstants field table came out empty")
    return fields


# --------------------------------------------------------------- ctor knowledge
# Hardcoded by GameConstants..ctor (0x1811EBF50) per the machine-code pass
# documented in il2cpp/BATTLE_SIM_INVENTORY.md DECODE STATUS.  The data-driven
# Min/PerLevel/Base* fields are deliberately NOT here (not in the binary).
CTOR_ANCHOR_NAMES = [
    ("UnitSpeedFillMax",                  100.0),
    ("UnitAttackCritRatingMod",           100.0),
    ("UnitAbilityCritRatingMod",          100.0),
    ("UnitArmorRatingMod",                100.0),
    ("UnitSuppressionRatingMod",          100.0),
    ("UnitAttackCritChanceLevelDeltaMax",   1.0),
    ("UnitAbilityCritChanceLevelDeltaMax",  1.0),
    ("UnitDamageModLevelDeltaMax",          1.0),
    ("UnitDamageModEraLevelDeltaMax",       1.0),
]
# Server-hydrated fields -- the prize.
PRIZE_NAMES = [
    "UnitAttackCritChancePerLevelDelta",
    "UnitAbilityCritChancePerLevelDelta",
    "UnitAttackCritChanceLevelDeltaMin",
    "UnitAbilityCritChanceLevelDeltaMin",
    "UnitBaseAttackCritChance",
    "UnitBaseAbilityCritChance",
    "UnitBaseCritDamage",
    "UnitDamageModPerLevelDelta",
    "UnitDamageModLevelDeltaMin",
    "UnitDamageModPerEraLevelDelta",
    "UnitDamageModEraLevelDeltaMin",
    "ShipMitigationAttackerLevelOffset",
]
# Window dumped as floats+ints into the JSON for cross-checking.
DUMP_LO, DUMP_HI = 0x2200, 0x2500


def build_anchor_list(fields_by_name, ctor_written=None):
    """anchors = [(offset, float_value), ...].  Default: the nine documented
    ctor immediates.  If `ctor_written` (offset -> imm32 bits, from a real
    GameAssembly.dll) is given, every ctor-written *float* field with a nonzero
    imm inside the battle window joins the signature."""
    anchors = []
    for name, val in CTOR_ANCHOR_NAMES:
        f = fields_by_name.get(name)
        if f is None:
            raise RuntimeError(f"anchor field {name} missing from field table")
        if f.type != "float":
            raise RuntimeError(f"anchor field {name} is {f.type}, expected float")
        anchors.append((f.offset, float(val)))
    if ctor_written:
        extra = []
        for off, bits in sorted(ctor_written.items()):
            f = fields_by_offset.get(off)
            if f is None or f.type != "float" or bits == 0:
                continue
            val = struct.unpack("<f", struct.pack("<I", bits))[0]
            if np.isfinite(val) and val != 0.0:
                extra.append((off, val))
        known = {o for o, _ in anchors}
        anchors += [a for a in extra if a[0] not in known]
    anchors.sort()
    return anchors


# -------------------------------------------------------------- process helpers
PROCESS_QUERY_INFORMATION = 0x0400
PROCESS_VM_READ = 0x0010
MEM_COMMIT = 0x1000
MEM_PRIVATE = 0x20000
PAGE_GUARD = 0x100
READABLE_PROTECT = {0x02, 0x04, 0x08, 0x20, 0x40, 0x80}  # R, RW, WCOPY, ER, ERW, EWCOPY

_k32 = ctypes.WinDLL("kernel32", use_last_error=True)
_k32.OpenProcess.argtypes = (ctypes.c_uint32, ctypes.c_int, ctypes.c_uint32)
_k32.OpenProcess.restype = ctypes.c_void_p
_k32.CloseHandle.argtypes = (ctypes.c_void_p,)
_k32.VirtualQueryEx.argtypes = (ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t)
_k32.VirtualQueryEx.restype = ctypes.c_size_t
_k32.ReadProcessMemory.argtypes = (ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p,
                                   ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t))
_k32.ReadProcessMemory.restype = ctypes.c_int
_k32.GetCurrentProcess.argtypes = ()
_k32.GetCurrentProcess.restype = ctypes.c_void_p


class MBI64(ctypes.Structure):
    """Native-pointer-size MEMORY_BASIC_INFORMATION.  NOTE: on x64,
    VirtualQueryEx fills THIS layout (48 bytes), not the DWORD64-wide
    MEMORY_BASIC_INFORMATION64 -- verified empirically: the RegionSize of the
    null-page free region (0x7ffe0000) showed up at the MBI offset 24."""
    _fields_ = [
        ("BaseAddress", ctypes.c_uint64),
        ("AllocationBase", ctypes.c_uint64),
        ("AllocationProtect", ctypes.c_uint32),
        ("_pad1", ctypes.c_uint32),
        ("RegionSize", ctypes.c_uint64),
        ("State", ctypes.c_uint32),
        ("Protect", ctypes.c_uint32),
        ("Type", ctypes.c_uint32),
        ("_pad2", ctypes.c_uint32),
    ]


def open_process(pid: int) -> int:
    h = _k32.OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, False, pid)
    if not h:
        raise OSError(f"OpenProcess({pid}) failed: {ctypes.get_last_error()}")
    return h


def iter_regions(hproc: int, max_addr: int = 0x7FFFFFFF0000):
    addr = 0
    while addr < max_addr:
        mbi = MBI64()
        got = _k32.VirtualQueryEx(hproc, ctypes.c_void_p(addr), ctypes.byref(mbi), ctypes.sizeof(mbi))
        if not got:
            addr += 0x1000
            continue
        base, size = mbi.BaseAddress, mbi.RegionSize
        if size == 0:
            break
        yield base, size, mbi.State, mbi.Protect, mbi.Type
        addr = base + size


def read_memory(hproc: int, addr: int, size: int) -> bytes:
    buf = ctypes.create_string_buffer(size)
    got = ctypes.c_size_t(0)
    ok = _k32.ReadProcessMemory(hproc, ctypes.c_void_p(addr), buf, ctypes.c_size_t(size), ctypes.byref(got))
    if not ok:
        return b""
    return buf.raw[: got.value]


def read_window(hproc: int, addr: int, size: int) -> bytes:
    """Small read that raises (used for candidate windows -- must succeed)."""
    data = read_memory(hproc, addr, size)
    if len(data) != size:
        raise OSError(f"short read at 0x{addr:X} ({len(data)}/{size})")
    return data


def find_process_ids(name: str) -> list[int]:
    """Enumerate PIDs by image name via CreateToolhelp32Snapshot."""
    TH32CS_SNAPPROCESS = 0x2

    class PROCESSENTRY32W(ctypes.Structure):
        _fields_ = [("dwSize", ctypes.c_uint32),
                    ("cntUsage", ctypes.c_uint32),
                    ("th32ProcessID", ctypes.c_uint32),
                    ("th32DefaultHeapID", ctypes.c_size_t),
                    ("th32ModuleID", ctypes.c_uint32),
                    ("cntThreads", ctypes.c_uint32),
                    ("th32ParentProcessID", ctypes.c_uint32),
                    ("pcPriClassBase", ctypes.c_long),
                    ("dwFlags", ctypes.c_uint32),
                    ("szExeFile", ctypes.c_wchar * 260)]

    k32 = _k32
    k32.CreateToolhelp32Snapshot.argtypes = (ctypes.c_uint32, ctypes.c_uint32)
    k32.CreateToolhelp32Snapshot.restype = ctypes.c_void_p
    k32.Process32FirstW.argtypes = (ctypes.c_void_p, ctypes.POINTER(PROCESSENTRY32W))
    k32.Process32FirstW.restype = ctypes.c_int
    k32.Process32NextW.argtypes = (ctypes.c_void_p, ctypes.POINTER(PROCESSENTRY32W))
    k32.Process32NextW.restype = ctypes.c_int
    snap = k32.CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)
    if not snap or snap == ctypes.c_void_p(-1).value:
        raise OSError("CreateToolhelp32Snapshot failed")
    pids = []
    entry = PROCESSENTRY32W()
    entry.dwSize = ctypes.sizeof(entry)
    if k32.Process32FirstW(snap, ctypes.byref(entry)):
        while True:
            if entry.szExeFile.lower() == name.lower():
                pids.append(entry.th32ProcessID)
            if not k32.Process32NextW(snap, ctypes.byref(entry)):
                break
    _k32.CloseHandle(snap)
    return pids


# ------------------------------------------------------------------- scanning
CHUNK = 16 * 1024 * 1024


def match_mask(buf: bytes, window: int, anchors) -> np.ndarray:
    """Boolean mask over 4-aligned candidate positions.  Index t of the mask
    corresponds to candidate base byte_pos = 4*t; anchor (off, val) tests
    f32[byte_pos + off] == val.  All anchor offsets must be 4-aligned (they
    are: il2cpp float fields are 4-aligned)."""
    n = len(buf)
    n4 = (n // 4) * 4
    m = (n4 - window) // 4 + 1
    if m <= 0:
        return np.zeros(0, dtype=bool)
    a = np.frombuffer(buf, dtype=np.uint8, count=n4)
    f = a.view("<f4")            # f[k] is the dword at byte 4k
    mask = np.ones(m, dtype=bool)
    for off, val in anchors:
        assert off % 4 == 0, f"anchor offset 0x{off:X} not 4-aligned"
        shift = off >> 2
        lim = min(len(f) - shift, m)
        if lim <= 0:
            return np.zeros(m, dtype=bool)
        cond = np.zeros(m, dtype=bool)
        cond[:lim] = f[shift: shift + lim] == np.float32(val)
        mask &= cond
        if not mask.any():
            return mask
    return mask


def scan_process(hproc: int, window: int, anchors, all_memory=False, log=print) -> list[int]:
    """Return deduped absolute base addresses of matching candidate windows."""
    candidates: list[tuple[int, int]] = []
    seen = set()
    bytes_read = 0
    regions_scanned = 0
    overlap = window + 8
    for base, size, state, protect, mtype in iter_regions(hproc):
        if state != MEM_COMMIT:
            continue
        if protect & PAGE_GUARD or protect == 0 or protect not in READABLE_PROTECT:
            continue
        if not all_memory and mtype != MEM_PRIVATE:
            continue
        regions_scanned += 1
        pos = 0
        while pos < size:
            take = min(CHUNK, size - pos)
            data = read_memory(hproc, base + pos, take)
            if len(data) < window:
                break
            mask = match_mask(data, window, anchors)
            for t in np.nonzero(mask)[0]:
                byte_pos = int(t) * 4
                if byte_pos % 8:
                    continue
                abs_base = base + pos + byte_pos
                if abs_base not in seen:
                    seen.add(abs_base)
                    candidates.append(abs_base)
            bytes_read += len(data)
            if len(data) < take:
                break
            if pos + take >= size:
                break
            pos += max(1, take - overlap)
    log(f"  regions scanned: {regions_scanned}, bytes read: {bytes_read / 1e6:.1f} MB, "
        f"candidate windows: {len(candidates)}")
    return candidates


# ------------------------------------------------------------------- decoding
def decode_fields(buf: bytes, fields: list[GCField]) -> dict:
    """Decode every scalar instance field; returns name -> dict."""
    out = {}
    for f in fields:
        if f.is_static or f.offset < 0x10:
            continue
        spec = SCALAR_TYPES.get(f.type)
        if spec is None:
            continue
        size, fmt = spec
        end = f.offset + size
        if end > len(buf):
            continue
        if f.type == "bool":
            raw = int(buf[f.offset])
            val = raw
            bits = raw
        else:
            val = struct.unpack_from(fmt, buf, f.offset)[0]
            bits = val if size == 4 else (val & 0xFFFFFFFFFFFFFFFF)
            if f.type == "float":
                bits = struct.unpack_from("<I", buf, f.offset)[0]
        out[f.name] = {"offset": f.offset, "type": f.type, "value": val, "bits": bits}
    return out


def cluster_key(decoded: dict) -> tuple:
    parts = []
    for name in PRIZE_NAMES:
        d = decoded.get(name)
        if d is None:
            parts.append(None)
        else:
            parts.append(d["bits"] if d["type"] == "float" else d["value"])
    return tuple(parts)


def prize_all_zero(decoded: dict) -> bool:
    for name in PRIZE_NAMES:
        d = decoded.get(name)
        if d is None:
            continue
        if d["type"] == "float":
            if d["bits"] != 0:
                return False
        elif d["value"] != 0:
            return False
    return True


def window_dump(decoded_buf: bytes) -> list:
    words = []
    for off in range(DUMP_LO, DUMP_HI, 4):
        bits = struct.unpack_from("<I", decoded_buf, off)[0]
        f32 = struct.unpack("<f", struct.pack("<I", bits))[0]
        i32 = struct.unpack("<i", struct.pack("<I", bits))[0]
        words.append([f"0x{off:X}", f"0x{bits:08X}",
                      float(f"{f32:.10g}") if np.isfinite(f32) else repr(f32), i32])
    return words


# ------------------------------------------------------------------- selftest
SELFTEST_PLANT = {
    # prize fields -- distinctive values so any offset confusion is caught
    "UnitAttackCritChancePerLevelDelta":   0.0315,
    "UnitAbilityCritChancePerLevelDelta":  0.0335,
    "UnitAttackCritChanceLevelDeltaMin":  -0.0115,
    "UnitAbilityCritChanceLevelDeltaMin": -0.0135,
    "UnitBaseAttackCritChance":            0.0525,
    "UnitBaseAbilityCritChance":           0.0545,
    "UnitBaseCritDamage":                  1.5125,
    "UnitDamageModPerLevelDelta":          0.0615,
    "UnitDamageModLevelDeltaMin":         -0.0715,
    "UnitDamageModPerEraLevelDelta":       0.0815,
    "UnitDamageModEraLevelDeltaMin":      -0.0915,
    # anchors must also be planted (they ARE the signature)
    "UnitSpeedFillMax":                    100.0,
    "UnitAttackCritRatingMod":             100.0,
    "UnitAbilityCritRatingMod":            100.0,
    "UnitArmorRatingMod":                  100.0,
    "UnitSuppressionRatingMod":            100.0,
    "UnitAttackCritChanceLevelDeltaMax":     1.0,
    "UnitAbilityCritChanceLevelDeltaMax":    1.0,
    "UnitDamageModLevelDeltaMax":            1.0,
    "UnitDamageModEraLevelDeltaMax":         1.0,
}
SELFTEST_PLANT_INT = {"ShipMitigationAttackerLevelOffset": 42}


def selftest(fields_by_name, window: int, anchors) -> int:
    """Plant a synthetic GameConstants in OUR process, scan ourselves with the
    production scanner, verify the planted values round-trip."""
    holder = {}  # keep alive

    def plant():
        buf = np.zeros(window, dtype=np.uint8)
        for name, val in SELFTEST_PLANT.items():
            f = fields_by_name[name]
            struct.pack_into("<f", buf, f.offset, val)  # numpy buffer OK for pack_into? -> use memoryview
        for name, val in SELFTEST_PLANT_INT.items():
            f = fields_by_name[name]
            struct.pack_into("<i", buf, f.offset, val)
        holder["buf"] = buf
        return int(buf.__array_interface__["data"][0])

    addr = plant()
    print(f"[selftest] planted synthetic GameConstants at 0x{addr:X} in own process")
    hproc = _k32.GetCurrentProcess()
    cands = scan_process(hproc, window, anchors, all_memory=True)
    exact = [c for c in cands if c == addr]
    if not exact:
        print(f"[selftest] FAIL: planted window not found among candidates: "
              f"{[hex(c) for c in cands][:8]}")
        return 5
    decoded = decode_fields(bytes(holder["buf"]), list(fields_by_name.values()))
    bad = []
    for name, want in list(SELFTEST_PLANT.items()) + list(SELFTEST_PLANT_INT.items()):
        got = decoded[name]["value"]
        if abs(float(got) - float(want)) > 1e-6 * max(1.0, abs(float(want))):
            bad.append((name, want, got))
    if bad:
        print("[selftest] FAIL: decoded values mismatch:")
        for name, want, got in bad:
            print(f"   {name}: want {want}, got {got}")
        return 5
    # also verify the scanner decodes the buffer it READS (not just the local copy)
    readback = read_window(hproc, addr, window)
    decoded_rt = decode_fields(readback, list(fields_by_name.values()))
    bad_rt = [(n, w, decoded_rt[n]["value"]) for n, w in SELFTEST_PLANT.items()
              if abs(float(decoded_rt[n]["value"]) - float(w)) > 1e-6 * max(1.0, abs(float(w)))]
    if bad_rt:
        print("[selftest] FAIL: read-back mismatch:", bad_rt)
        return 5
    extra = [hex(c) for c in cands if c != addr]
    if extra:
        print(f"[selftest] note: extra candidates (expected: our own read-buffer copies): {extra[:4]}")
    print("[selftest] PASS: signature matched planted window; all planted fields round-tripped")
    return 0


# ---------------------------------------------------------------------- main
def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description="Extract data-driven GameConstants from a running SWGoH client.")
    ap.add_argument("--process", default="SWGoH.exe", help="target image name (default SWGoH.exe)")
    ap.add_argument("--out", default=DEFAULT_OUT, help="output JSON path")
    ap.add_argument("--all-memory", action="store_true", help="also scan mapped/image regions")
    ap.add_argument("--dll", default=None, help="GameAssembly.dll path: re-derive the ctor-immediate "
                                                "anchor set from the binary instead of the documented nine")
    ap.add_argument("--selftest", action="store_true", help="dry-run: scan our own process for a planted "
                                                            "synthetic GameConstants window")
    ap.add_argument("--quiet", action="store_true")
    args = ap.parse_args(argv)
    log = (lambda *a, **k: None) if args.quiet else print

    fields = parse_game_constants_fields(IL2CPP_CS)
    fields_by_name = {f.name: f for f in fields}
    global fields_by_offset
    fields_by_offset = {f.offset: f for f in fields}

    ctor_written = None
    if args.dll:
        ctor_written = extract_ctor_immediates(args.dll, 0x1811EBF50, log=log)
    anchors = build_anchor_list(fields_by_name, ctor_written)
    window = max(max(f.offset for f in fields) + 16, DUMP_HI + 16)

    log(f"GameConstants: {len(fields)} fields, instance window 0x{window:X} bytes, "
        f"{len(anchors)} signature anchors")
    if not args.quiet:
        for off, val in anchors:
            print(f"   0x{off:04X} {fields_by_offset[off].name:44s} == {val}")

    if args.selftest:
        return selftest(fields_by_name, window, anchors)

    pids = find_process_ids(args.process)
    result = {
        "tool": os.path.relpath(TOOL_PATH, REPO_ROOT).replace("\\", "/"),
        "generated_at_utc": datetime.now(timezone.utc).isoformat(),
        "target": {"process": args.process, "pids": pids, "all_memory": args.all_memory},
        "signature": {
            "anchors": [f"0x{off:X}=={val}" for off, val in anchors],
            "anchors_source": "ctor machine-code pass (BATTLE_SIM_INVENTORY.md)" if not ctor_written
                              else os.path.abspath(args.dll),
        },
        "status": "ok",
        "status_detail": "",
        "fields": {},
        "prize_offsets": [f"0x{fields_by_name[n].offset:X}" for n in PRIZE_NAMES if n in fields_by_name],
        "candidates": [],
    }

    if not pids:
        result["status"] = "game_not_running"
        result["status_detail"] = f"no process named {args.process}; launch + log into the game, then rerun"
        _write(result, args.out, log)
        return 2

    all_cands = []
    open_failures = 0
    for pid in pids:
        log(f"scanning pid {pid} ({args.process}) ...")
        try:
            hproc = open_process(pid)
        except OSError as e:
            log(f"  {e}")
            open_failures += 1
            continue
        try:
            for c in scan_process(hproc, window, anchors, args.all_memory, log):
                all_cands.append((pid, c))
        finally:
            _k32.CloseHandle(hproc)

    if not all_cands:
        result["status"] = "no_candidates"
        result["status_detail"] = ("signature not found in committed private memory"
                                   if open_failures < len(pids) else
                                   f"OpenProcess failed for all {len(pids)} pid(s) "
                                   f"(run from an elevated shell?)")
        _write(result, args.out, log)
        return 3

    # decode + cluster
    clusters: dict[tuple, list] = {}
    per_cand = []
    for pid, addr in all_cands:
        hproc = open_process(pid)
        try:
            buf = read_window(hproc, addr, window)
        except OSError as e:
            log(f"  candidate 0x{addr:X}: {e}")
            continue
        finally:
            _k32.CloseHandle(hproc)
        dec = decode_fields(buf, fields)
        key = cluster_key(dec)
        clusters.setdefault(key, []).append((pid, addr))
        per_cand.append((pid, addr, dec, buf))

    if not per_cand:
        result["status"] = "no_candidates"
        result["status_detail"] = "candidate windows found but none could be re-read intact"
        result["candidates"] = [{"pid": p, "address": f"0x{a:X}", "cluster_of_consensus": False}
                                for p, a in all_cands]
        _write(result, args.out, log)
        return 3

    best_key = max(clusters, key=lambda k: len(clusters[k]))
    agree = len(clusters[best_key])
    log(f"candidate clusters: {[len(v) for v in clusters.values()]} "
        f"(consensus: {agree} of {len(all_cands)})")
    consensus_pid, consensus_addr = clusters[best_key][0]
    consensus = next(d for p, a, d, b in per_cand if (p, a) == (consensus_pid, consensus_addr))
    consensus_buf = next(b for p, a, d, b in per_cand if (p, a) == (consensus_pid, consensus_addr))

    hydrated_zero = prize_all_zero(consensus)

    for f in fields:
        d = consensus.get(f.name)
        if d is None:
            continue
        off = d["offset"]
        val = d["value"]
        if d["type"] == "float":
            val = float(f"{val:.10g}") if np.isfinite(val) else str(val)
        result["fields"][f"0x{off:X}"] = {
            "name": f.name,
            "type": d["type"],
            "value": val,
            "bits": f"0x{d['bits']:08X}" if d["type"] == "float" else None,
            "source": "runtime-scan",
            "candidates_agree": agree,
            "clusters": sorted(len(v) for v in clusters.values()),
            "is_ctor_hardcoded": fields_by_offset.get(off) is not None and any(
                o == off for o, _ in CTOR_ANCHOR_NAMES),
            "prize": f.name in PRIZE_NAMES,
        }

    result["candidates"] = [
        {"pid": p, "address": f"0x{a:X}", "cluster_of_consensus": (p, a) in set(map(tuple, clusters[best_key]))}
        for p, a in all_cands
    ]
    result["consensus_candidate"] = {"pid": consensus_pid, "address": f"0x{consensus_addr:X}"}
    result["window_dump"] = {
        "pid": consensus_pid,
        "address": f"0x{consensus_addr:X}",
        "range": [f"0x{DUMP_LO:X}", f"0x{DUMP_HI:X}"],
        "words": window_dump(consensus_buf),
    }
    if hydrated_zero:
        result["status"] = "unhydrated"
        result["status_detail"] = ("signature found but every data-driven field reads zero -- "
                                   "the client has not hydrated the server config yet; log in and rerun")
        _write(result, args.out, log)
        return 4

    _write(result, args.out, log)
    _print_prize(consensus, fields_by_name, agree, log)
    return 0


def _print_prize(consensus, fields_by_name, agree, log):
    log("")
    log(f"data-driven GameConstants (consensus of {agree} candidate(s)):")
    for name in PRIZE_NAMES:
        d = consensus[name]
        val = d["value"]
        if d["type"] == "float":
            log(f"  +0x{d['offset']:04X} {name:40s} = {val:.10g}")
        else:
            log(f"  +0x{d['offset']:04X} {name:40s} = {val}")


def _write(result: dict, out: str, log) -> None:
    os.makedirs(os.path.dirname(os.path.abspath(out)), exist_ok=True)
    with open(out, "w", encoding="utf-8") as fh:
        json.dump(result, fh, indent=1, ensure_ascii=False)
    log(f"wrote {out}")


# ------------------------------------------------- optional ctor verification
def extract_ctor_immediates(dll_path: str, ctor_va: int, log=print) -> dict[int, int]:
    """Decode `C7 /0 imm32` (mov dword [reg+disp], imm32) writes in the ctor,
    keeping only displacements that are valid GameConstants field offsets.
    Offsets inside the ctor are object-relative, exactly what we need."""
    with open(dll_path, "rb") as fh:
        pe = fh.read(0x400)
        if pe[:2] != b"MZ":
            raise RuntimeError(f"{dll_path}: not a PE file")
        e_lfanew = struct.unpack_from("<I", pe, 0x3C)[0]
        fh.seek(e_lfanew)
        hdr = fh.read(0x400)
        if hdr[:4] != b"PE\0\0":
            raise RuntimeError(f"{dll_path}: bad PE header")
        image_base = struct.unpack_from("<Q", hdr, 24 + 24)[0]
        num_sections = struct.unpack_from("<H", hdr, 24 + 2)[0]
        opt_size = struct.unpack_from("<H", hdr, 24 + 16)[0]
        sect_off = 24 + 20 + opt_size
        fh.seek(sect_off)
        sections = []
        for _ in range(num_sections):
            s = fh.read(40)
            name = s[:8].rstrip(b"\0").decode(errors="replace")
            vsize, va, rsize, roff = struct.unpack("<IIII", s[8:24])
            sections.append((name, va, vsize, roff, rsize))
        rva = ctor_va - image_base
        file_off = None
        for name, va, vsize, roff, rsize in sections:
            if va <= rva < va + max(vsize, rsize):
                file_off = roff + (rva - va)
                break
        if file_off is None:
            raise RuntimeError(f"ctor VA 0x{ctor_va:X} (RVA 0x{rva:X}) not in any section")
        fh.seek(file_off)
        code = fh.read(0x4000)

    valid_offsets = set(fields_by_offset)
    out: dict[int, int] = {}
    i = 0
    n = len(code)
    while i < n - 7:
        if code[i] != 0xC7:
            i += 1
            continue
        modrm = code[i + 1]
        mod = modrm & 0xC0
        rm = modrm & 0x07
        reg = (modrm >> 3) & 0x07
        if reg != 0:
            i += 1
            continue
        if mod == 0x40 and rm != 4:            # [reg+disp8], imm32
            disp = struct.unpack_from("<b", code, i + 2)[0]
            imm_off = i + 3
        elif mod == 0x80 and rm != 4:          # [reg+disp32], imm32
            disp = struct.unpack_from("<i", code, i + 2)[0]
            imm_off = i + 6
        else:
            i += 1
            continue
        if imm_off + 4 > n:
            break
        if disp in valid_offsets:
            imm = struct.unpack_from("<I", code, imm_off)[0]
            out[disp] = imm
        i += 1
    log(f"ctor verification: decoded {len(out)} field writes from {os.path.basename(dll_path)}")
    return out


if __name__ == "__main__":
    sys.exit(main())
