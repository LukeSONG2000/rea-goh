# SWGoH 数值公式全书（反编译+运行时真值）

> 来源标注：【RT】= 运行进程内存真值（klass 锚定实例 0x2DCD1A5F000，2026-10-16）
> 【CT】= ctor 机器码立即数 【DC】= Ghidra 反编译方法体（带 RVA）【GD】= gamedata 镜像
> 实现参照：il2cpp/offline/battle.py（Python 逐位验证）、unity BattleSim.cs（C# 移植）

---

## 1. 属性体系

### 1.1 运行时枚举（UnitStatEnum，dump.cs:602463，共 62 项）
战斗管线实际使用：5=SPEED, 8=ARMOR, 9=SUPPRESSION, 10/11=ARMOR/SUPPRESSION_PENETRATION,
14/15=ATTACK/ABILITY_CRIT_RATING, 16=CRIT_DAMAGE, 22/23=CRIT_NEGATE_RATING,
28=CRIT_PERCENT_ADDITIVE, 34=SHIELD_PENETRATION(0..1 比例), 40=CRIT_NEGATE_PERCENT,
61=MASTERY(递归根)。

### 1.2 双枚举映射（重要坑）
gameData.json 用 **proto UnitStat**（2=ATTACK_DAMAGE, 5=SPEED, 17=ACCURACY, 28=MAX_SHIELD…），
运行时枚举错位（6=ATTACK_DAMAGE, 17=CRIT_DMG_NEGATE, 25=MAX_SHIELD）。映射表由
`game/data_units.py::proto_to_runtime()` 从 dump.cs 双枚举自动生成——勿手写。

### 1.3 数值缩放
| 数据源 | 缩放 | 显示值 |
|---|---|---|
| units.json baseStat.unscaledDecimalValue | ÷1e7 | 如速 1,000,000,000→100 |
| statProgression statValueDecimal | ÷1e4 | 71000→7.1 |
| statProgression unscaledDecimalValue | ÷1e8 | 200000000→2.0 |
| gameData.json unitData gearLvl stats | ×1e6 存储 | ÷1e6=显示 |
【GD】权威映射代码在 game/data_units.py。

## 2. 等级与成长
- 角色等级 1-85；装备等级 Gear I-XIII（gearLvl 1-13，每级属性表在 gameData.json unitData）
- 星级 1-7：statProgression.json `stattable_<UNIT>_<N>STAR`（2,772 张表）【GD】
- Relic（遗物）：ProcessSourceDamage 物理分支 ×(relicDelta+1.0)，relicDelta=GetRelicLevelDeltaMod
  （装备管线未解码，占位 0.0）【DC 0x1813658C0】
- 专精 MASTERY(61)：属性聚合时递归 → GetUnitMasteryTable(0x181BE74A0) 查表转换【DC】

## 3. 战斗常数（21 项，全部【RT】运行时真值 2026-10-16）
| 偏移 | 常数 | 真值 | ctor 旧值 |
|---|---|---|---|
| +0x24B4 | UnitSpeedFillMax | **1000.0** | 100.0 |
| +0x2308/+0x22F0 | CritRatingMod(攻/特) | **2400.0** | 100.0 |
| +0x22F4 | ArmorRatingMod | **7.5** | 100.0 |
| +0x24E0 | SuppressionRatingMod | **7.5** | 100.0 |
| +0x2300/+0x22E8 | CritChanceLevelDeltaPerLevel | **0.01** | 0 |
| +0x22FC/+0x22E4 | CritChanceLevelDeltaMin | **0.0**（config 置零） | 0 |
| +0x22F8/+0x22E0 | CritChanceLevelDeltaMax | **0.15** | 1.0 |
| +0x2314 | BaseAttackCritChance | **0.1**（10%） | 0 |
| +0x230C | BaseAbilityCritChance | **0.1** | 0 |
| +0x231C | BaseCritDamage | **1.5** | 0 |
| +0x2348 | DamageModPerLevelDelta | **0.0**（config 置零） | 0 |
| +0x2344/+0x2340 | DamageModLevelDeltaMin/Max | **0.0 / 0.0**（禁用） | 0/1.0 |
| +0x233C | DamageModPerEraLevelDelta | **0.075**（每纪元 +7.5%） | 0 |
| +0x2338/+0x2334 | EraDeltaMin/Max | **-0.75 / +0.75** | 0/1.0 |
| +0x1C34 | ShipMitigationAttackerLevelOffset | **60** | 0 |
另：839/839 个标量字段已全量捕获（catalog/game_constants_runtime.json，
含闪避/格挡/抗性族 +0x2370..+0x2498、回复百分比 +0x2458..+0x24B0）。

## 4. 速度与回合（ProcessUnitsSpeed 0x181CFFB50【DC】）
1. `ticks_i = max(0, SpeedFillMax(1000) − SpeedFill) / max(speed, 0)`，仅速>0/未逃/HP>0 者
2. 候选窗口：`|ticks − ticks_min| < max(max(|ticks|,|ticks_min|)×1e-6, 静态值×8.0)`
3. 优先级表(+0xB8)反向遍历剔除非候选；幸存者 score = SpeedFillWithOverflow + speed×ticks_min
4. 平手：组内最大 speed → 仍平手 `BattleRandom.nextInt(count)` 随机
5. 全员 `SpeedFill += speed×ticks`、`WithOverflow += speed×ticks`；冷却 `−= base×ticks` 钳[0,max]
6. **行动者回合开始双字段清零**（TriggerTurnStarted 0x18193B880）——无溢出结转

