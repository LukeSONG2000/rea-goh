# character_names.json — 模型内部 ID → 角色名 对照表

**文件**: `catalog/character_names.json`（747 条，对应 `models.json` 全部 char_* 预制体）
**生成脚本**: `catalog/build_names.py`（可重复运行；依赖同目录下的 gamedata 数据文件）

## 结论（数据在哪找到的）

1. **本地查不到**。游戏安装目录 `E:\Games\EA Games\SWGoH` 与 Unity 缓存
   `C:\Users\PPfavorite\AppData\LocalLow\Unity\Electronic Arts_SWGoH\`（11401 个 bundle，2.7 GB）中
   **没有任何 TextAsset/本地化表/gamedata**——此前项目抽样 981 个 bundle：TextAsset=0。
   `Prefs` 里只有版本号 key（`LOC_VERSION_KEY`、`LAST_GAMEDATA_VERSION_ONE_TIME_EVICTED_KEY`），
   说明 gamedata 与本地化文本是**运行时从 Capital Games CDN 下载、只在内存里用，不落盘**
   （唯一落盘例外是 `Cache/Conquest/*.dat`，与角色名无关）。
2. 因此改用**官方 gamedata 的社区镜像**：
   `https://github.com/swgoh-utils/gamedata`（= README 里亦写作 scuba75/swgoh-gamedata，
   由 c3pobot/gamedatasync 在每次游戏更新后约 10 分钟内自动同步，GPL-3.0）。
   直连 GitHub 被网络重置，用 `fastly.jsdelivr.net` 等镜像下载，已存到本目录：
   - `units.json`（2107 行，每行含 **`unitPrefab`: `"unit.char_jynerso_pre"`** + `nameKey`）
   - `units_pve.json`（6500 行，NPC/PVE 单位，同结构）
   - `units_gas.json`（2877 行，无 unitPrefab 字段，但含 baseId + nameKey，用于兜底）
   - `loc_en.json` / `loc_cn.json`（= Loc_ENG_US.txt.json / Loc_CHS_CN.txt.json，nameKey → 显示名，英文+中文）
3. 关键事实：官方 unit 表里的 `unitPrefab` 与导出文件名**完全一致**（`unit.char_jynerso_pre` ↔ `char_jynerso_pre`），
   所以对得上的部分是**权威精确映射**，不是猜测。

## 字段说明

```json
"char_jynerso_pre": {
  "defId": "JYNERSO",          // 游戏 defId / baseId
  "name_en": "Jyn Erso",       // 英文显示名（官方 Loc_ENG_US）
  "name_zh": "珍·厄索",         // 中文显示名（官方 Loc_CHS_CN）
  "match": "exact",            // 匹配方式，见下表
  "detail": null,              // 补充说明（用了哪个核心名/哪个 loc key）
  "obtainable": true,          // 是否玩家可获取单位（NPC=false）
  "name_verified": true,       // 名称是否经过官方数据验证
  "source": "https://github.com/swgoh-utils/gamedata ..."
}
```

`match` 取值：
| 值 | 含义 | 数量 |
|---|---|---|
| `exact` | unitPrefab 精确命中（权威） | 472 |
| `derived` | 去掉美术变体后缀（_raid/_noshield/_low/_lstb/_boss/_cin/_speederbike…）后命中 | 90 |
| `exact_baseId` | unitPrefab 缺失，prefab 核心名 == baseId（units_gas 表兜底，如 4LOM、CAPTAINREX） | 85 |
| `derived_baseId` | 词序变换命中（如 ahsokatano_gl → GLAHSOKATANO） | 4 |
| `derived_name` | 按英文显示名 slug 命中 | 1 |
| `+lockey_fallback` | loc 里 nameKey 缺失，改用 UNIT_<短名>_NAME（如 CAPTAINREX→UNIT_REX_NAME） | 4 |
| `+name_curated` | loc 快照缺该 key，显示名来自人工整理（参考 swgoh.gg/社区），**name_verified=false** | 68 |
| `unmatched_in_gamedata` | 镜像的 units 表里完全找不到（见下） | 104 |

## 覆盖率

- **defId 映射：643/747（86%）**
- **显示名：643/747（86%）**，其中官方 loc 验证 575，人工整理 68
- **中文名：643/747**（与英文同源，官方 CHS_CN 本地化）

### 为什么不是 100%

1. **镜像数据本身不完整/略滞后**：`units.json+units_pve.json` 只有 574 个不同 unitPrefab，
   明显少于当前游戏（0.40.7）的实际单位数；`loc_en.json` 只有 736 个 `UNIT_*_NAME`，
   缺 Baylan Skoll、Shin Hati、Kleya、Vel、Cinta 等 2023-24 新单位和 4-LOM、Zuckuss 等老单位。
2. **104 个 unmatched 的构成**：PVE/NPC 美术变体（`char_b1snow*`、`char_atst_boss*`、`char_bantha_pre`、
   `char_bibfortuna_pre`、`char_calkestisjediknight_pre` 等）——它们不在玩家单位表里，
   只在战斗环境配置里引用；R5 系列 astromech 等散件同样缺失。清单见 `_unmatched.json`。
3. 68 个人工名称中 `RACCOON`（社区只用 base_id 称呼，具体显示名待查）、`Hunter (Season 3)`、
   `The Ronin`、`Darth Vader (Duel's End)` 等写法是最佳推断，字段里已标 `name_verified: false`。

## 示例

```json
"char_jynerso_pre"      -> defId JYNERSO      "Jyn Erso"          / 珍·厄索 (exact)
"char_4lom_pre"         -> defId 4LOM         "4-LOM"             (exact_baseId + curated)
"char_captainrex_pre"   -> defId CAPTAINREX   "Captain Rex"       (exact_baseId + curated)
"char_ahsokatano_gl_pre"-> defId GLAHSOKATANO "Ahsoka Tano (GL)"  (derived_baseId + curated)
"char_acklay_pre"       -> defId ACKLAY_PVE   "Acklay"            (exact, NPC obtainable=false)
"char_000_pre"          -> defId TRIPLEZERO   "0-0-0"             (exact)
"char_50rt_pre"         -> defId 50RT         "50R-T"             (exact)
"char_aaylasecura_pre"  -> defId AAYLASECURA  "Aayla Secura"      / 艾拉·塞库拉 (exact)
"char_b1commander_pre"  -> defId B1COMMANDER  "B1 Commander"      (exact)
"char_ackbaradmiral_pre"-> defId ADMIRALACKBAR "Admiral Ackbar"   (exact)
```

## 复现

```bash
cd catalog && py build_names.py
```

脚本按 T1 unitPrefab 精确 → T2 变体剥离 → T3 baseId 直等 → T4 词序变换 → T5 名称 slug →
loc key 变体 → 人工名字典 的顺序匹配，全部数据文件已缓存于本目录，离线可重跑。
