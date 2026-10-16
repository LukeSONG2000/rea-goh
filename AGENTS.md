# AGENTS.md —— 多智能体协作规范

> 本文件是所有自动化智能体（Codex / Claude / 其他）在本仓库工作的**强制约定**。
> 目标：完整复现 SWGoH（星球大战：银河英雄传）为可玩的原生 Unity 游戏。
> 新接手先读：README.md → HANDOFF.md → catalog/PROGRESS.md（当前状态台账）。

## 1. 目录所有权（写权限边界）

| 目录 | 属主/用途 | 并发规则 |
|---|---|---|
| `raw_bundles/` (junction) | 只读镜像，永不修改 | 所有人可读 |
| `extracted/` `extracted_env/` `extracted_ui/` | 资产导出产物 | 同一时间只允许**一个**导出任务写；重导前在 PROGRESS.md 声明 |
| `catalog/` | 共享台账与逆向知识（PROGRESS.md / NUMBERS.md / BUGS.md / JSON 清单） | **只追加**：追加自己的条目，不改他人条目；每次变更即 commit |
| `tools/` | 解包/导出/验证脚本 | 改前 grep 是否有在跑的 agent 声明拥有；verify_*.py 四件套不许动语义 |
| `il2cpp/` | 逆向（dump.cs / 反编译 / Ghidra 工程） | ⚠️ **Ghidra 工程单实例**：同时只能有一个 pyghidra 会话打开 `proj`（否则锁冲突）；dump/ 目录不入库 |
| `unity/SWGoH-Restored/` | Unity 工程 | ⚠️ **Unity 单实例**：批处理前检查 `unity/SWGoH-Restored/Temp/UnityLockfile` 与 `tasklist //fi "IMAGENAME eq Unity.exe"`；有 GUI/批处理在跑就**等**，绝不 taskkill 别人的会话 |
| `game/` | 网页对战后端（Python） | 改后必须跑种子确定性验证再提交 |
| `viewer/` | 网页前端（已冻结为演示版） | 只修 bug，不加新功能（产品主线在 Unity） |
| `logs/` | 所有运行日志（*.log / *.txt / 截图副产物） | 根目录**禁止**落地任何日志/临时文件 |
| `unity/build/` | exe 构建产物（1.9GB，gitignored） | 构建期间他人不得再触发构建 |

## 2. 硬规则（违反 = 破坏他人工作，历史教训全部真实发生过）

1. **Unity 单实例**：`Temp/UnityLockfile` 存在 = 有会话在跑。**禁止** taskkill 别人的 Unity.exe；只允许杀自己确认无主的僵尸（进程内存 <200MB 且 >10 分钟无日志增长才可判定）。
2. **Git 单写者**：push 失败重试用循环（`for i in 1..8; git push || sleep 30`），网络间歇封锁必过；**绝不**删除 `index.lock`——等 60 秒重试三次，仍锁才检查是否有存活 git 进程。
3. **RPC/API 红线**：`sw-live.cgswprod.com` 及一切线上 API/RPC 端点**永不访问**。只允许 `eaassets-a.akamaihd.net` 公共资产 CDN（无授权静态文件）。
4. **本地优先**：宣布"资源缺失"前必须穷尽本地（raw_bundles 9323+ bundle、catalog/loc_cn+loc_en 全量文本、4,677 个运行时下载的新 bundle）。历史教训：三次"缺失"结论全被推翻（图标抽样未全、描述在 loc 表、UI prefab 随登录下载落盘）。
5. **保真铁律**：提取原件重组，不发明替代品。占位必须申报 PENDING-ORIGINAL。术语随文标注 English（中文）。
6. **E:\ 只读**：游戏安装目录永不写入。
7. **参考截图**（shots/reference/）只作验收对照，不作数据源。

## 3. 协作协议

1. **开工前**：读 `catalog/PROGRESS.md` 末尾 → 确认要做的事没被 in-flight 的线占用 → 在 PROGRESS.md 追加一行"【agent名】认领 <任务>"再动手。
2. **提交纪律**：小步提交；提交信息写清楚"做了什么+验证方式"；push 用重试循环；**绝不** force push。
3. **验证门槛**（提交前必跑对应项）：
   - 战斗改动 → `py il2cpp/offline/demo_turn_order.py`（确定性）+ 种子双跑
   - 资产导出改动 → `py tools/audit_fidelity.py`（1466 GLB 0 违规）+ `py tools/verify_variants_vfx.py` + `py tools/verify_attachments.py`
   - exe 改动 → 批处理构建 exit 0 + 玩家进程存活 15s + PlayMode 回归截图
   - 网页改动 → 浏览器实测 0 控制台错误
4. **环境坑**（全部真实踩过）：解释器用 `py`（裸 `python` 是坏的商店 stub）；git-bash heredoc 会展开 `$`、tar 文件列表要 `tr -d '\r'`；ssh 到 centos-laptop 的 sudo 需要显式 `sudo`；gh.exe 在 Program Files；Playwright 截图用 `--use-gl=angle`（swiftshader 白屏）。
5. **部署**：笔记本 `centos-laptop`（ssh 可达）承载公网服务：caddy :8443（TLS+密码）+ FRP 隧道 → 公网 `https://39.105.74.77:12021`。部署后必须从公网侧验证（带 -H "Host: goh.lukesong.cn"）。

## 4. 命令速查

```bash
# 重导出全部角色动画（~20 分钟）
PYTHONIOENCODING=utf-8 py tools/export_animation_v2.py --all --force > logs/export.log 2>&1
# 全库保真审计（1466 GLB）
PYTHONIOENCODING=utf-8 py tools/audit_fidelity.py
# Unity 构建 exe（含 DataListScene 自动创建）
"F:/Unity/2022.3.62f1/Editor/Unity.exe" -batchmode -quit -projectPath "F:\PPfavorite\Work\goh\unity\SWGoH-Restored" \
  -executeMethod SwgohRestored.EditorTools.BuildRestored.BuildWindowsPlayer -logFile logs/build.log
# 运行时抓取 GameConstants 真值（需原版游戏运行+登录）
PYTHONIOENCODING=utf-8 py il2cpp/tools/read_game_constants.py
# 同步到公网笔记本
tar -cf - -C . -T <清单文件> | ssh centos-laptop "cd /srv/goh && tar -xf -"
# 公网验证
curl -sk -u goh:swgoh2026 -H "Host: goh.lukesong.cn" https://39.105.74.77:12021/ -o /dev/null -w "%{http_code}"
```

## 5. 项目密码与凭据政策
- 公网 Basic Auth：goh / swgoh2026（仅预览用途，可随时让用户重置）
- 原版字体/贴图等 EA 授权资产：仅限本项目非商用复刻使用
- 一切访问令牌/设备凭据不入库不入聊天（历史承诺，持续有效）
