#pragma once
#include <iostream>
#include <vector>
#include <set>
#include "../Core/Player.hpp"
#include "../Core/LocalPlayer.hpp"
#include "../Core/Offsets.hpp"
#include "../Utils/Memory.hpp"
#include "../Math/Vector2D.hpp"
#include "../Math/Vector3D.hpp"
#include "../Utils/XDisplay.hpp"
#include "../Utils/Conversion.hpp"
#include "../Utils/Config.hpp"
#include "../Utils/InputTypes.hpp"

// UI //
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

struct Triggerbot {
    bool TriggerbotEnabled = true;
    bool TurboFireEnabled = true;
    float TriggerbotMinRange = 10;
    float TriggerbotMaxRange = 200;

    //Sniper ammo weapons
    int WEAPON_SENTINEL = 1;
    int WEAPON_CHARGE_RIFLE = 83;
    int WEAPON_LONGBOW = 85;
    //Shotgun ammo weapons
    int WEAPON_MOZAMBIQUE = 96;
    int WEAPON_EVA8 = 87;
    int WEAPON_PEACEKEEPER = 103;
    int WEAPON_MASTIFF = 95;
    //Light ammo weapons
    int WEAPON_P2020 = 105;
    int WEAPON_RE45 = 81;
    int WEAPON_ALTERNATOR = 80;
    int WEAPON_R99 = 104;
    int WEAPON_R301 = 0;
    int WEAPON_SPITFIRE = 106;
    int WEAPON_G7 = 89;
    //Heavy ammo weapons
    int WEAPON_CAR = 112;
    int WEAPON_RAMPAGE = 21;
    int WEAPON_3030 = 111;
    int WEAPON_HEMLOCK = 90;
    int WEAPON_FLATLINE = 88;
    //Energy ammo weapons
    int WEAPON_NEMESIS = 113;
    int WEAPON_VOLT = 110;
    int WEAPON_TRIPLE_TAKE = 107;
    int WEAPON_LSTAR = 93;
    int WEAPON_DEVOTION = 84;
    int WEAPON_HAVOC = 86;
    //Legendary ammo weapons
    int WEAPON_WINGMAN = 109;
    int WEAPON_PROWLER = 102;
    int WEAPON_BOCEK = 2;
    int WEAPON_KRABER = 92;
    int WEAPON_THROWING_KNIFE = 163;
    int BUSTER_SWORD_R2R5 = 3;
    int WEAPON_THERMITE_GRENADE = 213;

    // Weapon selection
    bool wSentinel = true;
    bool wLongbow = true;
    bool wChargeRfile = false;
    bool wLongBow = false;
    bool wMozambique = false;
    bool wEva8 = false;
    bool wPeaceKeeper = false;
    bool wMastiff = false;
    bool wP2020 = false;
    bool wRE45 = false;
    bool wAlternator = false;
    bool wR99 = false;
    bool wR301 = false;
    bool wSpitfire = false;
    bool wG7 = true;
    bool wCar = false;
    bool wRampage = true;
    bool w3030 = true;
    bool wHemlock = true;
    bool wFlatline = false;
    bool wNemesis = true;
    bool wVolt = false;
    bool wTripleTake = true;
    bool wLStar = false;
    bool wDevotion = false;
    bool wHavoc = false;
    bool wWingman = true;
    bool wProwler = false;
    bool wBocek = true;
    bool wKraber = true;
    bool wThrowingKnife = true;

    // Definition of auto triggered weapon (TriggerBot)
    //std::set<int> WeaponList = {1, 88, 20, 110, 89, 112, 106, 108, 2, 91, 158};
    std::set<int> WeaponList = {1, 85, 111, 90, 107, 2, 92, 163};
    // Definition of auto triggered weapon (Turbo Fire)
    std::set<int> WeaponListTF = {105};

    XDisplay* X11Display;
    LocalPlayer* Myself;
    std::vector<Player*>* Players;

    Triggerbot(XDisplay* X11Display, LocalPlayer* Myself, std::vector<Player*>* GamePlayers) {
        this->X11Display = X11Display;
        this->Myself = Myself;
        this->Players = GamePlayers;
    }

