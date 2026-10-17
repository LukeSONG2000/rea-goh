# 修复集成进度（PROGRESS TRACKER）

本文件是三个并行修复 Agent（A/B/C）交付物与集成的**唯一事实源**。
Tracker 每 ~5 分钟轮询交付标记，验证通过后集成（重导出→同步服务器→commit+push）。
缺陷定义见 `catalog/BUGS.md`（P1-P8）。

## 状态总表

| Agent | 任务 | 交付标记 | 状态 | 证据 | 集成 |
|---|---|---|---|---|---|
| A | 挂件绑定到骨骼 (P3/P6) + rig 倾斜 | `tools/verify_attachments.py` 存在 且 `extracted/char_ackbaradmiral_pre/` 重导出（内容变化） | ✅ 交付 | verify_attachments.py 23:40 落地；A 自跑全量重导出（23:44-23:51:52，719 anim.glb）；**7 char 广集验证 PASS**（ackbar/grievous/grievousraid/vaderduelsend/jynerso/obiwanep4/anakin_gl：所有 prop d=0.000-0.44 在 tol 0.5 内；upright probe 脚 y≈0、头在骨盆上方 = 倾斜修复）| ✅ **已集成**: commit `0d724a04` + 1919 文件(762MB) tar 同步 /srv/goh（md5 验证 b8c9dcfa 一致）+ push 首试成功 |
| B | 变体 m_IsActive + ParticleSystem 排除 + 发光 alphaMode (P5/P8) | `tools/verify_variants_vfx.py` 存在 且 `char_vaderduelsend_pre`/`char_jynerso_pre` 重导出 | ⚠️ **NEEDS-REVIEW** | B 自 commit `8b2fbc40`（20/20 verified @ B 自导出件）；**A/B fleet 扫盘后 2 项持续回归**（重试 #1 00:17 / #2 00:30 / #3 00:38 三次复跑均 FAIL，输入 GLB 未再变化）: ①jynerso P5 缺 `swd_jynerso_mesh` 隐藏标记 ②vaderduelsend P7 披风 t0(Idle@1.0) 147 顶点超 0.35 体包络(max 0.76)。fleet audit (V1/V2) 已由 agent `57baba25` 修复并经 tracker 独立验证 0 offender。**修复归属 B**: 导出器 P5 inactive 集合逻辑 + 披风 t0 采样 | 🔁 NEEDS-REVIEW（tracker 持续监视 B 更新）|
| C | 站位 staging + 环境地面 (P1/P2) | `catalog/battle_staging.json` + `tools/export_env.py` 地面支持 + 5 个 env GLB 重导出 + `game/server.py` + `viewer/battle.html` 站位补丁 | ✅ 交付 | 全部标记落盘；`verify_env_ground.py` **5/5 全绿**（每 env 4 项检查：ground_under_props / ground_at_y0 / ground_covers_staging / ground meshes 存在；env_ata_chopperbase 不在本地缓存，脚本声明为预期缺失） | ✅ 已集成：commit `1e91acc4`（文件域提交，避开 A/B 在产文件）+ tar 同步 /srv/goh（SYNC_OK）+ push 首试成功 |

## 集成流程（每个 Agent 验证通过后）

1. `PYTHONIOENCODING=utf-8 py tools/verify_<agent>.py` → 必须全绿
2. Agent A/B: `PYTHONIOENCODING=utf-8 py tools/export_animation_v2.py --all --force > export_fix.log 2>&1`（~20 分钟）
   Agent C: 环境包无需全量（5 个 GLB 已重导出）
