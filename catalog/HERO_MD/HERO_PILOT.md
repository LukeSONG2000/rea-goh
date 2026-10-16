# SWGoH 单位手册 · 试点卷（5 单位）

> 试点卷：DARTHVADER→`VADER`、R2D2→`R2D2_LEGENDARY`（恢复版 def_id 实名；DARTHVADER/R2D2 在恢复数据中不存在）。含完整模板示例（LORDVADER）。


## 体例（每卷通用）

- 术语约定：**English（中文）** 随文标注；所有公式出处为 `catalog/NUMBERS.md`（§5 暴击 / §6 减伤 / §7 伤害总管线 / §9 增减益聚合 / §13 抵抗）。
- **数据级机制**：来自 skill→ability→effect 链（game/abilities.py 同款解析），每个效果的 类型/倍率/几率/目标/方差 均为 gamedata 真值。
- **代码级机制（数据中不可见，反编译参照）**：
  - Keep-alive flag（不可致死）：运行时 effectFlag `+0xF0 & 1` → 伤害后 HP 钳 1，见反编译 `damage_apply_to_targets.c`；effect.json 中无此字段，只能代码级标注。
  - auto-battle AI 优先级：AbilityDecisionTreeProvider 0x1816A2EC0 族，默认树 17 节点（NUMBERS.md §11）；技能在自动战斗中的释放倾向由此树决定，数据级仅有冷却/几率可参照。
  - 暴击 roll / 方差 roll / 回合平手 全部走 BattleRandom（JavaRandom 同种子五流，§8）。
- 抵抗规则（§13，对所有施加类效果默认生效，除非标注 不可抵抗/UNRESISTABLE）：
  `resistChance = (施加方 Potency − 目标 Tenacity) + clamp(等级差×0.05, 0, 0.30) + 0.15`，roll `nextInt(100) < resistChance×100` → 被抵抗。
- 基础数值缩放：effect 小数 ÷10000（17450→1.745）；单位属性 ÷1e8；星级成长表 `statProgression stattable_<UNIT>_<N>STAR`（§2）。
- 2107 条 StreamingAssets/abilities.json 行 = 301 个唯一单位 × 稀有度变体（技能链完全一致），本手册按唯一单位合并。


---

## 达斯·维达（Darth Vader） · `VADER`
<div class="chips"><span class="chip align-黑暗">阵营 黑暗</span> <span class="chip">角色 · 7星</span> <span class="chip">定位:攻击者</span> <span class="chip">定位:领袖</span> <span class="chip">种族:人类</span> <span class="chip">帝国</span> <span class="chip">职业:西斯</span> <span class="chip">自身标签:vader</span> <span class="chip">可获取</span> <span class="chip">territory_hoth_empire</span> <span class="chip">territory_dark_platoon</span> <span class="chip">specialmission_empire</span> <span class="chip">西斯</span> <span class="chip">gac_tiepilot</span> <span class="chip">gac_trench</span> <span class="chip">gac_execbh</span></div>

> 可怕的攻击者，拥有持续群体伤害能力，并能在绝杀受减益效果影响的敌军目标后获得额外回合

**基础数值**（数据缩放：unscaledDecimalValue ÷ 1e8，proto→runtime 双枚举映射由 game/data_units.py 派生；低值取该属性首次出现的 Gear 等级）

| 属性 Stat | 基础（首次出现档） | 满配 (Gear 13) |
|---|---|---|
| 生命 Health | 170 | 5,120 |
| 攻击 Attack Damage | 18 | 1,345 |
| 特攻 Ability Power | 43（Gear 4 起） | 329 |
| 速度 Speed | 100 | 141 |
| 护甲 Armor | 2（Gear 2 起） | 231 |
| 压制 Suppression | 2（Gear 2 起） | 241 |
| 防护 Protection | 4,234（Gear 7 起） | 47,828 |
| 命中 Potency | 2%（Gear 3 起） | 28% |
| 抵抗 Tenacity | 3%（Gear 5 起） | 50% |

### 基本技 · 恐吓挥砍
`basicability_darthvader` `冷却 0` `技能等级 7`

**冷却 0 · 目标 敌方·选中目标 · 伤害类型 Physical（物理）**

> 对目标敌人造成实体伤害，并有 45% 的机会施加能力阻碍，持续1回合。 绝地武士和义军目标无法逃脱或抵抗此攻击的效果。

**伤害 = 6.482 × 攻击（Physical 物理，受目标护甲 Armor 减免 §6）** ±5% 方差

按技能等级：4.86 / 4.86 / 5.092 / 5.092 / 5.324 / 5.786 / 6.482

**效果链（tier[-1]，4 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Damage（伤害） | 3.241× | — | 100% | 敌方·选中目标 | ±5% | 物理 |
| 2 | Curse（诅咒） | — | — | 100% | 敌方·选中目标 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 3 | Damage（伤害） | 3.241× | — | 100% | 敌方·选中目标 | ±5% | 物理 |
| 4 | Curse（诅咒） | — | — | 80% | 敌方·选中目标 | — | 有过期条件 |

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
dmg_Physical = 6.482 × ATTACK_DAMAGE(6)
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (relicΔ+1.0)（等级差缩放已禁用）
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × (1 − rating/(rating+(攻Lv+60)×ArmorRatingMod 7.5))   §6
方差: v=int(dmg×0.05); dmg += nextInt(v×2|1) − v
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
</pre>

</details>

### 特殊技 · 原力碾压
`specialability_vader02` `冷却 5` `技能等级 7`

**冷却 5 · 目标 敌方·全体 · 伤害类型 Physical（物理）**

> 对所有敌人造成物理伤害并施加2次2回合的持续伤害效果。此攻击无法被反击。

**伤害 = 1.102 × 攻击（Physical 物理，受目标护甲 Armor 减免 §6）** ±5% 方差

按技能等级：0.874 / 0.912 / 0.988 / 0.988 / 1.026 / 1.102 / 1.102

**效果链（tier[-1]，5 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Damage（伤害） | 1.102× | — | 100% | 敌方·全体 | ±5% | 物理 |
| 2 | Apply Effect（施加效果） | — | — | 100% | 敌方·全体 | — | 引用:Damage（伤害）; 引用:Damage（伤害）; 引用:Damage（伤害）; 有过期条件 |
| 3 | Apply Effect（施加效果） | — | — | 100% | 敌方·全体 | — | 引用:Damage（伤害）; 引用:Damage（伤害）; 引用:Damage（伤害）; 有过期条件 |
| 4 | Apply Effect（施加效果） | — | — | 100% | 敌方·全体 | — | 引用:Damage（伤害）; 引用:Damage（伤害）; 引用:Damage（伤害）; 有过期条件 |
| 5 | Modify Stat（属性修正） | -0.25× | — | 100% | 敌方·全体 | — | 有过期条件 |

**特殊机制标注（数据级）**：施加 Buff/Debuff（生效走 §13 抵抗判定：基础 15%+等级差×5%[0,30%]+Potency−Tenacity）

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
dmg_Physical = 1.102 × ATTACK_DAMAGE(6)
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (relicΔ+1.0)（等级差缩放已禁用）
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × (1 − rating/(rating+(攻Lv+60)×ArmorRatingMod 7.5))   §6
方差: v=int(dmg×0.05); dmg += nextInt(v×2|1) − v
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
</pre>

