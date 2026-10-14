# 模型分类报告（models.json → 英雄 / 非英雄）

> 生成: `tools/classify_models.py` ｜ 数据（全本地）: `models.json` + `catalog/units.json` + `catalog/units_pve.json` + `catalog/units_gas.json` + `catalog/character_names.json`

## 一、总览

| 类别 | 数量 | 说明 |
|---|---|---|
| hero | 391 | 可玩单位：units.json 精确 prefab / defId 变体 / units_gas 独有单位 |
| pve_variant | 282 | 仅出现在 units_pve.json 的 PVE/敌方单位，或英雄的 PVE/剧情变体 |
| environment | 0 | models.json 无 env_* 前缀（见下） |
| cinematic | 0 | models.json 无 cin_* 前缀（见下） |
| ship_assets | 0 | models.json 无 ship_* 前缀（见下） |
| other | 0 | 无（非单位资产留在 unclassified 并给 guess） |
| unclassified | 74 | 三张单位表都找不到（附 guess 直觉标注） |
| **合计** | **747** |  |

**关于 env_*/cin_*/ship_* 前缀**：`models.json` 的 747 个 id **全部**以 `char_` 开头（740 个以 `_pre` 结尾），不存在 env_*/cin_*/ship_* 前缀，故这三个类别为 0。环境/飞船/过场内容在 `catalog/other_content.json`（env 269、ship 92、cinematic 284 个 bundle）里，不在本模型清单中。少量非角色资产（`char_crate_a3`、`char_damage`、`char_starmap_pre`、`char_shieldgenerator_pre` 等）留在 unclassified，用 `guess` 字段标注直觉判断。

## 二、hero 判定方式分布

| hero_match | 数量 | 含义 |
|---|---|---|
| prefab_exact | 240 | 模型 id 与 units.json 某可玩单位 unitPrefab 完全一致（去 unit. 前缀） |
| defId_variant | 57 | defId 对应 units.json 可玩单位但模型 id ≠ 其 prefab —— 该英雄的变体/备用/皮肤模型（speederbike、holo、remastered 等），带 hero_link |
| defId_gas | 72 | defId 仅在 units_gas.json 完整单位表（obtainable=True；gas 无 unitPrefab，是 units.json 超集，多 110 个较新单位） |
| fuzzy_prefab | 2 | 去尾部变体 token 后命中英雄 prefab 核心（低置信） |
| fuzzy_baseid | 11 | 去变体后缀后命中英雄 baseId（低置信） |
| alias | 9 | 社区绰号/缩写别名命中（低置信） |

## 三、pve_variant 判定方式分布

| pve_match | 数量 | 含义 |
|---|---|---|
| prefab_pve | 238 | 模型 id 与 units_pve.json 单位 unitPrefab 一致（含 char_xxx_pre.yyy 子 prefab 归一） |
| defId+hero_link | 10 | defId 是可玩英雄但模型 id 带 _pve/_raid/_nokill/_enemy/_boss/_noshield —— 英雄的 PVE 用变体（附 hero_link） |
| defId_pve | 32 | defId 仅存在于 units_pve.json |
| fuzzy+hero_link | 2 | 模糊命中英雄但模型 id 带 PVE 标记（低置信，附 hero_link） |

## 四、hero 阵营分布

| alignment | 数量 |
|---|---|
| light | 219 |
| dark | 171 |
| neutral | 1 |

Top 派系（affiliation_*，一个英雄可属多个）：

- rebels: 65
- empire: 61
- republic: 51
- galactic_republic_jedi: 31
- rebelfighter: 29
- imperialtrooper: 27
- separatist: 20
- resistance: 19
- sithlord: 16
- nightsisters: 15
- mandalorian: 14
- imperialremnant: 14
- huttcartel: 12
- firstorder: 12
- oldrepublic: 11

定位分布（role_*）：leader 180、attacker 171、support 128、tank 73、healer 19

## 五、判定规则（优先级从高到低）

