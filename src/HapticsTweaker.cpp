#include "HapticsTweaker.hpp"

#include "bsml/shared/BSML-Lite/Creation/Buttons.hpp"
#include "bsml/shared/BSML-Lite/Creation/Layout.hpp"
#include "bsml/shared/BSML-Lite/Creation/Settings.hpp"
#include "bsml/shared/BSML-Lite/Creation/Text.hpp"

#include "GlobalNamespace/HapticFeedbackManager.hpp"
#include "Libraries/HM/HMLib/VR/HapticPresetSO.hpp"
#include "TMPro/FontStyles.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/XR/XRNode.hpp"

#include "HapticsTweakerConfig.hpp"
#include "logger.hpp"

using namespace UnityEngine;
using namespace UnityEngine::XR;
using namespace GlobalNamespace;
using namespace Libraries::HM::HMLib::VR;

namespace {
    /// @brief 在设置界面中点击「测试」按钮时，向双手播放一次测试振动
    /// @param duration 振动时长（秒）
    /// @param strength 振动强度（0-2，超过 1 视硬件而定）
    void PlayTestHaptic(float duration, float strength) {
        if (duration <= 0.0f || strength <= 0.0f) {
            Logger.warn("时长或强度为 0，无法测试该触觉反馈");
            return;
        }
        auto manager = UnityEngine::Object::FindObjectOfType<HapticFeedbackManager*>();
        if (!manager) {
            Logger.warn("找不到 HapticFeedbackManager，无法测试");
            return;
        }
        auto preset = HapticPresetSO::New_ctor();
        preset->_continuous = false;
        preset->_duration = duration;
        preset->_strength = strength;
        manager->PlayHapticFeedback(XRNode::RightHand, preset);
        manager->PlayHapticFeedback(XRNode::LeftHand, preset);
    }

    /// @brief 创建指定高度的文本（不指定高度会导致元素重叠）
    HMUI::CurvedTextMeshPro* AddText(UnityEngine::Transform* parent, std::string_view text, TMPro::FontStyles style, float fontSize, float height) {
        auto t = BSML::Lite::CreateText(parent, text, style, fontSize, {0, 0}, {0, height});
        auto layoutElement = t->get_gameObject()->GetComponent<UnityEngine::UI::LayoutElement*>();
        layoutElement->set_preferredHeight(height);
        layoutElement->set_flexibleHeight(0);
        return t;
    }

    /// @brief 创建分组标题
    void AddHeader(UnityEngine::Transform* parent, std::string_view text) {
        AddText(parent, text, TMPro::FontStyles::Normal, 4.5f, 5.5f);
    }

    /// @brief 创建提示文本
    void AddHint(UnityEngine::Transform* parent, std::string_view text) {
        AddText(parent, text, TMPro::FontStyles::Italic, 3.5f, 4.5f);
    }

    /// @brief 创建时长滑块（秒，0 ~ 0.2，步进 0.001）
    void AddDurationSlider(UnityEngine::Transform* parent, std::string_view label, ConfigUtils::ConfigValue<float>& configValue) {
        BSML::Lite::CreateIncrementSetting(parent, label, 3, 0.001f, configValue.GetValue(), true, true, 0.0f, 0.2f, [&configValue](float value) {
            configValue.SetValue(value);
        });
    }

    /// @brief 创建强度滑块（0-2，步进 0.05；超过 1 视硬件支持而定）
    void AddStrengthSlider(UnityEngine::Transform* parent, std::string_view label, ConfigUtils::ConfigValue<float>& configValue) {
        BSML::Lite::CreateIncrementSetting(parent, label, 2, 0.05f, configValue.GetValue(), true, true, 0.0f, 2.0f, [&configValue](float value) {
            configValue.SetValue(value);
        });
    }

