# HapticsTweakerQuest

**Beat Saber Quest 版「触觉微调」——高度可配置的手柄触觉反馈模组（PC 移植 + 简体中文汉化）**

移植自 PC 模组 [HapticsTweaker](https://github.com/BlqzingIce/HapticsTweaker)，基于 Beat Saber Quest **1.40.6_6407**（bs-cordl 4008.0.0 类型转储）构建，界面与配置全部汉化。

**A Beat Saber Quest mod for highly configurable controller haptics (PC port + Simplified Chinese localization).**

Ported from the PC mod [HapticsTweaker](https://github.com/BlqzingIce/HapticsTweaker), built against Beat Saber Quest **1.40.6_6407** (bs-cordl 4008.0.0 type dump), with fully localized UI and config.

---

## 原模组信息 / Original Mod

| 项目 / Item | 内容 / Content |
|---|---|
| 原模组名称 / Name | HapticsTweaker |
| 原版本 / Version | 1.1.0 |
| 原作者 / Author | BlqzingIce |
| 原仓库 / Repository | <https://github.com/BlqzingIce/HapticsTweaker> |
| 原许可 / License | [MIT](LICENSE)（© 2024 BlqzingIce，本仓库保留其版权声明） |

> 本仓库是第三方**移植 + 汉化**作品，并非原作者发布。感谢原作者 BlqzingIce 的开源贡献。
>
> This repository is a third-party **port + localization**, not released by the original author. Thanks to BlqzingIce for the open-source contribution.

---

## 功能 / Features

高度可配置的手柄触觉反馈，可自定义以下振动的**时长（秒）**与**强度（0-2，1 为游戏默认，超过 1 视硬件支持而定）**：

Fully customizable haptics. **Duration (seconds)** and **strength (0-2; 1 = game default, above 1 depends on hardware)**:

| 类型 / Type | 时长 / Duration | 强度 / Strength |
|---|---|---|
| 普通音符 / Normal Notes | ✔（0–0.2s，步进 0.001） | ✔（0–2，步进 0.05） |
| 坏切 / Bad Cuts | ✔ | ✔ |
| 链头 / Chain Heads | ✔ | ✔ |
| 链节 / Chain Links | ✔ | ✔ |
| 炸弹 / Bombs | ✔ | ✔ |
| 弧线 / Arcs | —（连续振动 / continuous） | ✔ |
| 光剑碰撞 / Saber Clashes | —（连续振动 / continuous） | ✔ |
| 墙体碰撞 / Wall Clashes | —（连续振动 / continuous） | ✔ |

- 将时长或强度设为 0 可关闭该类型触觉反馈 / Set duration or strength to 0 to disable that haptic
- 设置界面内置 8 个「测试」按钮，可立即试振 / 8 built-in test buttons to preview each haptic
- `EnableMod` 开关可一键关闭全部模组功能 / One toggle to disable the whole mod
- 音符触觉继承原版预设的振动频率，默认强度下手感与原版一致 / Note haptics keep the vanilla preset frequency, so 1.0 feels stock
- 弧线强度需游戏内开启「弧线触觉反馈」才生效 / Arc strength requires the in-game "Arc Haptic Feedback" setting

## 汉化对照 / Localization

| PC 原文 / Original | 汉化 / Chinese |
|---|---|
| Haptics Tweaker | 触觉微调 |
| Enable Mod | 启用模组 |
| Normal / Bad Cut / Chain Head / Chain Link / Bomb | 普通音符 / 坏切 / 链头 / 链节 / 炸弹 |
| Duration / Strength | 时长 / 强度 |
| Arc / Saber Clash / Wall Clash | 弧线 / 光剑碰撞 / 墙体碰撞 |

## 移植说明 / Porting Notes（PC C# → Quest C++）

| PC（SiraUtil/IPA） | Quest（beatsaber-hook） |
|---|---|
| `NoteCutPatch` → `NoteCutHapticEffect.HitNote` prefix | `MAKE_HOOK_MATCH(NoteCutHapticEffect_HitNote, ...)` |
| `SliderHapticPatch` → `SliderHapticFeedbackInteractionEffect.Vibrate` prefix | `MAKE_HOOK_MATCH(SliderHapticFeedbackInteractionEffect_Vibrate, ...)` |
| `SaberClashPatch` → `SaberClashEffect.Start` prefix | `MAKE_HOOK_MATCH(SaberClashEffect_Start, ...)` |
| `WallClashPatch` → `ObstacleSaberSparkleEffectManager.Start` prefix | `MAKE_HOOK_MATCH(ObstacleSaberSparkleEffectManager_Start, ...)` |
| BSML `SettingsView.bsml` tabs | BSML-Lite `CreateScrollableSettingsContainer` + sliders/buttons |
| `saberType.Node()` extension | `SaberTypeExtensions::Node(saberType)` |
| IPA config `UserData/HapticsTweaker.json` | config-utils `Configs/hapticstweaker.json` |

## 构建 / Building

依赖 / Dependencies：

- QPM cache: beatsaber-hook 6.4.2, bs-cordl 4008.0.0, bsml 0.4.55, config-utils 2.0.3, custom-types 0.18.3, paper2_scotland2 4.6.4, scotland2 0.1.6
- Android NDK r27.2.12479018（路径见 `ndkpath.txt` / path in `ndkpath.txt`）
- CMake 3.22+ 与 Ninja / CMake 3.22+ and Ninja

```powershell
qpm restore                 # 恢复依赖 / restore dependencies (creates extern/)
.\scripts\build.ps1         # 编译 → build/libhapticstweaker.so
.\scripts\copy.ps1          # adb 推送到设备 / push to device via adb
```

注意：构建目录必须为纯英文路径（中文路径会导致 CMake post-build 脚本编码错误）。
Note: build paths must be pure ASCII (non-ASCII paths break CMake post-build scripts).

## 部署 / Installation

1. 将 `libhapticstweaker.so` 推送到设备 / Push to device:
   `ModData/com.beatgames.beatsaber/Modloader/mods/`
2. 重启游戏，在主菜单「**触觉微调**」按钮或 Mod Settings 列表中打开设置 / Restart the game and open "触觉微调" from the main menu or the Mod Settings list
3. 配置文件 / Config: `ModData/com.beatgames.beatsaber/Configs/hapticstweaker.json`

依赖库（BSML、custom-types 等）设备上已有原版，无需额外安装。
Required libraries (BSML, custom-types, etc.) are already on device; nothing else to install.

## 重要：加载器入口 / Loader Entry (scotland2)

scotland2 v0.1.x 加载器对普通模组（`Modloader/mods/`）**只调用 `setup()` 和 `late_load()`，从不调用 `load()`**（`load` 仅用于 `early_mods/`）。因此本模组的全部初始化都放在 `late_load()` 中。

scotland2 v0.1.x only calls `setup()` and `late_load()` for regular mods (`Modloader/mods/`) — it never calls `load()` (that is only for `early_mods/`). All initialization lives in `late_load()`.

## 致谢 / Credits

- 原 PC 模组 / Original PC mod: [BlqzingIce/HapticsTweaker](https://github.com/BlqzingIce/HapticsTweaker)
- 移植参考 / Porting references: Beat Saber Quest 1.40.6 模组汉化项目（SmoothedController、ClockMod 等 21 个项目的 qpm/CMake/BSML 模式）