1. **prefab_exact**：`model_id == strip(unitPrefab)`（`unit.char_vader_pre` → `char_vader_pre`）；PVE 子 prefab（`char_aat_pre.phase01`）先按 `.` 截断。
2. **prefab_pve**：同上命中 units_pve.json 且不在 units.json。
3. **defId 路径**：`character_names.json` 的官方 defId（源自 gamedata 镜像，`cn_map_confidence` 保留其 match 置信度）：命中 units.json → hero；若模型 id 带 `_pve/_raid/_nokill/_enemy/_boss/_noshield` → 该英雄的 pve_variant（附 hero_link）；仅命中 units_pve.json → pve_variant；仅在 units_gas.json → hero（见数据坑 #2）。
4. **模糊**：核心名从右侧剥变体 token（speederbike/holo/cin/gl/s3/remastered/jediknight/…，白名单校验），或"baseId 前缀 + 变体后缀"反向前缀匹配。所有模糊命中带 `note`（低置信，可复核）。
5. **别名兜底**：内嵌社区缩写表（JML/JMK/GAS/SEE/LV 等，运行时校验目标存在才启用）。参考了 LukeSONG2000/goh-skill `lib/names.py` 的检索逻辑；其绰号数据 `names.json` 被上游 .gitignore 无法拉取，故为策展内嵌版。
6. 未命中 → **unclassified**（`guess` 字段：prop/载具/生物/角色模型 直觉猜测）。

## 六、召唤（summon）结论 —— 先查证，不硬标

- `units.json`（可玩 301 单位×7 星）：**没有任何** `summoned_unit` 类别；`summonStatTable` 字段 2,107 行**全空**。"可玩单位"与"召唤物"在官方数据里是分开的，模型目录中不存在"可玩召唤物"。
- 任务猜测的 `hermityoda`/`BB8`：都是 units.json 的**普通可玩单位**（HERMITYODA、BB8，obtainable=True、any_obtainable、有 alignment 标签），**不是**召唤物的标记方式。
- 召唤物的真实标记在 `units_pve.json`：`categoryId` 含 **`summoned_unit`**（52 行：blaster turret、Geonosian Brute、Vulture Droid、VCX100/YT2400/YV666 舰船、TIE Scythe、Conquest 补给箱 OBJ_CRATE_01 等），以及 id 含 `*SUMMON*` 的标量行（`PVE_VULTUREDROID:SUMMON_VULTURE`、`...:MALEVOLENCE_SUMMON_VULTURE` 等）。
- `SOE_summoner` 标记的是**召唤者**：可玩侧仅 2 个（ARCTROOPER501ST、GEONOSIANBROODALPHA，各×7 星）。
- 因此不设 summon 类别；命中单位带上述标记时，JSON 输出 `is_summon`/`is_summoner` 字段。本表实测：21 个模型命中 is_summon（均为 PVE 侧召唤物变体：char_b1geo_pre、char_b1geocommander_pre、char_b2rocket_pre、char_b2rocket_pve_pre、char_blasterturret_pre、char_bx_generic_pre、char_bx_hacked_pre、char_crate_a3、char_geonosian_brute_pre、char_guildbountyhunter_01_pre、char_jedi_sentinel_01_pre、char_jedi_vanguard_04_pre、char_mando_turret_pre、char_pirate_01_pre、char_pirate_02_pre、char_pirate_03_pre、char_pirate_04_pre、char_pirate_05_pre、char_rebelsoldier_monmothma_pre、char_sniperdroid_pre、char_troopersnow_ops_tb_pre），2 个命中 is_summoner（char_geonosian_broodalpha_pre、char_trooperclone_arc_501st_pre）——后者正是可玩侧仅有的两个 SOE_summoner 单位，与官方数据完全吻合。

## 七、自检

- hero + pve_variant + environment + cinematic + ship_assets + other + unclassified = 747 ✅
- 抽查：`char_vader_pre` = dark / [empire, sithlord]（官方无 `affiliation_sith` 标签，西斯派系叫 `sithlord`，另有 `profession_sith`）✅ ｜ `char_luke_ep4_pre`（Luke Skywalker）= light / [rebels] ✅ ｜ Bad Batch（Hunter/Wrecker/Tech/Echo）= `affiliation_badbatch` ✅
- **hero 对应关系**：`prefab_exact` 的 240 个模型与 units.json 的 **240 个 CHARACTER 单位**（combatType=1，全部 any_obtainable）**一一对应**（prefab 集合双向无差集）✅。另有 61 个 SHIP 单位（`unit.ship_*`）不在 char_* 模型清单中；hero 其余数量来自 defId 变体与 units_gas 独有单位（见第二节）。

## 八、数据坑

