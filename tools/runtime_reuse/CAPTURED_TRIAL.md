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
`original-ai-trace` 包含 `rows/trees/errors/dropped`；最多保留 128 次决策，每次
最多 128 个节点及结果。手动操作、没有发生 AI 决策时允许零记录。
该选项不额外执行决策树或随机调用，不修改优先级，不是出招前预测。
无画面循环尚未捕获到这些回调，故拒绝与 headless 组合。
当前二进制直接从 context 的 GameDataDomainController 读取 `_abilityDecisionTreeDO`，
并不调用取树方法。记录器读取同一字段，保存完整成功/失败分支和结果标签；
`rows[].treeGraph` 对应 `trees[]`，`rows[].nodes[].nodeIndex` 对应树的节点索引。
节点索引是该图内编号，不是优先级数值；原分支顺序才决定实际求值路径。
图最多 512 节点，每节点最多 64 参数，超界记录错误而不伪装完整结果。
目前实测 `default` 树 20 个节点。图读取不调用条件求值器，也不改共享树。
返回的 allyTargets/enemyTargets 是原 AI 返回列表，不应直接解释为最终受击者。

## 隔离试玩中的敌方 AI 优先级

`-AiPriorityOrder logs/high_reuse/ai-priority.json` 接受一个 JSON 数组，数组必须
是原树全部节点索引的排列；例如已验证的 20 节点默认树，把 damage 节点 17
提前：`[17,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,18,19]`。
索引应从没有改序的 `-TraceAI` 记录读取，不把这个示例套到未知树上。
准备器参数为 `--ai-priority-order <JSON路径>`，自动开启 TraceAI，当前只支持 visual。

实现复制原节点、条件参数、成功结果和最终回退，在私有副本重新连接失败分支。
只接受完整的线性失败链、成功叶节点和中性继续分支；其他图结构明确拒绝。
原求值器进入该试玩敌方根节点时使用副本；我方和其他战斗不替换，GameData
共享树不写入。技能可用性、目标筛选和伤害仍由原引擎处理。

结束证据 `original-ai-priority` 给出 order/originalOrder、实际替换次数、私有图、
错误及 `originalUnchanged` 检查。原顺序副本须与基线决策一致；改序属于明确的
模拟策略变更，不是原 AI 行为的忠实重放，也不等于出招前预测。

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
node tools/runtime_reuse/runtime_cli.mjs enemy-unit --pid PID --request logs/high_reuse/enemy-unit.json
node tools/runtime_reuse/runtime_cli.mjs enemy-cancel --pid PID --request logs/high_reuse/enemy-request.json
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
true 才可提交；执行前再次调用原可用性检查。支持 ACTIVATED 和具有原友军
目标规则的 ACTIVATED_ALLY_SELECT；终极技能独立 UI 仍待接通。
动作调用原 PerformManualMove 和原输入完成回调，效果、伤害及冷却由原引擎结算。

当 `requiresAllySelection=true`，`enemy-ability`（或该角色的 `enemy-basic`）
先调用原 SelectAlly，回执 `selectionStarted=true` 表示等待选择，尚未释放技能。
用回执的新 token，或重新读快照中的 `enemyInput.token`，发送
`enemy-unit`：`{"token":"新token","targetId":"selectionCandidates 中的 id"}`。
这会调用原 HandleUnitTapped，核对原 AllyTarget，再执行之前选择的技能。
`enemy-cancel` 只需要 token，调用原 CancelSelectUnit 后返回新 token，可重新选技能。
选择/取消不延长最初的 10 秒期限；超时先清理选择，再交还原 AI。
原候选为空时回执为 `selectionEmpty=true`，没有释放技能；应重新读取状态。
`ok=true` 表示请求对应阶段已确认，只有 `started=true` 才表示动作已经启动。
待选友军期间禁止改选技能或敌方目标；使用 enemy 命令，不能通过 battle-unit 绕过。
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
活动/有限次数模式、终极技能独立 UI、敌方预测和决策树编辑。当前截图和确定性验证
不代表这些范围已通过。

## 收藏与详情 CLI

日常显示宿主运行时，可通过同一宿主操作原收藏页面：

```powershell
node tools/runtime_reuse/runtime_cli.mjs inventory-characters --pid PID
node tools/runtime_reuse/runtime_cli.mjs inventory-list --pid PID
node tools/runtime_reuse/runtime_cli.mjs inventory-unit --pid PID --request logs/high_reuse/unit-detail.json
node tools/runtime_reuse/runtime_cli.mjs snapshot --pid PID
node tools/runtime_reuse/runtime_cli.mjs inventory-back --pid PID
node tools/runtime_reuse/runtime_cli.mjs inventory-home --pid PID
```