    /// @brief 创建一行两个测试按钮（不设高度会导致按钮行重叠）
    void AddTestButtonRow(UnityEngine::Transform* parent, std::string_view leftLabel, std::function<void()> leftAction, std::string_view rightLabel, std::function<void()> rightAction) {
        auto row = BSML::Lite::CreateHorizontalLayoutGroup(parent);
        auto rowLayoutElement = row->get_gameObject()->AddComponent<UnityEngine::UI::LayoutElement*>();
        rowLayoutElement->set_preferredHeight(10.0f);
        rowLayoutElement->set_flexibleHeight(0);

        BSML::Lite::CreateUIButton(row->get_transform(), leftLabel, DEFAULT_BUTTONTEMPLATE, {0, 0}, {50, 9}, leftAction);
        BSML::Lite::CreateUIButton(row->get_transform(), rightLabel, DEFAULT_BUTTONTEMPLATE, {0, 0}, {50, 9}, rightAction);
    }
}  // namespace

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (!firstActivation) {
        return;
    }

    auto& config = getHapticsTweakerConfig();

    auto container = BSML::Lite::CreateScrollableSettingsContainer(self->get_transform());
    auto t = container->get_transform();

    // ===== 常规 =====
    AddText(t, "触觉微调", TMPro::FontStyles::Normal, 6.0f, 7.0f);
    BSML::Lite::CreateToggle(t, "启用模组", config.EnableMod.GetValue(), [](bool value) {
        getHapticsTweakerConfig().EnableMod.SetValue(value);
    });
    AddHint(t, "时长：手柄振动的持续时间（秒）");
    AddHint(t, "强度：手柄振动的力度（0-2，1 为默认，超过 1 视硬件而定）");
    AddHint(t, "将时长或强度设为 0 将关闭该类型触觉反馈");

    // ===== 普通音符 =====
    AddHeader(t, "―― 普通音符 ――");
    AddDurationSlider(t, "普通音符时长", config.NormalHapticDuration);
    AddStrengthSlider(t, "普通音符强度", config.NormalHapticStrength);
    AddDurationSlider(t, "坏切时长", config.BadCutHapticDuration);
    AddStrengthSlider(t, "坏切强度", config.BadCutHapticStrength);
    AddTestButtonRow(t, "普通音符测试", []() {
        auto& c = getHapticsTweakerConfig();
        PlayTestHaptic(c.NormalHapticDuration.GetValue(), c.NormalHapticStrength.GetValue());
    }, "坏切测试", []() {
        auto& c = getHapticsTweakerConfig();
        PlayTestHaptic(c.BadCutHapticDuration.GetValue(), c.BadCutHapticStrength.GetValue());
    });

    // ===== 链音符 =====
    AddHeader(t, "―― 链音符 ――");
    AddDurationSlider(t, "链头时长", config.ChainHeadHapticDuration);
    AddStrengthSlider(t, "链头强度", config.ChainHeadHapticStrength);
    AddDurationSlider(t, "链节时长", config.ChainLinkHapticDuration);
    AddStrengthSlider(t, "链节强度", config.ChainLinkHapticStrength);
    AddTestButtonRow(t, "链头测试", []() {
        auto& c = getHapticsTweakerConfig();
        PlayTestHaptic(c.ChainHeadHapticDuration.GetValue(), c.ChainHeadHapticStrength.GetValue());
    }, "链节测试", []() {
        auto& c = getHapticsTweakerConfig();
        PlayTestHaptic(c.ChainLinkHapticDuration.GetValue(), c.ChainLinkHapticStrength.GetValue());
    });

    // ===== 炸弹 / 弧线 =====
    AddHeader(t, "―― 炸弹 / 弧线 ――");
    AddDurationSlider(t, "炸弹时长", config.BombHapticDuration);
    AddStrengthSlider(t, "炸弹强度", config.BombHapticStrength);
    AddHint(t, "弧线强度需要游戏内开启弧线触觉反馈才生效");
    AddStrengthSlider(t, "弧线强度", config.ArcHapticStrength);
    AddTestButtonRow(t, "炸弹测试", []() {
        auto& c = getHapticsTweakerConfig();
        PlayTestHaptic(c.BombHapticDuration.GetValue(), c.BombHapticStrength.GetValue());
    }, "弧线测试", []() {
        auto& c = getHapticsTweakerConfig();
        PlayTestHaptic(1.0f, c.ArcHapticStrength.GetValue());
    });

    // ===== 碰撞 =====
    AddHeader(t, "―― 碰撞 ――");
    AddStrengthSlider(t, "光剑碰撞强度", config.SaberClashHapticStrength);
    AddStrengthSlider(t, "墙体碰撞强度", config.WallClashHapticStrength);
    AddTestButtonRow(t, "光剑碰撞测试", []() {
        auto& c = getHapticsTweakerConfig();
        PlayTestHaptic(1.0f, c.SaberClashHapticStrength.GetValue());
    }, "墙体碰撞测试", []() {
        auto& c = getHapticsTweakerConfig();
        PlayTestHaptic(1.0f, c.WallClashHapticStrength.GetValue());
    });

    Logger.info("触觉微调设置界面已创建");
}