1. **units.json 每单位 7 行**：2,107 行 = 301 单位 × 7 星级（id 形如 `VADER:SEVEN_STAR`），统计必须按 baseId 去重。
2. **units_gas.json 两层嵌套 `{version, data}` 且无 unitPrefab**，只能按 baseId/defId 关联；它是 units.json 的**真超集**（411 vs 301 个 baseId），多出的 110 个（4LOM、Zuckuss、Kix、Maz Kanata、Jaxxon、Baylan Skoll、Shin Hati、Captain Enoch、GL Hondo、GL Leia、GL Ahsoka 等，多为 2024-25 新内容）在 units_pve.json 中**零重叠**，判 hero 的依据是官方 `any_obtainable` + obtainable=True。风险：无法 100% 排除个别是未实装数据；注意 Qimir、CX-2 等 Ahsoka/残次品角色连 gas 表都没有，只能留在 unclassified。
3. **PVE 子 prefab 带点号**：`char_aat_pre.phase01`、`char_rancor_pre.rancor_pit`、`char_veh_atst_pre.atst_hoth` 等，不截断会漏掉 AAT/Rancor/AT-ST 等大型载具与 Raid Boss 模型。
4. **character_names.json 有 104 个模型无 defId**（match=unmatched_in_gamedata），且约 90 个 defId 来自 token 剥离推导（match=derived_*）；只依赖它会传播误差 —— 本工具把它降为"路径之一"并原样输出 `cn_map_confidence`。
5. **defId 双重命中**：很多 defId 同时在 units.json 和 units_pve.json（英雄也会在 PVE 出怪），需靠模型 id 的 `_pve/_raid/_enemy…` 标记分辨本体还是敌方变体。
6. units_pve.json 同一 baseId 有十几行难度变体（`PVE_XXX:MODE_ELITE`），索引前必须去重。
7. 模型 id 本身的坑：`char_crate_a3`、`char_damage`、`char_traya_saberenemy` 不带 `_pre` 后缀；`char_maul_hatefueled_pre_holo`、`char_thrawnadmiral_pre_holo` 的后缀在 `_pre` 之后。

## 九、样例清单

### hero · prefab_exact 抽样 20

```
char_000_pre                       TRIPLEZERO             dark    
char_50rt_pre                      50RT                   light   oldrepublic
char_aaylasecura_pre               AAYLASECURA            light   galactic_republic_jedi,republic
char_ackbaradmiral_pre             ADMIRALACKBAR          light   rebels
char_admiralraddus_pre             ADMIRALRADDUS          light   rebels,rogue_one
char_admiraltrench_pre             TRENCH                 dark    separatist
char_ahsoka_adult_pre              FULCRUMAHSOKA          light   rebels
char_ahsoka_commander_pre          COMMANDERAHSOKA        light   republic
char_ahsoka_pre                    AHSOKATANO             light   501st,galactic_republic_jedi,republic
char_anakin_gl_pre                 LORDVADER              dark    empire
char_anakinknight_pre              ANAKINKNIGHT           light   galactic_republic_jedi,republic
char_armorer_pre                   ARMORER                light   mandalorian
char_astromech_bt1_pre             BT1                    dark    
char_astromech_r2d2_pre            R2D2_LEGENDARY         light   rebels,republic,resistance
char_aurrasing_pre                 AURRA_SING             dark    
char_b1_battledroid_pre            B1BATTLEDROIDV2        dark    separatist,separatist_droid
char_b2_pre                        B2SUPERBATTLEDROID     dark    separatist,separatist_droid
char_barriss_light_pre             BARRISSOFFEE           light   galactic_republic_jedi,republic
char_bastilashan_dark_pre          BASTILASHANDARK        dark    doubleblade,sithempire
char_bastilashan_pre               BASTILASHAN            light   doubleblade,oldrepublic
```

### hero · defId_variant 抽样 15（英雄的变体/备用模型）

```
char_ackbaradmiral_speederbike_pre         -> ADMIRALACKBAR
char_anakin_gl_order66_pre                 -> LORDVADER
char_anakin_gl_remastered_pre              -> LORDVADER
char_b2_battledroid_pre                    -> B2SUPERBATTLEDROID
char_bokatan_s3_pre                        -> BOKATAN
char_c3p0_endorgod_pre                     -> C3POLEGENDARY
char_c3p0_speederbiked_pre                 -> C3POLEGENDARY
char_chewbacca_ot_orig_pre                 -> CHEWBACCALEGENDARY
char_chewbacca_ot_speederbike_pre          -> CHEWBACCALEGENDARY
char_colonel_starck_speederbike_pre        -> COLONELSTARCK
char_commanderluke_cinematics_pre          -> COMMANDERLUKESKYWALKER
char_commanderportrait_thrawnadmiral_pre   -> GRANDADMIRALTHRAWN
char_ewok_chief_speederbike_pre            -> EWOKELDER
char_ewok_chirpa_speederbike_pre           -> CHIEFCHIRPA
char_ewok_logray_speederbike_pre           -> LOGRAY
```

