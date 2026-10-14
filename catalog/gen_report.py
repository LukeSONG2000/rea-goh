# -*- coding: utf-8 -*-
"""Generate catalog/BUNDLES.md from bundles.json + samples.json."""
import json, time
from collections import Counter

BASE = r"F:\PPfavorite\Work\goh\catalog"
es = json.load(open(BASE + r"\bundles.json", encoding="utf-8"))
sm = json.load(open(BASE + r"\samples.json", encoding="utf-8"))

TOTAL_N = len(es)
TOTAL_MB = sum(e["size_bytes"] for e in es) / 1024**2

cat_count, cat_mb = Counter(), Counter()
for e in es:
    cat_count[e["category"]] += 1
    cat_mb[e["category"]] += e["size_bytes"] / 1024**2

groups = [
    ("角色模型", {"char", "battle_character"}),
    ("技能特效", {"ability", "vfx", "persistentvfx", "shipskill", "shipability"}),
    ("场景/环境/地图", {"env", "territorymap", "tb3", "holoplanet", "node", "background",
                    "coruscant", "kashyyk", "col", "emp", "fo", "pas", "homebase",
                    "space", "territorywar", "era", "shipunlock"}),
    ("过场/演出", {"cin", "raidrancor", "raidaat", "raidsith", "raidkraytdragon", "raidnaboo",
                "raidorder66", "raidspeederbike", "patron", "challengetoast", "challengetoastui",
                "eventsparent"}),
    ("舰队", {"ship", "shippvp", "shippvp3v3", "shipmatchmaking", "fleet", "fleetcommanderskill",
             "gr75", "malevolence", "tie", "mg100", "speederbike"}),
    ("3D 肖像/道具", {"crewportrait", "commanderportrait", "cantinapatron", "droid", "bantha",
                  "ballista", "blaster", "greedo", "seatos", "pabu", "shar", "special"}),
    ("共享容器", {"shared"}),
    ("UI/系统杂项", None),  # fallback, computed last
]
ICON_CATS = {"pack", "packs", "icon", "chest", "consumable", "crystalpackart", "galacticbundle",
             "mosdpack", "purchase", "dynamicpack", "league", "guild", "territory", "newsletter",
             "neswletter", "vanity", "relic", "equip", "jg", "charui", "events", "abilityui",
             "datacronui", "contextualmessageui", "contexutalmessageui", "objective", "help",
             "circular", "cqonuest", "chip", "datacron", "datacrons", "conquest", "crewportrait"}
# order: specific groups first, then icon-tex, then fallback
gstat = []
for g, cats in groups[:-1]:
    n = sum(cat_count[c] for c in cats if c in cat_count)
    mb = sum(cat_mb[c] for c in cats if c in cat_mb)
    gstat.append((g, n, mb))
icon_n = sum(cat_count[c] for c in ICON_CATS if c in cat_count)
icon_mb = sum(cat_mb[c] for c in ICON_CATS if c in cat_mb)
gstat.append(("UI 图标/单张贴图", icon_n, icon_mb))
rest_n = TOTAL_N - sum(n for _, n, _ in gstat)
rest_mb = TOTAL_MB - sum(mb for _, _, mb in gstat)
gstat.append(("UI 界面 prefab / 系统杂项", rest_n, rest_mb))
gstat.sort(key=lambda x: -x[1])

def tag(cat):
    a = sm.get(cat)
    if not a:
        return "未采样"
    tt = a["types_total"]
    total = sum(tt.values()) or 1
    mesh = tt.get("Mesh", 0) + tt.get("SkinnedMeshRenderer", 0)
    ps = tt.get("ParticleSystem", 0)
    mb = tt.get("MonoBehaviour", 0)
    tex = tt.get("Texture2D", 0)
    anim = tt.get("AnimationClip", 0)
    if anim > total * 0.5:
        return "动画库"
    if total - tt.get("AssetBundle", 0) <= tex * 1.3 and mesh == 0 and anim == 0:
        return "单张贴图"
    if mesh >= 3 and (tt.get("MeshRenderer", 0) + tt.get("SkinnedMeshRenderer", 0)) > total * 0.08:
        return "3D 场景/模型"
    if anim > 0 and mesh >= 5:
        return "3D+动画"
    if ps > total * 0.15:
        return "粒子特效"
    if mb > total * 0.25 and tt.get("GameObject", 0) > 0:
        return "UI 界面"
    return "混合"

