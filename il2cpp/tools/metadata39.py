"""Parser for Unity IL2CPP global-metadata.dat, version 39 (Unity 2022.3.72f1+ / v39 metadata).

Self-contained (stdlib only). Layout was recovered by bootstrap analysis of
SWGoH's global-metadata.dat and cross-checked against Il2CppInspectorRedux's
C# definitions (redux/Il2CppInspector.Common/Next/Metadata/*.cs).

Key v39 facts (see METADATA39_NOTES.md for the full story):
  * Header = magic + version + 31 x Il2CppSectionMetadata (offset:int32, size:int32, count:int32)
  * Tables are contiguous, each 4-byte aligned after the previous one.
  * Several index fields are dynamically sized; for this file:
      TypeDefinitionIndex = uint16 (26498 types), GenericContainerIndex = uint16,
      TypeIndex = int32, ParameterIndex = int32, everything else int32 default.
  * StringLiteral entries are a single int32 DataIndex (v31 had Length+DataIndex);
    literal length = dataIndex[i+1] - dataIndex[i] (last entry = end sentinel).
"""

from __future__ import annotations

import struct
from dataclasses import dataclass, field as dc_field

MAGIC = 0xFAB11BAF
SUPPORTED_VERSION = 39

SECTION_NAMES = [
    'stringLiterals', 'stringLiteralData', 'strings', 'events', 'properties',
    'methods', 'parameterDefaultValues', 'fieldDefaultValues',
    'fieldAndParameterDefaultValueData', 'fieldMarshaledSizes', 'parameters',
    'fields', 'genericParameters', 'genericParameterConstraints',
    'genericContainers', 'nestedTypes', 'interfaces', 'vtableMethods',
    'interfaceOffsets', 'typeDefinitions', 'images', 'assemblies', 'fieldRefs',
    'referencedAssemblies', 'attributeData', 'attributeDataRanges',
    'unresolvedIndirectCallParameterTypes', 'unresolvedIndirectCallParameterRanges',
    'windowsRuntimeTypeNames', 'windowsRuntimeStrings', 'exportedTypeDefinitions',
]


@dataclass(frozen=True)
class Section:
    name: str
    offset: int
    size: int
    count: int


# ---------------------------------------------------------------------------
# record types
# ---------------------------------------------------------------------------

@dataclass(frozen=True, slots=True)
class TypeDef:
    name: str
    namespace: str
    name_index: int
    namespace_index: int
    byval_type_index: int          # index into binary Il2CppType array (not metadata)
    declaring_type: int            # typeDefinition index or -1
    parent_index: int              # binary Il2CppType index or -1
    generic_container_index: int   # uint16
    flags: int
    field_start: int
    method_start: int
    event_start: int
    property_start: int
    nested_types_start: int
    interfaces_start: int
    vtable_start: int
    interface_offsets_start: int
    method_count: int
    property_count: int
    field_count: int
    event_count: int
    nested_type_count: int
    vtable_count: int
    interfaces_count: int
    interface_offsets_count: int
    bitfield: int
    token: int
    index: int

    @property
    def full_name(self) -> str:
        return f'{self.namespace}.{self.name}' if self.namespace else self.name

    @property
    def is_value_type(self) -> bool:
        return bool(self.bitfield & 1)

    @property
    def is_enum(self) -> bool:
        return bool(self.bitfield & 2)


@dataclass(frozen=True, slots=True)
class MethodDef:
    name: str
    name_index: int
    declaring_type: int            # typeDefinition index (uint16 on disk)
    return_type_index: int         # binary Il2CppType index
    return_parameter_token: int    # 0x08000000 sentinel when unused
    parameter_start: int           # -1 when parameter_count == 0
    generic_container_index: int   # uint16, -1 if none
    token: int
    flags: int
    impl_flags: int
    slot: int
    parameter_count: int
    index: int


@dataclass(frozen=True, slots=True)
class FieldDef:
    name: str
    name_index: int
    type_index: int
    token: int


@dataclass(frozen=True, slots=True)
class ParameterDef:
    name: str
    name_index: int
    token: int
    type_index: int


@dataclass(frozen=True, slots=True)
class EventDef:
    name: str
    type_index: int
    add: int
    remove: int
    raise_: int
    token: int


@dataclass(frozen=True, slots=True)
class PropertyDef:
    name: str
    get: int
    set: int
    attrs: int
    token: int