3. 同步到公共服务器: 构建文件清单 → `tar -cf - -C . -T sl2.txt | ssh centos-laptop "cd /srv/goh && tar -xf -"`
   （A/B 用 extracted/*/*.anim.glb；C 加 extracted_env）
4. commit + push（8x30s 重试环）
5. 更新本表

## 三方全部集成后

- e2e: 本地 8137 起服务 + Playwright 访问 https://39.105.74.77:12021/viewer/battle.html（basic auth goh/swgoh2026，ignore_https_errors），断言站位生效、截图差异
- PHASE 2: 技能链接入 game/engine.py（读 il2cpp/BATTLE_SIM_INVENTORY.md DECODE STATUS：ApplyToTargets 链已解码于 `il2cpp/offline/battle.py`；用 catalog/gamedata/units.json 每单位 skillReference 选技能）→ 种子确定性验证 → commit+push → sync game/ → `sudo systemctl restart goh-demo` → 验证 /api/battle 事件带技能名
- 之后: 主城 roster UI（extracted_ui 头像）

## 路线图状态

| 项 | 状态 |
|---|---|
| 三 Agent 集成 | A ✅ (0d724a04+57baba25 delta 同步) / C ✅ (1e91acc4) / B ⚠️ NEEDS-REVIEW（2 项 GLB 内容回归，见总表） |
| E2E 站位断言 | ✅ 00:45（两场地 dist=0 + 截图差 md5） |
| PHASE 2 技能链 | ✅（abilities.py + engine v1，确定性 PASS，公共 /api/battle 带技能名） |
| roster UI 官方头像 | ✅ 01:1x（game/portraits.json 237/308 经 units.json thumbnailName 解析；/api/roster 下发 web-root 路径；battle.html 卡片优先头像、无头像回退 T-pose；本地+公共均验证 200；坑: 索引 portraits 路径无 tex. 前缀且为旧文件名 → 以 thumbnailName 为准） |
| B 回归修复 | ⏳ 等 B（tracker 每 ~5 分钟巡检 GLB mtime + verifier） |

## 终验矩阵（01:12 tracker 复跑）

| 门 | 结果 |
|---|---|
| tools/verify_attachments.py（A, 7 char 广集） | ✅ exit 0, all prop attachments within tolerance（upright probe: 脚 y≈0/头上位 = 倾斜修复保持） |
| tools/verify_variants_vfx.py（B, 4 char × 6 项） | ⚠️ 18 PASS / 2 FAIL（jynerso P5 缺 1 隐藏标记；vaderduelsend P7 披风 t0 max 0.76）— NEEDS-REVIEW |
| tools/verify_env_ground.py（C, 地面+站位覆盖） | ✅ 5/6（env_ata_chopperbase 不在缓存=预期缺失） |
| tools/audit_fidelity.py（fleet 1466 GLB） | ✅ exit 0, V1=0, V2=0 |
| 引擎确定性（本地 4 组对阵） | ✅ 同种子逐字段一致 / 异种子不同 |
| 公共确定性（seed=123 两次请求） | ✅ 事件 md5 一致 2b5e8e78 |
| 公共面 | ✅ battle.html 200（staging+头像代码在位）/ /api/battle 技能名 / portrait 200 / env+anim GLB 200 / goh-battle active |

## 事件日志

- 10-15 23:20 tracker 启动；快照：A 未交付、B 未交付、C 进行中（资产半数已落盘，服务端/前端补丁未到）
- 10-15 23:2x 新增未跟踪工具属 C 在产: dump_battle_staging.py / verify_env_ground.py / probe_anim_slab.py / probe_cape_containment.py(P7) / probe_materials.py / investigate_p578.py
- 10-15 23:33 **C 验证通过**（verify_env_ground.py 5/5 env 全绿，4 项检查/env）→ 开始集成 C（严格按文件路径 commit，避开 A/B 在产文件：B 的 char VFX 重导出扫盘正在进行中）
- 10-15 23:29 B 基线：verify_variants_vfx.py 自身崩溃（`re.search(compiled, flags=re.I)` ValueError）→ 属 B 在产代码，等 B 修后重跑；不改他人代码
- 10-15 23:36 **C 集成完成**: commit `1e91acc4` → /srv/goh 同步 OK → push `7e4951a6..1e91acc4 main`（首试成功）。e2e 站位断言延后到三方齐后统一做（届时重同步 battle.html 以防 C 后续微调）
- 10-15 23:43 **B 验证全绿**（ALL PASS）。但 B 与 A **共享导出器文件**（swgoh_extract.py / export_animation_v2.py，二者 charter 都改它们），A 仍在改+重导出中 → 决策：**等 A 落地后跑一次合并的 `export_animation_v2 --all --force` 全量重导出，一次 commit A+B、一次同步**，避免在半成品导出器上跑 20 分钟全量、也避免 commit 到 A 的中间态。B 当前已验证状态（5 GLB + 453 vfx.json）先留在工作树。
- 10-15 23:5x 发现 A/B 各自也在直接 commit：B `1038c9f0`（char vfx PSR 材质跨包解析）+ `8b2fbc40`（P5/P7/P8 主体修复, 20/20）；C `cd0995a0`（staging+ground）。tracker 的 push 已把它们带上远端。
- 10-15 23:44-23:51 **A 自跑全量重导出**（719 anim.glb，字母序 a→z 完成，代码自 23:40 稳定）
- 10-15 23:57 A 广集验证 **PASS**；B 复验出现 2 项回归（A 扫盘覆盖了 B 手导的 5 个 GLB）+ fleet audit exit 1（V1=9, V2=18 残留）→ 决策：**A 先集成**（其门全绿），B 回归 + fleet 残留记为 OPEN，进 10min×3 重试环等 B 修正；不代替 A/B 改导出器。
- 10-16 00:03 **A 集成完成**: commit `0d724a04`（719 GLB + 453 vfx.json + gates）→ /srv/goh 同步 md5 一致 → push 首试成功。公共 API 冒烟: `/api/staging` 正常返回 pvp 站位标记；`/api/battle` 正常（66 events, 全 basic —— 待 PHASE 2 技能链）
- 10-16 00:0x tracker 进入 PHASE 2（game/engine.py 技能链），B 重试环穿插进行
- 10-16 00:1x **PHASE 2 核心落地**: game/abilities.py（skill→abilityReference(字符串)→ability.tier[-1].effectReference→effect.json 全链解析 + loc_en 名字解析 + enums 枚举解码）+ game/engine.py v1（默认决策树 lite：治疗→伤害特技→普攻；DAMAGE 走完整解码管线按 damageType 分支；RECOVER=(mult+flat)×ABILITY_POWER；chance 用 BattleRandomnextInt(100)；未执行效果类型记入 skipped）。**顺带修了 battle.py 潜在 bug**: DAMAGE_MOD_ERA_LEVEL_DELTA_MAX 未定义（ability-damage 分支首触即崩，补 +0x2334 ctor imm=1.0）。验证: 4 组对阵**确定性全 PASS**（同种子事件逐字段一致、异种子不同）、ability 名字（"Sabacc Shuffle"/"Saber Storm"/"Furious Bowcaster"...）、AOE 多目标、治疗事件、事件含 skipped 效果清单
- 10-16 00:17 **B 重试 #1 仍 FAIL**（同 2 项: jynerso 缺 swd_jynerso_mesh 隐藏标记、vaderduelsend P7 披风 t0 max 0.76）——23:58/00:00 的重导出未修好；继续重试环（#2 ~00:25, #3 ~00:35, 之后 NEEDS-REVIEW）
- 10-16 00:2x **PHASE 2 部署完成**: game/ + il2cpp/offline/battle.py + gamedata(skill/ability/effect.json)+loc_en 同步 /srv/goh → `systemctl restart goh-battle` → active；公共 /api/battle (seed=42) 事件带真实技能名（"Sabacc Shuffle"/"Saber Storm"/"It's a Trap!"），units 携带 abilities 元数据；本地 8138 重启以加载新 server/engine
- 10-16 00:45 **E2E 全绿（本地 http://127.0.0.1:8138，= 公共页同码同数据；公共 URL 自签证书 IAB 无法 ignore_https_errors，API 层已用 curl 公共验证）**: ①卡希克海滩开战 → 5 fighter 全部 dist=0 站在原版 UnitMarker（1/2/3 号），沙地地面渲染 ②切霍雪平原再战 → 同样 maxDist=0，雪地地面 ③两张截图 md5 不同（b1bcf6be vs 89bf15e9）→ 站位/地面/朝向全链生效。P1/P2 用户可见症状闭环
- 10-16 00:2x agent 侧追加: `57baba25` fleet fidelity（ offending 字符全量重导，audit → 0 offender）+ `af07115c` Unity 工程同步 256 animator。tracker 独立复跑 audit_fidelity.py: **exit 0, V1=0, V2=0 属实**
- 10-16 00:30 29 个 fleet-fix GLB 增量同步 /srv/goh（md5 88366e9e 一致）；fleet 修复后 A 门复跑 **PASS**（7 char 全 tolerance）
- 10-16 00:30 **B 重试 #2 仍 FAIL**（同 2 项，GLB 内容自 23:58/00:00 未再变）→ #3 ~00:40，失败即 NEEDS-REVIEW（修复属 B 域: P5 导出器 inactive 集漏 swd_jynerso_mesh + P7 披风 t0 采样）
- 10-16 00:38 **B 重试 #3 FAIL → 正式 NEEDS-REVIEW**（三次复跑同结果，输入未变）
- 10-16 01:0x **roster UI 官方头像落地并部署**: portraits.json（thumbnailName 策略 237/308）+ /api/roster portrait 字段（web-root 路径）+ battle.html 卡片优先头像。调试记录: 8138 曾有**双进程 SO_REUSEADDR 影子监听**导致响应 alternation（全杀后单进程恢复）；相对路径 portrait 从 /viewer/ 页解析成 /viewer/extracted_ui/... 404 被 onerror 移除 → 改 web-root。验证: 本地 474 头像卡片渲染（截图），公共 /api/roster 237、portrait URL 200、battle API 正常
- 10-16 01:1x jynerso/vaderduelsend 显式补同步（首次全量同步时的竞态文件），md5 c8add6fe / c2d40e70 一致
- 10-16 01:02 公共 /api/battle 确定性: 两次独立请求（seed=123）事件流 md5 完全一致（2b5e8e78）——公共部署端到端确定性成立。公共静态面: battle.html 200（含 staging/头像代码 9 处命中）、env GLB 200、anim GLB 200
- agent 侧: 3f6da4f6 docs P1-P6 原版参考截图（验收基准）落库
- 10-16 01:29 tracker 巡检: B 仍无动作（jynerso/vaderduelsend GLB、导出器、verifier mtime 均未变）；unity/ 工程出现新在产写入（Scripts/Runtime/UI、Scenes、StreamingAssets、UiSprites —— Unity 侧 UI 工作流，不与 tracker/三 Agent 文件域冲突），tracker 不介入

## 2026-10-16 P5 milestone
- Battle HUD shipped (original atlas sprites): timer, auto/2x/lock, key chips, buff rows, per-actor ability bar, commander portrait. Gates: 48-event winner, 0 errors. Commit pushed.
- GameConstants runtime scanner ready + 12h watcher armed (auto-captures when game runs+logs in).
- P6 recon: no separate fleet simulator (CombatType byte); crew tables pre-extracted to catalog/fleet_crew_tables.json for the data pass.
- P2 cantina home shell: agent in flight.

## 2026-10-16 LIVE-GAME EXTRACTION (user ran the game)
- Game login triggered runtime downloads: **4,677 NEW bundles** into the standard cache (junction-visible). Includes env_home_screen_cantina_01_pre (P2 backdrop, already extracted 3.4MB), cantinapatron NPCs, homebase_camera_rig, and hundreds of **ui_*_view/ui_*_tile view prefab bundles**.
- **Original UI layout values ARE locally serialized**: homebaseui_view has 526/875 transforms with non-zero authored local positions + NGUI widget components (UIPanel/UIWidget/UISprite/UILabel) — REFERENCE-MEASURED approximations can now be replaced with ORIGINAL-EXTRACTED values.
- Font verdict final: Font objects are EmptyFont stubs everywhere (dynamic rendering) — original TTF does not exist; closest free font substitution is permanent, documented PENDING-ORIGINAL-unobtainable.
- NGUI layout extraction agent dispatched -> catalog/ngui_layouts.json (screen-definition bundles for P2/P3/P4/P5).
- Memory probes: bootstrap URL + cgswprod strings + 22 UnityFS headers found in live memory (walk valid); in-memory bundle bodies fragmented — disk cache is the reliable acquisition path (now proven).

## 2026-10-16 FINAL-MILE (Unity restoration delivery agent)
- STEP 1 P2 polish DONE: HomeScene camera widened (16.5 back/6.8 up/FOV 55, lookAt bar+3/0.9) — bar+stools+shelves framing like shots/reference/P2.png; hotspot labels re-anchored on a WORLD-SPACE canvas (billboarded to camera yaw, canvas scale 0.0135, depth-normalized row size) — the old SSC path re-scaled child offsets between build/capture pixel rects and crushed labels to screen center; anchors solved numerically vs true camera projection (bar-frame along/side/up in UiAssetImporter.HotspotFrame): 船运E/商店R on the bar counter, 竞技场S/战役G on the corner tables upper, 角斗场D/达斯·加·加F lower, F subtitle hangs below the row. Evidence: unity/ui_home.png (capture13-15 logs). Also fixed pre-existing CS0100 duplicate `anchor` param in HomeUI.NewLabel (project did not compile before this pass). Polish leftover: env GLB stages only 1 bar patron (char_barpatron_07_pre) — reference crowd not reproducible from cached assets.
- STEP 2 exe build DONE: Assets/Scripts/Editor/BuildRestored.cs (BuildWindowsPlayer + SmokeRunPlayer). Boot order HomeScene->RosterScene->DetailScene->BattleDemo in EditorBuildSettings; cross-nav verified (Home S/G -> roster/battle; roster -> detail; detail -> roster/battle). Build: F:\PPfavorite\Work\goh\unity\build\SWGoH-Restored.exe (666KB launcher) + SWGoH-Restored_Data 1.77GB (StreamingAssets+GLB atlases, bigger than the 100-300MB estimate because env+char GLBs ship uncompressed) + UnityPlayer.dll 31MB. Gates: batch build exit 0 result=Succeeded (summary.totalErrors can carry licensing handshake noise -> gate is on result only); player smoke: Start-Process alive at 15s, Player.log clean (D3D11 RTX3070Ti init, 0 errors). Fix shipped: runtime markers are now one-shot (BattleDriver/HomeUI/UiCaptureDriver consume+delete) -- a stale marker used to make the built player self-capture and Quit(0) 15s after boot. PlayMode regression: capture run -> 48 events, winnerSide=0, shot_240/480/800.png (HUD timer/auto-2x-lock/turn label/enemy portrait/QW ability bar all render).
- STEP 3 NGUI truth backfill DONE: replaced REFERENCE-MEASURED numbers with catalog/ngui_layouts_slim.json serialized values (NGUI 1280x720 design space, x1.5 onto the existing 1920x1080 canvas reference — mapping documented per-site). P3 RosterUI: grid = UIWrapGridContent _maxPerLine 7 / _itemWidth 196 / _itemHeight 192 / spacing 0 -> cellSize 294x288, 7 columns, card internals rescaled to the cell; accordion rail rows 354x114 at 118 pitch (76-design highlight x1.5, 68-design pitch). P4 DetailUI: skill list frames = character_ability_list_item 280x72 -> 420x108 rows, icon at -156, name/level fonts 36/30 (detail_ability_button sprite); tag chips 42 tall at the serialized 41.98-design pitch. P5 BattleHUD: BattleTimerLabel 512x34 fs32 @(0,306) -> 768x96 fs48 top-center (51px box truncated the glyphs -> 96px); commander = TargetContainer 376x112 @(436,291) -> 564x168 @(-24,-20); HealthBar-Long 768x32 belongs to the Boss variant — full width blanketed the timer, squad HUD keeps the compact in-container bar (documented); ability buttons = battle_ability_button 100x100 -> 150 canvas (ultimate 128x128 noted, ability_button_ult_bg sprite not harvested PENDING-ORIGINAL, gold frame stands in). Root-cause fix: HUDScreen CanvasScaler had NO referenceResolution (default 800x600) inflating all HUD numbers x2.4 — set 1920x1080/match 0.5, legacy controls x1.6-compensated (74px, outlined captions). Gates: roster/detail captures (ui_capture16-18), battle p5hud3-5, 48 events winnerSide=0 every run. Evidence: unity/ui_roster.png, ui_detail.png, shot_240/480/800.png vs shots/reference/P3/P4/P5.png.
- STEP 4 ability chain DONE: tools/build_abilities_data.py bakes StreamingAssets/abilities.json (2107 units / 5257 activatable abilities) through the game/abilities.py v1 chain (units.skillReference -> skill.abilityReference -> ability.json abilityType 7/10 + cooldown + tier[-1].effectReference -> effect.json type 1/2, /10000 decimals, loc_cn names); DAMAGE effects sum mult+flat per damageType, RECOVER effects sum heal with per-effect targeting (dmgSide/dmgSelect/healSide/healSelect). C# port: BattleSim.UnitAbility + BattleUnit.Abilities/CooldownLeft; RunTurn = engine.py _act lite tree (per-owner cooldown tick -> heal special when an ally < 55% -> damage special -> basic; chance roll nextInt(100); DAMAGE routed ATTACK_DAMAGE(2)->physical / ABILITY_POWER(3)+UNMITIGABLE(1)->suppression via the existing ApplyDamage pipeline; SELECT_ALL(1) hits every living foe else lowest-HP primary; heal = amount x ABILITY_POWER to lowest-HP-ally or self); negative heals (drain rows) not executed; BattleEvent carries AbilityName/AbilityId + "heal" events (green +N floats, commander bar follows). BattleDriver loads abilities.json, TurnStep labels/highlights the sim-used ability (log-driven truth; keys re-highlight only). Gates: 3v3 JYNERSO/CASSIANANDOR/K2SO vs VADER/GRANDMOFFTARKIN/DARTHTRAYA seed 4490 -> 32 events winnerSide=0 (specials first: 义军反击/终极火力/致残射击/传导冲击 then cooldown pacing), two same-seed runs identical (determinism PASS), ability names in turn label + log + HUD skill bar (shot_480: 第3回合 · 卡西安·安多尔 · 致残射击 with W highlighted). Evidence: unity/abilities1.log, abilities2.log, shot_*.png.
- STEP 5 fleet data layer DONE: tools/build_fleet_data.py bakes StreamingAssets/fleet.json (171KB) -- 427 combatType==2 ships (CN names, crew slots unitId/slot/skills, primary stat, ability-chain presence via abilities.json), 74 crew stattable_<UNIT>_<NSTAR> sets filtered to referenced crew units (fleet_crew_tables.json), and live runtime constants (ShipMitigationAttackerLevelOffset 60, UnitMaxMitigation 0.9, BattleShipDefaultTimeScale 1.5, GalacticPowerPerShip{Ability,ReinforcementAbility,Level}SimulatedCrewScalar 8.2/2.3/3.5, UnitSpeedFillMax 1000). NO fleet simulator added (P6 recon: fleet battle reuses the same turn core; the future sim = BattleSim + CombatType parameter + crew-scaled stats -- data is staged for it, FleetDemo scene intentionally not built). Commit-ready; no git commands run (integrator pushes).
- FINAL rebuild: build5.log exit 0, result=Succeeded; player StreamingAssets now ships abilities.json (1.5MB) + fleet.json (171KB) alongside roster/anim_states/ui_units/ui_labels; smoke: ALIVE at 15s (pid 46376), Player.log 0 errors/crash; no zombie Unity/player processes. All gates outside Unity untouched (verify_attachments / verify_variants_vfx / audit_fidelity / verify_env_ground not run or modified).

## 2026-10-16 HERO-PDF 单位手册（build_hero_pdfs.py）
- tools/build_hero_pdfs.py 落地：skill→ability→tier[-1].effectReference→effect 链解析（同 game/abilities.py 约定，扩展 leader/unique 被动 abilityType 8）；effect.json 103MB 一次性索引（63,035 effects / 45 种 type）。StreamingAssets/abilities.json 2107 行 = **301 唯一单位 × 稀有度变体**（技能链逐变体一致，手册按唯一单位合并）。
- 5 单位试点卷 HERO_PILOT（VADER/LORDVADER/JYNERSO/R2D2_LEGENDARY/GRANDMOFFTARKIN；用户指定 DARTHVADER、R2D2 在恢复数据实名分别为 VADER、R2D2_LEGENDARY）：25 页 / 572KB，pypdf 页数校验通过。复仇风暴抽查 = 3×1.25×攻击（物理，受目标护甲 Armor 减免）±5% 方差，按技能等级 2.5/3.75，7 效果链全列（×3.75 为三段伤害合计——NUMBERS.md §15"特殊类型伤害"描述与 gamedata damageType=ATTACK 不符，手册按数据真值标注物理）。
- 试点已按用户反馈修正：禁用语"吃护甲/吃压制"→"受目标护甲 Armor 减免 / 受目标抗性 Resist 减免"；全量文案同步。
- demo_lordvader.pdf（用户复查用，全量暂停）：GOH 自有 2D 资产版式——portrait tex.charui_lordvader.png（thumbnailName 链）、技能图标 tex.ability_anakinfallen_basic/special01/special02/ultimate（slug 由 roster char 文件夹 + character_names.json defId 前缀链推导，iconKey 字段为镜像占位不可用）、rarity_star(+_empty) 星排、bg_pillpanel 面板、detail_ability_button 技能行底、BIT_Portrait_Frame 头像环；调色板取自 GOH 裁片主色（teal #1296b4 / steel #304060 / 金 #b8933d）。4 页 / 590KB，Playwright 截图人工目检 2 轮。PENDING-ORIGINAL：效果持续时间（"2回合"等）只在 loc 描述文本，effect.json 无显式持续回合字段；技能 iconKey 全部为 tex.consumable_bacta_salvaged 占位。
- HERO-PDF demo v3（用户复查版）: demo_lordvader.pdf 10 页/660KB。重构：①图标全部本地化——ui_units.json 权威 ability→tex.ability_* 映射（黑暗先驱=special03 蒙面维达、首领=帝国徽章 abilityui_passive_empire、独有=abilityui_passive_anakinfallen，与 shots/reference 验收比对一致）；②官方描述=descKey 链 loc_cn(主)+loc_en(副)（maxTier→tier0→base DESC，_V4 空键回退）；③效果链可见表格逐行内联标注（是什么/怎么算/有什么用；内部行=内部机制一行；重复行=同第N行）；④公式零外部引用全内联（减伤=护甲÷(护甲+(攻方等级+60)×7.5) 含 85级vs1000护甲≈47.9% 算例、暴击/方差/护盾分流/抵抗判定全部展开）。根数据缺口修复：tools/extract_ui.py ability 提取 400→全量 2604 束 0 失败（extracted_ui/ability/ 全集 1745+ tex）。swgoh.gg 抓取已按 LOCAL-FIRST 指令撤回（catalog/swgohgg_units 已删，参考截图仅验收比对）。全量 run 继续暂停等用户批准。

## 2026-10-17 REA 原始内容与复现差距审查
- 【Codex-rea-gap-review】认领只读制品分析、现有 Unity 成果对照及指导意见；分工检查 UI/资产接入与战斗语义。原版安装 E: 只读，不访问游戏线上 API，不改实现。

- 【Codex-rea-gap-review】完成方向审查：详见 RESTORATION_REVIEW_20261017.json；新 REA 证据保存在 logs/rea_gap_review/rea-evidence.json。原版 GameAssembly SHA256 与历史 native 证据一致；本轮新检 actual build 的 CIL 与随附数据，不冒称重跑全量反编译。
- 确认 P0：构建程序集先整局模拟再回放，技能回调仅 HighlightSkill；VADER 普攻互斥的 3.241+3.241 两分支被导出为 6.482，工程及 build abilities.json 字节一致；leader/unique 与持续效果未完整执行，HUD 图标为固定示例。
- 确认 UI：7x294 网格超出1632可视宽，文本锚点使姓名跑到上一行；抽查三份头像源图与导入图一致，无需为该症状重导头像。原版字体已取得但未接入，详情 Animator 被禁用，表现链仍有缺口。
- 指导：保留资产及已核实公式，停止扩展演示覆盖；先修逐effect语义与真实手动命令，再交付固定3v3状态/目标/表现闭环，由独立验收复核 actual build。未改实现、未发送 Zcode 指令。
- 原版 computer use 限于关闭登录/公告/促销弹窗及回到主城观察，已停止遍历；无购买、无货币/体力消耗、无 Mod 操作。旧截图仅作验收参考，未用于生成数据。

## 2026-10-17 核心玩法纠偏
- 【Codex-battle-recovery】认领：原始战斗流程/效果语义交叉审查，并推进真实逐回合玩家指令闭环。写域 BattleSim、BattleDriver、BattleHUD 与专用回归；其他 UI/资产不动。Git 由主智能体单写。

- 【Codex-battle-recovery】第一闭环已实现：默认手动，单回合准备→技能/合法目标选择→校验提交→本步表现→下一行动者；自动同入口，等待/非法指令不推进状态与5条RNG。移除示例状态图标，事件用唯一单位ID，修复技能ID/abilityID图标映射。
- 原版来源复核：REA历史同SHA函数体 TriggerTurnStarted@18193B880 确有到 StartAbilitySelectPhase@18193BA20 的分支；其他手动选择入口仅metadata，不冒称完整FSM反编译。新实现明确为恢复桥接，非完整原函数移植。
- 验证：dotnet run --project tools/tests/BattleCommandRegression.csproj PASS（同种子同命令/不同选招目标/非法命令/冷却/唯一身份）；py il2cpp/offline/demo_turn_order.py PASS；PlayMode真实EventSystem射线及pointer事件、按钮四角边界、等待、选招/目标、双击与auto切换 PASS exit0；最终build exit0；exe存活15s、异常匹配0。日志和截图均在 logs/rea_gap_review/。
- actual build REA复检 ev_6e33e87a9f66819ab3c866bdf313c61bb4142a772d4f31ad455afee2a2873e56，Assembly-CSharp SHA256 54e5d69787c398499b6a308362c05a358929fef36e592abd0ebffa412257d01f。确认 SubmitTarget→TrySubmitCommand 与逐回合RunBattle调用链。详见 BATTLE_RECOVERY_20261017.json。
- 未完成保真项维持P0：扁平技能导出（包括VADER互斥倍率错误）、完整target/context/trigger/expiry、持续状态/护盾/助攻反击/驱散；场景/NGUI/动画/VFX仍不合原版。不以手动闭环替代完整玩法验收。
- ZCode CLI 0.16.9已定位，进程局部provider路径修复后调用仍Model creation failed；未取得模型辅助结果，未改全局配置。原版本轮零computer use、零货币/Mod操作、未访问线上API。

## 2026-10-17 原始效果链恢复
- 【Codex-effect-recovery】认领：维达普攻原始分支/类别条件、效果链无损导出与Unity逐效果执行、持续状态/伤害native证据交叉验证及回归构建。写域 tools/build_abilities_data.py、专用tests、BattleSim/Driver与新增效果模块、StreamingAssets技能数据；主智能体单写git，其他现有改动保留。
`n【Codex/root 2026-10-17】追加认领 BattleDemo.unity 单英雄效果管线验收场景（VADER 镜像），BattleHUD 状态/护盾展示与相关 PlayMode 门禁；其余英雄通过原始数据回归检查泛化，无运行时英雄名称分支。

【Codex/root 2026-10-17】完成单英雄原始效果管线首段：VADER 普攻四引用保持原序，互斥类别分支倍率6.482→3.241；通用DAMAGE/CURSE解释器恢复逐效果chance、技能内闪避缓存、独立状态实例、FULLTURN/DEATH/ENDENCOUNTER过期和基础/特殊技能门禁，修正角色减伤及护盾分账。完整导出6732 abilities/63035 effects，751菜单技能（301单位），未知图禁用而不折叠。SNOWTROOPER普攻无需英雄适配复用；20回合seed4490双跑hash ef47a580543af1371013d2a5241b107dcfcdb75928297c0d11b08ef30a676f4d。导出7测试、伤害/图/命令回归、demo_turn_order、Unity实际pointer PlayMode、build exit0和玩家15秒存活全部通过。REA成品SHA bc8d6880ccdd47378565a2940e10dca7b16eabcc506daa2f13eecf5c9a62cc0e。证据/范围：catalog/EFFECT_PIPELINE_PILOT_20261017.json。PENDING-ORIGINAL：完整Vader特殊技、被动/吸血/反击、DOT触发、驱散/context/额外回合、舰船/遗物差及原版AI和视觉；这是普攻管线样板，不是完整英雄或全量战斗完成。下一扩展单位是通用效果算子，不是英雄专用代码。

【Codex/root 2026-10-17 phase2】认领完整复现下一段：通用群体目标、持续触发/DOT及属性减速，优先打通VADER原力碾压完整数据链；写域Runtime/Battle、BattleDriver/HUD、专用tests与PlayMode门禁。visual_gap独占保存Ghidra项目只读分析；effect_export查原始图与跨英雄回归，Git由root单写。保持原E盘只读、零线上RPC、零货币/Mod操作。

【Codex/root phase2】追加认领 Resources/BattleStatus 原始状态图标导入：shared_uicontainer.bundle 的 battleui_view_rgba_atlas 精确mSprites裁片curse/DOT/speed，来源与rect/hash在logs/effect_phase2/status-icons.json；独立目录不覆盖其它UI同名图。GLB审计1466零V1/V2违规、variants及Vader/Snow附件验证通过，审计输出重定向logs以保持catalog只追加。

【Codex/root phase2 2026-10-17】完成通用ALL/SELF/SELECTED目标、持续触发APPLYEFFECT/DOT、SPEED原始叠加线及吸血；VADER原力碾压1.102群伤+三独立DOT+减速按原图执行，PHASMA同解释器回归，无英雄专用适配。所有触发先于过期、tag仅0→1触发、逐目标chance/RNG与SquadPositionPriority已由native函数交叉核验。HUD接真实状态/护盾与三份原atlas32x32裁片。算子校验覆盖10单位11主动技能，不代表完整kit。导出8测试、伤害/基础/群攻/命令、demo与seed双跑、真实UI点击PlayMode全部PASS；build exit0/Succeeded但保留现有4处缺prefab与许可诊断(totalErrors5)，玩家15秒存活无异常。REA新程序集SHA899d47b9c92ebec676c358c1d47fea972148f9054a67a541fe1392395de4915f，证据ev_3d1adaf4a47ccca0b785ff9d0212d0e615e9b47bee391d8d723646db53c61f13。详见EFFECT_PIPELINE_PHASE2_20261017.json（含native正文/纠正语义/追踪/验证）。PENDING-ORIGINAL仍包括完整Vader、被动/免疫/反击助攻、context/驱散/额外回合、AI、全游戏与视觉；下一步按通用算子推进，不逐英雄造规则。原版零CU/零线上RPC/零货币或Mod操作。

【Codex/root visual-phase3 2026-10-17】认领：用户小队/舰队截图仅验收，恢复本地原始相机/站位/HUD表现。root写BattleDriver及新增表现配置/验收；visual_gap只读相机/阵型/舰队资源，effect_export只读原UI prefab/atlas；Git/Unity单写root。运行时效果规则保持原图，不以截图造规则。

【Codex/root visual-phase3】追加认领新tools/export_battle_presentation.py和Resources/BattlePresentation/BattleChrome原始配置与精确UI裁片，effect_export独占实现；visual_gap唯一导出者写extracted/ship_*新舰船原件（不覆盖已导出的角色）；root写场景装配与Unity验收。

【Codex/root visual-phase3】追加认领tools/swgoh_extract.py缺失normal/UV通道判空修复（原星空mesh因None被漏导），非四件套；visual_gap在logs隔离目录重导单个Endor环境、核对原材质blend/cull/zwrite并导出原texture。root负责桥接材质，未恢复原shader数学项明确PENDING-ORIGINAL。

【Codex/root visual-phase3 2026-10-17】完成本轮原始战斗表现装配：替换活动战斗的自建Plane/两列x±6站位，直接组合原environment STANDARD attachment与layout相机/单位marker；独立舰队预览按role_capital解析transform身份，原Chimaera/Xwing/TIE比例不归一化。HUD恢复原尺寸三条/pips/圆键标/技能框与真值，SimpleFillMirror/SlicedFillMirror定点native闭合半图方向/几何border，source像素回归3PASS。修复mesh normal/UV None导致星空漏导，Endor现5mesh零错误；原材质状态/纹理桥接接回，但planet/cloud/fresnel程序仍PENDING-ORIGINAL（不是完整原shader）。1920x1080小队5v5阵位/舰队主力舰/真实点击门禁PASS，原AOE/DOT门禁与seed确定性回归PASS，资产四项PASS；build exit0/Succeeded，既存4缺prefab+许可诊断仍5条；正常与--fleet-preview玩家各存活15s无异常。REA成品ev_a6e23773fa66f1a8da92a9fb414cbec1d892d8219792c51930487ba91a5ead68 / SHA a4f243d52d84466da049e65a2b6edf57cf04752fdb4736779f384048767bc50d。详见BATTLE_PRESENTATION_RECOVERY_20261017.json。用户两张图仅验收（shots/reference本地保存），fixture塔图因/Endor非该场景像素复刻。未完成：完整船战/增援/crew接入、原材质程序/动画VFX/选择圈、完整NGUI、context/驱散/额外回合/被动等。原版零CU/零线上RPC/零货币或Mod操作；其他dirty工作保留。

【Codex/root vader-complete 2026-10-17】认领用户指定单英雄完整技能/动作/VFX闭环，舰队冻结。battle_gap独占Runtime/Battle+专用tests通用算子，visual_gap唯一Ghidra只读定点语义，effect_export独占原技能序列/动画/VFX解析及新导出配置（先logs），root写BattleDriver/新表现执行器与Unity验收；Git/Unity单写root。原始图/代码不足则PENDING，不造简化规则，不扩散逐英雄适配。

【Codex/root vader-demo 验收扩展 2026-10-17】用户确认阶段交付须有可运行demo，并新增存量bug：角色脚部/骨骼姿态、变体重叠/手腕武器挂接、缺剑刃及粉红VFX。认领通用资产链与Web同源bug修复（viewer仅修bug）；battle_gap只读骨骼动画根因+专用tests，effect_export唯一资产导出者核查variant/attachment，visual_gap独占原shader证据及新增OriginalAbility shader，root继续FSM/Unity门禁/运行demo；舰队冻结，不以替代材质或隐藏全部变体冒充原件。

【Codex/root 2026-10-17 phase4】交付 VADER 主动技能阶段demo：原四主动+unique效果链、原FSM/Animator/动画事件驱动结算与回位、原粒子和两份DXBC shader桥接；Unity native Avatar求值修正旧muscle弧度与RootT→hips错误。四角色40源动画/42语义动画、138597原标量keys读回PASS，共用显隐/手型/附件修复同步Web及公网。真实UI四技能PlayMode PASS；build Succeeded exit0（旧4缺prefab+license计errors5保留）；standalone存活50.8s、manual-ready、异常0。REA新成品SHA eff623b59b6604913c17f85411f433c40af846fa2d19d37990934624f65c5548。入口tools/launch_vader_demo.cmd，种子8；4490合法封锁保留双跑回归。详见VADER_STAGE_DEMO_20261017.json。队长技能/音效/受击遮罩/部分粒子版本差异及其余角色重烘焙仍PENDING，绝不标全英雄/全游戏完成。原E盘只读，零线上RPC/CU/货币/Mod操作。

【Codex/root 2026-10-17 high-reuse验证】认领原客户端运行时离线战斗与同版Unity原组件加载的可运行验证，暂停逐英雄扩展。用户最新明确允许必要线上API与少量computer use，覆盖本仓库旧API禁令；仍不得消费货币/改Mod，原E盘只读，凭据不得入库/聊天。root独占运行时副本与启动验证、Git；visual_gap独占保存Ghidra工程只读查原战斗离线入口；effect_export独占独立同版Unity验证项目（不得打开现有SWGoH-Restored）。所有运行日志logs/high_reuse，原版进程只做必要观测，隔离副本不提交线上战斗结果。

【Codex/root high-reuse 原组件验证 2026-10-17】独立安装Unity6000.3.17f1(cf0352b38e81)，OriginalComponentProbe直接AssetBundle.LoadFromFile原9bundle/5prefab：85Transform、39ParticleSystem、35材质引用/7原shader均supported，11unique原AnimationClip用原Avatar native求值全部有12–24骨骼运动。原9bundle运行前后SHA一致，原E盘无写入。独立build exit0，standalone25s exit0，0 Error/Exception，持续预览按键1–5/R实测PASS；入口tools/launch_original_component_probe.cmd。保留边界：39原游戏Mono实例缺失，Tron Trail原mesh为空需原脚本生成，诊断相机/灯光非原场景，不声称战斗或1:1画面。可替代GLB/重烘焙/粒子重写链，原runtime模拟验证仍在进行。详情logs/high_reuse/components/summary.json及player/report.json。

【Codex/root high-reuse runtime 2026-10-17】完成可运行验证：原客户端Simulation domain（模拟域）离线seed8双跑原FSM终态136/Outcome WIN，VADER对STORMTROOPER HP546→0，结果一致；原BattleGameState带画面流程真实进入手动技能态34，调用原普攻按钮HandleAbilityButtonClicked，原终态136后自动回Home，正式BattleService start/finish调用0，临时socket guard/GC清理全PASS。入口tools/launch_original_runtime_demo.cmd，REA安装内Frida宿主等完成消息再detach；原E盘只读，无货币/Mod操作。匹配Unity组件预览Release构建exit0/25s无异常/TCP UDP监听0。边界：warm已登录客户端，不是cold完全离线；只验证一次普攻/胜负，原截图未生成不得标视觉验收，SaveBattleOutcome回调未观察不得猜；原Mono缺39仍PENDING。决定主线保留原runtime并恢复本地启动/数据契约，同版Unity直接资源复用，停止逐英雄规则补丁。详见HIGH_REUSE_RUNTIME_VALIDATION_20261017.json。

【Codex/root 2026-10-17 tavern-luke】认领用户授权原客户端酒馆全部NPC临时替换原农场卢克；REA定位场景角色边界，仅当前进程视觉对象替换，保留恢复入口，原E盘只读，不改账号/库存/货币/Mod。root独占runtime注入与Git。

【Codex/root 2026-10-17 battle-info】用户说明上次为主动关闭并切换需求，暂停tavern-luke（已实例化14/恢复未验收，不能标完成）。认领原客户端长按详情HP/maxHP/护盾/状态回合与全模式目标技能CD显示；root独占runtime只读数据+UI改写，battle_gap只读数据契约，visual_gap唯一Ghidra只读显示门禁。禁止调用技能/推进当前战斗，不改数值/账号/E盘。

【Codex/root battle-info 2026-10-18】原客户端只读战斗数据+UI补丁：长按HP/maxHP、shield/maxShield、原状态事件剩余计数；统一模式显示门禁与onboarding UI分支，冷却值仍由原控件提供。修复writeRef多一级指针导致的早先崩溃，40例回归+原runtime scratch引用/空值读回PASS。针对用户指出文字重叠，修正原滚动面板topAnchor预留85单位而非移动会被约束复位的Table；UILabel自适应高度及行Widget测量，实际窗口截图确认两数值行/两状态行不重叠。本地JMK原模拟域零攻击/零回合/正式BattleService零调用，HP202948/202948、shield254034/254034、保护状态剩余4次自身完整回合；回Home/guard/GC清理PASS。常驻helper启停再启动PASS，当前已启用；入口tools/start_battle_info.cmd、stop_battle_info.cmd。全模式逐项、全部状态/长列表滚动未实测，不标全覆盖；原mastery标记无匹配计时明确报未读取到。详情BATTLE_INFO_UI_VALIDATION_20261018.json。
【Codex/root 2026-10-18 expanded-runtime】继续完整目标：临时护盾/实时属性/精通、AI预测、忠实试玩、操作接口、隔离模拟双边控制。root认领属性reader原runtime验证和共享桥接；先保存已有只读reader与试玩契约调查，不把原库存敌人fixture当关卡试玩。货币/装备变动须明确授权，体力可用。额度仅用户截图6%证据；低于1%使用一张重置卡授权保留，未查询到实时额度/重置调用入口。

【Codex/root expanded-runtime 2026-10-18 step1】新增JSON CLI capabilities/snapshot，原PID32636收藏页实测返回MasterInventoryGameState且无输入/RPC；属性reader原getter契约回归PASS。native body发现DTO displayOnly getter会改datacron上下文，已移除并加禁止调用回归；精通getter更正为系数，不伪称当前贡献。尝试隔离属性验证因原客户端收藏页被Home门禁阻止，未运行reader战斗调用，不标通过。新增试玩/AI静态契约脚本：当前fixture不能冒充真实关卡；原AI决策消耗RNG，预测不得调用live决策。用户新增右上角绝对值与隔离双边控制/AI预测/优先级修改均保持目标，尚待实现验证。完整台账RUNTIME_TOOLS_PROGRESS_20261018.json。

【Codex/root 2026-10-18 authorization更新】用户撤回重置卡授权并关闭开关：禁止使用/重开；全球重置前本周期周额度耗尽可使用已有额度继续，不含购买额度。用户明确授权自由启动/关闭游戏；货币消费和装备拆卸/分解仍需明确授权。此前低于1%重置一次授权已作废。

【Codex/root 2026-10-18 attributes-overlay】新增置顶只读窗口入口tools/start_attribute_overlay.cmd，初始/当前/净变化、总/永久/临时护盾及状态期限，过期或断连清空。原隔离JMK实测永久101823、临时152211、总254034，精通60→75净+15，属性warnings0，零攻击/正式BattleService0，回Home清理PASS。新增CLI inventory-home原UI导航实测及watch。早先持续观察出现frida-agent BEX64 c0000409，不能标稳定；改为脚本生命周期单一dispatcher hook+队列+优雅停止，40引用与100排队/重入/超时测试PASS，新PID41904持续读+并行隔离模拟+退场/停止后仍存活，无新错误观察。根因未完全证实，长时/全模式待验；右上角绝对值等完整目标未完。详见ATTRIBUTE_OVERLAY_VALIDATION_20261018.json。

【Codex/root 2026-10-18 target-header】长按面板及右上目标区新增生命/总护盾/永久护盾/临时护盾四行原getter绝对值；JMK与Stormtrooper隔离原UI截图确认不重叠，零攻击/回合/正式BattleService，回Home/guard/GC清理PASS。记录02:34:30再次frida-agent BEX64，dispatcher单改不足；本次改UI+observer单一宿主，悬浮窗只读JSON，CLI snapshot用fresh cache，互斥锁拒绝第二观察宿主。不能宣称长期崩溃已根治或全模式验收；详见BATTLE_HEADER_VALIDATION_20261018.json。

【Codex/root 2026-10-18 inventory-cli】共享宿主命令邮箱接通原角色/舰船收藏与返回酒馆：原TransitionData+StateController入口保留feature lock，实际type1/nav8与type2/nav10两轮闭环；最终PID19440 Home，宿主40468存活、错误日志空。请求绑定game/host PID、截止时间、白名单，先claim再执行，重复完成ID不重放；未确认目标页返回失败，超时不盲重试。单宿主锁及邮箱回归PASS，酒馆误调用inventory-home原门禁拒绝。无购买/装备操作，不宣称其他菜单/忠实试玩/AI预测或长期稳定已完成。证据INVENTORY_CLI_VALIDATION_20261018.json。

【Codex/root 2026-10-18 battle-input-cli】新增通用BattleInputBridge读取原技能列表及短期状态token，CLI battle-ability经共享宿主调用原HandleAbilityButtonClicked；原被动/禁用/隐藏门禁保留，过期/无效index/重复token拒绝。第一次验证纠正Idle枚举0→原1，保留失败证据。修正后直接模块及完整CLI两轮VADER普攻原终局136/turn1、正式BattleService0、回Home/guard/GC全通过；完整CLI关闭probe自动点击，legacy verifiedVisualBattle因此false，按外部回执+原终局独立验收，不掩盖。恢复日常宿主36580。REA同SHA选目标原SelectManualTarget与嘲讽候选契约已获，接口待接；全部技能/终极/选友军/模式/忠实关卡试玩/AI目标未完。详见BATTLE_INPUT_CLI_VALIDATION_20261018.json。

【Codex/root 2026-10-18 battle-target-cli】接通battle-target原SelectManualTarget，候选由原GetValidTargets(null)提供，提交后核对current/manual目标；重复已选目标no-op避免原toggle取消。快照token改进程宿主epoch+序号避免长JSON参数；选友军pending时普通输入拒绝。隔离单敌验证非法ID拒绝/重复no-op，双敌STORMTROOPER+JEDIKNIGHTCONSULAR实际切目标并两普攻原终局136/turn2，正式BattleService0，Home/guard/GC全PASS；错误JEDICONSULAR测试保留，未进入战斗。邮箱/互斥锁/turn-order回归PASS。原特殊选友军HandleUnitTapped无效ID仍会结束选择，后续必须先校验候选；本轮未开放。日常宿主32960已恢复，PID19440存活。证据BATTLE_TARGET_CLI_VALIDATION_20261018.json；嘲讽fixture/其他模式/终极/忠实关卡试玩/AI仍待验证实现。

【Codex/root 2026-10-18 battle-unit-cli】新增battle-unit完成原特殊技能选友军：只接受原_units候选，防止原HandleUnitTapped无效ID仍消费选择的行为。JMK special02→原候选GENERALKENOBI→无效拒绝且保持选择→有效提交，原伤害免疫1回合/保护2回合等状态实测出现；重复token拒绝。发现CD4但按钮NORMAL，REA核对原IsAbilityReady纯读、ManuallyActivateMove经CanPerformMove保留全部门禁；CLI新增ready原getter提前拒绝。第二轮原skill→ally→CD技能拒绝→普攻→原终局136/turn2、正式BattleService0、Home/guard/GC PASS。第一轮观察到turn1后按deadline返回Home如实保留。邮箱/turn-order回归PASS。详见BATTLE_UNIT_CLI_VALIDATION_20261018.json；终极/取消选择/全模式与真实关卡试玩/AI等完整目标仍待推进。

【Codex/root 2026-10-18 battle-cancel-cli】新增battle-cancel直接原CancelSelectUnit；JMK special02进入选友军→取消实测awaitingAbility恢复，turn0/CD0不变，再普攻原终局136/turn1，正式BattleService0，Home/guard/GC PASS。邮箱畸形取消请求回归及种子turn-order PASS，日常显示宿主恢复。REA确认取消不执行完成回调，按钮重复点击只额外取消音效。真实试玩新增body证据：F8F750响应会改账号库存禁止当纯转换；GetProto1D0B430有损不能roundtrip；Squad copy1005180共享Datacron/AbilityDO，cell copy待核。目标保持捕获原proto+原队伍精确隔离，不以库存fixture替代；详见BATTLE_CANCEL_VALIDATION_20261018.json。
