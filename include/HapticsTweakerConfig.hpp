#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(HapticsTweakerConfig) {
    CONFIG_VALUE(EnableMod, bool, "EnableMod", true, "启用模组（开关全部模组功能）");
    CONFIG_VALUE(NormalHapticDuration, float, "NormalHapticDuration", 0.13f, "普通音符触觉时长（秒）");
    CONFIG_VALUE(NormalHapticStrength, float, "NormalHapticStrength", 1.0f, "普通音符触觉强度（0-2，1 为默认，超过 1 视硬件而定）");
    CONFIG_VALUE(ChainHeadHapticDuration, float, "ChainHeadHapticDuration", 0.13f, "链头触觉时长（秒）");
    CONFIG_VALUE(ChainHeadHapticStrength, float, "ChainHeadHapticStrength", 1.0f, "链头触觉强度（0-2，1 为默认，超过 1 视硬件而定）");
    CONFIG_VALUE(ChainLinkHapticDuration, float, "ChainLinkHapticDuration", 0.13f, "链节触觉时长（秒）");
    CONFIG_VALUE(ChainLinkHapticStrength, float, "ChainLinkHapticStrength", 1.0f, "链节触觉强度（0-2，1 为默认，超过 1 视硬件而定）");
    CONFIG_VALUE(BadCutHapticDuration, float, "BadCutHapticDuration", 0.13f, "坏切触觉时长（秒）");
    CONFIG_VALUE(BadCutHapticStrength, float, "BadCutHapticStrength", 1.0f, "坏切触觉强度（0-2，1 为默认，超过 1 视硬件而定）");
    CONFIG_VALUE(BombHapticDuration, float, "BombHapticDuration", 0.13f, "炸弹触觉时长（秒）");
    CONFIG_VALUE(BombHapticStrength, float, "BombHapticStrength", 1.0f, "炸弹触觉强度（0-2，1 为默认，超过 1 视硬件而定）");
    CONFIG_VALUE(ArcHapticStrength, float, "ArcHapticStrength", 0.75f, "弧线触觉强度（0-2，需在游戏设置中启用弧线触觉反馈）");
    CONFIG_VALUE(SaberClashHapticStrength, float, "SaberClashHapticStrength", 0.75f, "光剑碰撞触觉强度（0-2）");
    CONFIG_VALUE(WallClashHapticStrength, float, "WallClashHapticStrength", 0.75f, "墙体碰撞触觉强度（0-2）");
};