@dataclass(frozen=True, slots=True)
class ImageDef:
    name: str
    assembly_index: int
    type_start: int
    type_count: int
    exported_type_start: int
    exported_type_count: int
    entry_point_index: int
    token: int
    custom_attribute_start: int
    custom_attribute_count: int


@dataclass(frozen=True, slots=True)
class AssemblyDef:
    name: str
    version: tuple
    image_index: int
    token: int
    referenced_assembly_start: int
    referenced_assembly_count: int
    unknown_word: int              # word @8; observed value 1 in this file (see notes)
    culture: str


# ---------------------------------------------------------------------------
# parser
# ---------------------------------------------------------------------------

class Metadata39:
    """Parse a v39 global-metadata.dat file.

    Usage:
        m = Metadata39('global-metadata.dat')
        m.typedefs[0].full_name
        m.methods[0].name
        m.string_literal(0)
    """

    def __init__(self, path, validate=True):
        with open(path, 'rb') as f:
            self.data = f.read()
        self.file_size = len(self.data)
        self._parse_header()
        self._string_blob_cache = None
        self._literals_cache = None
        self._tables = {}
        if validate:
            self.validate()

    # -- header ------------------------------------------------------------

    def _parse_header(self):
        d = self.data
        self.sanity, self.version = struct.unpack_from('<Ii', d, 0)
        if self.sanity != MAGIC:
            raise ValueError(f'bad magic 0x{self.sanity:08x} (expected 0x{MAGIC:08x})')
        if self.version != SUPPORTED_VERSION:
            raise ValueError(f'unsupported metadata version {self.version} (expected 39)')
        words = struct.unpack_from('<93i', d, 8)
        self.header = {}
        for i, nm in enumerate(SECTION_NAMES):
            off, size, count = words[3 * i:3 * i + 3]
            self.header[nm] = Section(nm, off, size, count)
        self.header_size = self.header['stringLiterals'].offset  # 380 here

    def section(self, name) -> Section:
        return self.header[name]

    # -- strings -----------------------------------------------------------

    def string(self, index: int) -> str:
        """Identifier string at byte offset `index` inside the Strings blob
        (index is a byte offset into the blob, not a string ordinal)."""
        if index < 0 or index >= self.header['strings'].size:
            raise IndexError(f'string index {index} out of range')
        d = self.data
        end = d.index(b'\x00', self.header['strings'].offset + index)
        return d[self.header['strings'].offset + index:end].decode('utf-8', 'replace')

    def try_string(self, index: int) -> str | None:
        try:
            return self.string(index)
        except (IndexError, ValueError):
            return None

    @property
    def string_blob(self) -> bytes:
        sec = self.header['strings']
        return self.data[sec.offset:sec.offset + sec.size]

    @property
    def identifiers(self) -> list[str]:
        """All NUL-terminated identifier strings in the Strings blob, in order."""
        if self._string_blob_cache is None:
            self._string_blob_cache = [s.decode('utf-8', 'replace')
                                       for s in self.string_blob.split(b'\x00')]
        return self._string_blob_cache

    # -- string literals -----------------------------------------------------

    @property
    def literal_offsets(self) -> tuple:
        sec = self.header['stringLiterals']
        return struct.unpack_from(f'<{sec.count}i', self.data, sec.offset)

    def string_literal(self, index: int) -> str:
        offs = self.literal_offsets
        sec = self.header['stringLiteralData']
        a = offs[index]
        b = offs[index + 1]  # last entry is the end sentinel
        return self.data[sec.offset + a:sec.offset + b].decode('utf-8', 'replace')

    @property
    def literals(self) -> list[str]:
        if self._literals_cache is None:
            self._literals_cache = [self.string_literal(i)
                                    for i in range(self.header['stringLiteralData'].count)]
        return self._literals_cache

    # -- tables --------------------------------------------------------------
    # unpack formats per record (v39 field layout, see METADATA39_NOTES.md)

    _TD = struct.Struct('<5i Hi 8i 8H 2I')  # 82 bytes
    _MD = struct.Struct('<i Hi i i H I 4H')  # 32 bytes
    _FD = struct.Struct('<2i I')             # 12 bytes: name, typeIndex, token
    _PD = struct.Struct('<2i I')             # 12 bytes: name, token, typeIndex
    _ED = struct.Struct('<5i I')             # 24 bytes
    _PRD = struct.Struct('<4i I')            # 20 bytes
    _IM = struct.Struct('<iiHIHIiIiI')       # 36 bytes, 10 fields

    def _slice(self, name):
        sec = self.header[name]
        return self.data[sec.offset:sec.offset + sec.size], sec

    @property
    def typedefs(self) -> list[TypeDef]:
        if 'typedefs' not in self._tables:
            blob, sec = self._slice('typeDefinitions')
            out = []
            s = self._TD
            for t in range(sec.count):
                (name_i, ns_i, byval, decl, parent, gc, flags, fstart, mstart,
                 estart, pstart, nstart, istart, vstart, ostart, mc, pc, fc, ec,
                 nc, vc, ic, oc, bitfield, token) = s.unpack_from(blob, t * 82)
                out.append(TypeDef(
                    name=self.string(name_i), namespace=self.string(ns_i),
                    name_index=name_i, namespace_index=ns_i, byval_type_index=byval,
                    declaring_type=decl, parent_index=parent,
                    generic_container_index=gc & 0xFFFF if gc != -1 else -1,
                    flags=flags, field_start=fstart, method_start=mstart,
                    event_start=estart, property_start=pstart,
                    nested_types_start=nstart, interfaces_start=istart,
                    vtable_start=vstart, interface_offsets_start=ostart,
                    method_count=mc, property_count=pc, field_count=fc,
                    event_count=ec, nested_type_count=nc, vtable_count=vc,
                    interfaces_count=ic, interface_offsets_count=oc,
                    bitfield=bitfield, token=token, index=t))
            self._tables['typedefs'] = out
        return self._tables['typedefs']

    @property
    def methods(self) -> list[MethodDef]:
        if 'methods' not in self._tables:
            blob, sec = self._slice('methods')
            out = []
            s = self._MD
            for m in range(sec.count):
                (name_i, decl, ret, rpt, pstart, gc, token, flags, iflags,
                 slot, pcount) = s.unpack_from(blob, m * 32)
                out.append(MethodDef(
                    name=self.string(name_i), name_index=name_i,
                    declaring_type=decl, return_type_index=ret,
                    return_parameter_token=rpt, parameter_start=pstart,
                    generic_container_index=gc & 0xFFFF if gc != -1 else -1,
                    token=token, flags=flags, impl_flags=iflags, slot=slot,
                    parameter_count=pcount, index=m))
            self._tables['methods'] = out
        return self._tables['methods']

    @property
    def fields(self) -> list[FieldDef]:
        if 'fields' not in self._tables:
            blob, sec = self._slice('fields')
            s = self._FD
            self._tables['fields'] = [
                FieldDef(name=self.string(n), name_index=n, type_index=ti, token=tok)
                for n, ti, tok in (s.unpack_from(blob, k * 12) for k in range(sec.count))]
        return self._tables['fields']

    @property
    def parameters(self) -> list[ParameterDef]:
        if 'parameters' not in self._tables:
            blob, sec = self._slice('parameters')
            s = self._PD
            self._tables['parameters'] = [
                ParameterDef(name=self.string(n), name_index=n, token=tok, type_index=ti)
                for n, tok, ti in (s.unpack_from(blob, k * 12) for k in range(sec.count))]
        return self._tables['parameters']

    @property
    def events(self) -> list[EventDef]:
        if 'events' not in self._tables:
            blob, sec = self._slice('events')
            s = self._ED
            self._tables['events'] = [
                EventDef(name=self.string(n), type_index=ti, add=a, remove=r,
                         raise_=ra, token=tok)
                for n, ti, a, r, ra, tok in
                (s.unpack_from(blob, k * 24) for k in range(sec.count))]
        return self._tables['events']

    @property
    def properties(self) -> list[PropertyDef]:
        if 'properties' not in self._tables:
            blob, sec = self._slice('properties')
            s = self._PRD
            self._tables['properties'] = [
                PropertyDef(name=self.string(n), get=g, set=st, attrs=at, token=tok)
                for n, g, st, at, tok in
                (s.unpack_from(blob, k * 20) for k in range(sec.count))]
        return self._tables['properties']

    @property
    def images(self) -> list[ImageDef]:
        if 'images' not in self._tables:
            blob, sec = self._slice('images')
            s = self._IM
            self._tables['images'] = [
                ImageDef(name=self.string(n), assembly_index=ai, type_start=ts,
                         type_count=tc, exported_type_start=ets,
                         exported_type_count=etc, entry_point_index=ep,
                         token=tok, custom_attribute_start=cas,
                         custom_attribute_count=cac)
                for n, ai, ts, tc, ets, etc, ep, tok, cas, cac in
                (s.unpack_from(blob, k * 36) for k in range(sec.count))]
        return self._tables['images']

    @property
    def assemblies(self) -> list[AssemblyDef]:
        if 'assemblies' not in self._tables:
            blob, sec = self._slice('assemblies')
            out = []
            for k in range(sec.count):
                base = k * 68
                image_i, token, gap, ref_start, ref_count = struct.unpack_from('<5i', blob, base)
                (aname_i, culture_i, _pubkey_i, _hashalg, _hashlen, aflags,
                 major, minor, build, revision) = struct.unpack_from('<3iIi5i', blob, base + 20)
                out.append(AssemblyDef(
                    name=self.string(aname_i), culture=self.string(culture_i),
                    version=(major, minor, build, revision), image_index=image_i,
                    token=token, referenced_assembly_start=ref_start,
                    referenced_assembly_count=ref_count, unknown_word=gap))
            self._tables['assemblies'] = out
        return self._tables['assemblies']

    # -- convenience accessors ------------------------------------------------

    def methods_of(self, td: TypeDef) -> list[MethodDef]:
        return self.methods[td.method_start:td.method_start + td.method_count]

    def fields_of(self, td: TypeDef) -> list[FieldDef]:
        return self.fields[td.field_start:td.field_start + td.field_count]

    def parameters_of(self, m: MethodDef) -> list[ParameterDef]:
        return self.parameters[m.parameter_start:m.parameter_start + m.parameter_count]

    def nested_types_of(self, td: TypeDef) -> list[TypeDef]:
        sec = self.header['nestedTypes']
        blob = self.data[sec.offset:sec.offset + sec.size]
        idx = struct.unpack_from(f'<{td.nested_type_count}i', blob, td.nested_types_start * 4)
        return [self.typedefs[i] for i in idx]

    def typedef_by_full_name(self, full_name: str) -> TypeDef | None:
        return self._by_full_name().get(full_name)

    def _by_full_name(self):
        cache = getattr(self, '_full_name_map', None)
        if cache is None:
            cache = {td.full_name: td for td in self.typedefs}
            self._full_name_map = cache
        return cache

    # -- validation ------------------------------------------------------------

    def validate(self, strict=True):
        """Cross-checks that prove the layout is parsed correctly.
        Raises AssertionError on any inconsistency."""
        h = self.header
        assert self.sanity == MAGIC and self.version == 39
        assert self.header_size == 380, self.header_size

        # sections must be contiguous and 4-byte aligned, last ends at EOF
        secs = [h[n] for n in SECTION_NAMES]
        prev_end = self.header_size
        for s in secs:
            if s.count == 0 and s.size == 0:
                continue
            assert s.offset >= prev_end and s.offset % 4 == 0, s
            if s.count and s.size % s.count == 0:
                pass  # fixed-size table
            prev_end = max(prev_end, s.offset + s.size)
        assert prev_end == self.file_size, (prev_end, self.file_size)

        # string literal offsets are monotonic and end at the blob size
        offs = self.literal_offsets
        assert offs[-1] == h['stringLiteralData'].size
        assert all(offs[i] <= offs[i + 1] for i in range(len(offs) - 1))
        assert h['stringLiteralData'].count == h['stringLiterals'].count - 1

        # type definition ranges must exactly tile methods/fields/params
        td_total = sum(td.method_count for td in self.typedefs)
        assert td_total == h['methods'].count, (td_total, h['methods'].count)
        assert sum(td.field_count for td in self.typedefs) == h['fields'].count

        methods = self.methods
        pcount_total = 0
        for td in self.typedefs:
            for m in methods[td.method_start:td.method_start + td.method_count]:
                assert m.declaring_type == td.index, (m, td.index)
                pcount_total += m.parameter_count
                if m.parameter_count:
                    assert 0 <= m.parameter_start
                    assert m.parameter_start + m.parameter_count <= h['parameters'].count
        assert pcount_total == h['parameters'].count

        # tokens: methods are MethodDef (0x06), fields 0x04, params 0x08
        if strict:
            assert all((m.token >> 24) == 6 for m in methods)
            assert all((f.token >> 24) == 4 for f in self.fields)
            assert all((p.token >> 24) == 8 for p in self.parameters)
            assert all((t.token >> 24) == 2 for t in self.typedefs)
        return True