</details>

### 特殊技 · 淘汰之刃
`specialability_vader01` `冷却 4` `技能等级 7`

**冷却 4 · 目标 友方·自身 · 伤害类型 Physical（物理）**

> 对目标敌人造成物理伤害，驱散他们身上的所有减益状态。 每个消除的减益状态可造成额外30%伤害，造成绝命打击时可获得100%回合条。

**伤害 = 4.064 × 攻击（Physical 物理，受目标护甲 Armor 减免 §6）** ±5% 方差

按技能等级：1.734 / 1.806 / 1.806 / 1.881 / 1.956 / 2.032 / 4.064

**效果链（tier[-1]，6 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Recover（治疗）; 有过期条件 |
| 2 | Query Count（计数查询） | — | — | 100% | 敌方·选中目标 | — | — |
| 3 | Dispel（驱散） | — | 100 | 100% | 敌方·选中目标 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 4 | Modify Stat（属性修正） | — | 0.25 | 100% | 友方·自身 | — | 有过期条件 |
| 5 | Damage（伤害） | 2.032× | — | 100% | 敌方·选中目标 | ±5% | 物理 |
| 6 | Damage（伤害） | 2.032× | — | 100% | 敌方·选中目标 | ±5% | 物理 |

**特殊机制标注（数据级）**：施加 Buff/Debuff（生效走 §13 抵抗判定：基础 15%+等级差×5%[0,30%]+Potency−Tenacity）、驱散 Dispel

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
dmg_Physical = 4.064 × ATTACK_DAMAGE(6)
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (relicΔ+1.0)（等级差缩放已禁用）
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × (1 − rating/(rating+(攻Lv+60)×ArmorRatingMod 7.5))   §6
方差: v=int(dmg×0.05); dmg += nextInt(v×2|1) − v
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
</pre>

</details>

### 领袖技(被动) · 恐吓激励
`leaderability_vader_BASE` `被动 aura (abilityType 8)` `技能等级 7` `zeta`

> 帝国和西斯友军获得15%进攻力。

**效果链（tier[-1]，5 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Modify Stat（属性修正） | 0.3× | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 2 | Modify Stat（属性修正） | 0.3× | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 3 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Recover（治疗） |
| 4 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Apply Effect（施加效果） |
| 5 | Apply Effect（施加效果） | — | — | 100% | 敌方·全体(含阵亡) | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Query Count（计数查询）; 引用:Apply Effect（施加效果）; 引用:Blackboard Modify（黑板修改） |

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>

### 独特技(被动) · 无路可逃
`uniqueability_vader01` `被动 aura (abilityType 8)` `技能等级 7` `zeta`

> 每逢遭遇战开始，战场上每有一名帝国友军、西斯友军、绝地敌人或义军敌人单位，达斯·维达都将获得2点速度直到遭遇战结束。

**效果链（tier[-1]，5 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Immunity（免疫） | — | — | 100% | 友方·自身(含阵亡) | — | 不可抵抗(跳过 §13 抵抗判定) |
| 2 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Query Count（计数查询）; 引用:Modify Stat（属性修正）; 引用:Query Count（计数查询）; 引用:Modify Stat（属性修正）; 引用:Query Count（计数查询）; 引用:Modify Stat（属性修正）; 引用:Query Count（计数查询）; 引用:Modify Stat（属性修正） |
| 3 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Apply Effect（施加效果） |
| 4 | Immunity（免疫） | — | — | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定); 标记:隐身（merciless_stealth_dispel_marker） |
| 5 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Query Count（计数查询）; 引用:Recover（治疗）; 引用:Dispel（驱散）; 引用:Dispel（驱散）; 引用:Dispel（驱散）; 引用:Dispel（驱散）; 引用:Dispel（驱散） |

**特殊机制标注（数据级）**：免疫 Immunity

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>

### 特殊技 · 冷血屠杀
`specialability_vader03` `冷却 9` `技能等级 7` `zeta`

**冷却 9 · 目标 友方·自身**

> 得到1回合的冷血[ffff33]冷血：+25%进攻力（不与进攻提升叠加），+10%暴击率，+25%暴击伤害

**效果链（tier[-1]，15 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Modify Stat（属性修正） | 0.5× | — | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 2 | Modify Stat（属性修正） | 0.5× | — | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 3 | Modify Stat（属性修正） | — | 0.5 | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 4 | Modify Stat（属性修正） | — | 0.5 | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 5 | Modify Stat（属性修正） | — | 0.5 | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 6 | Query Count（计数查询） | — | — | 100% | 友方·自身 | — | — |
| 7 | Dispel（驱散） | — | 100 | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 8 | Immunity（免疫） | — | — | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 标记:嘲讽（visible_taunt_ignore_effect）; 有过期条件 |
| 9 | Immunity（免疫） | — | — | 100% | 友方·自身(含阵亡) | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 10 | Immunity（免疫） | — | — | 100% | 友方·自身(含阵亡) | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 11 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 标记:嘲讽（specialability_vader03_taunt_handler）; 引用:Modify Stat（属性修正） |
| 12 | Immunity（免疫） | — | — | 100% | 友方·自身 | — | 有过期条件 |
| 13 | Immunity（免疫） | — | — | 100% | 友方·自身 | — | 有过期条件 |
| 14 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Dispel（驱散） |
| 15 | Recover（治疗） | 1× | — | 100% | 友方·自身 | — | — |

**特殊机制标注（数据级）**：免疫 Immunity、施加 Buff/Debuff（生效走 §13 抵抗判定：基础 15%+等级差×5%[0,30%]+Potency−Tenacity）、驱散 Dispel

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>

---

## 维达大人（Lord Vader） · `LORDVADER`
<div class="chips"><span class="chip align-黑暗">阵营 黑暗</span> <span class="chip">角色 · 7星</span> <span class="chip">定位:攻击者</span> <span class="chip">定位:领袖</span> <span class="chip">星际传奇 Galactic Legend</span> <span class="chip">种族:人类</span> <span class="chip">帝国</span> <span class="chip">职业:西斯</span> <span class="chip">巨伤减免</span> <span class="chip">可获取</span> <span class="chip">自身标签:lordvader</span> <span class="chip">gac_thechosenone</span> <span class="chip">conq_hokey_religion</span> <span class="chip">territory_tb3_darkside</span> <span class="chip">territory_tb3_hero</span></div>

> 传奇西斯尊主，可利用其他人的行动增强自身潜能，实力不容低估

**基础数值**（数据缩放：unscaledDecimalValue ÷ 1e8，proto→runtime 双枚举映射由 game/data_units.py 派生；低值取该属性首次出现的 Gear 等级）

| 属性 Stat | 基础（首次出现档） | 满配 (Gear 13) |
|---|---|---|
| 生命 Health | 30,000 | 43,400 |
| 攻击 Attack Damage | 1,000 | 2,935 |
| 特攻 Ability Power | 50（Gear 13 起） | 50 |
| 速度 Speed | 315 | 390 |
| 护甲 Armor | 5（Gear 5 起） | 85 |
| 压制 Suppression | 46（Gear 11 起） | 46 |
| 防护 Protection | 8,621（Gear 7 起） | 107,765 |
| 命中 Potency | 20% | 59% |
| 抵抗 Tenacity | 6%（Gear 4 起） | 69% |

