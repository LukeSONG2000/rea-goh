# SWGoH 资源解包指南

从游戏安装/Unity 缓存中逆向提取角色模型、贴图、动画映射的完整流程与避坑手册。所有步骤均在本库构建过程中实测通过（游戏版本 **0.40.7**，Unity **2022.3.72f1**，IL2CPP）。

## 1. 环境

| 依赖 | 版本 | 说明 |
|---|---|---|
| Python | 3.13 | ⚠️ 用 `py` 启动器；裸 `python` 常是 WindowsApps 商店 stub（静默失败 exit 49）|
| UnityPy | 1.25.4 | `pip install UnityPy Pillow numpy` |
| Pillow / numpy | — | 贴图导出与蒙皮数组处理 |
| three.js | 本库 `viewer/` 内置 | 离线预览，无需 Unity |

## 2. 原始资源在哪

游戏本体资源不在安装目录，而在 **Unity 资源下载缓存**（明文 UnityFS，未加密）：

```
%USERPROFILE%\AppData\LocalLow\Unity\Electronic Arts_SWGoH\<bundle_name>\<hash>\__data
```

- 同一 bundle 名可能有多个 `<hash>` 版本目录 → **取 mtime 最新的 `__data`**
- 本库 `raw_bundles/` 即该缓存全量镜像（11,400 bundle，~2.7GB）
- 常用前缀：`char_*`（角色预制体）、`env_*`（场景）、`ship_*`（飞船）、`cin_*`（过场）、`shared_resourcecontainer`（共享网格+贴图池）、`shared_animationcontainer`（动画）、`shared_shadercontainer`

角色预制体的依赖在共享容器里——解析 `char_xxx_pre` 时要一并 `UnityPy.load` 相关 shared bundle，否则 PPtr 解引用会断链。

## 3. 十分钟最小示例（拉一个贴图 + 一个网格）

```python
import UnityPy, os, glob

bundle_dir = r"C:\Users\<你>\AppData\LocalLow\Unity\Electronic Arts_SWGoH\char_jynerso_pre"
data_file = max(glob.glob(os.path.join(bundle_dir, "*", "__data")), key=os.path.getmtime)
env = UnityPy.load(data_file)

for r in env.objects:
    if r.type.name == "Texture2D":
        img = r.read()          # .image 即 PIL Image（UnityPy 已处理翻转）
        img.save(f"{img.m_Name}.png")
    if r.type.name == "Mesh":
        mesh = r.read()
        print(mesh.m_Name, mesh.m_VertexCount)
```

## 4. 完整角色导出

`tools/swgoh_extract.py` 是本库 747 个 GLB 的生产导出器：

```bash
py tools/swgoh_extract.py list                       # 列出全部 char_*
py tools/swgoh_extract.py extract char_jynerso_pre   # 导出 GLB + textures/ + report.json
py tools/swgoh_extract.py extract-all --only char_   # 全量（747 个约数小时）
```

输出：`extracted/<char_name>/<char_name>.glb`（骨骼+蒙皮+贴图内嵌）+ `textures/` + `report.json`。
默认导出 **prefab rest 姿态**（= 引擎加载帧渲染的姿态，最忠实）；`--bind-pose` 为可选吸附模式，仅适用单骨架且 rest 姿态损坏的特例（如 krayt）。

`tools/verify_skin.py` 做全库蒙皮复算校验（从 GLB 端重算 GPU 蒙皮，报飞出顶点/坏权重）：

```bash
py tools/verify_skin.py extracted/char_jynerso_pre/char_jynerso_pre.glb   # 单文件
py tools/verify_skin.py                                                    # 批量全库
```

## 5. 踩坑实录（按破坏力排序，全部实测）

### 5.1 UnityPy 压缩网格权重 bug（"拉伸拖尾"元凶）
CompressedMesh 解压 bone weights 时，第 4 权重写成 `1 - sum`（整数）而非 `(31 - sum)/31`。结果：大量顶点权重为负，蒙皮后飞出模型数个单位，渲染成黑色拖尾。必须重算：

```python
s = int(w0 * 31) + int(w1 * 31) + int(w2 * 31)   # 前 3 个 5bit 值
w3 = (31 - s) / 31.0
```

