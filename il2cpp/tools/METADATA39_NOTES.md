# global-metadata.dat v39 — reverse-engineering notes

Target: SWGoH (`Unity 2022.3.72f1`), `global-metadata.dat`, 25,125,572 bytes,
magic `0xFAB11BAF`, version **39**.

Parser: `metadata39.py` (stdlib `struct` only). Test: `test_metadata39.py`.
Layout was recovered by bootstrap analysis of the file itself and cross-checked
against Il2CppInspectorRedux C# sources (`../redux/Il2CppInspector.Common/`,
read-only): `Next/Metadata/Il2CppGlobalMetadataHeader.cs`,
`Next/Metadata/Il2Cpp*Definition.cs`, `IL2CPP/Metadata.cs`.

All offsets below are for **this file**; the *structure* is what generalizes.

---

## 1. Header layout

v39 header = 8 bytes (magic `int32` + version `int32`) followed by **31 ×
`Il2CppSectionMetadata`**, each `(offset:int32, size:int32, count:int32)` —
i.e. 31 *triples*, not the `(offset,size)` *pairs* of v31 and earlier.

```
header size = 8 + 31*12 = 380 bytes
```

Section order (must be exact; names from the C# header):

| # | section                       | offset     | size       | count      | elem |
|---|-------------------------------|-----------:|-----------:|-----------:|-----:|
| 0 | stringLiterals                | 380        | 122,660    | 30,665     | 4    |
| 1 | stringLiteralData             | 123,040    | 973,689    | 30,664     | blob |
| 2 | strings                       | 1,096,732  | 4,085,961  | 200,706*   | blob |
| 3 | events                        | 5,182,696  | 45,840     | 1,910      | 24   |
| 4 | properties                    | 5,228,536  | 699,060    | 34,953     | 20   |
| 5 | methods                       | 5,927,596  | 6,386,496  | 199,578    | 32   |
| 6 | parameterDefaultValues        | 12,314,092 | 77,532     | 6,461      | 12   |
| 7 | fieldDefaultValues            | 12,391,624 | 324,888    | 27,074     | 12   |
| 8 | fieldAndParameterDefaultValueData | 12,716,512 | 1,428,083 | =size  | blob |
| 9 | fieldMarshaledSizes           | 14,144,596 | 100,920    | 8,410      | 12   |
| 10 | parameters                   | 14,245,516 | 2,327,916  | 193,993    | 12   |
| 11 | fields                       | 16,573,432 | 1,635,528  | 136,294    | 12   |
| 12 | genericParameters            | 18,208,960 | 57,876     | 4,134      | 14   |
| 13 | genericParameterConstraints  | 18,266,836 | 3,668      | 917        | 4    |
| 14 | genericContainers            | 18,270,504 | 39,184     | 2,449      | 16   |
| 15 | nestedTypes                  | 18,309,688 | 34,648     | 8,662      | 4    |
| 16 | interfaces                   | 18,344,336 | 34,972     | 8,743      | 4    |
| 17 | vtableMethods                | 18,379,308 | 1,348,924  | 337,231    | 4    |
| 18 | interfaceOffsets             | 19,728,232 | 209,056    | 26,132     | 8    |
| 19 | typeDefinitions              | 19,937,288 | 2,172,836  | 26,498     | 82   |
| 20 | images                       | 22,110,124 | 3,600      | 100        | 36   |
| 21 | assemblies                   | 22,113,724 | 6,800      | 100        | 68   |
| 22 | fieldRefs                    | 22,120,524 | 8,784      | 1,098      | 8    |
| 23 | referencedAssemblies         | 22,129,308 | 2,204      | 551        | 4    |
| 24 | attributeData                | 22,131,512 | 2,122,924  | =size      | blob |
| 25 | attributeDataRanges          | 24,254,436 | 769,984    | 96,248     | 8    |
| 26 | unresolvedIndirectCallParameterTypes  | 25,024,420 | 53,860 | 13,465 | 4 |
| 27 | unresolvedIndirectCallParameterRanges | 25,078,280 | 35,384 | 4,423  | 8 |
| 28 | windowsRuntimeTypeNames      | 25,113,664 | 0          | 0          | —    |
| 29 | windowsRuntimeStrings        | 25,113,664 | 0          | 0          | —    |
| 30 | exportedTypeDefinitions      | 25,113,664 | 11,908     | 2,977      | 4    |

\* see §5 uncertainties for the `strings` count slot.

Structural invariants (used as `validate()` assertions):

* Sections are **contiguous** in header order: `offset[i+1] == align4(offset[i]+size[i])`;
  last section ends exactly at EOF. This invariant is what allowed the header
  triples to be bootstrapped before the C# source was available.
* Empty sections (size 0) share the offset of the next section.
* `header['stringLiterals'].offset == 380 == header size` (first table directly
  follows the header).

### Element sizes (empirical, confirmed against C# field lists)

| table | size | notes |
|---|---:|---|
| TypeDefinition | **82** | see §2 |
| MethodDefinition | **32** | see §2 |
| Field / Parameter / FieldDefaultValue / ParameterDefaultValue / FieldMarshaledSize | 12 | `(int, int, uint)`-ish triples |
| Event | 24 | 5×int32 + uint32 token |
| Property | 20 | 4×int32 + uint32 token |
| Image | 36 | 10 fields, mixed i32/u16 |
| Assembly | **68** | no `moduleToken` on disk (see §5) |
| GenericParameter | 14 | `u16 owner, i32 name, u16 start, i16 count, u16 num, u16 flags` |
| GenericContainer | 16 | 4×int32 (pre-"v1060" int layout) |
| StringLiteral | **4** | single `int32 DataIndex` (see §3) |

### Dynamic index sizes (v38+)

Per `IL2CPP/Metadata.cs`, several index columns shrink from `int32` based on
table counts (`GetIndexSize`): ≤255 → `uint8`, ≤65,535 → `uint16`, else `int32`.

For this file:

* `TypeDefinitionIndex` → **uint16** (26,498 types) — affects
  `MethodDefinition.DeclaringType` and `GenericParameter.OwnerIndex`.
* `GenericContainerIndex` → **uint16** (2,449 containers) — affects the
  `GenericContainerIndex` field of typedefs and methods.
* `TypeIndex` → **int32** (proven via `interfaceOffsets` elem size 8 =
  `TypeIndex + TypeDefinitionIndex`... i.e. 4 + 4; the type table in the binary
  has >65k entries).
* `ParameterIndex` → **int32** (193,993 > 65,535).
* Everything else: default int32.

This is why `MethodDefinition` is 32 B on disk while a naive "all int32" C#
reading suggests 36 B: `DeclaringType` (u16) and `GenericContainerIndex` (u16)
each save 2 bytes, and there is no `CustomAttributeIndex` (dropped ≤24.0).

---

## 2. Record layouts (v39, byte offsets)

### TypeDefinition — 82 bytes

`struct '<5i Hi 8i 8H 2I'`

| off | type | field |
|---:|---|---|
| 0 | i32 | nameIndex (byte offset into strings blob) |
| 4 | i32 | namespaceIndex |
| 8 | i32 | byvalTypeIndex (Il2CppType index in the **binary**, not metadata) |
| 12 | i32 | declaringTypeIndex (-1 = none) — *int32 here despite u16 in methods, see §5* |
| 16 | i32 | parentIndex (Il2CppType index, -1 = none) |
| 20 | u16 | genericContainerIndex (-1 = none) |
| 22 | i32 | flags (TypeAttributes) |
| 26 | i32 | fieldStart |
| 30 | i32 | methodStart |
| 34 | i32 | eventStart |
| 38 | i32 | propertyStart |
| 42 | i32 | nestedTypesStart |
| 46 | i32 | interfacesStart |
| 50 | i32 | vtableStart |
| 54 | i32 | interfaceOffsetsStart |
| 58 | u16 | methodCount |
| 60 | u16 | propertyCount |
| 62 | u16 | fieldCount |
| 64 | u16 | eventCount |
| 66 | u16 | nestedTypeCount |
| 68 | u16 | vtableCount |
| 70 | u16 | interfacesCount |
| 72 | u16 | interfaceOffsetsCount |
| 74 | u32 | bitfield (bit0 = is value type, bit1 = is enum — packing info) |
| 78 | u32 | token (table 0x02) |

Present in v31 but **gone** by v39: `CustomAttributeIndex` (≤24.0),
`ByRefTypeIndex` (≤24.5), `ElementTypeIndex` (≤31.0), `RgctxStartIndex/Count`
(≤24.1). v31's layout is otherwise field-compatible.

### MethodDefinition — 32 bytes

`struct '<i Hi i i H I 4H'`

| off | type | field |
|---:|---|---|
| 0 | i32 | nameIndex |
| 4 | u16 | declaringType (TypeDefinitionIndex — **uint16** on disk) |
| 6 | i32 | returnType (Il2CppType index) |
| 10 | u32 | returnParameterToken — new in ≥31; `0x08000000` sentinel when unused |
| 14 | i32 | parameterStart (-1 if parameterCount == 0) |
| 18 | u16 | genericContainerIndex (-1 = none) |
| 20 | u32 | token (table 0x06) |
| 24 | u16 | flags (MethodAttributes) |
| 26 | u16 | implFlags (MethodImplAttributes) |
| 28 | u16 | slot |
| 30 | u16 | parameterCount |

⚠ If you read `parameterStart` at offset 12 (i.e. treat returnParameterToken as
part of returnType), every value is corrupted by the `0x08000000` high bits —
this was the single trickiest bootstrap trap.

### Others

* **Field** 12 B: `i32 nameIndex, i32 typeIndex, u32 token` (table 0x04)
* **Parameter** 12 B: `i32 nameIndex, u32 token` (table 0x08), `i32 typeIndex`
  — note the token sits in the *middle*, unlike Field.
* **Event** 24 B: `i32 name, typeIndex, add, remove, raise, u32 token`
* **Property** 20 B: `i32 name, get, set, attrs, u32 token`
* **Image** 36 B: `i32 name, assemblyIndex; u16 typeStart; u32 typeCount;
  u16 exportedTypeStart; u32 exportedTypeCount; i32 entryPointIndex;
  u32 token; i32 customAttributeStart; u32 customAttributeCount`
* **Assembly** 68 B: `i32 imageIndex, token, <unknown=1>, refStart, refCount`,
  then `Il2CppAssemblyName` at +20:
  `i32 nameIndex, cultureIndex, publicKeyIndex; u32 hashAlg; i32 hashLen;
  u32 flags; i32 major, minor, build, revision; u8 publicKeyToken[8]`.
  Assembly-CSharp = `mscorlib 4.0.0.0`-style version tuples are per-assembly;
  SWGoH's own assemblies report `(0,0,0,0)` or real versions.
* **GenericContainer** 16 B: 4×i32 `ownerIndex, typeArgc, isMethod,
  genericParameterStart` (the "pre-V1060" int layout — v39 < 1060)
* **GenericParameter** 14 B: `u16 owner, i32 name, u16 constraintsStart,
  i16 constraintsCount, u16 num, u16 flags`

## 3. String literals (changed vs v31)

v31 `Il2CppStringLiteral` = `{length:int32, dataIndex:int32}` = 8 B/entry.
v39 = **single int32** `DataIndex` = 4 B/entry; the length is derived from the
*next* entry: `len[i] = dataIndex[i+1] - dataIndex[i]`.

Consequences:

* `stringLiterals.count` = **literalCount + 1** — the last entry is an end
  sentinel equal to `stringLiteralData.size` (30,665 slots → 30,664 literals).
* Validation: offsets must be non-decreasing and `offs[-1] == literalData.size`.

## 4. Tokens

Standard ECMA-335 tables, unchanged from earlier versions:
`TypeDef 0x02`, `Field 0x04`, `MethodDef 0x06`, `Param 0x08`.
All 199,578 method / 136,294 field / 193,993 param / 26,498 typedef tokens in
this file carry the correct table byte (validated exhaustively).

Note typedef tokens are **not** strictly `index+1` ordered — the compiler emits
them in metadata-row order which mostly matches but occasionally swaps
(e.g. rows `1,2,4,3` near `<>c__DisplayClass` compiler-generated types).

## 5. Uncertainties / open questions

1. **Assembly word @8**: constant `1` across all 100 assemblies. The C#
   `Il2CppAssemblyDefinition` has no field between `Token` and
   `ReferencedAssemblyStart` that would explain it for v39 (and its
   `ModuleToken` — ≥38.0 — would land at offset 72, past the 68-byte record,
   so it is **not present on disk**). Possibly a versioning artifact of the
   metadata writer. Harmless: skipped as `unknown_word`.
2. **strings count slot** = 200,706 but the blob actually contains 201,177
   NUL terminators (201,176 strings + 1 trailing empty from the final NUL).
   The count slot is apparently not maintained for blob sections (same for
   `fieldAndParameterDefaultValueData` and `attributeData`, whose count ==
   size in bytes). Treat `count` as meaningless for the three blob tables.
3. **returnParameterToken sentinel** `0x08000000`: appears in every method
   where no return-parameter is named; exact semantics (named return
   parameters, C# 7-style `out var`?) not exercised by this file's parse.
4. **typedef `declaringType` width**: C# declares `TypeDefinitionIndex`
   (dynamically u16), but empirically offset 12 must be read as **int32**:
   with u16, the `parentIndex` field lands wrong and the
   all-methods-declare-owner cross-check fails. The method-side
   `declaringType` *is* u16. It is possible the writer simply pads/aligns the
   typedef struct differently than the reader model assumes; the int32 reading
   is proven by 199,578/199,578 methods agreeing with their owning typedef.
5. **`deleteMe` type** (row 1) and string "deleteMe": Unity's metadata
   generator placeholder; expected noise, not a parse error.

## 6. Validation summary (all pass)

* header 380 B; 31 sections contiguous, 4-aligned, end == EOF
* literal offsets monotonic, sentinel == data blob size, count = n+1
* Σ typedef.methodCount == methods.count (199,578)
* Σ typedef.fieldCount == fields.count (136,294)
* every method's `declaringType` == owning typedef index (0 mismatches)
* method parameter ranges tile [0, 193,993) exactly
* token table bytes: 0x06 / 0x04 / 0x08 / 0x02 exhaustively

Sample counts: **26,498 types, 199,578 methods, 136,294 fields, 193,993
parameters, 34,953 properties, 1,910 events, 30,664 string literals, 100
images/assemblies**. Identifier printability ≥ 99.9%.