### 基本技 · 复仇风暴
`basicability_lordvader` `冷却 0` `技能等级 2` `zeta`

**冷却 0 · 目标 敌方·选中目标 · 伤害类型 Physical（物理）**

> 对敌方目标造成物理伤害。
> 
> 共和国余烬：再次造成伤害

**伤害 = 3.75 × 攻击（Physical 物理，受目标护甲 Armor 减免 §6）** ±5% 方差

按技能等级：2.5 / 3.75

**效果链（tier[-1]，7 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Damage（伤害） | 1.25× | — | 100% | 敌方·选中目标 | ±5% | 物理 |
| 2 | Blackboard Query（黑板查询） | — | 30 | 100% | 友方·自身(含阵亡) | — | — |
| 3 | Modify Stat（属性修正） | — | 0.25 | 100% | 友方·自身 | — | 有过期条件 |
| 4 | Damage（伤害） | 1.25× | — | 100% | 敌方·选中目标 | ±5% | 物理 |
| 5 | Query Count（计数查询） | — | — | 100% | 友方·自身 | — | — |
| 6 | Modify Stat（属性修正） | — | 0.25 | 100% | 友方·自身 | — | 有过期条件 |
| 7 | Damage（伤害） | 1.25× | — | 100% | 敌方·选中目标 | ±5% | 物理 |

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
dmg_Physical = 3.75 × ATTACK_DAMAGE(6)
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (relicΔ+1.0)（等级差缩放已禁用）
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × (1 − rating/(rating+(攻Lv+60)×ArmorRatingMod 7.5))   §6
方差: v=int(dmg×0.05); dmg += nextInt(v×2|1) − v
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
</pre>

</details>

### 特殊技 · 黑暗先驱
`specialability_lordvader01` `冷却 3` `技能等级 2` `zeta`

**冷却 3 · 目标 敌方·选中目标 · 伤害类型 Physical（物理）+Unmitigable（真实伤害(无视减伤)）**

> 对敌方目标施加2回合的增益免疫和治疗免疫，并造成物理伤害。每叠加一层“被低估”状态，维达大人获得1%终极充能。
> 
> 共和国余烬：此攻击还会造成纯伤害

**伤害 = 1.85 × 攻击（Unmitigable 真实伤害(无视减伤)，无视减伤） + 1.55 × 攻击（Physical 物理，受目标护甲 Armor 减免 §6）** ±5% 方差

**效果链（tier[-1]，12 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Immunity（免疫） | — | — | 100% | 敌方·选中目标 | — | 标记:免疫（immunity_debuff）; 标记:免疫（featcounter_immunity_debuff）; 有过期条件 |
| 2 | Immunity（免疫） | — | — | 100% | 敌方·选中目标 | — | 标记:免疫（health_recovery_immunity）; 标记:免疫（featcounter_health_recovery_immunity）; 有过期条件 |
| 3 | Blackboard Query（黑板查询） | — | — | 100% | 友方·自身 | — | — |
| 4 | Modify Stat（属性修正） | — | — | 100% | 友方·自身 | — | 有过期条件 |
| 5 | Damage（伤害） | 1.55× | — | 100% | 敌方·选中目标 | ±5% | 物理 |
| 6 | Blackboard Query（黑板查询） | — | — | 100% | 友方·全体(含阵亡) | — | — |
| 7 | Recover（治疗） | — | — | 100% | 友方·全体 | — | — |
| 8 | Modify Ultimate Charge（终极技充能） | — | — | 100% | 友方·自身 | — | — |
| 9 | Query Count（计数查询） | — | — | 100% | 友方·自身 | — | — |
| 10 | Damage（伤害） | 1.85× | — | 100% | 敌方·选中目标 | — | 真实伤害(无视减伤) |
| 11 | Blackboard Query（黑板查询） | — | 5 | 100% | 友方·自身(含阵亡) | — | — |
| 12 | Blackboard Modify（黑板修改） | — | 1 | 100% | 友方·自身 | — | — |

**特殊机制标注（数据级）**：免疫 Immunity、终极充能

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
dmg_Unmitigable = 1.85 × —
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × 1（Unmitigable 无视减伤）
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
dmg_Physical = 1.55 × ATTACK_DAMAGE(6)
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (relicΔ+1.0)（等级差缩放已禁用）
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × (1 − rating/(rating+(攻Lv+60)×ArmorRatingMod 7.5))   §6
方差: v=int(dmg×0.05); dmg += nextInt(v×2|1) − v
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
</pre>

</details>

### 特殊技 · 不羁情感
`specialability_lordvader02` `冷却 5` `技能等级 2` `zeta`

**冷却 5 · 目标 敌方·全体 · 伤害类型 Physical（物理）**

> 对所有敌人造成物理伤害并施加2回合的某持续伤害效果，此效果无法抵抗。每叠加一层“被低估”状态，所有黑暗面友军获得1%精通（可叠加）直到遭遇战结束。
> 
> 共和国余烬：施加1回合的技能封锁，此效果无法驱散或抵抗。将敌方目标的冷却时间增加2

**伤害 = 1 × 攻击（Physical 物理，受目标护甲 Armor 减免 §6）** ±5% 方差

**治疗 = 2 × 特攻**（Recover 治疗，§7 管线，无减伤项）

**效果链（tier[-1]，13 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Immunity（免疫） | — | — | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 标记:免疫（speed_recovery_immunity）; 有过期条件 |
| 2 | Modify Stat（属性修正） | — | -9999 | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 3 | Blackboard Query（黑板查询） | — | — | 100% | 友方·全体(含阵亡) | — | — |
| 4 | Modify Stat（属性修正） | — | — | 100% | 友方·全体 | — | 有过期条件 |
| 5 | Modify Stat（属性修正） | — | — | 100% | 友方·自身 | — | 有过期条件 |
| 6 | Query Count（计数查询） | — | — | 100% | 友方·自身 | — | — |
| 7 | Curse（诅咒） | — | — | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 8 | Recover（治疗） | — | 2（治疗量×特攻） | 100% | 敌方·选中目标 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 9 | Damage（伤害） | 1× | — | 100% | 敌方·全体 | ±5% | 物理 |
| 10 | Apply Effect（施加效果） | — | — | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Damage（伤害）; 引用:Damage（伤害）; 引用:Damage（伤害）; 有过期条件 |
| 11 | Apply Effect（施加效果） | — | — | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Damage（伤害）; 引用:Damage（伤害）; 引用:Damage（伤害）; 有过期条件 |
| 12 | Apply Effect（施加效果） | — | — | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Damage（伤害）; 引用:Damage（伤害）; 引用:Damage（伤害）; 有过期条件 |
| 13 | Apply Effect（施加效果） | — | — | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Damage（伤害）; 引用:Damage（伤害）; 引用:Damage（伤害）; 有过期条件 |

**特殊机制标注（数据级）**：免疫 Immunity、施加 Buff/Debuff（生效走 §13 抵抗判定：基础 15%+等级差×5%[0,30%]+Potency−Tenacity）

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
dmg_Physical = 1 × ATTACK_DAMAGE(6)
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (relicΔ+1.0)（等级差缩放已禁用）
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × (1 − rating/(rating+(攻Lv+60)×ArmorRatingMod 7.5))   §6
方差: v=int(dmg×0.05); dmg += nextInt(v×2|1) − v
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
</pre>

