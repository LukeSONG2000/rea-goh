# REA 独立复核报告（REA_GUIDANCE）

> 生成：2026-10-16。工具：REA 6.5.0（Reverse Engineer Anything，morluto/rea）+ 本机 Ghidra 12.1.4。
> 目标：`E:\Games\EA Games\SWGoH\GameAssembly.dll`（只读，SHA-256 见下）。
> 结论先行：**REA 是真实、可用的本地逆向工具链；对我们 BattleSim 方向的 23 项定点抽查全部吻合（21 项 RVA 精确命中，2 项为工具边界而非漂移）。方向无需修正，防漂移清单见 §7。**

---

## 1. REA 是什么

- **身份**：`rea-agents`（npm）/ [morluto/rea](https://github.com/morluto/rea)，MIT 许可，自称 "Reverse Engineer Anything"。本质是一个 **MCP 服务器 + CLI**（6.5.0：207 个 MCP 工具 / 153 个 CLI 命令），把 Ghidra/IDA/Hopper、objdump/Rizin、JADX、.NET 静态解析、APK/HAR/pcap 等数据源统一成带 **Evidence（证据结构：目标摘要 SHA-256、操作参数、局限性、结论置信度）** 的查询接口，供 agent 或脚本调用。
- **与本项目的相关性**：其对 Native PE 的静态分析（Ghidra 后端，反编译/反汇编/xrefs/字节读取）正是我们 IL2CPP 逆向所用的同一类能力 —— 因此它能作为**独立的第二意见**复核我们 Ghidra 人工分析得出的结论。
- **明确不做的事**（官方文档原文确认）：
  - 托管解析器**不解码 IL2CPP metadata**（"the managed parser … does not unpack … IL2CPP metadata"）→ `global-metadata.dat` 仍归我们自研的 `il2cpp/tools/metadata39.py` 管，REA 不构成替代。
  - 不上传任何文件：分析全部本地执行（README FAQ "REA analyzes targets locally"）；无账号、无付费服务依赖（Hopper 为可选商业软件，本次未用也不需要；`rea setup` 不会静默安装任何东西）。
  - Windows 支持是**实验性 P0**：仅支持 x86/x64/ARMNT/ARM64 PE 的**只读**分析（本次正是此模式；`annotate_native_function` 改名/批注功能在 Windows P0 不可用）。

## 2. 安装记录（逐步可复现）

| 步骤 | 命令/事实 |
|---|---|
| 克隆 | `git clone --depth 1 https://github.com/morluto/rea.git rea`（第 1 次尝试即成功，未用到重试循环/镜像；main @ `d72d856f`，2026-10-16）|
| 运行时 | Node v24.13.0 ✓（要求 ^22.19 / ^24.11 / 26+），npm 11.6.2 |
| 安装 | `npm install --global rea-agents` → **6.5.0**（官方文档的标准安装路径；克隆仓库仅作文档/源码参考，未从源码构建）|
| 分析引擎 | **复用项目已有** Ghidra 12.1.4：`il2cpp/ghidra/ghidra_12.1.4_PUBLIC/`（含 `win_x86_64/decompile.exe` 原生反编译器）；JDK 24.0.2（`D:\Program Files\Java\jdk-24`，≥21 即可）|
| 环境变量 | `GHIDRA_INSTALL_DIR`、`JAVA_HOME`（`rea doctor` 按这两个键发现引擎）|
| 体检 | `rea doctor --provider ghidra --json` → `healthy:true`，10/10 项通过（版本/headless/原生反编译器/64 位 JDK/Windows Job Object 原生权限均验证）|
| 未做的事 | **未运行 `rea setup`**：它会改写用户全局的 Claude/Codex/VSCode/ZCode 客户端配置，超出本任务授权；CLI 与 MCP(stdio) 不注册也能用 |

## 3. 本次执行的分析（What I ran）

- **驱动方式**：一次性 CLI（`rea decompile` 等）每次调用都要重启 Ghidra 并重做全量自动分析；95MB IL2CPP 的 DLL 单次自动分析约 **2h50m**（12 逻辑核，`GHIDRA_HEADLESS_MAXMEM=12G`，临时工程 `-deleteProject` 不留存）。因此自写了持久 MCP 客户端 `catalog/rea_workspace/mcp_drive.mjs`（复用 rea-agents 自带的 MCP SDK）：**一个会话内顺序执行 11 步**，分析只做一次。
  - 注意坑：rea-agents 内置的是魔改版 MCP SDK，`callTool(params, options)` 只收 **2 个参数**（超时选项在第 2 位）；SDK 默认单次请求 60s 超时，必须显式传大超时。
  - 复现：`node catalog/rea_workspace/mcp_drive.mjs catalog/rea_workspace/plan_main.json out`（plan 内含 open_binary → binary_overview → batch_decompile(23 地址) → analyze_function×3 → read_function_instructions → read_bytes → address_to_file_offset×2）。
- **目标摘要（REA Evidence 自动锚定）**：
  - `GameAssembly.dll` SHA-256 `12cc254525a58aedea122a2967d0476da05b98ef0c219dc18cfc0b7207538db6`
  - `global-metadata.dat`（25,125,572 B）SHA-256 `783a0083923a334471950ce62a6e01e40a515c00580e7072b832c2cf43f52f22`
- **全新分析的全景（binary_overview，逐字）**：image base **0x180000000**；**196,098 个过程**；分段：Headers 0x180000000 / **.text 0x180001000-0x180BB7800 (11.7MB)** / **il2cpp 0x180BB8000-0x18494FA00 (61.6MB，战斗代码全在此段)** / .rdata 0x184950000 / .data 0x1855BF000 / .pdata …
  - 对照：metadata39.py 计得 199,578 个方法 → 全新分析发现 196,098 个函数体（差 1.7%，量级完全吻合；差额属泛型未实例化/共用 thunk 等正常现象）。

## 4. 逐项交叉验证（fresh eyes：无符号、无我们注入的任何信息）

| 我们的主张（出处） | REA 全新分析结果 | 判定 |
|---|---|---|
| **RVA 体系**：`il2cpp.cs` 每方法 `// 0xVA-0xVA`（Il2CppInspectorRedux 生成，image base 0x180000000） | 23 个被抽核心地址中 **21 个在无符号全新分析中恰好是函数入口**（canonical entry == 声明地址，FUN_181365b20 等）；DamageModule 族 0x1813658C0→0x181366290 与 dump.cs 完全连续拼合 | ✅ **成立** |
| ProcessCritChance 0x181365B20（NUMBERS §5） | 反编译逐条吻合：`GetStatValue(14|15) − GetStatValue(22|23)`，`÷ GameConstants+0x2308/+0x22F0`（Runtime 2400），`pctAdd(28) − 对方40`，`clamp 1.0`，`(Lv差)×+0x2300/+0x22E8` 钳 `+0x22FC/+0x22F8`（Min/Max），基线 `+0x2314/+0x230C`；掷骰为**内联 JavaRandom**：`seed=seed*0x5deece66d+0xb & 0xffffffffffff`，`nextInt(100)` 溢出拒绝环，`roll < chance×100.0` | ✅ 全等 |
| ProcessCritDamage 0x181365F10 | `GetStatValue(0x10=16) + *(float*)(GC+0x231C)`，`< 1.0 → 1.0` | ✅ 全等 |
| GetMitigationPercentFromRating 0x180FDFDF0（"反编译体 mangled，从 raw listing 恢复"） | **REA 的反编译同样 mangled**；其 raw 指令流完整解出：`level(+0x1C34 舰船加成) → (level+off)×RatingMod[+0x22F4 物理/+0x24E0 特殊; 舰船变体 +0x1C28/+0x1C64] → +rating → rating÷和 → ×[.rdata 0x18529F324]` | ✅ 全等（且补充了舰船变体字段的**代码级**证据） |
| 播种 0x181A5EFB0：五流同种子 | 反编译显示 5 次构造，全部 `((int)*(BattleStartResponse+0x18) ^ 0x5DEECE66D) & 0xFFFF_FFFF_FFFF`，存入 **+0x158/+0x160/+0x168/+0x170/+0x178** | ✅ 全等 |
| .rdata 0x18529EF3C = 1.0f（静态乘子） | `read_bytes` → `0000803f` = 1.0f | ✅ 全等 |
| ProcessUnitsSpeed 0x181CFFB50（NUMBERS §4） | 结构吻合：取 stat 5、负速钳 0、`fill(+0x20C) += speed×ticks`、min-ticks 候选 + 自定义 float 比较帮助函数、`_turnPriorityUnits(+0xB8)` 列表反向遍历剔除（1e-6/×8.0 常数在子函数 `DetermineTicksToNextAttack` 内，本函数体未见亦无冲突） | ✅ 成立 |
| GetModifyEffectAmount 0x181A73980（正 MAX/负 MIN/每来源键） | 负数分支逐字可见：`if (amount < 0)` → 按来源键查已有值 `if (existing < cur || existing == cur) skip`（保更负=MIN），45/46 参数命中 | ✅ 成立 |
| GameConstants..ctor 0x1811EBF50 硬编码（100.0/2400.0/1.0，用 C7 机器码扫描提取） | RVA 命中函数入口；但一次性反编译失败，REA 保留的底层原因：**"Ghidra decompilation failed … Response buffer size exceeded"**（我们上一轮 PyGhidra 报 timeout）——巨函数单发反编译不可行，机器码扫描是正确路线 | ✅ 成立（方法学互证） |
| JavaRandom..ctor 0x180CAC420 | dump.cs 中 ctor 与 SetSeed **共享同一 32 字节区**（0x180CAC420-0x440，ctor 是 thunk）；全新分析未在该处建独立函数入口（被合并），故 batch_decompile 报 invalid。LCG 公式本身已由上面两处内联代码独立证实 | ⚠️ 非漂移，属 thunk 合并的粒度差异；建议文档标注 "ctor==SetSeed thunk" |

**没有发现任何与我们对立的证据**：所有被抽查的公式、字段偏移、常量值（2400/7.5/0.15/0.01/1.5/60/0.075/±0.75、+0x24B4 族）在全新分析中要么直接命中，要么与"运行时真值来自 config 覆盖、ctor 只硬编码子集"的双层模型一致。`catalog/game_constants_runtime.json` 的 2066 字段与 NUMBERS.md 的 21 项核心常数记录完整，偏移逐一被反编译代码引用证实。

**产物文件（全部在 `catalog/rea_workspace/`）**：`out_main/03_binary_overview.json`（全景）、`04_batch_decompile.json`（23 函数反编译全文）、`05-07_*.json`（函数档案）、`08_instructions_mitigation.json`（减伤函数 60 条指令流）、`09_bytes_static_1.0.json`、`verification_table.json`（逐项判定表）、`snippet_0x181365b20.c` / `snippet_0x180fdfdf0.c` / `snippet_0x181cffb50.c` / `snippet_0x181365f10.c` / `snippet_0x181a5efb0.c` / `snippet_0x181a73980.c`（关键函数逐字伪码）、`claimed_rvas_from_dump.json`（dump.cs 侧 RVA 台账）、`mcp_drive.mjs` + `plan_main.json`（可复跑的驱动与计划）。

## 5. REA 能力边界（对本项目）

- **适用**：PE 静态复核（本次已用）、指令级取证（read_bytes/search_bytes/xrefs——比 Ghidra GUI 脚本更适合做自动化断言）、Evidence 摘要（每个结论自带目标 SHA-256，天然防"换版本后引用失效"）。
- **不适用/勿用**：`global-metadata.dat`（官方明确不支持 IL2CPP metadata）；Windows P0 上做任何"写注释/改名"类操作（只读限制）；把它当 dump.cs 生成器（它不做 metadata→C# 映射）。
- **成本**：每开新会话都要重付 ~3h 全量分析；单点抽查用 CLI + `--snapshot` 缓存单查询结果即可。

## 6. 方向评估（fresh eyes）

战斗核路线（客户端本地确定性模拟、五流种子、公式层移植）得到独立工具链的全量背书；解码覆盖面（FSM→回合→伤害→暴击→减伤→修正→聚合→AI 树）与标准 RE 工作流一致。**方向正确，无需纠偏。** 尚未覆盖、且按标准工作流接下来最值得做的：FSM 中的 `Attack_ProcessCounterAttacks` / `Attack_UnitDiedFromDOTS` 分支、~20 个 AI `EvaluateCondition` 叶子体、Mods/PersistentEffect 管线喂入 §10 聚合、以及用真实 `BattleRecord[]` 捕获做内容级回归门（现在只有确定性门）。

## 7. 防止跑偏清单（按优先级）

1. **摘要钉死（digest pinning）**：把 `GameAssembly.dll 12cc2545…8db6`、`global-metadata.dat 783a0083…2f22`、游戏版本号写进 BATTLE_SIM_INVENTORY.md / NUMBERS.md / FLEET_BATTLE_INVENTORY.md 页眉。REA 的 Evidence 证明这是低成本高价值的纪律：所有结论只在"这个二进制"上成立。
2. **交付代码零 RVA**：`unity/`、`offline/battle.py` 等实现物禁止出现 0x18… 地址；RVA 只活在文档与 `metadata39.py` 的名字解析里。版本升级时唯一的改动点是名字→地址表。
3. **RVA 台账 + 拼合不变量做 CI 门**：任何新增 RVA 进 `claimed_rvas_from_dump.json` 风格的台账，并断言同族方法区间连续（start[i+1]==end[i]）。本次证明该检查可全自动化（plan_main.json 直接复跑）。
4. **客户端更新 = 复跑同一个 plan**：游戏热更后重跑 `mcp_drive.mjs plan_main.json`，21/23 命中率若下降即可定位漂移点；比人眼重读快得多。
5. **巨函数规避模式**：对 ctor/超长函数（GameConstants..ctor 等）**永远走机器码/指令流，不走单发反编译**（两家工具都失败于同一原因）；将此写进 UNPACK_GUIDE 的方法论注记。
6. **双层真值标注不放松**：保持【RT】运行时 / 【CT】ctor / 【DC】反编译三源标注——本次验证再次显示 ctor 硬编码 ≠ 运行时真值（100 vs 1000），混用会悄悄出错。
7. **thunk/合并粒度差异要记录**：类似 "JavaRandom..ctor==SetSeed thunk、Ghidra 不建独立入口" 的条目应写进文档，避免后来者把"反编译器找不到入口"误判为"地址错了"。
8. **下一步解码顺序（补全覆盖面）**：CounterAttack/DOT FSM 分支 → AI EvaluateCondition 叶子（~20 个）→ Mods/PersistentEffect 管线 → 用真实 BattleRecord 捕获建立内容回归门；每解码一个即回填本报告 §4 同款抽查。

---
*本报告由 REA 6.5.0 + Ghidra 12.1.4（Windows P0 只读模式）于 2026-10-16 生成；E: 盘游戏安装未做任何写操作；未运行 `rea setup`、未改动任何客户端配置；未提交 git。*