## 5. 暴击（ProcessCritChance 0x181365B20 / CritDamage 0x181365F10【DC】）
```
chance = (攻方Rating − 守方NegateRating) / 2400
       + (攻方PctAdd28 − 守方NegatePctAdd40)        # 上限钳 1.0
       + clamp((攻Lv − 守Lv) × 0.01, 0.0, 0.15)
       + BaseCritChance(0.1)                         # 物理=0x2314 / 特殊=0x230C
roll:  BattleRandom.nextInt(100) < chance × 100
critDmg = max(攻方CRIT_DAMAGE(16) + 1.5, 1.0)
```

## 6. 减伤（GetMitigationPercentFromRating 0x180FDFDF0，反汇编恢复）
```
mitigation% = rating / (rating + (攻方等级 + 60) × RatingMod) × 100
RatingMod：物理=ArmorRatingMod 7.5 / 特殊=SuppressionRatingMod 7.5（舰船变体同 7.5）
```
例：护甲 1000、攻 85 级 → 1000/(1000+727.5) = 57.9% 减伤。

## 7. 伤害总管线（DamageModule.ApplyToTargets 0x181364CA0【DC】）
```
dmg = casterPower × effectMult(+0x2C) [+ flat(+0x24)]      # casterPower=前序模块上下文和
    × ProcessSourceDamage: (baseStat×srcMult + srcFlat)
        × 物理: (relicΔ+1.0) × (clamp(LvΔ×0.0, 0, 0)+1)    # 等级差缩放已被 config 禁用
        × 特殊: (clamp(EraΔ×0.075, -0.75, +0.75) + 1.0)
    × (crit ? CritDamage : 1)
    × GetModifyEffectAmount(stat 28)                        # 见 §9
    × (1 − mitigation%)
方差: v = int(dmg × effectVariance(+0x30)); dmg += BattleRandom.nextInt(v×2|1) − v   # ±v 整数均匀
护盾: split = clamp(守方GetStatValue(34), 0, 1);  hpPart = dmg×(1−split), shieldPart = dmg×split
保留: effectFlag(+0xF0 & 1) = 不可致死（HP 钳 1）
```
注意：LevelDiff 伤害缩放当前 config 置零（DamageModPerLevelDelta=0），等级差不放大伤害。

## 8. 随机数系统（JavaRandom，与 JDK 位级一致）
- LCG：`seed = (seed × 0x5DEECE66D + 0xB) & (2⁴⁸−1)`；nextInt(bound) = next31 % bound 带溢出拒绝环
- **五条流同一种子**（BattleStartResponse+0x18，播种点 0x181A5EFB0 闭包）：
  BattleRandom(+0x158) / PlayerAbility(+0x160) / OpponentAbility(+0x168) / PlayerTarget(+0x170) / OpponentTarget(+0x178)
- 概率卷宗使用位置：暴击 nextInt(100)、方差 nextInt(v×2|1)、回合平手 nextInt(count)

## 9. 效果修正聚合（GetModifyEffectAmount 0x181A73980【DC】）
- 按来源键聚合：**正修正取 MAX、负修正取 MIN**（同键最强生效）
- 总量 = (1 + outgoing 和) × (1 + incoming 和)，下限钳 0
- EffectParamEnum：45=INCOMING / 46=OUTGOING

## 10. 属性聚合（GetCalculatedStatValue 0x180FDE5C0【DC】）五段
```
v = base + init
v += Σ平铺加值(GetAdditivesForStat 0x180FE0480)
if stat ≠ MASTERY(61): v += calc(61) × GetMasteryContribution(0x180FDFF40)
p = Σ(GetPercentAdditivesForStat 0x180FE0280)
v = 0 + (1 + p) × v
v += Σ感知型平铺
钳制：HP/攻击/特攻(1,6,7) 下限 1.0（含 inf/nan 防护）；MAX_SHIELD(25)≥0；
     {0-15,17,20-23,46-52} 截断为整数，其余（16,18-19,24-45,53-62）保持小数
```

## 11. AI 决策树（AbilityDecisionTreeProvider 0x1816A2EC0 族【DC】）
默认树 17 节点顺序：UltimatePriority→Priority→Revive→TeamHeal×4→SelfHeal→KillLowHP→
TargetLock→AllyDispel→SelfDispel→AOE→EnemyDispel→KillHighHP→Damage→Default→Taunt→Reinforce→Summon。
节点 schema：type/target/chance(掷骰)/negate/GT-EQ-LT/成败链。行走器 0x1816A30C0。

## 12. 尚未解码 / 占位（诚实清单）
| 项 | 状态 |
|---|---|
| 改装件(Mods)属性管线 | 未解码——statmod JSON 在 gamedata，预计走 §10 修正列表管线 |
| PersistentEffect 完整管线 | 骨架就绪（§9 聚合规则已解码），喂给 §10 的钩子已留 |
| AI 各 EvaluateCondition 模块体（~20 个） | 未逐一反编译（树结构已全解）|
| 原版字体 | 不存在（动态渲染）——永久替代 |
| GetRelicLevelDeltaMod | 占位 0.0（relic 等级差）|