### hero · defId_gas 全列表（units_gas 独有单位）

```
char_4lom_pre                              -> 4LOM                     dark   
char_ahsokatano_gl_pre                     -> GLAHSOKATANO             light  
char_baylanskoll_pre                       -> BAYLANSKOLL              dark   
char_bishop_pre                            -> BISHOP                   light  
char_bossnass_pre                          -> BOSSNASS                 light  
char_brutus_pre                            -> BRUTUS                   dark   
char_captaindrogan_pre                     -> CAPTAINDROGAN            light  
char_captainenoch_pre                      -> CAPTAINENOCH             dark   
char_captainrex_pre                        -> CAPTAINREX               light  
char_captainsilvo_pre                      -> CAPTAINSILVO             dark   
char_captaintarpals_pre                    -> CAPTAINTARPALS           light  
char_carsonteva_pre                        -> CARSONTEVA               light  
char_cassianundercover_pre                 -> CASSIANUNDERCOVER        light  
char_cerejunda_pre                         -> CEREJUNDA                light  
char_cinta_pre                             -> CINTA                    light  
char_cobbvanth_pre                         -> COBBVANTH                light  
char_darthbane_pre                         -> DARTHBANE                dark   
char_darthjarjar_pre                       -> DARTHJARJAR              dark   
char_dedrameero_pre                        -> DEDRAMEERO               dark   
char_depabillaba_pre                       -> DEPABILLABA              light  
char_ezraexile_pre                         -> EZRAEXILE                light  
char_greatmothers_pre                      -> GREATMOTHERS             dark   
char_gunganphalanx_pre                     -> GUNGANPHALANX            light  
char_hondo_gl_pre                          -> GLHONDO                  dark   
char_hunters3_pre                          -> HUNTERS3                 light  
char_huyang_pre                            -> HUYANG                   light  
char_ig12_pre                              -> IG12                     light  
char_ig90_pre                              -> IG90                     dark   
char_inquisitorbarriss_pre                 -> INQUISITORBARRISS        dark   
char_jarjarbinks_pre                       -> JARJARBINKS              light  
char_jaxxon_pre                            -> JAXXON                   light  
char_kelleranbeq_pre                       -> KELLERANBEQ              light  
char_kix_pre                               -> KIX                      light  
char_kleya_pre                             -> KLEYA                    light  
char_kxsecuritydroid_pre                   -> KXSECURITYDROID          dark   
char_leia_gl_pre                           -> GLLEIA                   light  
char_leiajeditraining_pre                  -> LEIAJEDITRAINING         light  
char_lukestarkiller_pre                    -> LUKESTARKILLER           light  
char_luthenrael_pre                        -> LUTHENRAEL               light  
char_majorpartagaz_pre                     -> MAJORPARTAGAZ            dark   
char_marajadeskywalker_pre                 -> MARAJADESKYWALKER        light  
char_marrok_pre                            -> MARROK                   dark   
char_masterquigon_pre                      -> MASTERQUIGON             light  
char_mazkanata_pre                         -> MAZKANATA                light  
char_morganelsbeth_pre                     -> MORGANELSBETH            dark   
char_padawanobiwan_pre                     -> PADAWANOBIWAN            light  
char_padawansabine_pre                     -> PADAWANSABINE            light  
char_pazvizsla_pre                         -> PAZVIZSLA                light  
char_princesskneesaa_pre                   -> PRINCESSKNEESAA          light  
char_queenamidala_pre                      -> QUEENAMIDALA             light  
char_quiggold_pre                          -> QUIGGOLD                 dark   
char_raccoon_pre                           -> RACCOON                  light  
char_rangerzeb_pre                         -> RANGERZEB                light  
char_remnantsnowcommander_pre              -> REMNANTSNOWCOMMANDER     dark   
char_sateleshan_pre                        -> SATELESHAN               light  
char_sawgerrera_pre                        -> SAWGERRERA               light  
char_scorch_pre                            -> SCORCH                   dark   
char_shinhati_pre                          -> SHINHATI                 dark   
char_sm33_pre                              -> SM33                     dark   
char_stap_pre                              -> STAP                     dark   
char_stormtrooperconcept_pre               -> STORMTROOPERCONCEPT      dark   
char_tarfful_pre                           -> TARFFUL                  light  
char_taronmalicos_pre                      -> TARONMALICOS             dark   
char_theronin_pre                          -> THERONIN                 dark   
char_tuskenchieftain_pre                   -> TUSKENCHIEFTAIN          dark   
char_tuskenhuntress_pre                    -> TUSKENHUNTRESS           dark   
char_vaderduelsend_pre                     -> VADERDUELSEND            dark   
char_vane_pre                              -> VANE                     dark   
char_vel_pre                               -> VEL                      light  
char_yodachewbacca_pre                     -> YODACHEWBACCA            light  
char_yodadsv_pre                           -> YODADSV                  dark   
char_zuckuss_pre                           -> ZUCKUSS                  dark   
```