def rep_names(cat, k=3):
    a = sm.get(cat)
    if not a:
        return ""
    names = a["go"] or a["ta"] or a["tex"] or a["clip"] or a["mesh"] or a["container_samples"] or []
    out = []
    for n in names[:k]:
        s = str(n).replace("|", "/")
        if len(s) > 34:
            s = s[:32] + ".."
        out.append(s)
    return " / ".join(out)

L = []
w = L.append
w("# SWGoH Unity 缓存全量资源清单（BUNDLES.md）")
w("")
w(f"- 扫描根目录：`C:\\Users\\PPfavorite\\AppData\\LocalLow\\Unity\\Electronic Arts_SWGoH\\`")
w(f"- 生成时间：{time.strftime('%Y-%m-%d %H:%M')}")
w(f"- 总计：**{TOTAL_N} 个 bundle**，**{TOTAL_MB:.0f} MB（约 {TOTAL_MB/1024:.2f} GB）**，{len(cat_count)} 个名称前缀分类")
w("- 结构：每个 bundle 是 `<名称>/<hash>/__data`；同名多 hash 时本清单取 `__data` mtime 最新的一个")
w("- 明细数据：`bundles.json`（全部 11400 条：bundle 名 / 最新 __data 路径 / 大小 / 分类）；"
  "`samples.json`（每类抽样打开的 UnityPy 对象统计）")
w("- 采样方法：除 `char_`（按要求只计数）外，每个前缀分类均匀抽样 2~10 个用 UnityPy 打开，"
  "共 981 个、0 失败；`char_` 单独对照已知管线结论，不重复打开")
w("")
w("## 一、宏观分类统计")
w("")
w("| 大类 | bundle 数 | 体积 |")
w("|---|---:|---:|")
for g, n, mb in gstat:
    w(f"| {g} | {n} | {mb:.1f} MB |")
w(f"| **合计** | **{TOTAL_N}** | **{TOTAL_MB:.1f} MB** |")
w("")
w("## 二、主前缀分类明细（数量 ≥ 8 的分类，按数量排序）")
w("")
w("「内容」列为抽样实测结论（UnityPy 对象类型推断）。")
w("")
w("| 前缀 | 数量 | 体积 MB | 内容 | 代表对象名 |")
w("|---|---:|---:|---|---|")
big = [c for c in cat_count if cat_count[c] >= 8]
big.sort(key=lambda c: (-cat_count[c], c))
for c in big:
    w(f"| {c} | {cat_count[c]} | {cat_mb[c]:.2f} | {tag(c)} | {rep_names(c)} |")
w("")
w("其余约 250 个前缀均为 1~7 个 bundle 的小分类（绝大多数是单一 UI 界面 prefab 或单张贴图），"
  "完整清单见 `bundles.json`，不再列表。")
w("")
w("## 三、内容大类说明")
w("")
w("### 1. char_ 角色模型（747 个，约 460 MB）")
w("每个角色一个 bundle（如 `char_ackbar_pre`），内含角色 prefab、SkinnedMeshRenderer 网格、"
  "骨架 Avatar、材质与贴图。已有主导出管线负责，本次只计数未打开。")
w("另有 `char_kylo_cutscene_pre` 这类过场专用角色变体。")
w("")
w("### 2. ability_* 技能特效（2596 个，约 200 MB）+ vfx/persistentvfx（101 个）")
w("数量最多的分类。每个技能一个 bundle，内容是粒子系统（ParticleSystem/ParticleSystemRenderer 约占一半对象）、"
  "弹道网格、冲击波、烟火花等，自带少量贴图与 shader 引用。"
  "抽样还发现 ability bundle 内同时携带 `tex.ability_XXX_basic.png` 技能图标。"
  "`vfx_*`（95 个）是环境级通用特效（雾、背景光效），`persistentvfx` 是常驻特效库（13 个 shader + 通用粒子）。"
  "对恢复技能视觉表现价值极高，但纯粒子无法直接转成普通模型资产。")