    void UpdateTriggeredWeaponList() {
        WeaponList.clear();
        if (wSentinel)
            WeaponList.insert(WEAPON_SENTINEL);
        if (wLongbow)
            WeaponList.insert(WEAPON_LONGBOW);
        if (wChargeRfile)
            WeaponList.insert(WEAPON_CHARGE_RIFLE);
        if (wChargeRfile)
            WeaponList.insert(WEAPON_LONGBOW);
        if (wMozambique)
            WeaponList.insert(WEAPON_MOZAMBIQUE);
        if (wEva8)
            WeaponList.insert(WEAPON_EVA8);
        if (wPeaceKeeper)
            WeaponList.insert(WEAPON_PEACEKEEPER);
        if (wMastiff)
            WeaponList.insert(WEAPON_MASTIFF);
        if (wP2020)
            WeaponList.insert(WEAPON_P2020);
        if (wRE45)
            WeaponList.insert(WEAPON_RE45);
        if (wAlternator)
            WeaponList.insert(WEAPON_ALTERNATOR);
        if (wR99)
            WeaponList.insert(WEAPON_R99);
        if (wR301)
            WeaponList.insert(WEAPON_R301);
        if (wSpitfire)
            WeaponList.insert(WEAPON_SPITFIRE);
        if (wG7)
            WeaponList.insert(WEAPON_G7);
        if (wCar)
            WeaponList.insert(WEAPON_CAR);
        if (wRampage)
            WeaponList.insert(WEAPON_RAMPAGE);
        if (w3030)
            WeaponList.insert(WEAPON_3030);
        if (wHemlock)
            WeaponList.insert(WEAPON_HEMLOCK);
        if (wFlatline)
            WeaponList.insert(WEAPON_FLATLINE);
        if (wNemesis)
            WeaponList.insert(WEAPON_NEMESIS);
        if (wVolt)
            WeaponList.insert(WEAPON_VOLT);
        if (wTripleTake)
            WeaponList.insert(WEAPON_TRIPLE_TAKE);
        if (wLStar)
            WeaponList.insert(WEAPON_LSTAR);
        if (wDevotion)
            WeaponList.insert(WEAPON_DEVOTION);
        if (wHavoc)
            WeaponList.insert(WEAPON_HAVOC);
        if (wWingman)
            WeaponList.insert(WEAPON_WINGMAN);
        if (wProwler)
            WeaponList.insert(WEAPON_PROWLER);
        if (wBocek)
            WeaponList.insert(WEAPON_BOCEK);
        if (wKraber)
            WeaponList.insert(WEAPON_KRABER);
        if (wThrowingKnife)
            WeaponList.insert(WEAPON_THROWING_KNIFE);
    }

    void RenderUI() {
        if (ImGui::BeginTabItem("Triggerbot", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton | ImGuiTabItemFlags_NoReorder)) {
            ImGui::Checkbox("P2020 Turbo Fire (Right Click)", &TurboFireEnabled);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Turbo Fire non auto weapon");

            ImGui::Checkbox("Triggerbot", &TriggerbotEnabled);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Will automatically shoot the target\nWill only activate when your crosshair is at target whilst holding down Triggerbot key");

            if (TriggerbotEnabled) {
                ImGui::SliderFloat("Triggerbot Max Range (no zoom)", &TriggerbotMinRange, 0, 20, "%.0f");
                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                    ImGui::SetTooltip("Triggerbot's Maximum activation range (No Zoom).");

                ImGui::SliderFloat("Triggerbot Max Range 9zoom)", &TriggerbotMaxRange, 0, 200, "%.0f");
                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                    ImGui::SetTooltip("Triggerbot's Maximum activation range (Zoom).");

                if (ImGui::CollapsingHeader("Sniper Weapons", nullptr)) {
                    ImGui::Checkbox("Sentinel", &wSentinel);
                    ImGui::SameLine();
                    ImGui::Checkbox("Longbow", &wLongbow);
                    ImGui::SameLine();
                    ImGui::Checkbox("ChargeRfile", &wChargeRfile);
                }

                if (ImGui::CollapsingHeader("Shotgun Weapons", nullptr)) {
                    ImGui::Checkbox("Mozambique", &wMozambique);
                    ImGui::SameLine();
                    ImGui::Checkbox("Eva8", &wEva8);
                    ImGui::SameLine();
                    ImGui::Checkbox("PeaceKeeper", &wPeaceKeeper);
                    ImGui::SameLine();
                    ImGui::Checkbox("Mastiff", &wMastiff);
                }

                if (ImGui::CollapsingHeader("Light Weapons", nullptr)) {
                    ImGui::Checkbox("P2020", &wP2020);
                    ImGui::SameLine();
                    ImGui::Checkbox("RE45", &wRE45);
                    ImGui::SameLine();
                    ImGui::Checkbox("Alternator", &wAlternator);
                    ImGui::Checkbox("R99", &wR99);
                    ImGui::SameLine();
                    ImGui::Checkbox("R301", &wR301);
                    ImGui::SameLine();
                    ImGui::Checkbox("G7", &wG7);
                }

                if (ImGui::CollapsingHeader("Heavy Weapons", nullptr)) {
                    ImGui::Checkbox("Car", &wCar);
                    ImGui::SameLine();
                    ImGui::Checkbox("Rampage", &wRampage);
                    ImGui::SameLine();
                    ImGui::Checkbox("3030", &w3030);
                    ImGui::Checkbox("Hemlock", &wHemlock);
                    ImGui::SameLine();
                    ImGui::Checkbox("Flatline", &wFlatline);
                }

                if (ImGui::CollapsingHeader("Energy Weapons", nullptr)) {
                    ImGui::Checkbox("Nemesis", &wNemesis);
                    ImGui::SameLine();
                    ImGui::Checkbox("Volt", &wVolt);
                    ImGui::SameLine();
                    ImGui::Checkbox("TripleTake", &wTripleTake);
                    ImGui::Checkbox("LStar", &wLStar);
                    ImGui::SameLine();
                    ImGui::Checkbox("Devotion", &wDevotion);
                    ImGui::SameLine();
                    ImGui::Checkbox("Havoc", &wHavoc);
                }

                if (ImGui::CollapsingHeader("Supply Drop Weapons", nullptr)) {
                    ImGui::Checkbox("Wingman", &wWingman);
                    ImGui::SameLine();
                    ImGui::Checkbox("Prowler", &wProwler);
                    ImGui::SameLine();
                    ImGui::Checkbox("Bocek", &wBocek);
                    ImGui::Checkbox("Kraber", &wKraber);
                    ImGui::SameLine();
                    ImGui::Checkbox("ThrowingKnife", &wThrowingKnife);
                }
                
            }
            ImGui::EndTabItem();
            UpdateTriggeredWeaponList();
        }
    }

