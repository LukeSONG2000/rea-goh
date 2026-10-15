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
