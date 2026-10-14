# SWGoH 动画体系逆向报告

调查范围：`shared_animationcontainer`（178 个共享 AnimationClip）+ 8 个 `char_*_pre` bundle。
产出数据：`animations.json`（clip 清单 + 角色→动画映射）。

## 1. 总体结构

```
char_<name>_pre bundle
├── unit.<name>  prefab（GameObject 层级， Animator 根 = unit.<name>/Scaler 节点）
│   ├── Animator          → m_Controller: hmn_<name>_ctrl（角色私有 AnimatorController）
│   │                     → m_Avatar: 角色 Avatar（humanoid 重定向用）
│   ├── Animation(legacy) → 只在武器/光剑子物体上，播放 saber_on/off FX clip
│   ├── PlayMakerFSM / UnitFSMController   ← 战斗逻辑用 FSM 状态机驱动 Animator 参数/触发
│   ├── UnitAnimationEventHandler / UnitSubObjectAnimationEventHandler ← 动画事件回调
│   └── SkinnedMeshRenderer ×N（骨骼蒙皮，已可导出）
├── hmn_<name>_ctrl (AnimatorController)  ← 角色→动画 的对应就在这里
│   ├── m_AnimationClips: PPtr 列表（fileID=0 → bundle 内私有 clip；fileID=3 → shared_animationcontainer）
│   ├── m_TOS: 状态机状态名 + 原始工程 .anim 路径（如 .../Anim/Humanoid/Shared/HomeScreenIdle/hmn_homescrn_any01.anim）
│   └── m_LayerArray / m_StateMachineArray: Base Layer / Homescreen / Special Ability ×2 等层
├── 角色**私有** AnimationClip（特色招式，如 hmn_sbr_massacre_vader）
└── Avatar（humanoid 映射）
依赖：shared_animationcontainer.bundle（共享 clip）、shared_resourcecontainer、shared_shadercontainer
```

`shared_animationcontainer` 内部：178 个 AnimationClip + 1 个 `SharedAssetReferences`
(Assembly-CSharp) MonoBehaviour，其 `References` 数组正好枚举全部 178 个 clip PPtr ——
即共享容器的"目录"。

## 2. 命名规律与分组（共 178 个）

前缀：`hmn_` = humanoid 类共享动作（128），`gen_` = 非人形生物（25），`shp_` = 载具/飞船（11），
`cantina_` = 酒馆社交（9），`saber_*` = 光剑点燃/关闭 FX（5，Legacy 类型）。

| 分组 | 数量 | 内容 |
|---|---|---|
| hmn_spcl_* | 49 | 特殊技能动作（force_crush / saberthrow / 各种角色专属手势） |
| hmn_pst_* | 13 | 主武器基础套件（idle/run/atk，var01/var02 变体） |
| gen_geonosian | 13 | 吉奥诺西斯人全套（fly/land/stf/rfl/atk/idle/dth） |
| shp_* | 11 | 飞船动作 |
| hmn_cantina / cantina / hmn_barpatron | 27 | 酒馆待机/喝彩/舞蹈类 |
| hmn_sbr_* | 9 | 光剑特化动作 |
| gen_dlc13 / gen_combatremote / gen_astromech | 11 | 其他生物/机器人 |
| hmn_homescrn_* | 5 | 主界面待机（hmn_homescrn_any01 为通用待机） |
| hmn_rfl_* / hmn_stf_* / hmn_cstr / hmn_protodrd / hmn_gtlng | 20 | 步枪/电杖/建造机器人/原型机器人/炮艇 |
| fx_saber_ignite (saber_on/off/short) | 5 | Legacy FX clip，只绑 `Inner_Glow`/`Outer_Glow` 材质光效 |
| 零散 | — | hmn_dth_01（死亡）、hmn_hit_01（受击）、hmn_hurt_idle、hmn_consume 等 |