</details>

### 领袖技(被动) · 我的新帝国
`leaderability_lordvader` `被动 aura (abilityType 8)` `技能等级 2` `zeta`

> 黑暗面友军+20速度，+20%精通，+15%最大生命值，帝国或黑暗面无阵营原力使用者友军效果加倍。
> 
> 战斗开始时，其他帝国和黑暗面无阵营原力使用者友军失去所有防护力并获得等量最大生命值。如果战斗开始时所有友军都是黑暗面，帝国和黑暗面无阵营原力使用者友军将对恐惧免疫，帝国肉盾获得2回合的嘲讽，帝国友军失去嘲讽时，他们将被施加1回合的被标记状态，此效果无法驱散、阻止或抵抗。
> 
> 每当一名遭受减益的黑暗面友军受到伤害时，维达大人可得到2%终极充能，如果这名友军是无阵营原力使用者，则增至3%。
> 
> 帝国和黑暗面无阵营原力使用者友军在锁定银河共和国敌人时+35%暴击率和暴击伤害。
> 
> 当帝国和黑暗面无阵营原力使用者友军达到一定生命值门槛时，他们将得到不同的助益。
> 
> - 生命值高于35%时：[ffff33]生命值高于50%时：[ffff33]生命值低于80%时：不会被暴击

**效果链（tier[-1]，36 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Immunity（免疫） | — | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 2 | Query Count（计数查询） | — | — | 100% | 友方·全体 | — | — |
| 3 | Query Count（计数查询） | — | — | 100% | 友方·全体 | — | — |
| 4 | Immunity（免疫） | — | — | 100% | 友方·全体 | — | — |
| 5 | Query Count（计数查询） | — | — | 100% | 友方·全体 | — | — |
| 6 | Immunity（免疫） | — | — | 100% | 友方·全体 | — | 标记:恐惧（lordvader_fear_immunity）; 有过期条件 |
| 7 | Modify Stat（属性修正） | — | 1 | 100% | 友方·全体 | — | 标记:嘲讽（taunt_buff）; 标记:嘲讽（featcounter_taunt_buff）; 有过期条件 |
| 8 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Modify Stat（属性修正） |
| 9 | Modify Stat（属性修正） | 0.2× | — | 100% | 友方·全体(含阵亡) | — | — |
| 10 | Modify Stat（属性修正） | 0.4× | — | 100% | 友方·全体(含阵亡) | — | — |
| 11 | Modify Stat（属性修正） | — | 20 | 100% | 友方·全体(含阵亡) | — | — |
| 12 | Modify Stat（属性修正） | — | 40 | 100% | 友方·全体(含阵亡) | — | — |
| 13 | Modify Stat（属性修正） | 0.15× | — | 100% | 友方·全体(含阵亡) | — | — |
| 14 | Modify Stat（属性修正） | 0.3× | — | 100% | 友方·全体(含阵亡) | — | — |
| 15 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Blackboard Modify（黑板修改）; 引用:Immunity（免疫） |
| 16 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Immunity（免疫） |
| 17 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Immunity（免疫） |
| 18 | Apply Effect（施加效果） | — | — | 100% | 友方·自身(含阵亡) | — | 引用:Blackboard Query（黑板查询）; 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 引用:Blackboard Modify（黑板修改） |
| 19 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Query Count（计数查询）; 引用:Modify Ultimate Charge（终极技充能） |
| 20 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Query Count（计数查询）; 引用:Modify Ultimate Charge（终极技充能） |
| 21 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Query Count（计数查询）; 引用:Modify Ultimate Charge（终极技充能） |
| 22 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Query Count（计数查询）; 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正） |
| 23 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Query Stat（属性查询）; 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正） |
| 24 | Immunity（免疫） | — | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 25 | Apply Effect（施加效果） | — | — | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 标记:免疫（leaderability_lordvader_health_threshold_percent_health_immunity_trigger）; 引用:Immunity（免疫） |
| 26 | Apply Effect（施加效果） | — | — | 100% | 友方·自身(含阵亡) | — | 引用:Apply Effect（施加效果） |
| 27 | Apply Effect（施加效果） | — | — | 100% | 友方·自身(含阵亡) | — | 引用:Immunity（免疫） |
| 28 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正） |
| 29 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Modify Stat（属性修正） |
| 30 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Apply Effect（施加效果）; 引用:Apply Effect（施加效果）; 引用:Apply Effect（施加效果）; 引用:Apply Effect（施加效果）; 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 引用:Apply Effect（施加效果）; 引用:Apply Effect（施加效果） |
| 31 | Query Count（计数查询） | — | — | 100% | 友方·全体 | — | — |
| 32 | Modify Damage（伤害修正） | — | -0.3 | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 33 | Immunity（免疫） | — | — | 100% | 友方·自身 | — | — |
| 34 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Query Count（计数查询）; 引用:Modify Damage（伤害修正） |
| 35 | Modify Stat（属性修正） | — | 99999 | 100% | 友方·全体 | — | 有过期条件 |
| 36 | Modify Stat（属性修正） | — | 99999 | 100% | 友方·全体 | — | 有过期条件 |

**特殊机制标注（数据级）**：免疫 Immunity

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>

### 独特技(被动) · 扭曲预言
`uniqueability_lordvader01` `被动 aura (abilityType 8)` `技能等级 2` `zeta`

> 维达大人对回合条操纵免疫，他的攻击无法躲避。战斗开始时，他将失去所有防护力并获得等量的最大生命值，黑暗面无阵营原力使用者友军得到2回合的速度提升和隐形。
> 
> 在每个其他角色的回合开始时，维达大人叠加1层被低估[ffff33]被低估：复仇风暴、黑暗先驱和不羁情感获得额外效果

**效果链（tier[-1]，12 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Query Stat（属性查询）; 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 有过期条件 |
| 2 | Immunity（免疫） | — | — | 100% | 友方·自身(含阵亡) | — | 不可抵抗(跳过 §13 抵抗判定) |
| 3 | Immunity（免疫） | — | — | 100% | 友方·自身(含阵亡) | — | 不可抵抗(跳过 §13 抵抗判定) |
| 4 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Apply Effect（施加效果） |
| 5 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Apply Effect（施加效果） |
| 6 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Apply Effect（施加效果） |
| 7 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Apply Effect（施加效果）; 引用:Apply Effect（施加效果） |
| 8 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Apply Effect（施加效果）; 引用:Apply Effect（施加效果） |
| 9 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 标记:免疫（uniqueability_lordvader01_DOT_turn_meter_immunity_handler_handler）; 引用:Apply Effect（施加效果） |
| 10 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 有过期条件 |
| 11 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Blackboard Modify（黑板修改）; 引用:Dispel（驱散）; 引用:Dispel（驱散） |
| 12 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Apply Effect（施加效果） |

**特殊机制标注（数据级）**：免疫 Immunity

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>

### 独特技(被动) · 银河传奇
`uniqueability_galacticlegend01` `被动 aura (abilityType 8)` `技能等级 2` `zeta`

> 此单位受到的生命值百分比伤害效果和巨大伤害效果减弱。他们会因毁灭效果而受到巨大伤害（公会副本Boss不受此影响）。