`unit-detail.json` 为 `{"unitId":"inventory-list 返回的 unitId"}`。飞船先从主页执行
`inventory-ships`，其余操作相同。列表只在显式请求时读取，采用当前原生筛选列表并与
当前玩家库存 ID 交叉校验；不把未拥有单位的预览对象当作库存。`loaned` 来自原
`IsLoanedUnit`。等级、星级和 `tier` 保留原数值／枚举（例如 `tier:12` 是原枚举值，
不要直接当作 UI 装备等级文字）。详情快照位于 `snapshot.detail`。

进入收藏使用原 `EnterState(..., asyncEntry:true)` 准备流程。确认成功要求收藏子视图
已激活，而不只是状态名称变化；冷启动时默认预设页签尚未加载，跳过该流程会出现空白页。
打开详情调用原角色／飞船 tile 回调；返回调用原详情 Back 回调。

当前接口未接入升级、装备变更、商店购买、扫荡或奖励领取；不是这些目标已经完成。
试玩持有运行时期间继续拒绝收藏导航。购买与装备拆卸／分解须另外获得明确授权。

详情快照新增 `detail.skills`：原技能 ID、能力引用 ID、真实能力名称 key、能力类型、
定义冷却、当前保存 tier、定义 tier 数量、解锁要求及船员来源。技能定义本身的名称
可能是通用占位，单独放在 `skillDefinitionNameKey`；展示名称应使用 `abilityNameKey`。
`currentTier` 是原 `UnitDO.GetSkillTier` 返回值；缺席技能返回 `present:false/currentTier:null`。
它不是 UI 显示等级。原 UI 的显示计算还包含 `RequiredSkillTier` 偏移，不能直接加一。
`definitionIsZeta` 是定义标志，不代表当前已经购买 Zeta；`baseCooldown` 是能力定义值，
不是战斗中经过升级和效果修正后的实际剩余 CD。读数失败明确返回 `skillsError`，不填假值。
`detail.skillDisplay`（目前角色详情）读取当前原 UI 控制器的数据：技能/能力 ID、本地化名称与 levelText、内部 currentTier、requiredSkillTier、原 getter 的 displayLevel、maxed 和 locked。不要以 currentTier+1 替代 displayLevel；维达实测内部6对应界面8。船员技能显示仍待接入。数据未初始化返回 null，读取失败返回 skillDisplayError，不影响 detail.skills。
飞船详情现也提供 `detail.skillDisplay`：`container` 区分 ship、advanced、crew；船员容器以零起始 `crewSlot` 标记，非船员为 null。角色为 character。全部 displayLevel 仍调用原 UI getter；实测 TIEADVANCED 为 8/8/3/8。

### Original skill detail navigation

With an original character/ship detail page open, save a request JSON containing
`unitId` from `snapshot.detail.unitId` and `skillId` from `detail.skillDisplay`.
Run `node tools/runtime_reuse/runtime_cli.mjs inventory-skill --pid <PID> --request <request.json>`.
The original click handler chooses its normal single-level or multi-level popup.
`detail.skillPopup` reports the active original popup; multi-level popups expose
their original skill/ability IDs. The receipt confirms the observed popup rather
than merely confirming a callback invocation.

Use `inventory-skill-close` with a JSON containing the same current `unitId` to
invoke the original popup Back callback. Closing is confirmed only after the
popup disappears. Changed units, unknown/ambiguous skills and already-open
popups are rejected. These commands do not upgrade, unlock or purchase skills;
trial mode does not expose inventory navigation. Live verification covers Vader
basic and Ghost crew skill multi-level popups; single-level popup behavior is
implemented but not yet live-validated.
多级弹窗 `detail.skillPopup.levels` 和 `selectedLevel` 提供原等级文本、描述、冷却、条件锁定、upgradeStatus/upgradeAvailable及货币费用枚举。`tier`仍为内部值；已获得等级的费用只是UI配方值，不代表下一次可购买报价。材料列表尚未解码，所有等级明确返回 `costComplete:false`，不能据此批准完整消费。
等级对象新增 `materials`：原 itemType/renderType/id/label/quantityRaw。RecipeData 是值类型 KeyValuePair 列表，使用原 get_Item 装箱读取，不能当对象指针数组。维达普攻各等级 quantityRaw 与原 label 的需求分母吻合；尚未独立核对库存持有数，继续保留 costComplete:false。
