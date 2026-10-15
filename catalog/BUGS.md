# 资产保真度缺陷清单（2026-10-15 用户验收）

| # | 症状 | 根因假设 | 忠实来源 | 状态 |
|---|------|---------|---------|------|
| P1 | 对战中人物在场景外 | 未使用原版站位 | `catalog/ui_layouts.json`（142 个 battle_*_layout = 3D 竞技场定位树）| 修复中 |
| P2 | 场景没有地面 | 地面/地形在独立 bundle（同 char_aat 案例）| 环境包依赖链 m_Dependencies | 修复中 |
| P3 | 立面不垂直 + 武器不在手中 | ①部分 rig 整体倾斜（Avatar axes 应用错误）②道具 authored 在 bind/rest 空间未挂骨骼 | Avatar m_TOS / 挂点路径；骨骼世界姿态 | 修复中 |
| P5 | 多装扮不生效/偏移（Vader 头盔+脸同显） | 导出器忽略源 prefab 的 GameObject m_IsActive | 源 prefab 序列化的 active 态 = 原版装扮开关真值 | 修复中 |
| P6 | 多武器同时出现（Obiwan 四件悬浮） | 挂件未按 loadout 挂到手部 socket | 同 P3 挂点机制 | 修复中 |
| P7 | 披风偏移（Anakin 披风飞离） | 披风蒙皮骨骼未正确绑定/姿态错位 | 骨骼表 vs 渲染器 m_Bones | 排查中 |
| P8 | 光剑/枪械特效错误（白色斑块） | 粒子系统四边形被当静态网格导出；自发光混合模式丢失 | ParticleSystem 组件排除；glTF alphaMode BLEND+emissive | 修复中 |
| +1 | 多手部角色分离（Grievous） | 同 P3/P6 挂件家族 | 同上 | 修复中 |

修复原则：每项都从反编译数据/原始 bundle 序列化状态取真值，不猜测。
产出同时服务于 web 资源（extracted/ 重导出）与 Unity 工程（管线共用）。