**效果链（tier[-1]，11 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Query Unit Value（单位数值查询） | — | — | 100% | 友方·自身 | — | — |
| 2 | Blackboard Modify（黑板修改） | — | — | 100% | 友方·自身 | — | — |
| 3 | Blackboard Query（黑板查询） | — | — | 100% | 友方·自身 | — | — |
| 4 | Blackboard Query（黑板查询） | — | — | 100% | 友方·自身 | — | — |
| 5 | Modify Stat（属性修正） | — | — | 100% | 友方·自身 | — | — |
| 6 | Modify Stat（属性修正） | — | — | 100% | 友方·自身 | — | — |
| 7 | Apply Effect（施加效果） | — | — | 100% | 双方·全体(含阵亡) | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Query Stat（属性查询）; 引用:Blackboard Modify（黑板修改）; 有过期条件 |
| 8 | Apply Effect（施加效果） | — | — | 100% | 双方·全体(含阵亡) | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Apply Effect（施加效果） |
| 9 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Apply Effect（施加效果）; 引用:Apply Effect（施加效果）; 引用:Immunity（免疫） |
| 10 | Immunity（免疫） | — | — | 100% | 友方·自身 | — | — |
| 11 | Modify Damage（伤害修正） | — | -0.3 | 100% | 友方·自身 | — | — |

**特殊机制标注（数据级）**：免疫 Immunity

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>

---

## 琴·厄索（Jyn Erso） · `JYNERSO`
<div class="chips"><span class="chip align-光明">阵营 光明</span> <span class="chip">角色 · 7星</span> <span class="chip">定位:攻击者</span> <span class="chip">定位:领袖</span> <span class="chip">territory_light_platoon</span> <span class="chip">territory_tb3_lightside</span> <span class="chip">territory_tb3_hero</span> <span class="chip">义军</span> <span class="chip">义军</span> <span class="chip">affiliation_rogue_one</span> <span class="chip">specialmission_rogueone</span> <span class="chip">specialmission_corerebels</span> <span class="chip">自身标签:jynerso</span> <span class="chip">可获取</span> <span class="chip">gac_rogueone</span></div>

> 进攻型义军首领，可以获得优势、眩晕敌人、复活义军友军并为他们带来效能提升

**基础数值**（数据缩放：unscaledDecimalValue ÷ 1e8，proto→runtime 双枚举映射由 game/data_units.py 派生；低值取该属性首次出现的 Gear 等级）

| 属性 Stat | 基础（首次出现档） | 满配 (Gear 13) |
|---|---|---|
| 生命 Health | 172 | 14,092 |
| 攻击 Attack Damage | 16 | 1,729 |
| 特攻 Ability Power | 21（Gear 7 起） | 62 |
| 速度 Speed | 100 | 183 |
| 护甲 Armor | 15（Gear 11 起） | 70 |
| 压制 Suppression | 30（Gear 13 起） | 30 |
| 防护 Protection | 2,579（Gear 7 起） | 32,234 |
| 命中 Potency | 3%（Gear 4 起） | 22% |
| 抵抗 Tenacity | — | — |

### 基本技 · 加紧进攻
`basicability_jynerso` `冷却 0` `技能等级 7`

**冷却 0 · 目标 敌方·选中目标 · 伤害类型 Physical（物理）**

> 对敌方目标造成物理伤害，有40%几率在2回合内获得优势。

**伤害 = 1.467 × 攻击（Physical 物理，受目标护甲 Armor 减免 §6）** ±5% 方差

按技能等级：1.063 / 1.164 / 1.164 / 1.214 / 1.316 / 1.316 / 1.467

**效果链（tier[-1]，2 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Damage（伤害） | 1.467× | — | 100% | 敌方·选中目标 | ±5% | 物理 |
| 2 | Apply Effect（施加效果） | — | — | 70% | 友方·自身 | — | 引用:Apply Effect（施加效果）; 有过期条件 |

**特殊机制标注（数据级）**：施加 Buff/Debuff（生效走 §13 抵抗判定：基础 15%+等级差×5%[0,30%]+Potency−Tenacity）

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
dmg_Physical = 1.467 × ATTACK_DAMAGE(6)
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (relicΔ+1.0)（等级差缩放已禁用）
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × (1 − rating/(rating+(攻Lv+60)×ArmorRatingMod 7.5))   §6
方差: v=int(dmg×0.05); dmg += nextInt(v×2|1) − v
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
</pre>

</details>

### 特殊技 · 义军反击
`specialability_jynerso01` `冷却 4` `技能等级 7`

**冷却 4 · 目标 友方·选中目标 · 伤害类型 Physical（物理）**

> 对所有敌人造成物理伤害，为其他友军赋予100%回合条。每达成一次暴击效果，复活一名随机义军友军，并恢复其20%最大生命值。

**伤害 = 1.102 × 攻击（Physical 物理，受目标护甲 Armor 减免 §6）** ±5% 方差

按技能等级：0.732 / 0.792 / 0.896 / 0.93 / 0.999 / 1.102 / 1.102

**效果链（tier[-1]，10 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Recover（治疗） | 1× | — | 100% | 友方·选中目标 | — | — |
| 2 | Apply Effect（施加效果） | — | — | 100% | 友方·选中目标 | — | 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 引用:Apply Effect（施加效果）; 有过期条件 |
| 3 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Immunity（免疫）; 有过期条件 |
| 4 | Damage（伤害） | 1.102× | — | 100% | 敌方·全体 | ±5% | 物理 |
| 5 | Query Count（计数查询） | — | — | 100% | 友方·自身 | — | — |
| 6 | Revive（复活） | 0.01× | — | 100% | 友方·随机 | — | 标记:复活（revive） |
| 7 | Immunity（免疫） | — | — | 100% | 友方·上一个目标 | — | 不可抵抗(跳过 §13 抵抗判定); 标记:复活（specialability_jynerso01_revive_marker）; 有过期条件 |
| 8 | Query Count（计数查询） | — | — | 100% | 友方·自身 | — | — |
| 9 | Query Count（计数查询） | — | — | 100% | 友方·自身 | — | — |
| 10 | Recover（治疗） | — | — | 100% | 友方·全体 | — | — |

**特殊机制标注（数据级）**：免疫 Immunity、复活 Revive、施加 Buff/Debuff（生效走 §13 抵抗判定：基础 15%+等级差×5%[0,30%]+Potency−Tenacity）

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
dmg_Physical = 1.102 × ATTACK_DAMAGE(6)
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (relicΔ+1.0)（等级差缩放已禁用）
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × (1 − rating/(rating+(攻Lv+60)×ArmorRatingMod 7.5))   §6
方差: v=int(dmg×0.05); dmg += nextInt(v×2|1) − v
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
</pre>

</details>

### 特殊技 · 棍击
`specialability_jynerso02` `冷却 4` `技能等级 7`

**冷却 4 · 目标 友方·自身 · 伤害类型 Physical（物理）**

> 对敌方目标造成物理伤害并去除所有回合条。暴击时让目标眩晕1回合。

**伤害 = 2.131 × 攻击（Physical 物理，受目标护甲 Armor 减免 §6）** ±5% 方差

