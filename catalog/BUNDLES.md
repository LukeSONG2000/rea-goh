# SWGoH Unity 缓存全量资源清单（BUNDLES.md）

- 扫描根目录：`C:\Users\PPfavorite\AppData\LocalLow\Unity\Electronic Arts_SWGoH\`
- 生成时间：2026-10-14 16:02
- 总计：**11400 个 bundle**，**2012 MB（约 1.96 GB）**，292 个名称前缀分类
- 结构：每个 bundle 是 `<名称>/<hash>/__data`；同名多 hash 时本清单取 `__data` mtime 最新的一个
- 明细数据：`bundles.json`（全部 11400 条：bundle 名 / 最新 __data 路径 / 大小 / 分类）；`samples.json`（每类抽样打开的 UnityPy 对象统计）
- 采样方法：除 `char_`（按要求只计数）外，每个前缀分类均匀抽样 2~10 个用 UnityPy 打开，共 981 个、0 失败；`char_` 单独对照已知管线结论，不重复打开

## 一、宏观分类统计

| 大类 | bundle 数 | 体积 |
|---|---:|---:|
| UI 图标/单张贴图 | 6186 | 371.5 MB |
| 技能特效 | 2702 | 219.6 MB |
| 角色模型 | 843 | 468.4 MB |
| UI 界面 prefab / 系统杂项 | 776 | 81.5 MB |
| 场景/环境/地图 | 367 | 644.2 MB |
| 过场/演出 | 318 | 141.6 MB |
| 舰队 | 106 | 29.4 MB |
| 3D 肖像/道具 | 98 | 27.8 MB |
| 共享容器 | 4 | 28.1 MB |
| **合计** | **11400** | **2012.1 MB** |

## 二、主前缀分类明细（数量 ≥ 8 的分类，按数量排序）

「内容」列为抽样实测结论（UnityPy 对象类型推断）。

| 前缀 | 数量 | 体积 MB | 内容 | 代表对象名 |
|---|---:|---:|---|---|
| ability | 2596 | 207.42 | 粒子特效 | Side_Glw  / blursmoke / Muzl_Flash  |
| pack | 1990 | 136.43 | 单张贴图 | tex.pack_cobbvanth_bundle / tex.pack_4lom / tex.pack_energy_pve_ship |
| charui | 769 | 8.49 | 单张贴图 | tex.charui_4lom / tex.charui_calonord / tex.charui_espalpatine_pre |
| events | 751 | 63.79 | 单张贴图 | tex.events_4lom / tex.events_dathomir_battle / tex.events_icon_calkestis |
| char | 747 | 464.23 | 未采样 |  |
| newsletter | 536 | 29.16 | 单张贴图 | tex.newsletter_4lom / tex.newsletter_chirrutbaze / tex.newsletter_executor |
| abilityui | 364 | 3.25 | 单张贴图 | tex.abilityui_active_defensiveba.. / tex.abilityui_passive_assist / tex.abilityui_passive_compassion.. |
| vanity | 350 | 3.37 | 单张贴图 | tex.vanity_4lom / tex.vanity_c3po / tex.vanity_crate |
| relic | 315 | 7.63 | 单张贴图 | tex.relic_bow_ewokscout / tex.relic_item_chewiec3po / tex.relic_item_gunganphalanx |
| cin | 283 | 137.44 | 3D+动画 | Disable / Materialize Actor / DefaultUnitStartCutscene |
| env | 269 | 545.22 | 3D 场景/模型 | mana_console_03_mesh (1) / Quad (26) / vfx_ray_light |
| equip | 152 | 1.53 | 单张贴图 | tex.equip_armatek_armor_plating / tex.equip_armatek_wrist_band_pro.. / tex.equip_armatek_data_salvage |
| ui | 152 | 7.67 | UI 界面 | Pending Label / Four Star Rarity / Pip (12) |
| icon | 135 | 1.61 | 单张贴图 | tex.icon_cascading_offer / tex.icon_currency_modreroll / tex.icon_datacrons_upgrade_b_3 |
| battle_character | 96 | 4.22 | 3D+动画 | All / FrontRow / Actor Track |
| objective | 95 | 45.41 | 单张贴图 | tex.objective_4lom / tex.objective_captainsilvo / tex.objective_crosshair |
| vfx | 95 | 11.45 | 3D+动画 | background_fog (1) / env.vfx_bar_conquest8_pre / background_fog |
| ship | 91 | 25.99 | 3D+动画 | Scaler / unit.ship_geonosis_fighter_soldi.. / HitArea2 |
| guild | 90 | 4.29 | 单张贴图 | tex.guild_banner_starter / tex.guild_events_speederbike / tex.guild_icon_empire_small |
| jg | 73 | 5.29 | 单张贴图 | tex.jg_charui__jedimastermacewindu / tex.jg_charui_espalpatine / tex.jg_charui_cassianundercover |
| territory | 73 | 5.55 | 单张贴图 | tex.territory_backdrop_firstaid / tex.territory_backdrop_geonosis_.. / tex.territory_backdrop_hoth_turr.. |
| crewportrait | 63 | 9.23 | 3D+动画 | firstordertiepilot_sf_pip_mesh / Left_wpn_offsetJNT / Right_elbwJNT |
| battle | 60 | 2.66 | 混合 | 3-UnitMarker / 6-UnitMarker (Ally) / Camera |
| packs | 57 | 3.37 | 单张贴图 | tex.packs_4daysofxmas_bundle_01 / tex.packs_4daysofxmas_hothleia_b.. / tex.packs_holidaywelcome_bundle |
| datacronui | 45 | 0.44 | 单张贴图 | tex.datacronui_affix_501st / tex.datacronui_affix_bountyhunter / tex.datacronui_affix_ewok |
| conquest | 44 | 15.15 | UI 界面 | standard_conquestflexibletile / Are you sure / futura_bold |
| contextualmessageui | 44 | 4.01 | 单张贴图 | tex.contextualmessageui_abednedo / tex.contextualmessageui_abednedo04 / tex.contextualmessageui_bartende.. |
| datacron | 41 | 8.75 | 3D+动画 | ui.datacron_affix_effecttargetIVP / standard_body_font_ref / Dimmed |
| league | 38 | 0.87 | 单张贴图 | tex.league_icon_aurodium_3 / tex.league_icon_all / tex.league_icon_bronzium_1 |
| territoryui | 30 | 3.51 | UI 界面 | Background / Scanlines / ui.territoryui_guildorderbackgro.. |
| coliseum | 26 | 8.97 | UI 界面 | Table / standard_body_font_ref / Ability Name |
| packopening | 26 | 3.84 | 混合 | Data_scroll / flare (4) / prefab.packopening_datacard_boba.. |
| scavenger | 26 | 1.04 | UI 界面 | Header Container / futura_bold / Progress Overflow Label |
| inboxui | 24 | 1.24 | UI 界面 | InboxMessage / Idle Animation / StrikeThru |
| node | 22 | 3.14 | 3D+动画 | prefab.node_icon_deathstar_pre / planet_deathstar_mesh / planet_mesh |
| unitguide | 21 | 2.19 | UI 界面 | futura_bold / EventTitleText / EventTypeText |
| datacrons | 20 | 1.88 | UI 界面 | ui.datacrons_animating_bracket / Bracket / futura_bold |
| raidrancor | 20 | 0.62 | 混合 | Character Track Group / Camera / Camera Shake Action |
| consumable | 19 | 0.16 | 单张贴图 | tex.consumable_promo_star01_blue / tex.consumable_promo_star01_yellow / tex.consumable_promo_star01_purple |
| guildraidmission | 18 | 1.02 | UI 界面 | All Rules Content / Ability Table Prototype / Background |
| guildui | 18 | 1.69 | UI 界面 | standard_badge_label / CornerHighlight / Percent Label |
| purchase | 18 | 6.39 | 单张贴图 | tex.purchase_era01_back / tex.purchase_era02_back / tex.purchase_era03_back |
| charactermods | 16 | 1.75 | UI 界面 | Sale Tab / Button / Boost Icon |
| chest | 16 | 0.43 | 单张贴图 | tex.chest_gc_00 / tex.chest_gc_04 / tex.chest_gc_02 |
| neswletter | 16 | 1.22 | 单张贴图 | tex.neswletter_era03_erabattle01 / tex.neswletter_era04_erabattle01 / tex.neswletter_era06_erabattle01 |
| objectives | 16 | 3.46 | UI 界面 | futura_bold / vfx effect / WrapGrid |
| help | 15 | 0.60 | 单张贴图 | tex.help_achievements / tex.help_basicattack / tex.help_cooldown |
| inventoryui | 15 | 1.89 | UI 界面 | Stamina Percentage Boosted / Background / scanlines |
| mosdpack | 14 | 1.27 | 单张贴图 | tex.mosdpack_jp_3pack_a / tex.mosdpack_jp_3pack_a_plus / tex.mosdpack_jp_3pack_b_plus |
| battleui | 13 | 1.69 | UI 界面 | Keyboard Binding Label / futura_bold / Rewards Scroll |
| era | 13 | 6.89 | 3D+动画 | TEL Brackets / Background / scanlines |
| holoplanet | 13 | 1.60 | 混合 | prefab.holoplanet_bespin / planet_bespin / planet_concorddawn |
| territorywar | 13 | 3.87 | 3D 场景/模型 | MeshBaker_rock_02b-mesh-mesh / MeshBaker_xwing-mesh-mesh / stardestroyer_engines |
| commanderportrait | 12 | 5.46 | 3D+动画 | Right_ankleJNT / Right_wpnJNT / Right_kneeJNT |
| territorymap | 12 | 38.78 | 混合 | glow / VFX / tb_outline_11_mesh |
| circular | 11 | 0.06 | 单张贴图 | tex.circular_glow / tex.circular_overlay / tex.circular_progress_02 |
| guildraid | 11 | 1.56 | UI 界面 | Unit Name Label / Texture / Unit Container |
| sliceabundle | 11 | 1.51 | UI 界面 | Table / Selected Arrow / Bg |
| challenge | 10 | 0.89 | 单张贴图 | tex.challenge_agi / tex.challenge_astromech / tex.challenge_hardware |
| storeui | 10 | 1.48 | 3D+动画 | standard_flexibletile_tag / alt_background / badge |
| skill | 9 | 0.15 | 单张贴图 | tex.skill_abilitymaterial_ultimate / tex.skill_hexagon_white / tex.skill_omicron |
| unitdetailsui | 9 | 1.60 | UI 界面 | Tag 1 / Button / Overflow Tag Template |
| arena | 8 | 0.51 | UI 界面 | Background / Inactive Scrim / Panel Title Bar |
| crystalpackart | 8 | 0.89 | 单张贴图 | tex.crystalpackart_1 / tex.crystalpackart_2 / tex.crystalpackart_3 |
| questgiver | 8 | 0.07 | 单张贴图 | tex.questgiver_icon_abednedo / tex.questgiver_icon_cantinakeeper / tex.questgiver_icon_gran |
| settingsui | 8 | 0.60 | UI 界面 | UI Root / futura_bold / Music Label |

其余约 250 个前缀均为 1~7 个 bundle 的小分类（绝大多数是单一 UI 界面 prefab 或单张贴图），完整清单见 `bundles.json`，不再列表。

## 三、内容大类说明

### 1. char_ 角色模型（747 个，约 460 MB）
每个角色一个 bundle（如 `char_ackbar_pre`），内含角色 prefab、SkinnedMeshRenderer 网格、骨架 Avatar、材质与贴图。已有主导出管线负责，本次只计数未打开。
另有 `char_kylo_cutscene_pre` 这类过场专用角色变体。

### 2. ability_* 技能特效（2596 个，约 200 MB）+ vfx/persistentvfx（101 个）
数量最多的分类。每个技能一个 bundle，内容是粒子系统（ParticleSystem/ParticleSystemRenderer 约占一半对象）、弹道网格、冲击波、烟火花等，自带少量贴图与 shader 引用。抽样还发现 ability bundle 内同时携带 `tex.ability_XXX_basic.png` 技能图标。`vfx_*`（95 个）是环境级通用特效（雾、背景光效），`persistentvfx` 是常驻特效库（13 个 shader + 通用粒子）。对恢复技能视觉表现价值极高，但纯粒子无法直接转成普通模型资产。

### 3. env_* 场景环境（269 个，约 540 MB）——最大的非角色 3D 资产
完整战斗/展示场景：抽样平均每场景约 170 个 GameObject、180 个 MeshRenderer/MeshFilter、200+ Mesh、40+ 材质、30+ 贴图，含粒子光效和部分 Timeline。容器路径形如 `assets/resourcestobundle/base/environment/bespin/docks/env.env_bespin_docks_pre.prefab`。场景覆盖 Bespin、Tatooine（莫斯艾斯利两个拼写变体都缓存了）、Hoth（含 wampa 洞穴）、Geonosis、Kashyyyk、Ord Mantell、太空场景（Crait 等）、纳布 raid（9.4 MB，最大 env）、主界面 cantina 等。**这是角色之外最值得批量提取的一类**。

### 4. cin_* 过场动画（283 个，约 130 MB）
战斗入场/胜利过场与活动剧情演出：GameObject + Timeline（.playable，PlayableDirector）+ AnimationClip + 粒子 + 部分 SkinnedMeshRenderer（26 个/抽样，说明内嵌简易角色或道具网格）+ blaster bolt 等特效贴图。路径如 `cinematics/events/maulhf/cin_maulhf_tier04_intro_tl.playable`。对还原过场镜头、复用其中的动画与特效资产有价值。

### 5. ship_* 舰队（91 个，约 30 MB）+ 散装舰船
舰船模型 bundle：X-wing/A-wing/B-wing、Slave I、Chimaera、ARC-170、Imperial Shuttle、Blockade Runner、HMP Droid Gunship 等，带 HitArea/引擎火焰粒子/材质 mask 贴图（`ship_slave1_tex` + `ship_slave1_mask`）。容器路径 `base/unit/ship/<名>/unit.ship_XXX_pre.prefab`。另有散装：`gr75_transport_pre`（GR-75 货船）、`malevolence`、`tie`（TIE fighter）、`mg100`。`shipunlock_celebrationview`（0.5 MB）是全息台 unlock 演出场景，含 holotable 网格。

### 6. 独立场景 *_scene 与地图类
- `col_01_arena_01_scene`（7.6 MB）+ `col_01_arena_01_cin_scene`（9.5 MB）：**大竞技场完整 3D 场景**，44 个网格含观众席排、横幅、岩壁，43 张专属贴图。
- `pas_desert_01_scene`（5.7 MB，沙漠/贾库）、`kashyyk_beach_scene`（2.6 MB，卡希克海滩，含 Yavin 植被与 gunship 低模）、`emp_throne_room_dest_scene`（2.4 MB，**毁坏后的皇帝王座厅**，带 Lightmap 与死星天空盒）、`fo_kylo_quarters_kijimi_scene`（3.0 MB，凯洛·伦寝室）、`coruscant_jeditemple`（0.7 MB，绝地圣殿）。
- `territorymap_*`（12 个，约 40 MB）：领土战 3D 星图/地图，`territorymap_tb3_mixed` 16 MB、6100+ 对象、405 个网格（行星、扇区面板、边界线），Geonosis/Hoth 明暗两版。
- `tb3_*`（6 个）：Territory Battle 死星/Kafrene/基地等 3D 面板。
- `node_*`（22 个）：战役星图上的 3D 行星节点（deathstar、kashyyyk 等 planet_mesh）。
- `holoplanet_*`（13 个）：全息行星模型。

### 7. 3D 肖像与角色道具
- `crewportrait_*`（63 个）：**带骨架的 3D 头雕/半身像**（SkinnedMeshRenderer + JNT 骨骼 + 动画），飞行员等船员肖像。
- `commanderportrait_*`（12 个）：指挥官 3D 半身像，同上带骨架与动画。
- `cantinapatron_*`（6 个）：酒馆 NPC 3D 模型（12 网格 + SkinnedMeshRenderer）。
- 小型道具/载具：`bantha_bomb_pile_pre`、`ballista_bolt01_pre`、`blaster_bolt01_pre`、`greedo_hand`、`droid_damage_01..03`（战斗伤害残骸）、`speederbike_*`、`shar`、`patron_playable_director_pre`。

### 8. UI 图标/单张贴图类（约 6000 个 bundle，约 340 MB）
数量最大的形态：**一个 bundle 只装一张 Texture2D**，无网格。包括：
- `charui`（769）：角色头像立绘 `tex.charui_*`
- `pack`/`packs`（2047）：商城卡包/捆绑包宣传图
- `events`（751）+ `objective`（95）：活动横幅与目标图
- `newsletter`+`neswletter`（552）：新闻角色宣传图（后者为官方拼写错误变体）
- `abilityui`（364）：技能图标；`vanity`（350）：玩家头像框；`relic`（315）：遗物物品图
- `equip`（152）：装备图标；`icon`（135）：通用 UI 图标；`jg`（73）：Journey Guide 头像（`glhondo` 一张就 0.9 MB）
- `guild`/`territory`/`league`/`datacronui`/`contextualmessageui`/`consumable`/`chest`/`purchase` 等：公会徽章、领土战背景、联盟段位框、酒馆 NPC 对话立绘等
这类对插画/立绘收集有价值（都是官方原画 PNG），对 3D 恢复无直接价值，但可批量一键导出。

### 9. UI 界面 prefab 类（约 1800 个 bundle）
`ui`、`conquest`（44 个但对象最多，抽样合计 2 万+ MonoBehaviour）、`inboxui`、`guildui`、`territoryui`、`coliseum`、`datacron(s)`、`unitguide`、`charactermods`、`inventoryui`、数百个 `*ui` 单体界面等。内容是 UGUI 面板：GameObject+RectTransform+BoxCollider+字体（futura_bold、standard_body_font）+图集引用，部分带 PlayableDirector 动效（modenhancement 抽样 210 个）。对 UI 还原/参考有价值，对 3D 资产恢复价值低。

### 10. shared_* 共享容器（4 个，约 28 MB）——单位体积价值最高
- `shared_animationcontainer`（4 MB）：**178 个人形共享 AnimationClip**（hmn_pst_var01_run、hmn_rfl_var01_atk、双持 idle、cantina 互动动画等），挂在通用 humanoid 骨架上，可跨角色复用。
- `shared_resourcecontainer`（7.1 MB）：77 个共享网格 + **71 个 SkinnedMeshRenderer + 15 个 Avatar（通用骨架）** + 156 张共享贴图（matcap_skin/leather、VFX 噪声、scan_line 等）。共享身体部件（风暴兵肩甲、贾瓦、起义军飞行员等）。
- `shared_shadercontainer`（0.15 MB）：39 个 shader（particles-additive、vfx-holo-fade、unlit 系列等），是解读所有材质渲染方式的钥匙。
- `shared_uicontainer`（16.8 MB，全场最大 bundle）：UI 图集（unit_atlas、standard_atlas、battleui atlas 等 8 张大图 + 材质 + UI 动画 asset）。

### 11. 音频 / 视频 / 本地化文本：**缓存中不存在**
全部 981 个抽样 bundle 中 **AudioClip=0、VideoClip=0、TextAsset=0**，bundle 名里也没有 audio/video/lang/movie/sound 类目。结论：SWGoH 的音频（应为 FMOD bank）、过场视频与本地化文本表不走 Unity AssetBundle 缓存（在安装包内、其他目录或服务端下发），无法从本缓存恢复。

### 12. 其他杂项
- `5-lighting-do-not-touch`：1 个 bundle，内含 **210 个 Light** 的全局光照 rig（无网格），是所有场景的标准布光方案，对场景重建打光有参考价值。
- `mat`：仅 1 张 `sliceabundle_atlas.mat` 图集；`bak`/`scv_001..007`/`empty`：几百字节~8KB 的占位/系统小 bundle。
- `background_episode_04..09`：每张约 1 MB 的章节大背景图（单 Texture2D）。
- `battle_character_*`（96 个）：战斗布局容器（`prefab.battle_character_event_glrey_layout`），是站位/队列配置 prefab，不是角色模型本体，模型仍在 char_ 里。
- `battle_*`（60 个）：战斗摄像机 rig、队伍标记、raid 模板。
- `packopening`（26 个）：抽卡数据卡 3D 模型 + 演出粒子。
- `challengetoast` 系列：奖杯/挑战 3D 展示（大量 PlayableDirector）。

## 四、值得后续提取的 Top 10（非角色）

| # | 目标 | 为什么 |
|---|---|---|
| 1 | `env_*` 全部 269 个 | 完整场景网格+贴图+光照，量最大、还原度最高 |
| 2 | `shared_animationcontainer` | 178 个人形动画，直接适配通用骨架 |
| 3 | `shared_resourcecontainer` | 共享网格 + 15 个 Avatar 骨架定义 + matcap/VFX 贴图 |
| 4 | `cin_*` 全部 283 个 | 过场演出：动画、特效、内嵌网格与镜头 |
| 5 | `ship_*` 91 个 + gr75/malevolence/tie/mg100 | 全部舰船模型（含 mask 贴图） |
| 6 | 5 个 `*_scene` + `coruscant_jeditemple` + `col_01_arena_01_*` | 独立完整场景（竞技场/王座厅/绝地圣殿等） |
| 7 | `crewportrait_*` + `commanderportrait_*` + `cantinapatron_*` | 81 个带骨架的 3D 头雕/半身像，char_ 之外唯一的角色级模型 |
| 8 | `territorymap_*` + `tb3_*` | 3D 星图与战场地图（最大单 bundle 16 MB） |
| 9 | `vfx_*` + `persistentvfx` + `shared_shadercontainer` | 通用特效库 + shader 集，是还原材质的关键 |
| 10 | `node_*` + `holoplanet_*` + `shipunlock_celebrationview` + `5-lighting-do-not-touch` | 3D 行星、全息台场景、标准灯光 rig |

## 五、限制与备注
- 缓存只有约 2 GB，明显是「按需下载」的子集：本地化/音频/视频/部分旧活动资源不在其中。
- 图标类 bundle 单独打开成本极低（每包 1 张贴图），可整类批量导出；env/cin/ship 建议逐包处理。
- `char_` 共 747 个按约定未打开；数量、体积来自文件系统统计。
- 同名 bundle 的旧 hash 目录仍留在磁盘上，如需考古旧版本可改取非最新 hash。
