# -*- coding: utf-8 -*-
"""read_game_constants.py -- pull the data-driven GameConstants values out of a
RUNNING SWGoH client (local read of the user's own game session; the tool never
writes to the target, only ReadProcessMemory).

REWRITE 2 (2026-10-16) -- why the nine-anchor float signature died and what
replaced it:

  GameConstants..ctor (VA 0x1811EBF50) hardcodes 100.0/1.0 into nine fields,
  but at login the client hydrates the object from the server-sent ConfigEntry
  list (GameConstants._cachedConfigEntries @ +0x18, Protos.ConfigEntry,
  il2cpp.cs:129251 -- Key: string @0x10, Value: string @0x18, extensionObject
  @0x20; values are STRINGS, parsed by field type later).  Whenever a config
  value differs from the ctor immediate, hydration OVERWRITES the anchor
  float -- so the old nine-anchor signature no longer exists in a logged-in
  process (0 candidates over 1.65 GB, verified).  This version finds hydrated
  truth three ways and cross-checks them:

  S1  config-entry hunt -- sweep all committed memory for UTF-16LE occurrences
      of every GameConstants field name.  The raw metadata string pool yields
      exactly one header-less hit per key: those are DISCARDED by the
      object-header check (il2cpp System.String: int32 length @ obj+0x10,
      UTF-16 chars @ obj+0x14).  Surviving object-valid copies are GC heap
      strings; a second sweep finds 8-byte pointers TO each string base, each
      pointer site S is a ConfigEntry.Key field, so the ConfigEntry object is
      S-0x10 and its Value string (@+0x18) is read and parsed per target field
      type.  These are the literal server-sent strings -- highest trust.
  S1b instance walk    -- for every instance candidate found by S2, walk
      W+0x18 -> List<ConfigEntry> (_items @ List+0x10, _size @ List+0x18),
      array max_length @ items+0x18, elements @ items+0x20, and read every
      Key/Value pair directly.  Works even if the needle sweep misses a heap
      region, and independently validates that a candidate really is the
      hydrated GameConstants singleton.
  S2  relaxed instance -- no anchors.  A window W (object base, 8-aligned)
      where
        f32[W+0x230C] ~= f32[W+0x2314]   UnitBaseAbilityCritChance ~=
                                         UnitBaseAttackCritChance (both
                                         0.01..1.0, |diff| <= 1e-6)
        f32[W+0x2300] in 0.001..0.5      UnitAttackCritChancePerLevelDelta
        f32[W+0x22E4] <= f32[W+0x22F8]   ability delta min <= attack delta max
        f32[W+0x2344] <= f32[W+0x2340]   damage mod level min <= max
        f32[W+0x24B4] in 1..1000         UnitSpeedFillMax
      (--variant 2 drops only the equality, requiring both Base* chances in
      0.001..0.5, for the case where the server sent different values).
      Candidates are re-read + re-tested, decoded field-by-field (offsets from
      il2cpp/dump/cs/il2cpp.cs `// 0x...` comments, 2072 fields), clustered by
      prize-field bits, validated by the S1b walk and sanity gates.

Merge priority: S1b/S1 (server strings) > S2 consensus instance > ctor
hardcode > unknown.  Every field carries per-strategy provenance in the
output JSON, plus cross-check (S1 vs S2) and sanity-gate results.

Usage (while the game runs):
    py il2cpp/tools/read_game_constants.py
    py il2cpp/tools/read_game_constants.py --process SWGoH.exe --out catalog/game_constants_runtime.json
    py il2cpp/tools/read_game_constants.py --all-memory        # also mapped/image regions
    py il2cpp/tools/read_game_constants.py --variant 2         # drop the Base* equality
    py il2cpp/tools/read_game_constants.py --dll GameAssembly.dll  # annotate ctor hardcodes
    py il2cpp/tools/read_game_constants.py --selftest          # dry-run: plant a synthetic
                                                                # GameConstants + ConfigEntry list
                                                                # in OUR process, scan, verify
    py il2cpp/tools/read_game_constants.py --wait-minutes 60   # wait for relaunch+login

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
# documented in il2cpp/BATTLE_SIM_INVENTORY.md DECODE STATUS.  Used only as a
# provenance annotation now (hydration may have overwritten them).
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

# ---------------------------------------------------------------- S2 signature
# offsets used by the relaxed instance signature (see module docstring)
S2_OFF_BASE_AB   = 0x230C   # UnitBaseAbilityCritChance
S2_OFF_BASE_ATK  = 0x2314   # UnitBaseAttackCritChance
S2_OFF_ATK_PL    = 0x2300   # UnitAttackCritChancePerLevelDelta
S2_OFF_AB_MIN    = 0x22E4   # UnitAbilityCritChanceLevelDeltaMin
S2_OFF_ATK_MAX   = 0x22F8   # UnitAttackCritChanceLevelDeltaMax
S2_OFF_DM_MIN    = 0x2344   # UnitDamageModLevelDeltaMin
S2_OFF_DM_MAX    = 0x2340   # UnitDamageModLevelDeltaMax
S2_OFF_FILLMAX   = 0x24B4   # UnitSpeedFillMax


def instance_passes(buf: bytes, variant: int) -> bool:
    """Re-test the relaxed signature on a candidate window (pure struct --
    used to re-validate candidates after re-read, killing transient matches)."""
    if len(buf) < S2_OFF_FILLMAX + 4:
        return False
    g = lambda off: struct.unpack_from("<f", buf, off)[0]
    a, b = g(S2_OFF_BASE_AB), g(S2_OFF_BASE_ATK)
    if variant == 1:
        if not (abs(a - b) <= 1e-6 and 0.01 <= a <= 1.0):
            return False
    else:
        if not (0.001 <= a <= 0.5 and 0.001 <= b <= 0.5):
            return False
    if not (0.001 <= g(S2_OFF_ATK_PL) <= 0.5):
        return False
    if not g(S2_OFF_AB_MIN) <= g(S2_OFF_ATK_MAX):
        return False
    if not g(S2_OFF_DM_MIN) <= g(S2_OFF_DM_MAX):
        return False
    if not (1.0 <= g(S2_OFF_FILLMAX) <= 1000.0):
        return False
    return True


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


# -------------------------------------------------------- il2cpp object reads
def read_il2cpp_string(hproc: int, obj_addr: int, max_len: int = 65536):
    """System.String: klass @0x0, monitor @0x8, int32 length @0x10,
    UTF-16LE chars @0x14.  Returns (text, obj_addr) or None."""
    if not obj_addr or obj_addr < 0x10000 or obj_addr % 8:
        return None
    hdr = read_memory(hproc, obj_addr, 0x14)
    if len(hdr) < 0x14:
        return None
    (ln,) = struct.unpack_from("<I", hdr, 0x10)
    if ln > max_len:
        return None
    raw = read_memory(hproc, obj_addr + 0x14, ln * 2)
    if len(raw) < ln * 2:
        return None
    try:
        text = raw.decode("utf-16-le")
    except UnicodeDecodeError:
        return None
    if "\x00" in text or any(ord(c) < 0x20 for c in text):
        return None
    return text, obj_addr


def read_cstring(hproc: int, addr: int, max_len: int = 512):
    """Read a NUL-terminated UTF-8 C string (il2cpp metadata allocations)."""
    if not addr or addr < 0x10000:
        return None
    raw = read_memory(hproc, addr, max_len)
    if not raw:
        return None
    nul = raw.find(b"\x00")
    if nul < 0:
        nul = max_len
    try:
        text = raw[:nul].decode("utf-8")
    except UnicodeDecodeError:
        return None
    return text or None


def walk_config_list(hproc: int, inst_addr: int, key_names: set, cap: int = 200000):
    """S1b: GameConstants._cachedConfigEntries @ +0x18 is a List<ConfigEntry>.
    List layout: _items (T[]) @+0x10, _size (int32) @+0x18, _version @+0x1C.
    Array layout: max_length @+0x18, elements @+0x20.  ConfigEntry: klass @0,
    Key @+0x10, Value @+0x18.
    Returns (dict key->raw, diag).  diag["entry_klasses"] collects the klass
    pointers of live entries -- the trusted klass filter for the S1 hunt."""
    diag = {"list_ptr": 0, "items": 0, "count": 0, "max_length": 0, "read": 0,
            "known_keys": 0, "collisions": 0, "bad_entries": 0, "entry_klasses": set()}
    d = read_memory(hproc, inst_addr + 0x18, 8)
    if len(d) < 8:
        return None, diag
    (list_ptr,) = struct.unpack("<Q", d)
    diag["list_ptr"] = list_ptr
    if not list_ptr or list_ptr < 0x10000:
        return None, diag
    ld = read_memory(hproc, list_ptr, 0x20)
    if len(ld) < 0x20:
        return None, diag
    items = struct.unpack_from("<Q", ld, 0x10)[0]
    size = struct.unpack_from("<i", ld, 0x18)[0]
    diag["items"], diag["count"] = items, size
    if not items or items < 0x10000 or size < 0:
        return None, diag
    ad = read_memory(hproc, items, 0x20)
    if len(ad) < 0x20:
        return None, diag
    maxlen = struct.unpack_from("<Q", ad, 0x18)[0]
    diag["max_length"] = maxlen
    if maxlen < 0 or maxlen > cap:
        maxlen = cap
    n = min(size, maxlen, cap)
    if n <= 0:
        return None, diag
    ed = read_memory(hproc, items + 0x20, 8 * n)
    if len(ed) < 8 * n:
        return None, diag
    out: dict[str, str] = {}
    for i in range(n):
        (e,) = struct.unpack_from("<Q", ed, 8 * i)
        if not e or e < 0x10000:
            continue
        dd = read_memory(hproc, e, 0x28)
        diag["read"] += 1
        if len(dd) < 0x28:
            diag["bad_entries"] += 1
            continue
        # ConfigEntry: klass @0x00, pad @0x08, Key @0x10, Value @0x18
        klass, _pad, key_ptr, val_ptr = struct.unpack_from("<QQQQ", dd, 0x00)
        ks = read_il2cpp_string(hproc, key_ptr)
        if ks is None:
            diag["bad_entries"] += 1
            continue
        key = ks[0]
        if key in out:
            diag["collisions"] += 1
            continue
        vs = read_il2cpp_string(hproc, val_ptr)
        out[key] = vs[0] if vs is not None else ""
        if key in key_names:
            diag["known_keys"] += 1
            if klass:
                diag["entry_klasses"].add(klass)
    return out, diag


# ------------------------------------------------------------------- scanning
CHUNK = 16 * 1024 * 1024
OVERLAP = 0x8000   # 32 KB re-scan margin; > window(0x3398) and > needle length


def pack4(u: np.ndarray) -> np.ndarray:
    """Sliding 4-code-unit windows of a u16 array packed into u64 -- the
    multi-needle prefilter key."""
    a = u.astype(np.uint64)
    return a[:-3] | (a[1:-2] << np.uint64(16)) | (a[2:-1] << np.uint64(32)) | (a[3:] << np.uint64(48))


class Needles:
    """Every GameConstants instance-field name as a UTF-16 needle, bucketed by
    the first 4 code units so the sweep costs one searchsorted per position."""

    def __init__(self, names):
        self.names = sorted(set(names))
        self.u16 = {n: np.frombuffer(n.encode("utf-16-le"), dtype="<u2") for n in self.names}
        pmap: dict[int, list[str]] = {}
        for n in self.names:
            a = self.u16[n]
            p = int(a[0]) | (int(a[1]) << 16) | (int(a[2]) << 32) | (int(a[3]) << 48)
            pmap.setdefault(p, []).append(n)
        self.prefix_map = pmap
        self.prefix_keys = np.array(sorted(pmap), dtype=np.uint64)

    def lookup(self, w: np.ndarray) -> np.ndarray:
        if not len(self.prefix_keys):
            return np.zeros(0, dtype=np.intp)
        pos = np.searchsorted(self.prefix_keys, w)
        posc = np.clip(pos, 0, len(self.prefix_keys) - 1)
        hit = self.prefix_keys[posc] == w
        return np.nonzero(hit)[0]


def s2_mask(data: bytes, window: int, variant: int) -> np.ndarray:
    """Boolean mask over 8-aligned candidate bases satisfying the relaxed
    signature (vectorised)."""
    n = len(data)
    n4 = (n // 4) * 4
    m = (n4 - window) // 4 + 1
    if m <= 0:
        return np.zeros(0, dtype=bool)
    f = np.frombuffer(data, dtype="<f4", count=n4 // 4)

    def g(off):
        shift = off >> 2
        with np.errstate(all="ignore"):
            return f[shift: shift + m].astype(np.float64)

    ab, at = g(S2_OFF_BASE_AB), g(S2_OFF_BASE_ATK)
    if variant == 1:
        with np.errstate(all="ignore"):
            c1 = (np.abs(ab - at) <= 1e-6) & (ab >= 0.01) & (ab <= 1.0)
    else:
        c1 = (ab >= 0.001) & (ab <= 0.5) & (at >= 0.001) & (at <= 0.5)
    pl = g(S2_OFF_ATK_PL)
    mask = (c1
            & (pl >= 0.001) & (pl <= 0.5)
            & (g(S2_OFF_AB_MIN) <= g(S2_OFF_ATK_MAX))
            & (g(S2_OFF_DM_MIN) <= g(S2_OFF_DM_MAX))
            & (g(S2_OFF_FILLMAX) >= 1.0) & (g(S2_OFF_FILLMAX) <= 1000.0))
    return mask[:m]


def pass1_chunk(data: bytes, base: int, needles: Needles, window: int, variant: int):
    """One chunk: S2 instance mask + S1 needle hits.  Returns
    (instance_bases, string_hits) where string_hits items are
    (name, chars_addr, obj_base_or_None); obj_base None = header-less hit
    (metadata-pool char data, discarded)."""
    inst = []
    mask = s2_mask(data, window, variant)
    if len(mask):
        for t in np.nonzero(mask)[0]:
            off4 = int(t) * 4
            if off4 % 8 == 0:
                inst.append(base + off4)

    hits = []
    n2 = (len(data) // 2) * 2
    if n2 >= 8:
        u = np.frombuffer(data, dtype="<u2", count=n2 // 2)
        w = pack4(u)
        for t in needles.lookup(w):
            p = int(t)
            bucket = needles.prefix_map[int(w[t])]
            for name in bucket:
                L = len(name)
                if p + L > len(u):
                    continue
                if not np.array_equal(u[p:p + L], needles.u16[name]):
                    continue
                boff = p * 2
                obj_base = None
                if boff >= 0x14:
                    (ln,) = struct.unpack_from("<I", data, boff - 4)
                    if ln == L and (boff - 0x14) % 8 == 0:
                        obj_base = base + boff - 0x14
                hits.append((name, base + boff, obj_base))
    return inst, hits


def sweep(hproc: int, needles: Needles, window: int, variant: int,
          all_memory=False, want_strings=True, log=print):
    """Full sweep for S2 instance candidates (+ optionally S1 string hits).
    Returns (instance_bases, gc_string_bases{name:[obj_base]},
    pool_hit_counts{name:n}, stats)."""
    inst_seen, inst = set(), []
    gc: dict[str, list[int]] = {}
    pool: dict[str, int] = {}
    hit_seen = set()
    bytes_read = 0
    regions = 0
    for base, size, state, protect, mtype in iter_regions(hproc):
        if state != MEM_COMMIT:
            continue
        if protect & PAGE_GUARD or protect == 0 or protect not in READABLE_PROTECT:
            continue
        if not all_memory and mtype != MEM_PRIVATE:
            continue
        regions += 1
        pos = 0
        while pos < size:
            take = min(CHUNK, size - pos)
            data = read_memory(hproc, base + pos, take)
            if not data:
                break
            if len(data) >= window or (want_strings and len(data) >= 8):
                di, dh = pass1_chunk(data, base + pos, needles, window, variant)
                for a in di:
                    if a not in inst_seen:
                        inst_seen.add(a)
                        inst.append(a)
                if want_strings:
                    for name, chars_addr, obj_base in dh:
                        ck = (name, chars_addr)
                        if ck in hit_seen:
                            continue
                        hit_seen.add(ck)
                        if obj_base is None:
                            pool[name] = pool.get(name, 0) + 1
                        else:
                            lst = gc.setdefault(name, [])
                            if obj_base not in lst:
                                lst.append(obj_base)
            bytes_read += len(data)
            if len(data) < take:
                break
            if pos + take >= size:
                break
            pos += take if take <= OVERLAP else max(8, take - OVERLAP)
    stats = {"regions": regions, "bytes_read": bytes_read,
             "instance_candidates": len(inst)}
    return inst, gc, pool, stats


def pointer_sweep(hproc: int, targets: list[int], log=print):
    """Sweep for 8-byte pointer sites whose value is one of `targets`
    (GC string object bases).  Returns {target: [ptr_site, ...]}."""
    tset = np.array(sorted(set(targets)), dtype=np.uint64)
    out: dict[int, list[int]] = {t: [] for t in targets}
    bytes_read = 0
    regions = 0
    for base, size, state, protect, mtype in iter_regions(hproc):
        if state != MEM_COMMIT:
            continue
        if protect & PAGE_GUARD or protect == 0 or protect not in READABLE_PROTECT:
            continue
        if mtype != MEM_PRIVATE:
            continue
        regions += 1
        pos = 0
        while pos < size:
            take = min(CHUNK, size - pos)
            data = read_memory(hproc, base + pos, take)
            if not data:
                break
            n8 = (len(data) // 8) * 8
            if n8:
                v = np.frombuffer(data, dtype="<u8", count=n8 // 8)
                posn = np.searchsorted(tset, v)
                posc = np.clip(posn, 0, len(tset) - 1)
                hit = tset[posc] == v
                for t in np.nonzero(hit)[0]:
                    tgt = int(tset[int(posc[int(t)])])
                    site = base + pos + int(t) * 8
                    out[tgt].append(site)
            bytes_read += len(data)
            if len(data) < take:
                break
            if pos + take >= size:
                break
            pos += take if take <= OVERLAP else max(8, take - OVERLAP)
    log(f"  pointer sweep: {regions} regions, {bytes_read / 1e6:.1f} MB, "
        f"{sum(len(v) for v in out.values())} sites for {len(targets)} string base(s)")
    return out


# ------------------------------------------------ GameConstants klass discovery
def fieldinfo_chain(hproc: int, gc_bases: dict, fields_by_name: dict, log=print):
    """Discovery chain for the GameConstants Il2CppClass.

    Empirically (SWGoH live process, verified): each object-valid copy of a
    GameConstants field-name string is referenced by exactly one pointer site,
    and the qword BEFORE the site is an unmanaged il2cpp FieldInfo whose
    layout is {char* name @0, Il2CppType* type @8, Il2CppClass* parent @0x10,
    uint32 offset @0x18} -- parent == GameConstants class and offset == the
    field's il2cpp.cs offset.  Voting over all keys therefore yields the
    runtime class pointer AND per-field runtime offsets that independently
    cross-check the il2cpp.cs table.

    Returns (klass, offsets {name: runtime_offset}, diag)."""
    all_bases = sorted({lst[0] for lst in gc_bases.values() if lst})
    if not all_bases:
        return None, {}, {"sites": 0, "votes": 0, "offset_match": 0, "offset_mismatch": 0,
                          "checked": 0}
    sites = pointer_sweep(hproc, all_bases, log=log)
    base2name = {}
    for name, lst in gc_bases.items():
        for b in lst:
            base2name[b] = name
    votes: dict[int, int] = {}
    offsets: dict[str, int] = {}
    checked = match = mismatch = 0
    for b, site_list in sites.items():
        name = base2name.get(b)
        if name is None:
            continue
        for s in site_list:
            d8 = read_memory(hproc, s - 8, 8)
            if len(d8) < 8:
                continue
            (fi,) = struct.unpack("<Q", d8)
            if not fi or fi < 0x10000:
                continue
            fd = read_memory(hproc, fi, 0x20)
            if len(fd) < 0x20:
                continue
            name_ptr, _type_ptr, parent, off_packed = struct.unpack_from("<QQQQ", fd, 0)
            cname = read_cstring(hproc, name_ptr)
            if cname != name:
                continue
            if parent:
                votes[parent] = votes.get(parent, 0) + 1
            off = off_packed & 0xFFFFFFFF
            f = fields_by_name.get(name)
            if f is not None:
                checked += 1
                if off == f.offset:
                    match += 1
                    offsets[name] = off
                else:
                    mismatch += 1
    klass = None
    if votes:
        klass, nv = max(votes.items(), key=lambda kv: kv[1])
        log(f"  FieldInfo chain: {nv}/{sum(votes.values())} votes -> GameConstants klass "
            f"0x{klass:X}; runtime offsets vs il2cpp.cs: {match} match / {mismatch} mismatch "
            f"of {checked}")
    else:
        log("  FieldInfo chain: no parent votes (structure not found) -- "
            "falling back to loose signature only")
    diag = {"sites": sum(len(v) for v in sites.values()), "votes": sum(votes.values()),
            "offset_match": match, "offset_mismatch": mismatch, "checked": checked,
            "klass": hex(klass) if klass else None, "_sites": sites}
    return klass, offsets, diag


def klass_site_sweep(hproc: int, klass: int, log=print):
    """All 8-aligned sites whose qword == klass (object starts AND pointers to
    the class -- the plausibility filter sorts them out)."""
    return pointer_sweep(hproc, [klass], log=log).get(klass, [])


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
    for off in range(DUMP_LO, min(DUMP_HI, len(decoded_buf) - 4), 4):
        bits = struct.unpack_from("<I", decoded_buf, off)[0]
        f32 = struct.unpack("<f", struct.pack("<I", bits))[0]
        i32 = struct.unpack("<i", struct.pack("<I", bits))[0]
        words.append([f"0x{off:X}", f"0x{bits:08X}",
                      float(f"{f32:.10g}") if np.isfinite(f32) else repr(f32), i32])
    return words


# ---------------------------------------------------------------- value parse
def parse_config_value(raw: str, ftype: str):
    """ConfigEntry.Value is a STRING parsed per the target field's type.
    Returns (ok, value)."""
    s = (raw or "").strip()
    if not s:
        return False, None
    try:
        if ftype == "float":
            return True, float(s)
        if ftype == "bool":
            if s.lower() in ("true", "1"):
                return True, True
            if s.lower() in ("false", "0"):
                return True, False
            return False, None
        if ftype in ("int", "short", "sbyte", "long"):
            return True, int(s)
        if ftype in ("uint", "ushort", "byte", "ulong"):
            return True, int(s)
    except ValueError:
        return False, None
    return False, None  # non-numeric target type: raw string kept as-is


def sanity_check(name: str, ftype: str, val) -> str | None:
    """Return 'pass', 'FAIL: ...', or None when no rule applies.
    Gates (task mandate): deltas 0.001..0.5, bases 0..1, rating mods 50..500."""
    if not isinstance(val, (int, float)) or isinstance(val, bool):
        return None
    if "PerLevelDelta" in name and not name.endswith(("Min", "Max")):
        return "pass" if 0.001 <= val <= 0.5 else f"FAIL: PerLevelDelta {val} outside 0.001..0.5"
    if name.endswith("LevelDeltaMin"):
        return "pass" if -0.5 <= val <= 0.5 else f"FAIL: Min delta {val} outside -0.5..0.5"
    if name.endswith("LevelDeltaMax"):
        return "pass" if 0.1 <= val <= 2.0 else f"FAIL: Max delta {val} outside 0.1..2.0"
    if name.startswith("UnitBase") and "CritChance" in name:
        return "pass" if 0.0 <= val <= 1.0 else f"FAIL: base chance {val} outside 0..1"
    if name == "UnitBaseCritDamage":
        return "pass" if 1.0 <= val <= 3.0 else f"FAIL: crit damage {val} outside 1..3"
    if name in ("UnitAttackCritRatingMod", "UnitAbilityCritRatingMod", "UnitArmorRatingMod",
                "UnitSuppressionRatingMod", "UnitAttackCritNegateRatingMod",
                "UnitAbilityCritNegateRatingMod"):
        return "pass" if 50.0 <= val <= 500.0 else f"FAIL: rating mod {val} outside 50..500"
    if name == "UnitSpeedFillMax":
        return "pass" if 1.0 <= val <= 1000.0 else f"FAIL: speed fill max {val} outside 1..1000"
    if name == "ShipMitigationAttackerLevelOffset":
        return "pass" if -100 <= int(val) <= 100 else f"FAIL: mitigation offset {val} outside -100..100"
    return None


# ------------------------------------------------------------- instance stage
def validate_instance(hproc: int, addr: int, window: int, fields: list[GCField],
                      variant: int, key_names: set, klass_gc: int | None = None,
                      strict_monitor: bool = True, log=print):
    """Re-read a candidate, validate, decode fields, walk the config list.

    A candidate is klass-anchored when its first qword equals the runtime
    GameConstants class pointer (dispositive); otherwise it is loose-signature
    only and must additionally pass the relaxed signature.  Real objects also
    satisfy: _initialized/_hasOverridesApplied bools, null monitor (il2cpp
    objects carry a lock word that is 0 unless contended -- a pointer run of
    klass values has a non-zero 'monitor', so this kills pointer-to-class
    sites), and a sane UnitSpeedFillMax (ctor writes 100.0 before anything
    else, hydration keeps it in 1..1000) with finite prize floats.
    Returns dict or None."""
    try:
        buf = read_window(hproc, addr, window)
    except OSError as e:
        log(f"  candidate 0x{addr:X}: {e}")
        return None
    anchored = klass_gc is not None and struct.unpack_from("<Q", buf, 0)[0] == klass_gc
    if buf[0x10] > 1 or buf[0x20] > 1:
        return None  # _initialized/_hasOverridesApplied are bools
    monitor_zero = struct.unpack_from("<Q", buf, 0x08)[0] == 0
    if anchored:
        if strict_monitor and not monitor_zero:
            return None
        fm = struct.unpack_from("<f", buf, S2_OFF_FILLMAX)[0]
        if not (1.0 <= fm <= 1000.0):
            return None
        for n in PRIZE_NAMES:
            f = next((x for x in fields if x.name == n), None)
            if f is not None and f.type == "float":
                if not np.isfinite(struct.unpack_from("<f", buf, f.offset)[0]):
                    return None
    else:
        if not instance_passes(buf, variant):
            return None
        # plausibility: one strong scalar (blocks garbage windows)
        fm = struct.unpack_from("<f", buf, S2_OFF_FILLMAX)[0]
        if not (1.0 <= fm <= 1000.0):
            return None
    dec = decode_fields(buf, fields)
    cfg, diag = walk_config_list(hproc, addr, key_names)
    return {"addr": addr, "buf": buf, "decoded": dec, "config": cfg, "walk": diag,
            "anchored": anchored, "monitor_zero": monitor_zero}


# ------------------------------------------------------- S1 pointer-hunt stage
def hunt_config_entries(hproc: int, gc_bases: dict, fields_by_name: dict,
                        trusted_klasses: set | None = None, sites=None, log=print):
    """S1: for each GC key-string base find pointer sites (=ConfigEntry.Key
    fields), derive the entry object (site-0x10), read+parse its Value.
    Robustness (in priority order):
      1. if `trusted_klasses` is given (klass pointers of LIVE entries learned
         from the S1b object-graph walk), a candidate is only accepted when
         its klass @+0x00 is in that set -- stray references (stack slots,
         stale copies, unrelated structs that embed the pointer at +0x10)
         are rejected outright;
      2. otherwise every genuine ConfigEntry shares one modal klass: klass
         matches get 3 votes, the rest 1, and the weighted majority wins.
    `sites` may carry precomputed {base: [site, ...]} from the FieldInfo-chain
    pointer sweep (the same pass serves both consumers)."""
    all_bases = sorted({b for lst in gc_bases.values() for b in lst})
    if not all_bases:
        return {}
    if sites is None:
        log(f"  pointer-hunting {len(all_bases)} GC string base(s) ...")
        sites = pointer_sweep(hproc, all_bases, log=log)
    # (name, raw) -> {E: klass}
    cands: dict[tuple, dict] = {}
    for base, site_list in sites.items():
        for site in site_list:
            e = site - 0x10
            if e <= 0 or e % 8:
                continue
            dd = read_memory(hproc, e, 0x28)
            if len(dd) < 0x28:
                continue
            # ConfigEntry: klass @0x00, pad @0x08, Key @0x10, Value @0x18
            klass, _pad, key_ptr, val_ptr = struct.unpack_from("<QQQQ", dd, 0x00)
            if key_ptr != base:
                continue
            if trusted_klasses and klass not in trusted_klasses:
                continue
            vs = read_il2cpp_string(hproc, val_ptr)
            if vs is None:
                continue
            for name, lst in gc_bases.items():
                if base in lst:
                    cands.setdefault((name, vs[0]), {})[e] = klass

    # modal klass across all candidates (fallback filter)
    klass_votes: dict[int, int] = {}
    for d in cands.values():
        for k in d.values():
            klass_votes[k] = klass_votes.get(k, 0) + 1
    modal_klass = max(klass_votes, key=klass_votes.get) if klass_votes else None

    entries: dict[str, dict] = {}
    for (name, raw), d in cands.items():
        votes = sum(3 if k == modal_klass else 1 for k in d.values())
        rec = entries.setdefault(name, {})
        prev = rec.get(raw)
        if prev is None or votes > prev[0]:
            rec[raw] = [votes, sorted(d)]
        else:
            rec[raw][1] = sorted(set(rec[raw][1]) | set(d))
    out = {}
    for name, rec in entries.items():
        raw = max(rec, key=lambda r: rec[r][0])       # weighted majority
        votes, es = rec[raw]
        runner_up = sorted((rec[r][0] for r in rec if r != raw), reverse=True)
        out[name] = {"raw": raw, "votes": votes,
                     "entries": [hex(e) for e in es[:8]], "n_entries": len(es),
                     "runner_up_votes": runner_up[0] if runner_up else 0,
                     "raw_variants": sorted(rec),
                     "filter": "trusted-klass" if trusted_klasses else
                               (f"modal-klass {hex(modal_klass)}" if modal_klass is not None else "none")}
    if out:
        contested = sum(1 for r in out.values() if r["runner_up_votes"] * 2 > r["votes"])
        log(f"  hunt: {len(out)} key(s) resolved ({out[next(iter(out))]['filter']}, "
            f"contested: {contested})")
    return out


# ------------------------------------------------------------------- selftest
SELFTEST_FIELDS = {
    # prize fields -- planted in the synthetic instance; config-carried too
    "UnitAttackCritChancePerLevelDelta":   0.0315,
    "UnitAbilityCritChanceLevelDeltaMin": -0.0135,
    "UnitAttackCritChanceLevelDeltaMin": -0.0115,
    "UnitBaseAbilityCritChance":           0.0525,   # instance copy; config says 0.0545
    "UnitBaseAttackCritChance":            0.0525,
    "UnitBaseCritDamage":                  1.5125,
    "UnitDamageModPerLevelDelta":          0.0615,
    "UnitDamageModLevelDeltaMin":         -0.0715,
    # anchors + window shapers
    "UnitSpeedFillMax":                    100.0,
    "UnitArmorRatingMod":                  100.0,
    # distinctive non-config scalar (instance-only)
    "UnitStatAbilityPowerPerIntelligence": 2.71828,
}
SELFTEST_INT_FIELDS = {"ShipMitigationAttackerLevelOffset": 42}
SELFTEST_CONFIG = {
    "UnitAttackCritChancePerLevelDelta":   "0.0315",
    "UnitAbilityCritChanceLevelDeltaMin":  "-0.0135",
    "UnitAttackCritChanceLevelDeltaMin":   "-0.0115",
    "UnitBaseAbilityCritChance":           "0.0545",  # differs from instance copy on purpose
    "UnitBaseAttackCritChance":            "0.0525",
    "UnitBaseCritDamage":                  "1.5125",
    "UnitDamageModPerLevelDelta":          "0.0615",
    "UnitDamageModLevelDeltaMin":          "-0.0715",
    "UnitSpeedFillMax":                    "100.0",
    "UnitArmorRatingMod":                  "100.0",
    "ShipMitigationAttackerLevelOffset":   "42",
}


def _addr(buf) -> int:
    return int(buf.__array_interface__["data"][0])


def _plant_string(text: str) -> np.ndarray:
    raw = text.encode("utf-16-le")
    buf = np.zeros(0x14 + len(raw), dtype=np.uint8)
    struct.pack_into("<I", buf, 0x10, len(text))
    buf[0x14: 0x14 + len(raw)] = np.frombuffer(raw, dtype=np.uint8)
    return buf


def selftest(fields_by_name, fields, window, needles, log=print) -> int:
    """Plant a synthetic GameConstants + List<ConfigEntry> + GC strings + a
    metadata-pool decoy + a FieldInfo discovery chain in OUR process, run the
    production pipeline against ourselves, verify the round trip."""
    holder = []  # keep-alive
    PLANTED_KLASS_GC = 0x00007FF6ABCE0000   # fake GameConstants class pointer

    # -- GC key/value strings + ConfigEntry objects + List + instance
    key_bufs, val_bufs, entry_bufs = {}, {}, {}
    PLANTED_KLASS = 0x00007FF6ABCD0000   # consistent fake ConfigEntry klass
    for key, raw in SELFTEST_CONFIG.items():
        kb = _plant_string(key)
        vb = _plant_string(raw)
        eb = np.zeros(0x28, dtype=np.uint8)
        struct.pack_into("<Q", eb, 0x00, PLANTED_KLASS)
        struct.pack_into("<Q", eb, 0x10, _addr(kb))
        struct.pack_into("<Q", eb, 0x18, _addr(vb))
        key_bufs[key], val_bufs[key] = kb, vb
        entry_bufs[key] = eb
        holder += [kb, vb, eb]

    n = len(entry_bufs)
    list_buf = np.zeros(0x20, dtype=np.uint8)   # List<ConfigEntry>: _items@0x10,_size@0x18
    items_buf = np.zeros(0x20 + 8 * n, dtype=np.uint8)  # array: max_length@0x18, data@0x20
    struct.pack_into("<Q", list_buf, 0x10, _addr(items_buf))
    struct.pack_into("<i", list_buf, 0x18, n)   # _size int32 (+_version int32 at 0x1C)
    struct.pack_into("<i", list_buf, 0x1C, n)
    struct.pack_into("<Q", items_buf, 0x18, n)
    for i, eb in enumerate(entry_bufs.values()):
        struct.pack_into("<Q", items_buf, 0x20 + 8 * i, _addr(eb))
    holder += [list_buf, items_buf]

    inst = np.zeros(window, dtype=np.uint8)
    struct.pack_into("<Q", inst, 0x00, PLANTED_KLASS_GC)   # klass slot
    struct.pack_into("<B", inst, 0x10, 1)            # _initialized
    struct.pack_into("<Q", inst, 0x18, _addr(list_buf))
    struct.pack_into("<B", inst, 0x20, 1)            # _hasOverridesApplied
    for name, val in SELFTEST_FIELDS.items():
        struct.pack_into("<f", inst, fields_by_name[name].offset, val)
    for name, val in SELFTEST_INT_FIELDS.items():
        struct.pack_into("<i", inst, fields_by_name[name].offset, val)
    holder.append(inst)

    # FieldInfo chain plant: for one key, site array [K_GC, fieldinfo, keystr]
    # (site = the slot holding the key-string ptr; *(site-8) = unmanaged FieldInfo
    # {char* name, type, parent=GameConstants class, uint32 offset})
    fi_key = "UnitSpeedFillMax"
    cname = np.frombuffer(fi_key.encode("utf-8") + b"\x00", dtype=np.uint8)
    finfo = np.zeros(0x20, dtype=np.uint8)
    struct.pack_into("<Q", finfo, 0x00, _addr(cname))
    struct.pack_into("<Q", finfo, 0x10, PLANTED_KLASS_GC)
    struct.pack_into("<I", finfo, 0x18, fields_by_name[fi_key].offset)
    site_arr = np.zeros(0x18, dtype=np.uint8)
    struct.pack_into("<Q", site_arr, 0x00, PLANTED_KLASS_GC)
    struct.pack_into("<Q", site_arr, 0x08, _addr(finfo))
    struct.pack_into("<Q", site_arr, 0x10, _addr(key_bufs[fi_key]))
    holder += [cname, finfo, site_arr]

    # metadata-pool decoy: raw header-less needle chars (must be DISCARDED by
    # the object-header check and never produce a ConfigEntry)
    pool_decoys = [np.frombuffer(k.encode("utf-16-le") + b"|x", dtype=np.uint8)
                   for k in list(SELFTEST_CONFIG)[:3]]
    holder += pool_decoys

    planted = _addr(inst)
    log(f"[selftest] planted synthetic GameConstants at 0x{planted:X} "
        f"({n} ConfigEntries + GC strings + pool decoy) in own process")

    hproc = _k32.GetCurrentProcess()
    result = run_extraction(hproc, "selftest", fields, needles, window,
                            variant=1, all_memory=True, log=log)
    if result["status"] not in ("ok", "partial"):
        log(f"[selftest] FAIL: extraction status = {result['status']} "
            f"({result.get('status_detail', '')})")
        return 5

    inst_addrs = [c["address"] for c in result["candidates"]]
    if hex(planted) not in inst_addrs:
        log(f"[selftest] FAIL: planted instance not among candidates: {inst_addrs[:8]}")
        return 5
    # self-scan noise: chunk copies of the planted window also pass and read the
    # same live list, so only require the consensus to decode identically
    consensus_walk = result["strategy1"]["walk"]["config"]
    if consensus_walk != SELFTEST_CONFIG:
        log(f"[selftest] FAIL: consensus walk config mismatch: {dict(list(consensus_walk.items())[:4])} ...")
        return 5
    if result["strategy1"]["walk"]["diag"].get("known_keys") != len(SELFTEST_CONFIG):
        log(f"[selftest] FAIL: walk known-keys = "
            f"{result['strategy1']['walk']['diag'].get('known_keys')}, "
            f"want {len(SELFTEST_CONFIG)}")
        return 5

    ok = True
    s1 = result["strategy1"]

    # 1b walk: every planted key/value pair must have come back, verbatim
    walked = s1["walk"]["config"]
    for key, raw in SELFTEST_CONFIG.items():
        if walked.get(key) != raw:
            log(f"[selftest] FAIL: walk[{key!r}] = {walked.get(key)!r}, want {raw!r}")
            ok = False

    # klass-anchored path must have engaged
    s2 = result["strategy2"]
    if not s2["klass_anchored"]:
        log("[selftest] FAIL: klass-anchored instance path did not engage")
        ok = False
    fic = s1["fieldinfo_chain"]
    if fic.get("klass") != hex(PLANTED_KLASS_GC):
        log(f"[selftest] FAIL: FieldInfo chain klass {fic.get('klass')} != {hex(PLANTED_KLASS_GC)}")
        ok = False
    if fic.get("offset_match", 0) < 1:
        log("[selftest] FAIL: FieldInfo offset match count < 1")
        ok = False

    # pointer hunt: GC copies (not the pool decoy) must resolve to entries
    hunted = s1["pointer_hunt"]["config"]
    for key, raw in SELFTEST_CONFIG.items():
        if hunted.get(key, {}).get("raw") != raw:
            log(f"[selftest] FAIL: hunt[{key!r}] = {hunted.get(key)!r}, want raw {raw!r}")
            ok = False

    # merge: config truth wins over the instance copy (UnitBaseAbilityCritChance)
    fx = result["fields"]
    got = fx["0x230C"]["value"]
    if abs(got - 0.0545) > 1e-9:
        log(f"[selftest] FAIL: merge did not prefer config truth for "
            f"UnitBaseAbilityCritChance: {got} != 0.0545")
        ok = False
    got = fx["0x1C34"]["value"]
    if got != 42:
        log(f"[selftest] FAIL: ShipMitigationAttackerLevelOffset {got} != 42")
        ok = False
    got = fx[f"0x{fields_by_name['UnitStatAbilityPowerPerIntelligence'].offset:X}"]["value"]
    if abs(got - 2.71828) > 1e-4:
        log(f"[selftest] FAIL: instance-only field UnitStatAbilityPowerPerIntelligence "
            f"{got} != 2.71828")
        ok = False

    # pool decoy must show up as discarded header-less hits, not GC copies
    decoy_keys = list(SELFTEST_CONFIG)[:3]
    for key in decoy_keys:
        ph = s1["needle_stats"]["pool_hits"].get(key, 0)
        if ph < 1:
            log(f"[selftest] FAIL: pool decoy for {key!r} not detected/discarded "
                f"(pool_hits={ph})")
            ok = False

    if not ok:
        return 5
    log("[selftest] PASS: instance found, walk + pointer hunt + merge + pool discard all round-tripped")
    return 0


# ---------------------------------------------------------------- extraction
def run_extraction(hproc, pid_label: str, fields: list[GCField], needles: Needles,
                   window: int, variant: int = 1, all_memory=False, log=print,
                   ctor_map: dict | None = None) -> dict:
    """Full pipeline against one open process handle.  Returns the result dict
    (status, fields with provenance, strategy reports)."""
    fields_by_name = {f.name: f for f in fields}
    key_names = {f.name for f in fields if not f.is_static and f.offset >= 0x10}

    log(f"[{pid_label}] sweep A (variant {variant}, {'all memory' if all_memory else 'private memory'}) ...")
    inst_addrs, gc_bases, pool_hits, stats = sweep(
        hproc, needles, window, variant, all_memory=all_memory, log=log)
    log(f"  regions: {stats['regions']}, bytes read: {stats['bytes_read'] / 1e6:.1f} MB, "
        f"instance candidates: {stats['instance_candidates']}, "
        f"GC key strings: {sum(len(v) for v in gc_bases.values())} / "
        f"header-less (pool) hits: {sum(pool_hits.values())}")

    result: dict = {
        "status": "ok", "status_detail": "",
        "fields": {}, "candidates": [], "consensus_candidate": None,
        "strategy1": {
            "needle_stats": {"pool_hits": pool_hits,
                             "gc_string_bases": {k: [hex(b) for b in v] for k, v in gc_bases.items()}},
            "fieldinfo_chain": {},
            "walk": {"config": {}, "diag": {}},
            "pointer_hunt": {"config": {}},
            "config": {},   # merged S1 table key -> raw
        },
        "strategy2": {"variant": variant, "candidates": len(inst_addrs),
                      "clusters": [], "consensus_address": None,
                      "klass_anchored": False},
        "cross_check": {"agree": 0, "disagree": 0, "fields": {},
                        "fieldinfo_offsets": {}},
    }

    # ---- S1 discovery: FieldInfo chain -> GameConstants class --------------
    klass_gc, fi_offsets, fi_diag, chain_sites = None, {}, None, None
    if gc_bases:
        klass_gc, fi_offsets, fi_diag = fieldinfo_chain(hproc, gc_bases, fields_by_name, log=log)
        chain_sites = fi_diag.pop("_sites", None)
        result["strategy1"]["fieldinfo_chain"] = {
            **fi_diag,
            "note": ("each object-valid key string is the managed name of a runtime "
                     "FieldInfo {name, type, parent=GameConstants class, offset}; "
                     "the parent vote yields the runtime class pointer and the "
                     "offsets independently cross-check il2pp.cs/il2cpp.cs"),
        }
        for name, off in fi_offsets.items():
            result["cross_check"]["fieldinfo_offsets"][name] = "match"

    # ---- instance candidates: klass sites first, loose signature second ----
    cand_addrs: list[int] = []
    if klass_gc:
        ksites = klass_site_sweep(hproc, klass_gc, log=log)
        result["strategy1"]["fieldinfo_chain"]["klass_sites"] = len(ksites)
        cand_addrs.extend(ksites)
        log(f"  klass sweep: {len(ksites)} site(s) for GameConstants class")
    cand_addrs.extend(inst_addrs)

    # ---- validate + cluster instance candidates (S2) -----------------------
    insts = []
    seen_clusters: dict[tuple, list] = {}
    deduped_cands = list(dict.fromkeys(cand_addrs))[:8192]
    for a in deduped_cands:
        rec = validate_instance(hproc, a, window, fields, variant, key_names,
                                klass_gc=klass_gc, strict_monitor=True, log=log)
        if rec is None:
            continue
        insts.append(rec)
        key = tuple(rec["decoded"][n]["bits"] if rec["decoded"][n]["type"] == "float"
                    else rec["decoded"][n]["value"]
                    for n in PRIZE_NAMES if n in rec["decoded"])
        seen_clusters.setdefault(key, []).append(a)
    if not insts and klass_gc:
        # fallback: allow a non-null monitor (a lock held at scan time)
        for a in deduped_cands:
            rec = validate_instance(hproc, a, window, fields, variant, key_names,
                                    klass_gc=klass_gc, strict_monitor=False, log=log)
            if rec is None:
                continue
            insts.append(rec)
            key = tuple(rec["decoded"][n]["bits"] if rec["decoded"][n]["type"] == "float"
                        else rec["decoded"][n]["value"]
                        for n in PRIZE_NAMES if n in rec["decoded"])
            seen_clusters.setdefault(key, []).append(a)
    if not insts:
        result["status"] = "no_valid_instances"
        result["status_detail"] = ("no window passed the relaxed signature + re-read "
                                   "validation (variant %d)" % variant)
        return result

    # rank clusters: klass-anchored first, then walk success, then walk depth, size
    def cluster_key_of(rec):
        return tuple(rec["decoded"][n]["bits"] if rec["decoded"][n]["type"] == "float"
                     else rec["decoded"][n]["value"] for n in PRIZE_NAMES if n in rec["decoded"])

    def walk_ok(rec):
        return bool(rec["config"]) and rec["walk"]["known_keys"] >= 3

    def cluster_rank(key):
        members = [r for r in insts if cluster_key_of(r) == key]
        return (sum(1 for r in members if r["anchored"]),
                sum(1 for r in members if walk_ok(r)),
                max((r["walk"]["known_keys"] for r in members), default=0),
                len(members))

    cluster_keys = sorted(seen_clusters, key=cluster_rank, reverse=True)
    best_key = cluster_keys[0]
    best_members = [r for r in insts if cluster_key_of(r) == best_key]
    # within the cluster prefer klass-anchored, then walk success, then depth
    best_members.sort(key=lambda r: (r["anchored"], walk_ok(r), r["walk"]["known_keys"]),
                      reverse=True)
    consensus = best_members[0]
    agree = len(best_members)
    anchored_n = sum(1 for r in best_members if r["anchored"])
    log(f"  instance clusters: {[len(v) for v in seen_clusters.values()]} "
        f"(consensus: {agree} member(s), klass-anchored: {anchored_n}, walk-known-keys="
        f"{consensus['walk']['known_keys']})")

    validated_addrs = {r["addr"] for r in insts}
    result["candidates"] = [
        {"pid": pid_label, "address": hex(r["addr"]), "validated": True,
         "klass_anchored": r["anchored"]}
        for r in sorted(insts, key=lambda r: (not r["anchored"], not r["monitor_zero"]))[:200]
    ]
    result["candidates"].extend(
        {"pid": pid_label, "address": hex(a), "validated": False}
        for a in deduped_cands[:200] if a not in validated_addrs)
    result["consensus_candidate"] = {"pid": pid_label, "address": hex(consensus["addr"]),
                                     "klass_anchored": consensus["anchored"]}
    result["strategy2"]["clusters"] = sorted(len(v) for v in seen_clusters.values())
    result["strategy2"]["consensus_address"] = hex(consensus["addr"])
    result["strategy2"]["klass_anchored"] = bool(anchored_n)
    result["strategy2"]["anchored_members"] = anchored_n
    # independent cross-evidence: the SAME window also satisfying the anchor-free
    # relaxed signature (they were derived by disjoint methods)
    result["strategy2"]["also_passed_loose_signature"] = instance_passes(consensus["buf"], variant)
    result["window_dump"] = {
        "pid": pid_label,
        "address": hex(consensus["addr"]),
        "range": [f"0x{DUMP_LO:X}", f"0x{DUMP_HI:X}"],
        "words": window_dump(consensus["buf"]),
    }

    # ---- S1b: walk already ran inside validate_instance --------------------
    walk_cfg, walk_diag = consensus["config"], consensus["walk"]
    result["strategy1"]["walk"] = {"config": walk_cfg, "diag": walk_diag}
    # also aggregate walks from ALL validated members (agreement evidence)
    walks_agree = sum(1 for r in best_members
                      if r["config"] and r["config"] == walk_cfg)
    result["strategy1"]["walk"]["members_agree"] = f"{walks_agree}/{agree}"
    known_walk_keys = sum(1 for k in (walk_cfg or {}) if k in key_names)
    result["strategy1"]["walk"]["tuning_keys_found"] = known_walk_keys
    result["strategy1"]["walk"]["note"] = (
        "GameConstants._cachedConfigEntries holds client feature-flag config "
        "(kebab-case keys); the Protos.ConfigEntry objects carrying the "
        "GameConstants tuning keys are NOT retained post-Populate, so no "
        "literal server strings survive for the tuning fields -- the hydrated "
        "instance (strategy2, klass-anchored) is the only surviving truth.")

    # ---- S1: pointer hunt over GC key-string copies ------------------------
    # trusted klasses = klass pointers of LIVE ConfigEntry objects learned from
    # the walk; the hunt only accepts candidates carrying one of those.
    hunt_cfg = {}
    trusted = set(walk_diag.get("entry_klasses") or set()) if walk_diag else set()
    if gc_bases:
        hunt_cfg = hunt_config_entries(hproc, gc_bases, fields_by_name,
                                       trusted_klasses=trusted or None,
                                       sites=chain_sites, log=log)
        result["strategy1"]["pointer_hunt"]["config"] = hunt_cfg

    # ---- merged S1 table (walk wins over hunt on disagreement) -------------
    merged_s1: dict[str, dict] = {}
    for key, raw in (walk_cfg or {}).items():
        merged_s1[key] = {"raw": raw, "via": ["instance-walk"]}
    for key, rec in hunt_cfg.items():
        if key in merged_s1:
            if merged_s1[key]["raw"] != rec["raw"]:
                merged_s1[key]["disagreement"] = {"walk": merged_s1[key]["raw"], "hunt": rec["raw"]}
            merged_s1[key]["via"].append("pointer-hunt")
        else:
            merged_s1[key] = {"raw": rec["raw"], "via": ["pointer-hunt"]}
    result["strategy1"]["config"] = merged_s1
    log(f"  S1 config table: {len(merged_s1)} key(s) "
        f"(walk: {len(walk_cfg or {})}, hunt: {len(hunt_cfg)})")

    # ---- per-field merge + provenance --------------------------------------
    cross = result["cross_check"]
    for f in fields:
        if f.is_static or f.offset < 0x10:
            continue
        off = f.offset
        entry = {
            "name": f.name, "type": f.type, "offset": f"0x{off:X}",
            "prize": f.name in PRIZE_NAMES,
            "is_ctor_hardcoded": None, "ctor_value": None,
        }
        s2d = consensus["decoded"].get(f.name)
        s2_val = s2d["value"] if s2d else None
        if f.type == "float" and isinstance(s2_val, float) and not np.isfinite(s2_val):
            s2_val = None
        s1_rec = merged_s1.get(f.name)
        s1_val = None
        if s1_rec:
            okp, parsed = parse_config_value(s1_rec["raw"], f.type)
            if okp:
                s1_val = parsed
            else:
                # non-numeric field: keep the raw string as value
                entry["raw_string"] = s1_rec["raw"]
                s1_val = s1_rec["raw"]

        # merge priority: S1 (server strings) > S2 instance > ctor > unknown
        final, source = None, "unknown"
        if s1_val is not None:
            final, source = s1_val, "config-entry (" + " + ".join(s1_rec["via"]) + ")"
        elif s2_val is not None:
            final, source = s2_val, "instance-scan"

        if final is None and ctor_map and off in ctor_map:
            bits = ctor_map[off]
            val = struct.unpack("<f", struct.pack("<I", bits))[0] if f.type == "float" else bits
            if f.type == "float" and np.isfinite(val) and val != 0.0:
                final, source = val, "ctor-hardcode"
            elif f.type != "float" and bits != 0:
                final, source = val, "ctor-hardcode"

        # cross-check S1 vs S2 (both numeric)
        if isinstance(s1_val, (int, float)) and not isinstance(s1_val, bool) \
                and isinstance(s2_val, (int, float)) and not isinstance(s2_val, bool):
            tol = 1e-6 * max(1.0, abs(float(s2_val)))
            if abs(float(s1_val) - float(s2_val)) <= tol:
                cross["agree"] += 1
                cross["fields"][f.name] = "agree"
                entry["cross_check"] = "agree"
            else:
                cross["disagree"] += 1
                cross["fields"][f.name] = {"config": s1_val, "instance": s2_val,
                                           "resolution": "config wins"}
                entry["cross_check"] = "DISAGREE (config preferred)"
        entry["source"] = source
        if s2d is not None:
            fi_match = f.name in result["cross_check"]["fieldinfo_offsets"]
            entry["strategy2_instance"] = {
                "value": (float(f"{s2_val:.10g}") if isinstance(s2_val, float) and np.isfinite(s2_val)
                          else s2_val),
                "bits": f"0x{s2d['bits']:08X}" if f.type == "float" else None,
                "candidates_agree": agree,
                "clusters": result["strategy2"]["clusters"],
                "klass_anchored": result["strategy2"]["klass_anchored"],
                "fieldinfo_offset_verified": fi_match,
            }
        if s1_rec is not None:
            entry["strategy1_config"] = {
                "raw": s1_rec["raw"], "via": s1_rec["via"],
            }
            if "disagreement" in s1_rec:
                entry["strategy1_config"]["raw_disagreement"] = s1_rec["disagreement"]
            if f.type == "float" and isinstance(final, float):
                entry["bits"] = f"0x{struct.unpack('<I', struct.pack('<f', final))[0]:08X}"

        # sanity gate
        gate = sanity_check(f.name, f.type, final) if final is not None and not isinstance(final, str) else None
        if gate is not None:
            entry["sanity"] = gate
        if final is None:
            entry["value"] = None
            entry["note"] = ("non-scalar field (string/object type) -- not a numeric "
                             "constant" if f.type not in SCALAR_TYPES else
                             "unknown: no config entry, no surviving instance value")
        elif isinstance(final, float):
            entry["value"] = float(f"{final:.10g}") if np.isfinite(final) else str(final)
        else:
            entry["value"] = final
        if ctor_map is not None and any(f.name == n for n, _ in CTOR_ANCHOR_NAMES):
            entry["is_ctor_hardcoded"] = True
            cv = dict(CTOR_ANCHOR_NAMES)
            entry["ctor_value"] = cv[f.name]
        result["fields"][f"0x{off:X}"] = entry

    # hydration status -- a klass-anchored instance is authoritative even when
    # some prize fields are 0 (config legitimately sets zeros, e.g. the level
    # damage-delta family is disabled in the live config)
    zeros = sum(1 for n in PRIZE_NAMES
                if result["fields"].get(f"0x{fields_by_name[n].offset:X}", {}).get("value") in (0, 0.0, None))
    if result["strategy2"]["klass_anchored"]:
        result["status"] = "ok"
        result["status_detail"] = (f"klass-anchored hydrated instance; all {len(PRIZE_NAMES)} prize fields "
                                   f"resolved ({zeros} are config-set zeros)" if zeros else
                                   f"klass-anchored hydrated instance; all {len(PRIZE_NAMES)} prize fields resolved")
    elif zeros == len(PRIZE_NAMES):
        result["status"] = "unhydrated"
        result["status_detail"] = ("instance found but every data-driven prize field reads "
                                   "zero/unknown -- the client has not hydrated yet; log in and rerun")
    elif zeros:
        result["status"] = "partial"
        result["status_detail"] = f"{len(PRIZE_NAMES) - zeros}/{len(PRIZE_NAMES)} prize fields resolved"
    else:
        result["status"] = "ok"
        result["status_detail"] = f"all {len(PRIZE_NAMES)} prize fields resolved"
    return result


# ---------------------------------------------------------------------- main
def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description="Extract data-driven GameConstants from a running SWGoH client.")
    ap.add_argument("--process", default="SWGoH.exe", help="target image name (default SWGoH.exe)")
    ap.add_argument("--out", default=DEFAULT_OUT, help="output JSON path")
    ap.add_argument("--all-memory", action="store_true", help="also scan mapped/image regions")
    ap.add_argument("--variant", type=int, default=1, choices=(1, 2),
                    help="S2 signature variant: 1 = Base* chances equal (default), "
                         "2 = drop the equality (both in 0.001..0.5)")
    ap.add_argument("--wait-minutes", type=float, default=0.0,
                    help="if the game is not running, poll up to N minutes for relaunch+login")
    ap.add_argument("--dll", default=None, help="GameAssembly.dll path: annotate which fields are "
                                                "ctor-hardcoded (provenance only)")
    ap.add_argument("--selftest", action="store_true", help="dry-run: scan our own process for a planted "
                                                            "synthetic GameConstants + ConfigEntry list")
    ap.add_argument("--quiet", action="store_true")
    args = ap.parse_args(argv)
    log = (lambda *a, **k: None) if args.quiet else print

    fields = parse_game_constants_fields(IL2CPP_CS)
    fields_by_name = {f.name: f for f in fields}
    max_off = max(f.offset for f in fields)
    window = max(max_off + 16, DUMP_HI + 16)
    needles = Needles(f.name for f in fields if not f.is_static and f.offset >= 0x10)

    ctor_map = None
    if args.dll:
        ctor_map = extract_ctor_immediates(args.dll, 0x1811EBF50, log=log)

    log(f"GameConstants: {len(fields)} fields, instance window 0x{window:X} bytes, "
        f"{len(needles.names)} UTF-16 needles (S1), relaxed S2 signature variant {args.variant}")
    if not args.quiet:
        for name, val in CTOR_ANCHOR_NAMES:
            f = fields_by_name.get(name)
            if f:
                print(f"   ctor anchor: 0x{f.offset:04X} {name:44s} imm {val}")

    if args.selftest:
        return selftest(fields_by_name, fields, window, needles, log=log)

    # wait-loop for the game (gate: up to 60 min for relaunch+login)
    deadline = time.monotonic() + args.wait_minutes * 60
    pids = []
    while True:
        pids = find_process_ids(args.process)
        if pids:
            break
        if time.monotonic() >= deadline:
            break
        log(f"game not running; waiting for {args.process} (retrying every 30 s, "
            f"{(deadline - time.monotonic()) / 60:.0f} min left) ...")
        time.sleep(30)
    result = {
        "tool": os.path.relpath(TOOL_PATH, REPO_ROOT).replace("\\", "/"),
        "generated_at_utc": datetime.now(timezone.utc).isoformat(),
        "target": {"process": args.process, "pids": pids, "all_memory": args.all_memory,
                   "variant": args.variant},
        "signature": {
            "strategy1": "UTF-16 needle sweep -> GC string objects -> pointer sites -> "
                         "ConfigEntry{Key@0x10,Value@0x18} -> server strings",
            "strategy1b": "GameConstants+0x18 _cachedConfigEntries object-graph walk",
            "strategy2": {
                "equalities": [f"f32[+0x{S2_OFF_BASE_AB:X}] ~= f32[+0x{S2_OFF_BASE_ATK:X}] (|d|<=1e-6, 0.01..1.0)",
                               f"f32[+0x{S2_OFF_ATK_PL:X}] in 0.001..0.5",
                               f"f32[+0x{S2_OFF_AB_MIN:X}] <= f32[+0x{S2_OFF_ATK_MAX:X}]",
                               f"f32[+0x{S2_OFF_DM_MIN:X}] <= f32[+0x{S2_OFF_DM_MAX:X}]",
                               f"f32[+0x{S2_OFF_FILLMAX:X}] in 1..1000"],
                "variant": args.variant,
                "note": "variant 2 drops the Base* equality (both 0.001..0.5 instead)",
            },
            "anchors_source": "ctor machine-code pass retained as provenance only "
                              "(hydration overwrites the anchors; BATTLE_SIM_INVENTORY.md)",
        },
        "prize_offsets": [f"0x{fields_by_name[n].offset:X}" for n in PRIZE_NAMES if n in fields_by_name],
        "merge_priority": "config-entry (S1/S1b server strings) > instance-scan (S2) > ctor-hardcode > unknown",
    }

    if not pids:
        result["status"] = "game_not_running"
        result["status_detail"] = (f"no process named {args.process} within the "
                                   f"{args.wait_minutes:.0f}-min wait; launch + log into the game, then rerun")
        _write(result, args.out, log)
        return 2

    all_results = []
    exit_code = 0
    for pid in pids:
        log(f"scanning pid {pid} ({args.process}) ...")
        try:
            hproc = open_process(pid)
        except OSError as e:
            log(f"  {e}")
            result["status"] = "open_failed"
            result["status_detail"] = f"{e} (run from an elevated shell?)"
            _write(result, args.out, log)
            return 3
        try:
            r = run_extraction(hproc, str(pid), fields, needles, window,
                               variant=args.variant, all_memory=args.all_memory, log=log,
                               ctor_map=ctor_map)
        finally:
            _k32.CloseHandle(hproc)
        if r["status"] in ("no_valid_instances",) and args.variant == 1:
            # server may have sent different Base* chances: drop the equality, one retry
            log("  variant 1 found no valid instance -- retrying with variant 2 "
                "(Base* equality dropped) ...")
            try:
                hproc = open_process(pid)
            except OSError as e:
                log(f"  {e}")
                break
            try:
                r2 = run_extraction(hproc, str(pid), fields, needles, window,
                                    variant=2, all_memory=args.all_memory, log=log,
                                    ctor_map=ctor_map)
            finally:
                _k32.CloseHandle(hproc)
            r2["variant_fallback_from"] = 1
            r = r2
        all_results.append(r)
        if r["status"] in ("ok", "partial"):
            break
        if r["status"] == "unhydrated":
            exit_code = 4
        else:
            exit_code = 3

    if len(all_results) == 1:
        result.update(all_results[0])
    else:
        result["status"] = "multi_pid"
        result["status_detail"] = "multiple game processes; per-pid results below"
        result["per_pid"] = all_results
    _write(result, args.out, log)
    if result.get("status") in ("ok", "partial") and "fields" in result:
        _print_prize(result, fields_by_name, log)
    return exit_code if exit_code else (0 if result.get("status") in ("ok", "partial") else 3)


def _print_prize(result: dict, fields_by_name, log):
    log("")
    log("data-driven GameConstants (server-sent config truth where available):")
    hdr = f"  {'offset':<8} {'value':>14}  {'source':<42} {'cross':<8} {'field'}"
    log(hdr)
    for name in PRIZE_NAMES + [n for n, _ in CTOR_ANCHOR_NAMES]:
        f = fields_by_name.get(name)
        if f is None:
            continue
        e = result["fields"].get(f"0x{f.offset:X}")
        if e is None:
            continue
        val = e["value"]
        vs = f"{val:.10g}" if isinstance(val, float) else str(val)
        cross = e.get("cross_check", "-")
        cross = "agree" if cross == "agree" else ("DISAGREE" if isinstance(cross, str) and "DISAGREE" in cross else "-")
        gate = e.get("sanity", "")
        log(f"  +0x{f.offset:04X} {vs:>14}  {e['source']:<42} {cross:<8} {name}"
            + (f"  [{gate}]" if gate and gate.startswith("FAIL") else ""))


def _write(result: dict, out: str, log) -> None:
    os.makedirs(os.path.dirname(os.path.abspath(out)), exist_ok=True)
    with open(out, "w", encoding="utf-8", newline="\n") as fh:
        json.dump(result, fh, indent=1, ensure_ascii=False, default=_json_default)
    log(f"wrote {out}")


def _json_default(o):
    if isinstance(o, (set, frozenset)):
        return sorted(hex(x) if isinstance(x, int) else str(x) for x in o)
    if isinstance(o, (np.integer,)):
        return int(o)
    if isinstance(o, (np.floating,)):
        return float(o)
    return str(o)


# ------------------------------------------------- optional ctor verification
def extract_ctor_immediates(dll_path: str, ctor_va: int, log=print,
                            fields_by_offset: dict | None = None) -> dict[int, int]:
    """Decode `C7 /0 imm32` (mov dword [reg+disp], imm32) writes in the ctor,
    keeping only displacements that are valid GameConstants field offsets.
    Offsets inside the ctor are object-relative, exactly what we need."""
    if fields_by_offset is None:
        fields_by_offset = globals().get("fields_by_offset") or {}
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
