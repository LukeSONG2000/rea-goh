# 原客户端关卡快照试玩（Captured trial）

在原客户端酒馆运行。当前已验证光明面 1-A、原保存队伍 type 1 和 type 9 的操作、无好友支援。
使用原版资源、技能、战斗状态机和 UI；不提交线上开战/结算请求。
这是已捕获关卡的试玩入口，尚不能直接试玩任意未捕获活动。

从仓库根目录执行，默认手动操作，300 秒后返回酒馆：

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tools/runtime_reuse/start_captured_trial.ps1 -Battle logs/high_reuse/battle-proto-v2-19440.pb -Inventory logs/high_reuse/inventory-v2-19440.pb -SquadType 1
```

- `-Duration 600`：延长本次会话，支持 15–600 秒。加载和退出还需要少量时间。
- `-Auto -Duration 60`：触发一次原基础技能，然后开启原版自动战斗，供回归验证。
- `-Mode headless`：无画面原版模拟，使用捕获种子，独立的 30 秒模拟截止时间。

`SquadType` 是原保存队伍类型，不是队伍列表序号。入口不自行选择替代阵容；
若所选队伍缺失、引用私有库存以外的单位、带未完成隔离审计的全息记录仪
（Datacron）或持久战斗状态，则拒绝进入。

## 数据与运行范围

两个输入必须是带 `.pb.json` 元信息的 direct-protobuf-v2 原始捕获。
准备器检查类型、长度与 SHA256，按原元数据补全省略的 struct 默认值。
它不会刷新账号；玩家配置使用明确指定的库存快照，可能早于当前账号状态。
旧 DeepClone 捕获已丢失语义，拒绝使用。

启动器先生成日志目录中的唯一脚本和清单，再正常停止已识别的日常显示宿主；
原客户端版本 SHA256 与单宿主锁均通过后才附加。正常退出会恢复先前运行的
显示宿主。请保持启动命令运行，使用游戏内操作或等待期限结束，不要直接
关闭终端。若原状态清理未完成，宿主保留连接和锁并报告需要处理，不自动
卸载仍被原游戏引用的私有对象。

证据输出到 `logs/high_reuse/trial-时间戳-evidence.json`，包含原服务调用计数、
终态、回合及退出清理结果。手动会话成功进入并安全返回可为 `valid:true`，
不等于已经打赢；是否打完看 `terminal`。自动和无画面验证则要求终态成立。

有画面试玩可附加 `-TraceAI`（准备器对应 `--trace-ai`），在同一宿主记录
本场原 AI 自然执行的决策节点、结果标签、最终技能和目标。结束证据中的
`original-ai-trace` 包含 `rows/errors/dropped`；最多保留 128 次决策，每次
最多 128 个节点及结果。手动操作、没有发生 AI 决策时允许零记录。
该选项不额外执行决策树或随机调用，不修改优先级，不是出招前预测。
无画面循环尚未捕获到这些回调，故拒绝与 headless 组合。
原树 ID 回调在当前验证中未触发，缺失的 `treeId` 不代表没有使用决策树。
返回的 allyTargets/enemyTargets 是原 AI 返回列表，不应直接解释为最终受击者。

## 敌方控制验证 demo

在上述启动命令附加 `-EnemyBasicDemo`，敌方到达原输入提供器时暂存出招请求，
等待约 2.2 秒，再经原 `CanPerformMove` 检查、`PerformManualMove` 和原完成回调
提交普攻。保留原目标和战斗结算管线，不改队伍归属、伤害、冷却或共享决策树。
这会主动改变敌方技能选择，属于隔离实验，不是原 AI 重放。

已验证捕获 1-A、保存小队类型 8、`-Auto -Duration 90 -EnemyBasicDemo`：
两次敌方 B1 普攻、等待期间回合/FSM不前进、原终局和清理通过。
该自动演示只执行普攻；显式 CLI 还支持下述普通主动特殊技能。
该选项仅支持 visual，日志记录 held/manual-input；显式控制接口见下节。

## 敌方输入 CLI（隔离试玩）

启动时使用 `-EnemyControl`（与 `-EnemyBasicDemo` 互斥）。快照中的
`enemyInput` 给出 `pending/token/actor/turn/expiresAt`。待输入时保存
`{"token":"enemyInput.token 的实际值"}` 到请求 JSON，然后调用：

```powershell
node tools/runtime_reuse/runtime_cli.mjs enemy-basic --pid PID --request logs/high_reuse/enemy-request.json
node tools/runtime_reuse/runtime_cli.mjs enemy-resume --pid PID --request logs/high_reuse/enemy-request.json
node tools/runtime_reuse/runtime_cli.mjs enemy-target --pid PID --request logs/high_reuse/enemy-target.json
node tools/runtime_reuse/runtime_cli.mjs enemy-ability --pid PID --request logs/high_reuse/enemy-ability.json
```

`enemy-basic` 执行原普攻，`enemy-resume` 恢复原 AI。出招 token 只消费一次。
`enemy-target` 请求为 `{"token":"当前token","targetId":"enemyInput.targets 中的 id"}`，
调用原 `CalculateAndSetCurrentTarget(null, preferredTarget)` 并核对实际目标；
重复选择当前目标是 no-op。选择目标不消费出招 token，也不延长等待时间。
候选列表来自原 `GetValidTargets(null)`，不是任意角色 ID。选目标后执行普攻，
回执中的 `actualTarget` 取自原动作启动后的 `EnemyTarget`。恢复 AI 时原 AI
仍可重新选目标，不能把手动锁定视作 AI 最终目标的保证。
10 秒没有收到输入会自动恢复原 AI；过期普攻拒绝。只在待输入期间排除该私有
战斗的防卡死计时，其他战斗和技能执行中的原检测保持正常。该等待不会暂停
整个客户端或比赛倒计时。试玩外命令拒绝，未启用控制的试玩也拒绝。

已验证真实 CLI 普攻、重复 token 拒绝、下一敌方恢复 AI，以及两次无人输入
超时自动恢复。

`enemy-ability` 请求为 `{"token":"当前token","moveType":1,"index":0}`。
必须从当前 `enemyInput.abilities` 读取 moveType/index，不按技能显示顺序猜测；
moveType 0 是普攻，1 是原特殊技能列表。该列表同时给出原技能 ID、类型、
冷却、ready、canPerform 和 supported。只有 supported/ready/canPerform 全为
true 才可提交；执行前再次调用原可用性检查。技能类型须为 ACTIVATED，且
原 AllyTargetingRuleId 为空。需要友军选择的技能和终极技能仍待接通。
动作调用原 PerformManualMove 和原输入完成回调，效果、伤害及冷却由原引擎结算。
验证使用私有库存凯勒兰对维达自定义对战，不冒充捕获关卡原敌方阵容：
维达 specialability_vader02 产生原持续伤害/减速状态，下一敌方回合冷却为 4，
再次提交被原可用性检查拒绝，然后可正常恢复原 AI。

敌方目标切换已在原私有库存组成的凯勒兰＋农场卢克诊断编队中双跑验证：
目标由卢克变为凯勒兰，原普攻 `EnemyTarget` 同步变化，终局与清理通过。
这是明确的自定义测试编队，不代表原保存队伍不变；嘲讽/隐身组合尚未单独验收。

## AI 操作接口

有画面试玩运行期间，在另一个终端使用现有 CLI，`PID` 替换为原游戏进程号：

```powershell
node tools/runtime_reuse/runtime_cli.mjs snapshot --pid PID
node tools/runtime_reuse/runtime_cli.mjs battle-target --pid PID --request logs/high_reuse/target.json
node tools/runtime_reuse/runtime_cli.mjs battle-ability --pid PID --request logs/high_reuse/ability.json
node tools/runtime_reuse/runtime_cli.mjs battle-unit --pid PID --request logs/high_reuse/unit.json
node tools/runtime_reuse/runtime_cli.mjs battle-cancel --pid PID --request logs/high_reuse/cancel.json
```

`snapshot` 返回 `trial:true`、宿主进程号、实时属性和 `input`。每次操作前读取
最新 `input.token`；释放技能请求为 `{"token":"当前token","index":0}`，
选择目标/友军为 `{"token":"当前token","targetId":"候选id"}`，取消为
`{"token":"当前token"}`。目标来自 `input.targets`，技能来自 `input.abilities`，
等待选友军时使用 `input.selectionCandidates`。不要盲目重试旧 token。

命令发送到试玩的同一个宿主，不新建 Frida 连接；执行前校验准确的原战斗状态
实例与 context，再调用共用的原 UI 输入桥。试玩中拒绝收藏/战役等导航。
试玩结束后 CLI 自动恢复日常显示宿主路由。若命令恰逢宿主切换，其 owner
校验会拒绝执行，应重新读取快照后再决定操作。

仍待完成：新鲜快照采集入口、好友支援、非空 Datacron、Era 借用单位、其他
活动/有限次数模式、终极技能独立 UI、敌方友军选择和预测。当前截图和确定性验证
不代表这些范围已通过。