### pve_variant 抽样 20

```
char_aat_pre                                 PVE_AAT                      dark   
char_acklay_pre                              ACKLAY_PVE                   neutral
char_agenkolar_pre                           PVE_AGENKOLAR                light  
char_ajuntapall_pre                          PVE_MAUL                     dark   
char_anakin_fallen_pre                       LORDVADER_GLE                dark   
char_anakin_gl_as_general_pre                GENERALSKYWALKER_GLE_INHERIT_T3 dark   
char_anakinpadawan_pre                       ANAKINPADAWAN                light  
char_astromech_r2d2_pve_alt01_pre            PVE_R2D2_LEGENDARY           light  
char_atstdriver_pre                          PVE_ATSTDRIVER               dark   
char_b1_low_pre                              B1BATTLEDROID                dark   
char_b1_low_raid_noshield_pre                B1BATTLEDROID                dark   
char_b1_low_raid_pre                         B1BATTLEDROID                dark   
char_b1_pre                                  B1BATTLEDROID                dark   
char_b1_repeatingblaster_pre                 PVE_B1HEAVYGUNNER            dark   
char_b1commander_lstb_pre                    PVE_COMMANDBATTLEDROID       dark   
char_b1commander_pre                         PVE_OOMCOMMANDBATTLEDROID    dark   
char_b1commander_raid_noshield_pre           PVE_OOMCOMMANDBATTLEDROID    dark   
char_b1commander_raid_pre                    PVE_OOMCOMMANDBATTLEDROID    dark   
char_b1geo_pre                               PVE_B1RAID                   dark   
char_b1geocommander_pre                      PVE_B1COMMANDERRAID          dark   
```

### unclassified 全列表（附 guess）

