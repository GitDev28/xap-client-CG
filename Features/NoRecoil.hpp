#pragma once
#include <iostream>
#include <vector>

#include "../Core/LocalPlayer.hpp"

#include "../Math/Vector2D.hpp"
#include "../Math/QAngle.hpp"
#include "../Math/Resolver.hpp"

#include "../Utils/Memory.hpp"
#include "../Utils/XDisplay.hpp"
#include "../Utils/Config.hpp"

// UI //
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"


struct NoRecoil {
    bool RCSEnabled = true;
    float PitchPower = 30;
    float YawPower = 30;

    XDisplay* X11Display;
    LocalPlayer* localPlayer;
    Vector2D previous_weaponPunchAngles;

    NoRecoil(XDisplay* X11Display, LocalPlayer* localPlayer) {
        this->X11Display = X11Display;
        this->localPlayer = localPlayer;
    }

    void RenderUI() {
        if (ImGui::BeginTabItem("RCS", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton | ImGuiTabItemFlags_NoReorder)) {
            
            ImGui::Checkbox("Recoil Control System (RCS)", &RCSEnabled);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Toggle the Recoil Control System");

            if (RCSEnabled) {
                ImGui::SliderFloat("Pitch", &PitchPower, 1, 50, "%.1f");
                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                    ImGui::SetTooltip("Pitch Power (Up-Down recoil)");
                ImGui::SliderFloat("Yaw", &YawPower, 1, 50, "%.1f");
                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                    ImGui::SetTooltip("Yaw Power (Left-Right Recoil)");
            }

            ImGui::EndTabItem();
        }
    }

    bool Save() {
        try {
            Config::NoRecoil::Enabled = RCSEnabled;
            Config::NoRecoil::PitchPower = PitchPower;
            Config::NoRecoil::YawPower = YawPower;
            return true;
        } catch (...) {
            return false;
        }
    }

    void controlWeapon() {
        if (!NoRecoil::RCSEnabled)return;
        if (!localPlayer->IsCombatReady()) return;
        if (!localPlayer->IsInAttack) return;
        Vector2D punchAnglesDiff = localPlayer->PunchAnglesDifferent;
        if (punchAnglesDiff.IsZeroVector()) return;
        int pitch = (punchAnglesDiff.x > 0)
            ? roundHalfEven(punchAnglesDiff.x * NoRecoil::PitchPower)
            : 0;
        int yaw = roundHalfEven(-punchAnglesDiff.y * NoRecoil::YawPower);
        X11Display->MoveMouse(pitch, yaw);
    }

    int roundHalfEven(float x) {
        return (x >= 0.0)
            ? static_cast<int>(std::round(x))
            : static_cast<int>(std::round(-x)) * -1;
    }
};