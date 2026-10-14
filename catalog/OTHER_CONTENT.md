# SWGoH 角色之外游戏内容全景调查报告

调查日期：2026-10-14。只读侦察，未导出任何资产。
数据文件：`other_content.json`（完整清单）；`_bundle_names.json`（11400 个 bundle 名/大小/mtime）；`_samples.json`（逐 bundle 对象类型采样）；`_sample.py`/`_rollup.py`（可复跑脚本）。

## 缓存总量

Unity 缓存 `C:\Users\PPfavorite\AppData\LocalLow\Unity\Electronic Arts_SWGoH\`：**11400 个 bundle，共 2.82 GB**。结构统一为 `<name>/<hash>/__data`，同名多 hash 时取 mtime 最新的 `__data`。缓存中**没有任何音频/视频/字体 bundle**（全部 11400 个名字经关键词扫描确认）。

## 内容类别全景表

| 类别 | 数量 | 大小 | 是什么 | 提取价值 |
|---|---|---|---|---|
| char_* | 747 | 833.6 MB | 角色 prefab（主线已导出） | 已完成 |
| **env_*** | **269** | **670.3 MB** | 环境场景 prefab：星球地块、建筑、走廊、太空背景 | ★★★ 最高，GLB 管线直接适用 |
| cinematic (cin_*) | 284 | 249.1 MB | 过场演出：约半数为 Timeline 数据文件，半数为演出 prefab | ★★ 拆分后 prefab 部分可用 |
| ability_* | 2596 | 223.4 MB | 技能内容：多数是"1 张图标贴图"的极小包；少数是完整 VFX prefab（如 `ability_ship_cap_ultimate_profundity` 含 36 Mesh） | ★ 图标批量提 PNG；个别 VFX prefab 提贴图 |
| store_art（pack_*/newsletter_*/purchase_* 等） | 2678 | 200.8 MB | 商店礼包横幅、新闻报、月卡海报，每包恰好 1 张 Texture2D | ★★ 批量 PNG，零难度 |
| icon_art（icon/vanity/relic/equip/crewportrait/holoplanet/background 等） | 2299 | 197.8 MB | 图标、徽章、头像、装备图、星球全息图、章节背景，每包 1–7 张贴图 | ★★ 批量 PNG |
| ui_view（*_view/ui_*/atlas） | 363 | 90.9 MB | UI 界面 prefab（弹窗/列表/动效），少量贴图+图集 | ★ 只提图集/贴图，不提模型 |
| map_ui（conquest/coliseum/territory*ui/guild*ui 等） | 614 | 84.4 MB | 各玩法界面 prefab（征服地图节点等，纯 UI 层级） | ★ 同上 |
| **territorymap_*/tb3_*** | **18** | **66.3 MB** | 领土战/大会战 3D 地图（单包最多 230 Mesh） | ★★★ GLB 管线适用 |
| shared_* | 4 | 54.6 MB | 共享池：resourcecontainer（77 Mesh+156 贴图+71 SkinnedMesh）、animationcontainer（178 AnimationClip）、shadercontainer、uicontainer | ★★ 已知；是 ship/env 的外部依赖源 |
| scene（*_scene 后缀） | 6 | 34.9 MB | 命名场景：`col_01_arena_01(_cin)_scene` 竞技场、`pas_desert_01`、`kashyyk_beach`、`emp_throne_room_dest`、`fo_kylo_quarters_kijimi` | ★★★ 完整场景，44–47 Mesh/包 |
| **ship_*** | **92** | **29.4 MB** | 舰船模型 prefab，自含 Mesh+贴图（1–5 Mesh/包），如 Profundity、Outrider、B-28 瘟灭级轰炸机、Y-Wing、吉奥诺西斯战机 | ★★★ 舰队收藏，直接复用角色管线 |
| battle_*（含 arena_*/node_*） | 268 | 24.7 MB | 战斗布局配置：相机位、出生点、碰撞盒；`arena_fleet`/`arena_cantina` 等是竞技场背景板 | ★ 布局无美术；arena 背景板有 Mesh 可看 |
| other（未归类尾量） | 214 | 19.1 MB | 杂项：chip_*、circular_progress、`coruscant_jeditemple`（一处场景）、crate_bonus_zone、相机 rig | 混合，个别看 |
| charui_*/jg_* | 842 | 14.4 MB | 角色 UI 头像单贴图 | ★★ 批量 PNG |
| vfx_*/blaster_*/bantha_* 等 | 99 | 12.5 MB | 独立特效与投射物（爆能枪光束、炸弹），粒子为主 | ★ 只提特效贴图 |
| npc（cantinapatron_*/shar_*） | 7 | 11.4 MB | 酒馆 NPC 角色：完整 SkinnedMeshRenderer + Avatar + 21 个 AnimationClip（阿奎利什人、罗迪亚人、人类、赞格里安人等） | ★★★ 小而完整，管线直接适用 |

## 采样观察要点

- **env 两个流派**：`env_ruus_valley_jedi_01_pre`、`env_empire_hallway_pre` 等自含 Mesh（15–69 个/包）+ 贴图，可独立导出；`env_nab_raid_pre`（2485 GameObject、808 粒子系统、0 自含 Mesh）几乎全部引用共享池，导出需跟随外部依赖。
- **cin_*** 需二分：`*_tl`/`*_tltimeline`（67 个）是纯 Timeline/MonoBehaviour 数据，无美术；`cin_*_pre` 是含粒子/网格/AnimationClip 的演出 prefab。
- **单贴图包规律**：约 5800 个 bundle（store_art + icon_art + charui + ability 图标）内部只有一个 `tex.<bundle名>` 的 Texture2D，尺寸 64×64 到 2048×2048，是全游戏最简单的批量提取面。
- **conquest_map_layout_legendary_path_treasurenode_x3** 单包 9534 GameObject，是 UI 层级膨胀的极端个例，无模型价值。
- 投射物（`blaster_bolt01_pre`、`bantha_bomb_pile_pre`、`ballista_bolt01_pre`）在未归类列表中，属小型 VFX prefab。

## 安装目录结构（E:\Games\EA Games\SWGoH）

```
SWGoH.exe (319 KB)          Unity Player 启动器
GameAssembly.dll (99 MB)     IL2CPP 编译的全部游戏逻辑
UnityPlayer.dll (37 MB)      Unity 引擎
baselib.dll / UnityCrashHandler64.exe
Core/
Support/                     仅 EA 帮助 rtf / 用户协议 / readme（中英）
__Installer/
SWGoH_Data/ (168 MB)
├── il2cpp_data/Metadata/global-metadata.dat   (24 MB) ← IL2CPP 元数据
├── il2cpp_data/Resources/*.dat                (Newtonsoft/System.Data 资源)
├── Plugins/{ARM64,x86,x86_64}/
├── StreamingAssets/ (29 MB)
│   ├── Audio/GeneratedSoundBanks/StandaloneWindows64/*.pck   ← Wwise 音频银行，
│   │     仅 5 个共 ~14 MB（ENV_FTUE 13MB、MUS_Login、introCinematic 等）；
│   │     主音频不走本地文件，按需从 CDN 流
│   ├── SWGH_FTUE_Intro_Main_v01.mp4                           ← 唯一过场视频（FTUE 开场）
│   ├── ClientInfo.txt         ← 资源 CDN: assetssw.capitalgames.com，含 EA 加密 key
│   └── google-services-desktop.json
├── level0–2 (+resS)、globalgamemanagers(+assets)、resources.assets (35+89 MB resS)、sharedassets0–2
```

无 .json/.csv/.db 游戏数据表——数值与配置全部走服务器，本地只有 IL2CPP 二进制。缓存是全部美术资产的唯一本地来源。

## 美术资产价值结论与优先级

1. **env_*（670 MB）**：最大的未开发金矿。自含型子集可直接套角色 GLB 管线；引用共享型的需加外部依赖解析。建议先跑 10 个自含型验证。
2. **ship_*（92 个）**：舰队收藏，包小且自含，改动最小的管线复用，性价比最高，建议第一优先。
3. **scene（6 个）+ territorymap（18 个）**：完整 3D 场景/地图，单包信息量大。
4. **cantinapatron NPC（7 个）**：完整蒙皮角色+动画，管线零改动。
5. **约 5800 个单贴图包**：写一个 50 行的批量 PNG 转储即可全收（商店/图标/头像/UI 素材）。
6. **cin_* 演出 prefab**：二分后挑有 Mesh 的导出；Timeline 数据放弃。
7. **不可用**：shadercontainer、battle 布局配置、map_ui/ui_view 的控件层级、conquest 节点图、5 个 Wwise .pck（可用 wwise 解码器，但内容仅登录音乐与 FTUE 音效）。