w("")
w("### 3. env_* 场景环境（269 个，约 540 MB）——最大的非角色 3D 资产")
w("完整战斗/展示场景：抽样平均每场景约 170 个 GameObject、180 个 MeshRenderer/MeshFilter、"
  "200+ Mesh、40+ 材质、30+ 贴图，含粒子光效和部分 Timeline。容器路径形如 "
  "`assets/resourcestobundle/base/environment/bespin/docks/env.env_bespin_docks_pre.prefab`。"
  "场景覆盖 Bespin、Tatooine（莫斯艾斯利两个拼写变体都缓存了）、Hoth（含 wampa 洞穴）、"
  "Geonosis、Kashyyyk、Ord Mantell、太空场景（Crait 等）、纳布 raid（9.4 MB，最大 env）、"
  "主界面 cantina 等。**这是角色之外最值得批量提取的一类**。")
w("")
w("### 4. cin_* 过场动画（283 个，约 130 MB）")
w("战斗入场/胜利过场与活动剧情演出：GameObject + Timeline（.playable，PlayableDirector）+ "
  "AnimationClip + 粒子 + 部分 SkinnedMeshRenderer（26 个/抽样，说明内嵌简易角色或道具网格）+ "
  "blaster bolt 等特效贴图。路径如 `cinematics/events/maulhf/cin_maulhf_tier04_intro_tl.playable`。"
  "对还原过场镜头、复用其中的动画与特效资产有价值。")
w("")
w("### 5. ship_* 舰队（91 个，约 30 MB）+ 散装舰船")
w("舰船模型 bundle：X-wing/A-wing/B-wing、Slave I、Chimaera、ARC-170、Imperial Shuttle、"
  "Blockade Runner、HMP Droid Gunship 等，带 HitArea/引擎火焰粒子/材质 mask 贴图"
  "（`ship_slave1_tex` + `ship_slave1_mask`）。容器路径 `base/unit/ship/<名>/unit.ship_XXX_pre.prefab`。"
  "另有散装：`gr75_transport_pre`（GR-75 货船）、`malevolence`、`tie`（TIE fighter）、`mg100`。"
  "`shipunlock_celebrationview`（0.5 MB）是全息台 unlock 演出场景，含 holotable 网格。")
w("")
w("### 6. 独立场景 *_scene 与地图类")
w("- `col_01_arena_01_scene`（7.6 MB）+ `col_01_arena_01_cin_scene`（9.5 MB）：**大竞技场完整 3D 场景**，"
  "44 个网格含观众席排、横幅、岩壁，43 张专属贴图。")
w("- `pas_desert_01_scene`（5.7 MB，沙漠/贾库）、`kashyyk_beach_scene`（2.6 MB，卡希克海滩，含 Yavin 植被与 gunship 低模）、"
  "`emp_throne_room_dest_scene`（2.4 MB，**毁坏后的皇帝王座厅**，带 Lightmap 与死星天空盒）、"
  "`fo_kylo_quarters_kijimi_scene`（3.0 MB，凯洛·伦寝室）、`coruscant_jeditemple`（0.7 MB，绝地圣殿）。")
w("- `territorymap_*`（12 个，约 40 MB）：领土战 3D 星图/地图，`territorymap_tb3_mixed` 16 MB、6100+ 对象、"
  "405 个网格（行星、扇区面板、边界线），Geonosis/Hoth 明暗两版。")
