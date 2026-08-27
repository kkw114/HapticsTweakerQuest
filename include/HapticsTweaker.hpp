#pragma once

#include "HMUI/ViewController.hpp"

namespace HapticsTweaker {
    /// @brief 安装所有触觉相关 Hook
    void InstallHooks();
}  // namespace HapticsTweaker

/// @brief 设置界面 DidActivate 回调（BSML 注册使用）
void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