按技能等级：1.721 / 1.885 / 1.885 / 1.967 / 2.131 / 2.131 / 2.131

**效果链（tier[-1]，4 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Paralysis（麻痹/眩晕）; 有过期条件 |
| 2 | Recover（治疗） | -1× | — | 100% | 敌方·选中目标 | — | — |
| 3 | Recover（治疗） | — | — | 100% | 友方·自身 | — | — |
| 4 | Damage（伤害） | 2.131× | — | 100% | 敌方·选中目标 | ±5% | 物理 |

**特殊机制标注（数据级）**：施加 Buff/Debuff（生效走 §13 抵抗判定：基础 15%+等级差×5%[0,30%]+Potency−Tenacity）

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
dmg_Physical = 2.131 × ATTACK_DAMAGE(6)
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (relicΔ+1.0)（等级差缩放已禁用）
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × (1 − rating/(rating+(攻Lv+60)×ArmorRatingMod 7.5))   §6
方差: v=int(dmg×0.05); dmg += nextInt(v×2|1) − v
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
</pre>

</details>

### 领袖技(被动) · 投身战场
`leaderability_jynerso` `被动 aura (abilityType 8)` `技能等级 7` `zeta`

> 义军友军+20%效能。在义军友军回合中遭受减益效果的敌人还有35%的几率暴露2回合。该暴露效果无法抵抗。

**效果链（tier[-1]，3 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Modify Stat（属性修正） | — | 0.35 | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 2 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Apply Effect（施加效果） |
| 3 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Recover（治疗） |

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>

### 独特技(被动) · 炽烈决心
`uniqueability_jynerso01` `被动 aura (abilityType 8)` `技能等级 8`

> 琴·厄索在每次暴击一个敌人时获得1%效能。

**效果链（tier[-1]，4 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Query Count（计数查询）; 引用:Modify Stat（属性修正） |
| 2 | Immunity（免疫） | — | — | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 3 | Immunity（免疫） | — | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 4 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Query Battle Type（战斗类型查询）; 引用:Immunity（免疫）; 引用:Query Count（计数查询）; 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 引用:Apply Effect（施加效果）; 引用:Query Count（计数查询）; 引用:Query Battle Type（战斗类型查询）; 引用:Apply Effect（施加效果） |

**特殊机制标注（数据级）**：免疫 Immunity

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>

---

## R2D2_LEGENDARY（R2-D2） · `R2D2_LEGENDARY`
<div class="chips"><span class="chip align-光明">阵营 光明</span> <span class="chip">角色 · 7星</span> <span class="chip">定位:辅助</span> <span class="chip">种族:机器人</span> <span class="chip">共和国</span> <span class="chip">义军</span> <span class="chip">specialmission_legendarylight</span> <span class="chip">抵抗组织</span> <span class="chip">自身标签:r2d2_legendary</span> <span class="chip">可获取</span> <span class="chip">territory_light_platoon</span> <span class="chip">platoon_legendary_light</span> <span class="chip">platoon_legendary_restricted</span> <span class="chip">territory_geonosis_republic</span> <span class="chip">gac_droidpilot</span> <span class="chip">gac_hothvet</span></div>

> 用途广泛的辅助型机器人，能施加燃烧伤害并让友军隐形

**基础数值**（数据缩放：unscaledDecimalValue ÷ 1e8，proto→runtime 双枚举映射由 game/data_units.py 派生；低值取该属性首次出现的 Gear 等级）

| 属性 Stat | 基础（首次出现档） | 满配 (Gear 13) |
|---|---|---|
| 生命 Health | 240 | 23,495 |
| 攻击 Attack Damage | 20 | 432 |
| 特攻 Ability Power | 3（Gear 3 起） | 585 |
| 速度 Speed | 104 | 177 |
| 护甲 Armor | 7（Gear 4 起） | 7 |
| 压制 Suppression | 26（Gear 11 起） | 52 |
| 防护 Protection | 2,956（Gear 7 起） | 36,950 |
| 命中 Potency | 2%（Gear 4 起） | 41% |
| 抵抗 Tenacity | 4%（Gear 5 起） | 53% |

### 基本技 · 电棍戳击
`basicability_r2d2_legendary` `冷却 0` `技能等级 7`

**冷却 0 · 目标 敌方·选中目标 · 伤害类型 Physical（物理）**

> 对敌方目标造成物理伤害，有35%几率让其眩晕1回合。此攻击对燃烧目标造成30%额外伤害。

**伤害 = 2.023 × 攻击（Physical 物理，受目标护甲 Armor 减免 §6）** ±5% 方差

按技能等级：1.465 / 1.604 / 1.604 / 1.674 / 1.814 / 1.814 / 2.023

**效果链（tier[-1]，3 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Query Count（计数查询） | — | — | 100% | 敌方·选中目标 | — | — |
| 2 | Damage（伤害） | 2.023× | — | 100% | 敌方·选中目标 | ±5% | 物理 |
| 3 | Paralysis（麻痹/眩晕） | — | — | 80% | 敌方·选中目标 | — | 有过期条件 |

**特殊机制标注（数据级）**：眩晕

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
dmg_Physical = 2.023 × ATTACK_DAMAGE(6)
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (relicΔ+1.0)（等级差缩放已禁用）
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × (1 − rating/(rating+(攻Lv+60)×ArmorRatingMod 7.5))   §6
方差: v=int(dmg×0.05); dmg += nextInt(v×2|1) − v
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
</pre>

</details>

### 特殊技 · 烟幕
`specialability_r2d2_legendary01` `冷却 5` `技能等级 7`

**冷却 5 · 目标 友方·选中目标**

> 友军目标在2回合内获得预见能力，所有其他友军有70%几率在2回合内隐形。R2-D2获得10%回合条增益。

**效果链（tier[-1]，6 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Modify Stat（属性修正） | — | 9999 | 100% | 友方·选中目标 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 2 | Modify Stat（属性修正） | — | 9999 | 100% | 友方·选中目标 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 3 | Apply Effect（施加效果） | — | — | 100% | 友方·选中目标 | — | 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 引用:Apply Effect（施加效果）; 有过期条件 |
| 4 | Immunity（免疫） | — | — | 100% | 友方·选中目标 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 5 | Modify Stat（属性修正） | — | -1 | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 标记:隐身（stealth_buff）; 标记:隐身（featcounter_stealth_buff）; 有过期条件 |
| 6 | Recover（治疗） | 0.4× | — | 100% | 友方·自身 | — | — |

**特殊机制标注（数据级）**：免疫 Immunity、施加 Buff/Debuff（生效走 §13 抵抗判定：基础 15%+等级差×5%[0,30%]+Potency−Tenacity）

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>

### 特殊技 · 即兴发挥
`specialability_r2d2_legendary02` `冷却 5` `技能等级 7`

**冷却 5 · 目标 敌方·全体 · 伤害类型 Special（特殊）**

> 对所有敌人造成特殊伤害，并在2回合内施加燃烧效果。此攻击无法躲避。

**伤害 = 0.754 × 特攻（Special 特殊，受目标抗性 Resist 减免 §6）** ±5% 方差

按技能等级：0.495 / 0.542 / 0.612 / 0.636 / 0.683 / 0.754 / 0.754

