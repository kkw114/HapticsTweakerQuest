# HapticsTweakerQuest

**Beat Saber Quest 版「触觉微调」——高度可配置的手柄触觉反馈模组（PC 移植 + 简体中文汉化）**

移植自 PC 模组 [HapticsTweaker](https://github.com/BlqzingIce/HapticsTweaker)，基于 Beat Saber Quest **1.40.6_6407**（bs-cordl 4008.0.0 类型转储）构建，界面与配置全部汉化。

## 原模组信息

| 项目 | 内容 |
|---|---|
| 原模组名称 | HapticsTweaker |
| 原版本 | 1.1.0 |
| 原作者 | BlqzingIce |
| 原仓库 | <https://github.com/BlqzingIce/HapticsTweaker> |
| 原许可 | [MIT](LICENSE)（© 2024 BlqzingIce，本仓库保留其版权声明） |

> 本仓库是第三方**移植 + 汉化**作品，并非原作者发布。感谢原作者 BlqzingIce 的开源贡献。

## 功能

高度可配置的手柄触觉反馈，可自定义以下振动的**时长（秒）**与**强度（0-2，1 为游戏默认，超过 1 视硬件支持而定）**：

| 类型 | 时长 | 强度 |
|---|---|---|
| 普通音符 | ✔（0–0.2s，步进 0.001） | ✔（0–2，步进 0.05） |
| 坏切 | ✔ | ✔ |
| 链头 | ✔ | ✔ |
| 链节 | ✔ | ✔ |
| 炸弹 | ✔ | ✔ |
| 弧线 | —（连续振动） | ✔ |
| 光剑碰撞 | —（连续振动） | ✔ |
| 墙体碰撞 | —（连续振动） | ✔ |

- 将时长或强度设为 0 可关闭该类型触觉反馈
- 设置界面内置 8 个「测试」按钮，可立即试振
- `EnableMod` 开关可一键关闭全部模组功能
- 音符触觉继承原版预设的振动频率，默认强度下手感与原版一致
- 弧线强度需游戏内开启「弧线触觉反馈」才生效

## 汉化对照

| PC 原文 | 汉化 |
|---|---|
| Haptics Tweaker | 触觉微调 |
| Enable Mod | 启用模组 |
| Normal / Bad Cut / Chain Head / Chain Link / Bomb | 普通音符 / 坏切 / 链头 / 链节 / 炸弹 |
| Duration / Strength | 时长 / 强度 |
| Arc / Saber Clash / Wall Clash | 弧线 / 光剑碰撞 / 墙体碰撞 |

## 移植说明（PC C# → Quest C++）

| PC（SiraUtil/IPA） | Quest（beatsaber-hook） |
|---|---|
| `NoteCutPatch` → `NoteCutHapticEffect.HitNote` 前缀 | `MAKE_HOOK_MATCH(NoteCutHapticEffect_HitNote, ...)` |
| `SliderHapticPatch` → `SliderHapticFeedbackInteractionEffect.Vibrate` 前缀 | `MAKE_HOOK_MATCH(SliderHapticFeedbackInteractionEffect_Vibrate, ...)` |
| `SaberClashPatch` → `SaberClashEffect.Start` 前缀 | `MAKE_HOOK_MATCH(SaberClashEffect_Start, ...)` |
| `WallClashPatch` → `ObstacleSaberSparkleEffectManager.Start` 前缀 | `MAKE_HOOK_MATCH(ObstacleSaberSparkleEffectManager_Start, ...)` |
| BSML `SettingsView.bsml` 标签页 | BSML-Lite `CreateScrollableSettingsContainer` + 滑块/按钮 |
| `saberType.Node()` 扩展 | `SaberTypeExtensions::Node(saberType)` |
| IPA 配置 `UserData/HapticsTweaker.json` | config-utils `Configs/hapticstweaker.json` |

## 构建

依赖：

- QPM（Quest Package Manager）缓存：beatsaber-hook 6.4.2、bs-cordl 4008.0.0、bsml 0.4.55、config-utils 2.0.3、custom-types 0.18.3、paper2_scotland2 4.6.4、scotland2 0.1.6
- Android NDK r27.2.12479018（路径见 `ndkpath.txt`）
- CMake 3.22+ 与 Ninja

```powershell
qpm restore                 # 恢复依赖（生成 extern/）
.\scripts\build.ps1         # 编译 → build/libhapticstweaker.so
.\scripts\copy.ps1          # adb 推送到设备 Modloader/mods/
```

注意：构建目录必须为纯英文路径（中文路径会导致 CMake post-build 脚本编码错误）。

## 部署

1. 将 `build/libhapticstweaker.so` 推送到设备：
   `ModData/com.beatgames.beatsaber/Modloader/mods/`
2. 重启游戏，在主菜单「**触觉微调**」按钮或 Mod Settings 列表中打开设置
3. 配置文件位于 `ModData/com.beatgames.beatsaber/Configs/hapticstweaker.json`

依赖库（BSML、custom-types 等）设备上已有原版，无需额外安装。

## 重要：加载器入口（scotland2）

scotland2 v0.1.x 加载器对普通模组（`Modloader/mods/`）**只调用 `setup()` 和 `late_load()`，从不调用 `load()`**（`load` 仅用于 `early_mods/`）。因此本模组的全部初始化（il2cpp/BSML 初始化、设置注册、Hook 安装）都放在 `late_load()` 中。

## 致谢

- 原 PC 模组：[BlqzingIce/HapticsTweaker](https://github.com/BlqzingIce/HapticsTweaker)
- 移植参考：Beat Saber Quest 1.40.6 模组汉化项目（SmoothedController、ClockMod 等 21 个项目的 qpm/CMake/BSML 模式）