w("- `tb3_*`（6 个）：Territory Battle 死星/Kafrene/基地等 3D 面板。")
w("- `node_*`（22 个）：战役星图上的 3D 行星节点（deathstar、kashyyyk 等 planet_mesh）。")
w("- `holoplanet_*`（13 个）：全息行星模型。")
w("")
w("### 7. 3D 肖像与角色道具")
w("- `crewportrait_*`（63 个）：**带骨架的 3D 头雕/半身像**（SkinnedMeshRenderer + JNT 骨骼 + 动画），飞行员等船员肖像。")
w("- `commanderportrait_*`（12 个）：指挥官 3D 半身像，同上带骨架与动画。")
w("- `cantinapatron_*`（6 个）：酒馆 NPC 3D 模型（12 网格 + SkinnedMeshRenderer）。")
w("- 小型道具/载具：`bantha_bomb_pile_pre`、`ballista_bolt01_pre`、`blaster_bolt01_pre`、`greedo_hand`、"
  "`droid_damage_01..03`（战斗伤害残骸）、`speederbike_*`、`shar`、`patron_playable_director_pre`。")
w("")
w("### 8. UI 图标/单张贴图类（约 6000 个 bundle，约 340 MB）")
w("数量最大的形态：**一个 bundle 只装一张 Texture2D**，无网格。包括：")
w("- `charui`（769）：角色头像立绘 `tex.charui_*`")
w("- `pack`/`packs`（2047）：商城卡包/捆绑包宣传图")
w("- `events`（751）+ `objective`（95）：活动横幅与目标图")
w("- `newsletter`+`neswletter`（552）：新闻角色宣传图（后者为官方拼写错误变体）")
w("- `abilityui`（364）：技能图标；`vanity`（350）：玩家头像框；`relic`（315）：遗物物品图")
w("- `equip`（152）：装备图标；`icon`（135）：通用 UI 图标；`jg`（73）：Journey Guide 头像（`glhondo` 一张就 0.9 MB）")
w("- `guild`/`territory`/`league`/`datacronui`/`contextualmessageui`/`consumable`/`chest`/`purchase` 等："
  "公会徽章、领土战背景、联盟段位框、酒馆 NPC 对话立绘等")
w("这类对插画/立绘收集有价值（都是官方原画 PNG），对 3D 恢复无直接价值，但可批量一键导出。")
w("")
w("### 9. UI 界面 prefab 类（约 1800 个 bundle）")
w("`ui`、`conquest`（44 个但对象最多，抽样合计 2 万+ MonoBehaviour）、`inboxui`、`guildui`、`territoryui`、"
  "`coliseum`、`datacron(s)`、`unitguide`、`charactermods`、`inventoryui`、数百个 `*ui` 单体界面等。"
  "内容是 UGUI 面板：GameObject+RectTransform+BoxCollider+字体（futura_bold、standard_body_font）+图集引用，"
  "部分带 PlayableDirector 动效（modenhancement 抽样 210 个）。"
  "对 UI 还原/参考有价值，对 3D 资产恢复价值低。")
w("")
w("### 10. shared_* 共享容器（4 个，约 28 MB）——单位体积价值最高")
w("- `shared_animationcontainer`（4 MB）：**178 个人形共享 AnimationClip**（hmn_pst_var01_run、hmn_rfl_var01_atk、"
  "双持 idle、cantina 互动动画等），挂在通用 humanoid 骨架上，可跨角色复用。")
w("- `shared_resourcecontainer`（7.1 MB）：77 个共享网格 + **71 个 SkinnedMeshRenderer + 15 个 Avatar（通用骨架）** "
  "+ 156 张共享贴图（matcap_skin/leather、VFX 噪声、scan_line 等）。共享身体部件（风暴兵肩甲、贾瓦、起义军飞行员等）。")
w("- `shared_shadercontainer`（0.15 MB）：39 个 shader（particles-additive、vfx-holo-fade、unlit 系列等），"
  "是解读所有材质渲染方式的钥匙。")
