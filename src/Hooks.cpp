#include "beatsaber-hook/shared/utils/hooking.hpp"

#include "GlobalNamespace/HapticFeedbackManager.hpp"
#include "GlobalNamespace/NoteCutHapticEffect.hpp"
#include "GlobalNamespace/ObstacleSaberSparkleEffectManager.hpp"
#include "GlobalNamespace/SaberClashEffect.hpp"
#include "GlobalNamespace/SaberTypeExtensions.hpp"
#include "GlobalNamespace/SliderHapticFeedbackInteractionEffect.hpp"
#include "Libraries/HM/HMLib/VR/HapticPresetSO.hpp"
#include "UnityEngine/XR/XRNode.hpp"

#include "HapticsTweaker.hpp"
#include "HapticsTweakerConfig.hpp"
#include "logger.hpp"

using namespace GlobalNamespace;
using namespace Libraries::HM::HMLib::VR;

// ===== 音符切割触觉（普通音符 / 链头 / 链节 / 坏切 / 炸弹）=====
// 对应 PC 版 NoteCutPatch：替换 NoteCutHapticEffect.HitNote 的预设时长与强度
MAKE_HOOK_MATCH(
    NoteCutHapticEffect_HitNote,
    &NoteCutHapticEffect::HitNote,
    void,
    NoteCutHapticEffect* self,
    ::GlobalNamespace::SaberType saberType,
    ::GlobalNamespace::NoteCutHapticEffect_Type type
) {
    auto& config = getHapticsTweakerConfig();
    if (!config.EnableMod.GetValue()) {
        return NoteCutHapticEffect_HitNote(self, saberType, type);
    }

    static auto preset = HapticPresetSO::New_ctor();
    preset->_continuous = false;

    // 保留原版预设的振动频率，保证默认强度下手感与原版一致
    auto copyFrequency = [](HapticPresetSO* dst, HapticPresetSO* src) {
        if (src) {
            dst->_frequency = src->_frequency;
        }
    };

    using HapticType = ::GlobalNamespace::NoteCutHapticEffect_Type;
    using Unwrapped = HapticType::__NoteCutHapticEffect_Type_Unwrapped;
    switch (static_cast<Unwrapped>(type)) {
        case Unwrapped::__E_Normal:
            copyFrequency(preset, self->_normalPreset.ptr());
            preset->_duration = config.NormalHapticDuration.GetValue();
            preset->_strength = config.NormalHapticStrength.GetValue();
            break;
        case Unwrapped::__E_ArcHead:
            copyFrequency(preset, self->_arcHeadPreset.ptr());
            preset->_duration = config.NormalHapticDuration.GetValue();
            preset->_strength = config.NormalHapticStrength.GetValue();
            break;
        case Unwrapped::__E_ArcTail:
            copyFrequency(preset, self->_arcTailPreset.ptr());
            preset->_duration = config.NormalHapticDuration.GetValue();
            preset->_strength = config.NormalHapticStrength.GetValue();
            break;
        case Unwrapped::__E_ArcHeadAndTail:
            copyFrequency(preset, self->_arcHeadAndTailPreset.ptr());
            preset->_duration = config.NormalHapticDuration.GetValue();
            preset->_strength = config.NormalHapticStrength.GetValue();
            break;
        case Unwrapped::__E_ChainHead:
            copyFrequency(preset, self->_chainHeadPreset.ptr());
            preset->_duration = config.ChainHeadHapticDuration.GetValue();
            preset->_strength = config.ChainHeadHapticStrength.GetValue();
            break;
        case Unwrapped::__E_ChainLink:
            copyFrequency(preset, self->_chainLinkPreset.ptr());
            preset->_duration = config.ChainLinkHapticDuration.GetValue();
            preset->_strength = config.ChainLinkHapticStrength.GetValue();
            break;
        case Unwrapped::__E_BadCut:
            copyFrequency(preset, self->_badCutPreset.ptr());
            preset->_duration = config.BadCutHapticDuration.GetValue();
            preset->_strength = config.BadCutHapticStrength.GetValue();
            break;
        case Unwrapped::__E_Bomb:
            copyFrequency(preset, self->_bombPreset.ptr());
            preset->_duration = config.BombHapticDuration.GetValue();
            preset->_strength = config.BombHapticStrength.GetValue();
            break;
        default:
            Logger.warn("未知触觉类型，回退到原版触觉");
            return NoteCutHapticEffect_HitNote(self, saberType, type);
    }

    if (preset->_duration != 0 && preset->_strength != 0) {
        self->_hapticFeedbackManager->PlayHapticFeedback(SaberTypeExtensions::Node(saberType), preset);
    }
}

// ===== 弧线触觉（Slider 振动）=====
// 对应 PC 版 SliderHapticPatch：替换 SliderHapticFeedbackInteractionEffect.Vibrate 的连续振动强度
MAKE_HOOK_MATCH(
    SliderHapticFeedbackInteractionEffect_Vibrate,
    &SliderHapticFeedbackInteractionEffect::Vibrate,
    void,
    SliderHapticFeedbackInteractionEffect* self
) {
    auto& config = getHapticsTweakerConfig();
    if (!config.EnableMod.GetValue()) {
        return SliderHapticFeedbackInteractionEffect_Vibrate(self);
    }

    self->_hapticPreset->_continuous = true;
    self->_hapticPreset->_duration = 0.01f;
    self->_hapticPreset->_strength = config.ArcHapticStrength.GetValue();
    if (self->_hapticPreset->_strength != 0) {
        self->_hapticFeedbackManager->PlayHapticFeedback(
            SaberTypeExtensions::Node(self->_saberType),
            self->_hapticPreset.ptr()
        );
    }
}

// ===== 光剑碰撞触觉 =====
// 对应 PC 版 SaberClashPatch：替换 SaberClashEffect 的 rumble 预设强度
MAKE_HOOK_MATCH(
    SaberClashEffect_Start,
    &SaberClashEffect::Start,
    void,
    SaberClashEffect* self
) {
    self->_rumblePreset->_strength = 0.75f;
    if (getHapticsTweakerConfig().EnableMod.GetValue()) {
        self->_rumblePreset->_strength = getHapticsTweakerConfig().SaberClashHapticStrength.GetValue();
    }
    SaberClashEffect_Start(self);
}

// ===== 墙体碰撞触觉 =====
// 对应 PC 版 WallClashPatch：替换 ObstacleSaberSparkleEffectManager 的 rumble 预设强度
MAKE_HOOK_MATCH(
    ObstacleSaberSparkleEffectManager_Start,
    &ObstacleSaberSparkleEffectManager::Start,
    void,
    ObstacleSaberSparkleEffectManager* self
) {
    self->_rumblePreset->_strength = 0.75f;
    if (getHapticsTweakerConfig().EnableMod.GetValue()) {
        self->_rumblePreset->_strength = getHapticsTweakerConfig().WallClashHapticStrength.GetValue();
    }
    ObstacleSaberSparkleEffectManager_Start(self);
}

namespace HapticsTweaker {
    void InstallHooks() {
        Logger.info("正在安装 HapticsTweaker 钩子...");
        INSTALL_HOOK(Logger, NoteCutHapticEffect_HitNote);
        INSTALL_HOOK(Logger, SliderHapticFeedbackInteractionEffect_Vibrate);
        INSTALL_HOOK(Logger, SaberClashEffect_Start);
        INSTALL_HOOK(Logger, ObstacleSaberSparkleEffectManager_Start);
        Logger.info("HapticsTweaker 钩子安装完成！");
    }
}  // namespace HapticsTweaker
