# 资产保真度缺陷清单（2026-10-15 用户验收）

| # | 症状 | 根因假设 | 忠实来源 | 状态 |
|---|------|---------|---------|------|
| P1 | 对战中人物在场景外 | 未使用原版站位 | `catalog/battle_staging.json`（130 个 battle_*_layout 的 PlayerMarkers/EnemyMarkers 槽位已解码；layout↔env 绑定来自 gamedata encounters，空 layoutPrefab 用客户端默认 pvp/pve layout，见 il2cpp 字符串）| 已修复（2026-10-15：tools/dump_battle_staging.py；server /api/staging + battle payload `staging`；battle.html 按 marker 摆人，无 staging 时回退旧网格）|
| P2 | 场景没有地面 | 不是独立 bundle：地面渲染器引用 Unity 内置图元（"unity default resources"，pathID 10210=Quad 等，缓存里没有该外部文件），此前被当 mr_without_mesh 丢弃；222/269 个 env bundle 有此引用 | env bundle 内部：内置图元几何按 Unity 规范合成（Quad/Plane/Cube 精确，Sphere/Cylinder/Capsule 近似）| 已修复（2026-10-15：tools/export_env.py 合成内置图元并重导出 moseisley/nikto_base/conquest8/hoth_snow/kash_beach；tools/verify_env_ground.py 校验地面存在+Y 覆盖站位）|
| P3 | 立面不垂直 + 武器不在手中 | ①部分 rig 整体倾斜（Avatar axes 应用错误）②道具 authored 在 bind/rest 空间未挂骨骼 | Avatar m_TOS / 挂点路径；骨骼世界姿态 | 修复中 |
| P5 | 多装扮不生效/偏移（Vader 头盔+脸同显） | 导出器忽略源 prefab 的 GameObject m_IsActive | 源 prefab 序列化的 active 态 = 原版装扮开关真值 | 已修复（2026-10-15：swgoh_extract.py 给节点打 extensions.SWGOH_active=false，viewer/battle 默认隐藏。忠实证据：vaderduelsend fist=active / hand_l+hand_open=inactive；jynerso 的 swd_jynerso_mesh=inactive（白色手枪板即它）；注意 head_mesh 与 vader_de_helmetburn_mesh 在源里同为 active——烧蚀头盔是叠在头上的 overlay，并非互斥）|
| P6 | 多武器同时出现（Obiwan 四件悬浮） | 挂件未按 loadout 挂到手部 socket | 同 P3 挂点机制 | 修复中 |
| P7 | 披风偏移（Anakin 披风飞离） | 根因修正：静态蒙皮从来是对的（bindpose 一致、无 dummy 骨）；错在动画导出 humanoid 路径把 127 条肌肉绑定不计入曲线偏移 acc，披风/武器的 transform 曲线读到了外骨骼的曲线（vader 披风 cape_top 平移被写成 [0.93,-0.61,0.55]） | 通用绑定表全表维度累加（uTinyRipper FindBinding 规则）；绑定 CRC ↔ GLB 全路径精确匹配 | 已修复（2026-10-15：export_animation_v2.py acc 全表累加 + cmap 精确匹配 + (node,prop) 去重 + m_TOS 键归一化无符号；vader_pre/fallen/remastered 重导出后披风 t=0 贴体 ≤0.22，tools/verify_variants_vfx.py 校验）|
| P8 | 光剑/枪械特效错误（白色斑块） | 粒子系统四边形被当静态网格导出；自发光混合模式丢失 | ParticleSystem 组件排除；glTF alphaMode BLEND+emissive | 已修复（2026-10-15：swgoh_extract.py 排除 PS 组件 GO 及其子树的渲染器 + vfx 分支下 trail/beam 网格 + m_Enabled=0 渲染器；Legacy Shaders/Particles/Additive 等 effect 材质导出为 alphaMode=BLEND+emissive；vaderduelsend 的 vfx_vader_helm_burn 即此）。viewer 里剩余白球 = EnvFX 点精灵替身（vfx.json 72 发射器同屏、无模拟），真 VFX 播放为独立后续系统 |
| +1 | 多手部角色分离（Grievous） | 同 P3/P6 挂件家族 | 同上 | 修复中 |

修复原则：每项都从反编译数据/原始 bundle 序列化状态取真值，不猜测。
产出同时服务于 web 资源（extracted/ 重导出）与 Unity 工程（管线共用）。
