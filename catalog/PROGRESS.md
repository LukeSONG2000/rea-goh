# 修复集成进度（PROGRESS TRACKER）

本文件是三个并行修复 Agent（A/B/C）交付物与集成的**唯一事实源**。
Tracker 每 ~5 分钟轮询交付标记，验证通过后集成（重导出→同步服务器→commit+push）。
缺陷定义见 `catalog/BUGS.md`（P1-P8）。

## 状态总表

| Agent | 任务 | 交付标记 | 状态 | 证据 | 集成 |
|---|---|---|---|---|---|
| A | 挂件绑定到骨骼 (P3/P6) + rig 倾斜 | `tools/verify_attachments.py` 存在 且 `extracted/char_ackbaradmiral_pre/` 重导出（内容变化） | ⏳ 未交付 | verify_attachments.py 不存在；ackbar anim.glb mtime=10-15 16:51（旧批次） | 未开始 |
| B | 变体 m_IsActive + ParticleSystem 排除 + 发光 alphaMode (P5/P8) | `tools/verify_variants_vfx.py` 存在 且 `char_vaderduelsend_pre`/`char_jynerso_pre` 重导出 | ⏳ 未交付 | verify_variants_vfx.py 不存在；vader 16:54 / jyn 16:52（旧批次） | 未开始 |
| C | 站位 staging + 环境地面 (P1/P2) | `catalog/battle_staging.json` + `tools/export_env.py` 地面支持 + 5 个 env GLB 重导出 + `game/server.py` + `viewer/battle.html` 站位补丁 | 🔨 进行中 | battle_staging.json 23:15、export_env.py 23:18、5 env GLB 23:18-19 已就绪；**server.py(14:28)/battle.html(20:45) 尚未修改** | 未开始 |

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