```
char_astromech_r5d56_pre                       char_astromech_r5d56_pre             character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_atst_boss_cin_pre                         char_atst_boss_cin_pre               vehicle（AT-ST 步行机）
char_atst_boss_pre                             char_atst_boss_pre                   vehicle（AT-ST 步行机）
char_b1snow_pre                                char_b1snow_pre                      character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_b1snowcommander_pre                       char_b1snowcommander_pre             character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_b1snowsecurity_pre                        char_b1snowsecurity_pre              character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_bantha_pre                                char_bantha_pre                      creature（班萨兽）
char_bibfortuna_pre                            char_bibfortuna_pre                  character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_captaingrey_pre                           char_captaingrey_pre                 character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_captainithano_pre                         char_captainithano_pre               character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_cash_dlc13_pre                            char_cash_dlc13_pre                  prop（现金/测试道具）
char_chamsyndulla_pre                          char_chamsyndulla_pre                character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_cloneluke_pre                             char_cloneluke_pre                   character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_clonemedic_501st_pre                      char_clonemedic_501st_pre            character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_col_kraytdragon_cin_pre                   char_col_kraytdragon_cin_pre         creature（克雷特龙）
char_col_kraytdragon_pre                       char_col_kraytdragon_pre             creature（克雷特龙）
char_cx-2_pre                                  char_cx-2_pre                        character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_damage                                    char_damage                          prop/VFX
char_dryax_cin_pre                             char_dryax_cin_pre                   character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_dryax_pre                                 char_dryax_pre                       character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_dryax_variant_cin_pre                     char_dryax_variant_cin_pre           character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_dryax_variant_pre                         char_dryax_variant_pre               character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_embokeibu_pre                             char_embokeibu_pre                   character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_endorbag_pre                              char_endorbag_pre                    prop（袋子）
char_fallenbarriss_pre                         char_fallenbarriss_pre               character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_general_herasyndulla_pre                  char_general_herasyndulla_pre        character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_genericjedi_male_01_legacy_order66_raid_pre char_genericjedi_male_01_legacy_order66_raid_pre character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_genericjedi_male_02_legacy_order66_raid_pre char_genericjedi_male_02_legacy_order66_raid_pre character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_groguanz_pre                              char_groguanz_pre                    character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_gunganboomadier_pre                       char_gunganboomadier_pre             character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_haxionbroodbountydroid_a_pre              char_haxionbroodbountydroid_a_pre    character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_haxionbroodbountydroid_b_pre              char_haxionbroodbountydroid_b_pre    character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_haxionbroodbountydroid_c_pre              char_haxionbroodbountydroid_c_pre    character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_haxionbroodbountydroid_d_pre              char_haxionbroodbountydroid_d_pre    character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_imperialarmoredcommando_a_pre             char_imperialarmoredcommando_a_pre   character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_imperialarmoredcommando_b_pre             char_imperialarmoredcommando_b_pre   character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_imperialarmoredcommando_c_pre             char_imperialarmoredcommando_c_pre   character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_jedi_consular_01_pre                      char_jedi_consular_01_pre            character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_jeditempleguard_pre                       char_jeditempleguard_pre             character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_jotaz_cin_pre                             char_jotaz_cin_pre                   character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_jotaz_pre                                 char_jotaz_pre                       character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_k3p0_pre                                  char_k3p0_pre                        character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_kraytdragon_pre                           char_kraytdragon_pre                 creature（克雷特龙）
char_kxseriesdroid_pre                         char_kxseriesdroid_pre               character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_kylo_forcedyad_pre                        char_kylo_forcedyad_pre              character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_landojabbaguard_pre                       char_landojabbaguard_pre             character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_luke_jedi_pre                             char_luke_jedi_pre                   character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_mse6_pre                                  char_mse6_pre                        character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_nabooroyalofficer_pre                     char_nabooroyalofficer_pre           character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_nabooroyalsecurity_pre                    char_nabooroyalsecurity_pre          character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_obiwan_jedimasterechoes_pre               char_obiwan_jedimasterechoes_pre     character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_partisantrooper_pre                       char_partisantrooper_pre             character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_pykecapo_pre                              char_pykecapo_pre                    character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_qimir_pre                                 char_qimir_pre                       character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_queenamidala_decoy_pre                    char_queenamidala_decoy_pre          character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_rebeltrooper_specialforces_pre            char_rebeltrooper_specialforces_pre  character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_shieldgenerator_pre                       char_shieldgenerator_pre             prop（护盾发生器）
char_sith_ghost_pre                            char_sith_ghost_pre                  character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_stap_pve_raid_noshield_pre                char_stap_pve_raid_noshield_pre      vehicle（STAP 单兵飞行平台）
char_stap_pve_raid_pre                         char_stap_pve_raid_pre               vehicle（STAP 单兵飞行平台）
char_stap_pve_tf_pre                           char_stap_pve_tf_pre                 vehicle（STAP 单兵飞行平台）
char_starmap_pre                               char_starmap_pre                     prop（星图）
char_tauntaun_pre                              char_tauntaun_pre                    creature（汤汤坐骑）
char_tk_stormtrooper_pre                       char_tk_stormtrooper_pre             character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_traya_saberenemy_pre                      char_traya_saberenemy_pre            character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_tuskenscout_pre                           char_tuskenscout_pre                 character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_veh_atst_jfo_crumpledhead_pre             char_veh_atst_jfo_crumpledhead_pre   vehicle（AT-ST 步行机）
char_veh_atst_jfo_pre                          char_veh_atst_jfo_pre                vehicle（AT-ST 步行机）
char_zeffotombguardian_a_boss_cin_pre          char_zeffotombguardian_a_boss_cin_pre character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_zeffotombguardian_a_boss_pre              char_zeffotombguardian_a_boss_pre    character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_zeffotombguardian_a_pre                   char_zeffotombguardian_a_pre         character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_zeffotombguardian_b_boss_cin_pre          char_zeffotombguardian_b_boss_cin_pre character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_zeffotombguardian_b_boss_pre              char_zeffotombguardian_b_boss_pre    character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
char_zeffotombguardian_b_pre                   char_zeffotombguardian_b_pre         character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）
```
