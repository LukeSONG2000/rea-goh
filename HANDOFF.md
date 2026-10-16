# 交接手册：从原始游戏到可用复现的完整链路

> 交接对象：从原始内容开始接手的开发者。按阶段阅读，每阶段列出「产物状态 / 可用性 / 到下一阶段的努力」。

## 阶段 0：原始游戏安装（E:\Games\EA Games\SWGoH）

**产物**：SWGoH.exe、GameAssembly.dll（IL2CPP，**未加密**）、global-metadata.dat（metadata v39，25MB）、Unity 场景/资源。游戏本体为 Unity 6000.3.17f1（UnityPlayer.dll 可验证）。

**可用性**：二进制形态，不可直接编辑。但元数据未加密 ⇒ 完整逆向可达（客户端权威：战斗在客户端本地模拟）。

**努力**：无（这就是起点）。注意：启动游戏会触发版本更新改写缓存——镜像推送前曾是风险，现已无碍。

## 阶段 1：原始缓存镜像（raw_bundles/，未经修改的解包源）

**产物**：11,400 个 UnityFS bundle（2.7GB，junction 自 `%USERPROFILE%\AppData\LocalLow\Unity\Electronic Arts_SWGoH`）。全部已镜像至 GitHub。

**可用性**：标准 UnityFS，UnityPy 1.25.4 可读。六个必知陷阱（详见 UNPACK_GUIDE.md）：
1. 同名 bundle 多版本 → 取 mtime 最新的 `__data`
2. PPtr 外部引用存 CAB 内部名 → 按 path_id 回查共享容器
3. typetree 全部包一层 `'data'` 键；大小写敏感
4. **内置网格不在缓存**：Quad/Plane 引用 external `unity default resources`（pathID 10210 等）→ 需自行合成几何（tools/export_env.py 已实现）
5. **UI prefab、字体、gamedata 数值、UI 布局不在本地**——运行时从 CDN 下发只存内存 → UI 需自建 uGUI（用提取的原版贴图），数值需运行时内存读取或社区镜像
6. UI 图集是 **NGUI**（UIAtlas mSprites[] 精确切片），不是 UGUI

**努力**：镜像本身 0 努力（junction + tar）；可用化 = 阶段 2。

## 阶段 2：解包资产（extracted/ extracted_env/ extracted_ui/）

**产物与可用性**（全部为标准格式，任何 DCC/引擎可导入）：

| 目录 | 内容 | 状态 |
|---|---|---|
| extracted/char_*/ | 747 角色 GLB（静态）+ 719 个 .anim.glb（游戏状态机动画）+ textures/ + report.json | ✅ P1-P8 保真缺陷全修复，1466 GLB 审计 0 违规（catalog/audit_fidelity.json）|
| extracted_env/ | 269 场景 GLB + 地面合成 | ✅（222 bundle 曾缺内置地面，已合成）|
| extracted_ui/ | 769 头像 + **60,151 NGUI 原版切片**（atlas_sprites/ + index.json）+ 1,856 技能图标 + HUD | ✅ 可用（60k 切片不入库，`py extracted_ui/harvest_ui_assets.py` 再生）|
| catalog/ | bundle 全景、gamedata 镜像（11,484 单位/63,035 效果/6,732 技能）、名称溯源、战斗站位（130 布局）、船员表（2,772 张）、缺陷史 BUGS.md | ✅ |

**到这一阶段的努力**（即 UNPACK_GUIDE.md 的方法论 + 七大坑实录）：UnityPy 权重解压 bug、变长骨骼影响数、骨数/网格失配、bindpose 冲突 → rest 姿态决策、COLOR_0 顶点色陷阱、挂件三种序列化机制、X 镜像坐标约定。**交接者不必重走弯路：tools/swgoh_extract.py 与 export_animation_v2.py 已内置全部修复，直接调用即可。**

## 阶段 3：逆向知识（il2cpp/ + catalog/ 的逻辑层）

