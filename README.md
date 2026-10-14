# galaxy-of-homos

SWGoH（Star Wars: Galaxy of Heroes）资源恢复库。工程源文件丢失后，从本机游戏安装与 Unity 资源缓存中逆向恢复的全部资产。**私有仓库 — 仅限项目团队内部使用。**

## 结构

| 目录 | 内容 | 体量 |
|---|---|---|
| `raw_bundles/` | 原始资源：Unity AssetBundle 缓存全量镜像（11,400 个 bundle / 25,465 个文件，UnityFS 明文格式，游戏版本 0.40.7）| ~2.7 GB |
| `extracted/` | 模型资源：747 个角色预制体导出的 GLB（骨骼+蒙皮+贴图）+ 独立贴图 + report.json + 预览页 | ~890 MB |
| `catalog/` | 解包内容：全缓存逆向清单（bundles/other_content/animations）+ 角色名称溯源（643/747 → defId/英文/中文名）+ gamedata 单位表 | ~128 MB |
| `viewer/` + `models.json` | 离线预览器（three.js，`serve_preview.bat` 后访问 /viewer/viewer.html），列表支持中/英/内部 ID 搜索 | <1 MB |
| `tools/` | 导出器与校验器（`swgoh_extract.py` / `verify_skin.py` / 补丁脚本）| — |

**从零复现全流程见 [UNPACK_GUIDE.md](UNPACK_GUIDE.md)**（环境、bundle 定位、最小示例、完整导出、踩坑实录、名称溯源）。

## 角色名称溯源链

```
char_<name>_pre (内部 ID) → defId → name_en / name_zh
详见 catalog/character_names.json 与 catalog/NAMES.md
```

导出管线要点（UnityPy 1.25.4）：Unity→glTF X 镜像、压缩网格权重修复、变长骨骼影响数归一、共享网格库骨数优选；烘焙顶点色为运行时染色掩码、非漫反射，故不导出。其余见 catalog/ 内各 .md 说明。

## 原始 bundle 定位

`raw_bundles/<bundle_name>/<hash>/__data` — 同名多 hash 时取 mtime 最新的 `__data`（UnityFS，可直接用 UnityPy 打开）。