w("- `shared_uicontainer`（16.8 MB，全场最大 bundle）：UI 图集（unit_atlas、standard_atlas、battleui atlas 等 8 张大图 + 材质 + UI 动画 asset）。")
w("")
w("### 11. 音频 / 视频 / 本地化文本：**缓存中不存在**")
w("全部 981 个抽样 bundle 中 **AudioClip=0、VideoClip=0、TextAsset=0**，bundle 名里也没有 audio/video/lang/"
  "movie/sound 类目。结论：SWGoH 的音频（应为 FMOD bank）、过场视频与本地化文本表不走 Unity AssetBundle "
  "缓存（在安装包内、其他目录或服务端下发），无法从本缓存恢复。")
w("")
w("### 12. 其他杂项")
w("- `5-lighting-do-not-touch`：1 个 bundle，内含 **210 个 Light** 的全局光照 rig（无网格），"
  "是所有场景的标准布光方案，对场景重建打光有参考价值。")
w("- `mat`：仅 1 张 `sliceabundle_atlas.mat` 图集；`bak`/`scv_001..007`/`empty`：几百字节~8KB 的占位/系统小 bundle。")
w("- `background_episode_04..09`：每张约 1 MB 的章节大背景图（单 Texture2D）。")
w("- `battle_character_*`（96 个）：战斗布局容器（`prefab.battle_character_event_glrey_layout`），"
  "是站位/队列配置 prefab，不是角色模型本体，模型仍在 char_ 里。")
w("- `battle_*`（60 个）：战斗摄像机 rig、队伍标记、raid 模板。")
w("- `packopening`（26 个）：抽卡数据卡 3D 模型 + 演出粒子。")
w("- `challengetoast` 系列：奖杯/挑战 3D 展示（大量 PlayableDirector）。")
w("")
w("## 四、值得后续提取的 Top 10（非角色）")
w("")
w("| # | 目标 | 为什么 |")
w("|---|---|---|")
w("| 1 | `env_*` 全部 269 个 | 完整场景网格+贴图+光照，量最大、还原度最高 |")
w("| 2 | `shared_animationcontainer` | 178 个人形动画，直接适配通用骨架 |")
w("| 3 | `shared_resourcecontainer` | 共享网格 + 15 个 Avatar 骨架定义 + matcap/VFX 贴图 |")
w("| 4 | `cin_*` 全部 283 个 | 过场演出：动画、特效、内嵌网格与镜头 |")
w("| 5 | `ship_*` 91 个 + gr75/malevolence/tie/mg100 | 全部舰船模型（含 mask 贴图） |")
w("| 6 | 5 个 `*_scene` + `coruscant_jeditemple` + `col_01_arena_01_*` | 独立完整场景（竞技场/王座厅/绝地圣殿等） |")
w("| 7 | `crewportrait_*` + `commanderportrait_*` + `cantinapatron_*` | 81 个带骨架的 3D 头雕/半身像，char_ 之外唯一的角色级模型 |")
w("| 8 | `territorymap_*` + `tb3_*` | 3D 星图与战场地图（最大单 bundle 16 MB） |")
w("| 9 | `vfx_*` + `persistentvfx` + `shared_shadercontainer` | 通用特效库 + shader 集，是还原材质的关键 |")
w("| 10 | `node_*` + `holoplanet_*` + `shipunlock_celebrationview` + `5-lighting-do-not-touch` | 3D 行星、全息台场景、标准灯光 rig |")
w("")
w("## 五、限制与备注")
w("- 缓存只有约 2 GB，明显是「按需下载」的子集：本地化/音频/视频/部分旧活动资源不在其中。")
w("- 图标类 bundle 单独打开成本极低（每包 1 张贴图），可整类批量导出；env/cin/ship 建议逐包处理。")
w("- `char_` 共 747 个按约定未打开；数量、体积来自文件系统统计。")
w("- 同名 bundle 的旧 hash 目录仍留在磁盘上，如需考古旧版本可改取非最新 hash。")

out = BASE + r"\BUNDLES.md"
with open(out, "w", encoding="utf-8", newline="\n") as f:
    f.write("\n".join(L) + "\n")
print("OK wrote", out, len(L), "lines")