**产物**：
- dump.cs（34.5MB：26,399 类型/199,578 方法全带 RVA）+ func_vas.tsv（名字↔地址总表）+ 13+ 个定点反编译产物（turn_order/damage/mitigation/crew_stats/fleet_* 等）
- **自研 metadata v39 解析器**（il2cpp/tools/metadata39.py——主流工具均不支持 v39，这是我们自举破解的，26.5 万方法可编程检索）
- BATTLE_SIM_INVENTORY.md：战斗核心全部解码（回合制 tick 算法、五流 JavaRandom 播种、暴击/减伤/伤害管线、修正聚合规则、属性聚合、AI 决策树、GameConstants 偏移表）——**每条带反编译出处**
- FLEET_BATTLE_INVENTORY.md：舰队战斗 = 同一管线 + 1 字节 CombatType 开关（无独立模拟器，离线可行性高）
- 工具：metadata39.py、read_game_constants.py（运行时内存扫描器，自测通过，等游戏登录一次自动抓数据驱动真值）

**可用性**：结构化 + 可执行。努力 = 这是最贵的一段（Ghidra 12 + PyGhidra 工作流、自举校准法），但产物已沉淀为文档与工具，交接者只需读。

## 阶段 4：运行时成果（当前进度）

| 成果 | 状态 | 入口 |
|---|---|---|
| Web 模型库（747 角色/动画/分类/中文名/缩略图） | ✅ 公网 | https://39.105.74.77:12021（goh/swgoh2026；域名未备案走 IP）|
| Web 对战 demo（选将→确定性战斗→3D 回放 + 真实技能名 + 原版站位/地面） | ✅ 公网 | /viewer/battle.html |
| Unity：BattleDemo（P5 战斗 HUD：计时器/键位/技能条/首领头像） | ✅ 本地可玩 | unity/SWGoH-Restored → Assets/Scenes/BattleDemo.unity → Play |
| Unity：RosterScene（P3 物品栏）+ DetailScene（P4 详情） | ✅ 本地可玩 | 同工程；原版 NGUI 切片搭建，头像映射 98.1% |
| 战斗核 C# 移植（BattleSim.cs = 反编译逻辑忠实移植，确定性种子） | ✅ | Assets/Scripts/Runtime/Battle/ |
| GameConstants 运行时真值 | ⏳ 扫描器就绪 + 12h 守望器，游戏登录一次自动抓取 | il2cpp/tools/read_game_constants.py |
| P2 主城壳（cantina 背景 + 热键导航） | 🔄 agent 在跑 | — |
| Windows standalone exe 打包 | ⏳ 待 P2 交付 | — |

## 交接清单（给从原始内容开始的开发者）

**必读顺序**：
1. README.md → UNPACK_GUIDE.md（解包方法论 + 七坑）
2. catalog/BUGS.md（保真缺陷史与根因——避免重蹈）
3. il2cpp/BATTLE_SIM_INVENTORY.md + METADATA39_NOTES.md + FLEET_BATTLE_INVENTORY.md
4. catalog/UI_REFERENCE.md（P1-P6 验收标准 + 原版截图 shots/reference/）
5. catalog/PROGRESS.md（当前状态台账）

**工具链**：Python（`py`，非 `python`——WindowsApps stub 是坏的）+ UnityPy 1.25.4；Unity 2022.3.62f1（Hub 已装，Personal 授权需本人登录一次）+ PyGhidra；gh CLI。Git bash 注意：CRLF（tar 文件列表要 `tr -d '\r'`）、heredoc 转义、ssh heredoc 展开 `$`。

**环境对策**：GitHub 直连间歇封锁 → 重试循环（8×30s）必过；codeload/jsdelivr 可作下载备选；Unity 安装器走国内镜像（download.unitychina.cn），6000.3 系列 mirror 缺货用 LTS。

**铁律**：
1. 保真：提取原件重组，不发明（占位必须申报 PENDING-ORIGINAL）
2. 验证：一切修复走程序化门槛（tools/verify_*.py 四件套），不靠目测
3. 战斗确定性：同种子必同局；改逻辑先跑 demo_turn_order + engine 种子双跑
4. 资产版权：仓库与部署保持私有；公网走 IP+密码

**明确不在范围**：服务器独占系统（进度/经济/抽卡/社交）——那是网游服务端，源代码已随公司内部库丢失且我们不上服务器；离线复现不含它们。舰队战斗离线可行性=高（侦察完毕）。