通用战斗三件套几乎是每个角色必备：`hmn_dth_01`（死）、`hmn_hit_01`（受击）、
`hmn_hurt_idle`（濒死待机）、`hmn_homescrn_any01`（主界面待机）、`hmn_pst_var01_run/atk/idle`（移动/攻击基底）。

## 3. clip 绑定方式（决定能否重定向）

每 clip 的 GenericBinding 分三类（见 `kind` 字段）：

- **muscle_only（24）**：path=0、typeID=95(Animator) 的 humanoid 肌肉曲线。
  数据在 MuscleClip 里，是归一化关节角，**与骨骼命名无关**，可经 Avatar/muscle 映射重定向到任意人形。
- **transform_only（34）**：普通 Transform T/R/S 曲线，绑定到**路径哈希**。
- **mixed（115）**：主体是肌肉曲线 + 附加 Transform 曲线（武器 `*_wpn_offsetJNT/*_wpnJNT`、
  披风 `cape_*_JNT`、光剑 glow、面罩等）——人形 clip 的标准形态。
- **legacy_fx（5）**：saber_on/off，仅 Float 曲线驱动 Glow 材质/粒子。

**路径哈希 = 普通 CRC32(zlib/IEEE)**，内容是相对 Animator 根（`unit.<name>/Scaler` 节点）的
Transform 路径。已破解 114 种不同路径中的 39 种（覆盖 97% 的绑定实例），剩余未破解的属于
未采样的生物骨架（章鱼、飞船等）。人形骨架约定：

```
unit.<charname>/Scaler/pelvicJNT
 ├─ spine1JNT/spine2JNT/neckJNT/headJNT
 ├─ spine2JNT/Left|Right_clavJNT → shldrJNT → elbwJNT → wrstJNT → *_wpn_offsetJNT → *_wpnJNT
 ├─ spine2JNT/cape_top_JNT → cape_mid_JNT → cape_low_JNT
 └─ Left|Right_hipJNT → kneeJNT → ankleJNT → ballJNT
```

Maya 风格 `*JNT` 命名，左右用 `Left_/Right_` 前缀（不是 Bip01）。

## 4. 坐标系/镜像结论

- clip 本身只存局部 T/R/S 数值（Unity 单位、左手系、Y-up），**不含坐标系**；
  应用到我们导出的 X 镜像 glTF 骨骼时只需沿用模型导出时同一套 Unity→glTF 轴转换
  （对四元数做 (x, -z, -y?, …) 同样的分量重排），clip 数值可直接喂给对应关节。
- 路径哈希绑定要求目标骨架**同名同层级**（`pelvicJNT/...`）。我们的 GLB 导出保留原始骨骼名，
  所以 transform_only/mixed 的 Transform 部分可按名字直接匹配。
- muscle 曲线部分必须实现 Unity Humanoid muscle→关节旋转的换算（AvAtar human 映射 +
  muscle 归一化角公式），否则只能取 mixed clip 的 Transform 附加层。
- clip 的 `m_MuscleClip` 里还有 m_Mirror/m_LoopTime/m_StopTime 等设置可复用（时长即 m_StopTime，
  `m_SampleRate` 24/30/60Hz）。

## 5. 推荐导出方案（简述）

1. **每角色**：读 `hmn_<name>_ctrl` 的 `m_AnimationClips`（区分 fileID 0=私有 / 3=共享），
   得到角色动作集；配合 `m_TOS` 可还原状态机层名与原工程路径。
2. **共享 clip**：从 `shared_animationcontainer` 按 path_id 读出，名称已在 `animations.json`。
3. 首选实现 **transform 曲线回放**（骨骼名直接匹配，覆盖 34 transform_only + 115 mixed 的附加层），
   humaniod muscle 主体第二阶段再做 muscle→旋转换算。
4. saber_on/off Legacy FX clip 只影响光效物体（Inner_Glow/Outer_Glow），可跳过或做材质动画。
5. 未破解的路径哈希可用同样方法（加载对应生物 char bundle，枚举 Transform 层级后 CRC 匹配）补齐。