**效果链（tier[-1]，5 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Damage（伤害） | 0.754× | — | 100% | 敌方·全体 | ±5% | 特殊 |
| 2 | Immunity（免疫） | — | — | 100% | 敌方·全体 | — | 有过期条件 |
| 3 | Apply Effect（施加效果） | — | — | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Damage（伤害）; 引用:Damage（伤害）; 引用:Damage（伤害）; 有过期条件 |
| 4 | Modify Stat（属性修正） | — | -5000 | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 5 | Modify Stat（属性修正） | — | -5000 | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |

**特殊机制标注（数据级）**：免疫 Immunity、施加 Buff/Debuff（生效走 §13 抵抗判定：基础 15%+等级差×5%[0,30%]+Potency−Tenacity）

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
dmg_Special = 0.754 × ABILITY_POWER(7)
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (clamp(纪元Δ×0.075, -0.75, +0.75)+1.0)
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × (1 − rating/(rating+(攻Lv+60)×SuppressionRatingMod 7.5))   §6
方差: v=int(dmg×0.05); dmg += nextInt(v×2|1) − v
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
</pre>

</details>

### 独特技(被动) · 战斗分析
`uniqueability_r2d2_legendary01` `被动 aura (abilityType 8)` `技能等级 7` `zeta`

> R2-D2启用时，全体友军增加4%暴击率和4%精准度。

**效果链（tier[-1]，6 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Modify Stat（属性修正） | — | 0.1 | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 2 | Modify Stat（属性修正） | — | 0.1 | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 3 | Modify Stat（属性修正） | — | 0.1 | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 4 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正）; 引用:Modify Stat（属性修正） |
| 5 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Dispel（驱散） |
| 6 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Apply Effect（施加效果） |

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>

### 独特技(被动) · 数字处理
`uniqueability_r2d2_legendary02` `被动 aura (abilityType 8)` `技能等级 7` `zeta`

> 战斗开始时，每存在一个机器人友军，R2-D2即可获得5%最大防护力，每存在一个银河共和国友军可获得5%进攻力，每存在一个义军友军可获得5%最大生命值，每存在一个抵抗组织友军可获得5%效能。

**效果链（tier[-1]，22 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Query Count（计数查询） | — | — | 100% | 友方·全体 | — | — |
| 2 | Modify Stat（属性修正） | — | — | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 3 | Query Count（计数查询） | — | — | 100% | 友方·全体 | — | — |
| 4 | Modify Stat（属性修正） | — | — | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 5 | Query Count（计数查询） | — | — | 100% | 友方·全体 | — | — |
| 6 | Modify Stat（属性修正） | — | — | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 7 | Query Count（计数查询） | — | — | 100% | 友方·全体 | — | — |
| 8 | Modify Stat（属性修正） | — | — | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 9 | Query Count（计数查询） | — | — | 100% | 友方·全体 | — | — |
| 10 | Modify Stat（属性修正） | — | — | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 11 | Query Stat（属性查询） | — | — | 100% | 友方·自身 | — | — |
| 12 | Modify Stat（属性修正） | — | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 13 | Query Stat（属性查询） | — | — | 100% | 友方·自身 | — | — |
| 14 | Modify Stat（属性修正） | — | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 15 | Query Stat（属性查询） | — | — | 100% | 友方·自身 | — | — |
| 16 | Modify Stat（属性修正） | — | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 17 | Query Stat（属性查询） | — | — | 100% | 友方·自身 | — | — |
| 18 | Modify Stat（属性修正） | — | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 19 | Query Stat（属性查询） | — | — | 100% | 友方·自身 | — | — |
| 20 | Modify Stat（属性修正） | — | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 21 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Query Stat（属性查询）; 引用:Modify Stat（属性修正）; 引用:Query Stat（属性查询）; 引用:Modify Stat（属性修正）; 引用:Query Stat（属性查询）; 引用:Modify Stat（属性修正）; 引用:Query Stat（属性查询）; 引用:Modify Stat（属性修正）; 引用:Query Stat（属性查询）; 引用:Modify Stat（属性修正） |
| 22 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Dispel（驱散） |

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>

---

## 高级星区总督塔金（Grand Moff Tarkin） · `GRANDMOFFTARKIN`
<div class="chips"><span class="chip align-黑暗">阵营 黑暗</span> <span class="chip">角色 · 7星</span> <span class="chip">定位:辅助</span> <span class="chip">定位:领袖</span> <span class="chip">种族:人类</span> <span class="chip">帝国</span> <span class="chip">自身标签:grandmofftarkin</span> <span class="chip">可获取</span> <span class="chip">character_fleetcommander</span> <span class="chip">territory_dark_platoon</span> <span class="chip">specialmission_empire</span> <span class="chip">non_viable_leader</span></div>

> 可以提供进攻辅助的的帝国支援角色，具备强大的群体减益效果

**基础数值**（数据缩放：unscaledDecimalValue ÷ 1e8，proto→runtime 双枚举映射由 game/data_units.py 派生；低值取该属性首次出现的 Gear 等级）

| 属性 Stat | 基础（首次出现档） | 满配 (Gear 13) |
|---|---|---|
| 生命 Health | 165 | 15,140 |
| 攻击 Attack Damage | 15 | 739 |
| 特攻 Ability Power | 3（Gear 3 起） | 825 |
| 速度 Speed | 100 | 154 |
| 护甲 Armor | 10（Gear 5 起） | 65 |
| 压制 Suppression | 2（Gear 2 起） | 139 |
| 防护 Protection | 2,994（Gear 7 起） | 37,419 |
| 命中 Potency | 1%（Gear 3 起） | 26% |
| 抵抗 Tenacity | 2%（Gear 2 起） | 43% |

### 基本技 · 大总督攻击
`basicability_grandmofftarkin` `冷却 0` `技能等级 7`

**冷却 0 · 目标 友方·全体 · 伤害类型 Special（特殊）**

**伤害 = 1.962 × 特攻（Special 特殊，受目标抗性 Resist 减免 §6）** ±5% 方差

按技能等级：1.526 / 1.526 / 1.598 / 1.744 / 1.962 / 1.962 / 1.962

**效果链（tier[-1]，4 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Query Count（计数查询） | — | — | 100% | 友方·全体 | — | — |
| 2 | Query Count（计数查询） | — | — | 100% | 友方·全体 | — | — |
| 3 | Damage（伤害） | 1.962× | — | 100% | 敌方·选中目标 | ±5% | 特殊 |
| 4 | Modify Stat（属性修正） | — | 1 | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定) |

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
dmg_Special = 1.962 × ABILITY_POWER(7)
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (clamp(纪元Δ×0.075, -0.75, +0.75)+1.0)
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × (1 − rating/(rating+(攻Lv+60)×SuppressionRatingMod 7.5))   §6
方差: v=int(dmg×0.05); dmg += nextInt(v×2|1) − v
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
</pre>

</details>

### 特殊技 · 终极火力
`specialability_grandmofftarkin01` `冷却 4` `技能等级 7`

**冷却 4 · 目标 友方·自身 · 伤害类型 Special（特殊）**

**伤害 = 1.1 × 特攻（Special 特殊，受目标抗性 Resist 减免 §6）** ±5% 方差

按技能等级：0.318 / 0.348 / 0.348 / 0.364 / 0.364 / 0.394 / 1.1

