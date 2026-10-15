# 修复集成进度（PROGRESS TRACKER）

本文件是三个并行修复 Agent（A/B/C）交付物与集成的**唯一事实源**。
Tracker 每 ~5 分钟轮询交付标记，验证通过后集成（重导出→同步服务器→commit+push）。
缺陷定义见 `catalog/BUGS.md`（P1-P8）。

## 状态总表

| Agent | 任务 | 交付标记 | 状态 | 证据 | 集成 |
|---|---|---|---|---|---|
| A | 挂件绑定到骨骼 (P3/P6) + rig 倾斜 | `tools/verify_attachments.py` 存在 且 `extracted/char_ackbaradmiral_pre/` 重导出（内容变化） | 🔨 进行中 | verify_attachments.py 不存在；**swgoh_extract.py 23:28 正在被改**；ackbar anim.glb 仍是 16:51 旧批次 | 未开始 |
| B | 变体 m_IsActive + ParticleSystem 排除 + 发光 alphaMode (P5/P8) | `tools/verify_variants_vfx.py` 存在 且 `char_vaderduelsend_pre`/`char_jynerso_pre` 重导出 | 🔨 进行中 | verify_variants_vfx.py 23:2x 出现，但**自身有 bug**（compiled pattern 传 re.I → ValueError，23:29 基线跑全 FAIL）；vader/jyn GLB 仍是 16:5x 旧批；**char VFX 重导出扫盘正在进行**（extracted/char_*/vfx.json 23:29 持续写入）| 未开始 |
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