### 5.2 变长骨骼影响数
`MeshHandler.m_BoneIndices` 的行宽不固定：可能是 `(V,2)` 或 `(V,3)`，**不能**当 `(V,4)` 盲写。`len(m_BoneIndices)==m_VertexCount` 的守卫会误通过但只写一半数据（buffer 溢出/截断）。统一 numpy 归一到 `(V,4)`：右侧补零，坏行（负权重或和≠1）按前 3 权重重构后归一化，越界关节索引钳制为 (0, 0,0,0,1)。

### 5.3 共享网格库的骨数失配
`shared_resourcecontainer` 类 bundle 里几十个 SMR 共享网格：同一网格可能同时被 21 骨和 25 骨的同名 GO 渲染器引用。boneWeight 索引直接寻址 bindpose 空间，骨数≠bindpose 数即错位（顶点缩成一团或乱飞）。解法：扫全 bundle 的 SMR，为每个网格选「`len(m_Bones) == len(m_BindPose)`」的骨骼表（见导出器 `best_bones`）。

### 5.4 烘焙顶点色 ≠ 漫反射（"全黑角色"元凶）
149 个角色（ventress/aayla/全部 B1…）在 glTF 查看器里全黑，因为导出了 `COLOR_0`。实测：Ventress 游戏内苍白皮肤，烘焙顶点色 RGB≈0.08——这些顶点色是**运行时染色掩码**，游戏 shader 不乘进漫反射，但 glTF 规范里查看器会把 COLOR_0 乘进 baseColor。**结论：不要导出 COLOR_0**（`tools/patch_strip_vcolor.py` 可就地剥离已导出文件）。

### 5.5 `_shadow` 过滤不生效
`SMR.m_GameObject.deref()` 返回 ObjectReader（无 `m_Name`），拿它查名恒返回 "unnamed"，shadow 过滤形同虚设。必须先 `.parse_as_object()` 再读 `m_Name`。同理 **PPtr 陷阱**：`deref()` → ObjectReader，`deref_parse_as_object()` → 解析对象；跨 bundle 解引用需要把依赖 bundle 一起 load 进 env。缓存 ObjectReader 键不能直接用 reader 本身（不可哈希），用 `(id(assets_file), path_id)`。

### 5.6 bindpose 冲突：吸附是歧途
同一角色不同子网格对共享骨骼的 bindpose 可相差 1.0-2.0（分件绑定所致）。逐网格吸附 bind 会互相覆盖、永远差一点。**唯一正确解：直接导出 prefab rest 姿态**——引擎加载帧渲染的就是它。

### 5.7 Unity→glTF X 镜像
glTF 是右手系、Unity 是左手系，整体镜像 X：顶点/平移 `x` 取反；四元数 `(qx, -qy, -qz, qw)`；IBM 按 `Mx·M·Mx` 变换；三角形反绕；UV 的 `v` 取反（`1 - v`）。少做任何一步都会镜像错位或面片翻背。

## 6. 角色名称溯源（内部 ID → 人名）

本地**没有**本地化数据（安装目录+11,400 bundle 中 TextAsset=0；游戏运行时从 CDN 拉取只存内存）。权威来源是社区镜像 [swgoh-utils/gamedata](https://github.com/swgoh-utils/gamedata)（每次游戏更新后同步；直连 GitHub 会被重置，走 `fastly.jsdelivr.net` 中转）：

- `units.json` / `units_pve.json` 每行带 `unitPrefab: "unit.char_xxx_pre"` —— 与导出文件名精确对应
- 显示名经 `Loc_ENG_US` / `Loc_CHS_CN` 解析（含中文名）

构建脚本：`catalog/build_names.py`；结果：`catalog/character_names.json`（643/747，未匹配项多为镜像滞后的 PVE 美术变体，清单在 `catalog/_unmatched.json`）。注意官方中文译名与民间译法有出入（如 Jyn Erso 官方为「琴·厄索」）。

## 7. 预览

```bash
serve_preview.bat        # http://127.0.0.1:8137
```

浏览器开 `/viewer/viewer.html`：左侧列表支持 内部ID/英文/中文 三路搜索，右侧 3D 视口（线框 W / 骨骼 B / 网格 G）。

## 8. 目录速查

| 想找… | 去 |
|---|---|
| 全部 11,400 bundle 清单 | `catalog/bundles.json` + `BUNDLES.md` |
| 场景/飞船/贴图包全景 | `catalog/other_content.json` + `OTHER_CONTENT.md` |
| 动画导出方案 | `catalog/animations.json` + `ANIMATIONS.md` |
| 角色 ID→名字 | `catalog/character_names.json` + `NAMES.md` |
| 某角色的导出报告 | `extracted/<char>/report.json` |