    bool Save() {
        try {
            Config::Triggerbot::Enabled = TriggerbotEnabled;
            Config::Triggerbot::TurboFireEnabled = TurboFireEnabled;
            Config::Triggerbot::TriggerbotMinRange = TriggerbotMinRange;
            Config::Triggerbot::TriggerbotMaxRange = TriggerbotMaxRange;
            Config::Triggerbot::wSentinel = wSentinel;
            Config::Triggerbot::wLongbow = wLongbow;
            Config::Triggerbot::wChargeRfile = wChargeRfile;
            Config::Triggerbot::wMozambique = wMozambique;
            Config::Triggerbot::wEva8 = wEva8;
            Config::Triggerbot::wPeaceKeeper = wPeaceKeeper;
            Config::Triggerbot::wMastiff = wMastiff;
            Config::Triggerbot::wP2020 = wP2020;
            Config::Triggerbot::wRE45 = wRE45;
            Config::Triggerbot::wAlternator = wAlternator;
            Config::Triggerbot::wR99 = wR99;
            Config::Triggerbot::wR301 = wR301;
            Config::Triggerbot::wSpitfire = wSpitfire;
            Config::Triggerbot::wG7 = wG7;
            Config::Triggerbot::wCar = wCar;
            Config::Triggerbot::wRampage = wRampage;
            Config::Triggerbot::w3030 = w3030;
            Config::Triggerbot::wHemlock = wHemlock;
            Config::Triggerbot::wFlatline = wFlatline;
            Config::Triggerbot::wNemesis = wNemesis;
            Config::Triggerbot::wVolt = wVolt;
            Config::Triggerbot::wTripleTake = wTripleTake;
            Config::Triggerbot::wLStar = wLStar;
            Config::Triggerbot::wDevotion = wDevotion;
            Config::Triggerbot::wHavoc = wHavoc;
            Config::Triggerbot::wWingman = wWingman;
            Config::Triggerbot::wProwler = wProwler;
            Config::Triggerbot::wBocek = wBocek;
            Config::Triggerbot::wKraber = wKraber;
            Config::Triggerbot::wThrowingKnife = wThrowingKnife;
            return true;
        } catch (...) {
            return false;
        }
    }

    void TurboFire() {
        if (!TurboFireEnabled) return;
        if (!Myself->IsCombatReady()) return;
        if (WeaponListTF.find(Myself->WeaponIndex) == WeaponListTF.end()) return;

        while (InputManager::isKeyDown(static_cast<InputKeyType>(InputKeyType::MOUSE_Right))) {
            X11Display->MouseClickLeft();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    void Update() {
        if (!TriggerbotEnabled) return;
        if (!Myself->IsCombatReady()) return;

        if (WeaponList.find(Myself->WeaponIndex) == WeaponList.end()) return;

        const float TB_MIN_RANGE = Conversion::ToGameUnits(TriggerbotMinRange);
        const float TB_MAX_RANGE = Conversion::ToGameUnits(TriggerbotMaxRange);
        const float TB_FINAL_RANGE = (Myself->IsZooming) ? TB_MAX_RANGE : TB_MIN_RANGE;

        for (int i = 0; i < Players->size(); i++) {
            Player* player = Players->at(i);
            if (!player->IsCombatReady()) continue;
            if (!player->IsHostile) continue;
            if (!player->IsAimedAt) continue;
            if (player->DistanceToLocalPlayer <= TB_FINAL_RANGE) {
                //std::cout << "Final Distance: " << TB_FINAL_RANGE << std::endl;
                X11Display->MouseClickLeft();
                break;
            }
        }
    }
};