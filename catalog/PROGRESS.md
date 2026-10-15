# 修复集成进度（PROGRESS TRACKER）

本文件是三个并行修复 Agent（A/B/C）交付物与集成的**唯一事实源**。
Tracker 每 ~5 分钟轮询交付标记，验证通过后集成（重导出→同步服务器→commit+push）。
缺陷定义见 `catalog/BUGS.md`（P1-P8）。

## 状态总表

| Agent | 任务 | 交付标记 | 状态 | 证据 | 集成 |
|---|---|---|---|---|---|
| A | 挂件绑定到骨骼 (P3/P6) + rig 倾斜 | `tools/verify_attachments.py` 存在 且 `extracted/char_ackbaradmiral_pre/` 重导出（内容变化） | ✅ 交付 | verify_attachments.py 23:40 落地；A 自跑全量重导出（23:44-23:51:52，719 anim.glb）；**7 char 广集验证 PASS**（ackbar/grievous/grievousraid/vaderduelsend/jynerso/obiwanep4/anakin_gl：所有 prop d=0.000-0.44 在 tol 0.5 内；upright probe 脚 y≈0、头在骨盆上方 = 倾斜修复）| ✅ 集成中 |
| B | 变体 m_IsActive + ParticleSystem 排除 + 发光 alphaMode (P5/P8) | `tools/verify_variants_vfx.py` 存在 且 `char_vaderduelsend_pre`/`char_jynerso_pre` 重导出 | ⚠️ 交付但回归 | B 自 commit `8b2fbc40`（20/20 verified @ B 自导出件）；**A 全量扫盘后 2 项回归**: ①jynerso P5 缺 `swd_jynerso_mesh` 隐藏标记 ②vaderduelsend P7 披风 t0(Idle@1.0) 147 顶点超 0.35 体包络(max 0.76)。另 fleet 审计（audit_fidelity.py, exit 1）: V1 粒子网格残留 9（traya/palpatine 家族/dath_altar）、V2 不透明 fx 材质 18 | 🔁 重试环（10min × 3）|
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
