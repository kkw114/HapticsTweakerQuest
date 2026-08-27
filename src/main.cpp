#include "main.hpp"

#include <exception>

#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "bsml/shared/BSML.hpp"
#include "custom-types/shared/register.hpp"
#include "scotland2/shared/modloader.h"

#include "HapticsTweaker.hpp"
#include "HapticsTweakerConfig.hpp"
#include "logger.hpp"
#include "modInfo.hpp"

/// @brief Called at the early stages of game loading
/// @param info The mod info.  Update this with your mod's info.
/// @return
MOD_EXPORT_FUNC void setup(CModInfo& info) {
    // Convert the mod info to a C struct and set that as the modloader info.
    info = modInfo.to_c();

    // Initialize the config
    getHapticsTweakerConfig().Init(modInfo);

    Logger.info("Completed setup!");
}

/// @brief Called later on in the game loading - a good time to install function hooks.
///        注意：这台设备的 scotland2 加载器对普通模组（Modloader/mods/）只调用
///        setup() 和 late_load()，从不调用 load()（load 仅用于 early_mods）。
/// @return
MOD_EXPORT_FUNC void late_load() {
    Logger.info("late_load() 开始");
    // Initialize il2cpp functions
    il2cpp_functions::Init();
    Logger.info("il2cpp_functions::Init 完成");
    custom_types::Register::AutoRegister();
    Logger.info("AutoRegister 完成");
    BSML::Init();
    Logger.info("BSML::Init 完成");

    // 注册模组设置界面（Mod Settings）与主菜单按钮（汉化）
    try {
        auto settingsRegistered = BSML::Register::RegisterSettingsMenu("触觉微调", DidActivate, true);
        Logger.info("RegisterSettingsMenu 返回 {}", settingsRegistered);
        BSML::Register::RegisterMainMenuViewControllerMethod("触觉微调", "触觉微调", "管理音符与弧线触觉反馈", DidActivate);
        Logger.info("RegisterMainMenuViewControllerMethod 完成");
    } catch (std::exception const& e) {
        Logger.error("BSML 注册异常: {}", e.what());
    } catch (...) {
        Logger.error("BSML 注册未知异常");
    }

    // 安装触觉 Hook
    HapticsTweaker::InstallHooks();
    Logger.info("late_load() 完成");
}