**效果链（tier[-1]，5 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Query Stat（属性查询） | — | — | 100% | 友方·自身 | — | — |
| 2 | Damage（伤害） | 0.55× | — | 100% | 敌方·全体 | ±5% | 特殊 |
| 3 | Damage（伤害） | 0.55× | — | 100% | 敌方·全体 | ±5% | 特殊 |
| 4 | Recover（治疗） | -0.5× | — | 100% | 敌方·全体 | — | — |
| 5 | Dispel（驱散） | — | 100 | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定) |

**特殊机制标注（数据级）**：驱散 Dispel

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
dmg_Special = 1.1 × ABILITY_POWER(7)
      × ProcessSourceDamage(baseStat×srcMult+srcFlat)
      × (clamp(纪元Δ×0.075, -0.75, +0.75)+1.0)
      × (暴击 ? max(CRIT_DMG+1.5,1.0) : 1)        §5
      × (1 + out)×(1 + in)                        §9 增减益聚合
      × (1 − rating/(rating+(攻Lv+60)×SuppressionRatingMod 7.5))   §6
方差: v=int(dmg×0.05); dmg += nextInt(v×2|1) − v
护盾分流: split=clamp(守方 SHIELD_PENETRATION(34),0,1)  §7
</pre>

</details>

### 特殊技 · 恐吓战术
`specialability_grandmofftarkin02` `冷却 4` `技能等级 7`

**冷却 4 · 目标 友方·自身**

**效果链（tier[-1]，15 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Modify Stat（属性修正） | — | 0.5 | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 2 | Modify Stat（属性修正） | -0.5× | — | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 3 | Modify Stat（属性修正） | -0.5× | — | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 4 | Modify Stat（属性修正） | -0.5× | — | 100% | 敌方·全体 | — | 有过期条件 |
| 5 | Modify Stat（属性修正） | -0.5× | — | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 6 | Modify Stat（属性修正） | — | -0.25 | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 7 | Modify Stat（属性修正） | — | -0.25 | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 8 | Modify Stat（属性修正） | — | -0.25 | 100% | 敌方·全体 | — | 有过期条件 |
| 9 | Modify Stat（属性修正） | — | -0.25 | 100% | 敌方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 有过期条件 |
| 10 | Query Count（计数查询） | — | — | 100% | 敌方·全体 | — | — |
| 11 | Recover（治疗） | — | — | 100% | 友方·自身 | — | — |
| 12 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Recover（治疗）; 有过期条件 |
| 13 | Modify Stat（属性修正） | — | 0.3 | 100% | 双方·全体 | — | 有过期条件 |
| 14 | Modify Stat（属性修正） | — | 0.3 | 100% | 双方·全体 | — | 有过期条件 |
| 15 | Apply Effect（施加效果） | — | — | 100% | 双方·全体 | — | 引用:Recover（治疗）; 引用:Apply Effect（施加效果）; 有过期条件 |

**特殊机制标注（数据级）**：施加 Buff/Debuff（生效走 §13 抵抗判定：基础 15%+等级差×5%[0,30%]+Potency−Tenacity）

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>

### 领袖技(被动) · 加强掌控
`leaderability_grandmofftarkin_BASE` `被动 aura (abilityType 8)` `技能等级 8`


**效果链（tier[-1]，23 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Query Battle Type（战斗类型查询） | — | — | 100% | — | — | — |
| 2 | Modify Stat（属性修正） | 0.3× | — | 100% | 友方·全体 | — | — |
| 3 | Modify Stat（属性修正） | 0.3× | — | 100% | 友方·全体 | — | — |
| 4 | Modify Stat（属性修正） | — | 30 | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定) |
| 5 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Apply Effect（施加效果） |
| 6 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Apply Effect（施加效果） |
| 7 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Recover（治疗）; 引用:Recover（治疗） |
| 8 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Paralysis（麻痹/眩晕）; 引用:Apply Effect（施加效果） |
| 9 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Dispel（驱散）; 引用:Immunity（免疫）; 引用:Modify Stat（属性修正） |
| 10 | Modify Stat（属性修正） | — | -1 | 100% | 友方·全体 | — | 标记:隐身（stealth_buff）; 标记:隐身（featcounter_stealth_buff）; 有过期条件 |
| 11 | Grant Shield（给予护盾） | 0.5× | — | 100% | 友方·全体 | — | 有过期条件 |
| 12 | Modify Stat（属性修正） | 1.5× | — | 100% | 友方·全体 | — | 有过期条件 |
| 13 | Modify Stat（属性修正） | 1.5× | — | 100% | 友方·全体 | — | 有过期条件 |
| 14 | Modify Stat（属性修正） | 1.5× | — | 100% | 友方·全体 | — | 有过期条件 |
| 15 | Modify Stat（属性修正） | 1.5× | — | 100% | 友方·全体 | — | 有过期条件 |
| 16 | Modify Stat（属性修正） | 1.5× | — | 100% | 友方·全体 | — | 有过期条件 |
| 17 | Modify Stat（属性修正） | 1.5× | — | 100% | 友方·全体 | — | 有过期条件 |
| 18 | Modify Stat（属性修正） | 1.5× | — | 100% | 友方·全体 | — | 有过期条件 |
| 19 | Modify Stat（属性修正） | 1.5× | — | 100% | 友方·全体 | — | 有过期条件 |
| 20 | Modify Stat（属性修正） | 1.5× | — | 100% | 友方·全体 | — | 有过期条件 |
| 21 | Modify Stat（属性修正） | 1.5× | — | 100% | 友方·全体 | — | 有过期条件 |
| 22 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Dispel（驱散）; 引用:Dispel（驱散）; 有过期条件 |
| 23 | Immunity（免疫） | — | — | 100% | 敌方·全体(含阵亡) | — | 不可抵抗(跳过 §13 抵抗判定); 标记:免疫（revive_immunity） |

**特殊机制标注（数据级）**：免疫 Immunity、护盾 Shield

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>

### 独特技(被动) · 冷酷信念
`uniqueability_grandmofftarkin01` `被动 aura (abilityType 8)` `技能等级 7` `zeta`


**效果链（tier[-1]，6 个 effect）**

| # | 类型 Type | 倍率 | 数值 | 几率 | 目标 | 方差 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | Query Count（计数查询） | — | — | 100% | 友方·全体 | — | — |
| 2 | Modify Stat（属性修正） | — | — | 100% | 友方·自身 | — | — |
| 3 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 不可抵抗(跳过 §13 抵抗判定); 引用:Query Count（计数查询）; 引用:Modify Stat（属性修正）; 引用:Query Stat（属性查询）; 引用:Modify Stat（属性修正）; 引用:Query Stat（属性查询）; 引用:Modify Stat（属性修正） |
| 4 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Grant Shield（给予护盾） |
| 5 | Apply Effect（施加效果） | — | — | 100% | 友方·自身 | — | 引用:Grant Shield（给予护盾）; 有过期条件 |
| 6 | Apply Effect（施加效果） | — | — | 100% | 友方·全体 | — | 引用:Recover（治疗）; 引用:Recover（治疗） |

<details><summary>计算方式 · NUMBERS.md §7 伤害管线代入</summary>

<pre>
(非伤害技能：效果经 §13 抵抗判定施加；治疗走 §7 无减伤项)
</pre>

</details>