#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "IniReader.h"
#include "InputTypes.hpp"

constexpr char ConfigFile[] = "config.ini";

#define WriteSection(key) \
    conf << "[" #key "]" << "\n";
#define WritePair(section, key) \
    conf << #key " = " << Config::section::key << "\n";
#define WriteSectionEnd() conf << "\n";
#define WriteComment(msg) conf << "; " << msg << '\n';

#define ReadPairBool(section, key) \
    Config::section::key = reader.GetBoolean(#section, #key, Config::section::key);
#define ReadPairFloat(section, key) \
    Config::section::key = reader.GetFloat(#section, #key, Config::section::key);
#define ReadPairInt(section, key) \
    Config::section::key = reader.GetInteger(#section, #key, Config::section::key);

namespace Config {
    namespace Aimbot {
        bool Enabled = true;
        bool PredictMovement = true;
        bool PredictBulletDrop = true;
        bool AllowTargetSwitch = false;
        int aimHotKey = 60;
        float FinalDistance = 0;
        float Speed = 40;
        float Smooth = 10;
        float FOV = 7;
        float ZoomScale = 1.2;
        float MinDistance = 1;
        float MaxDistance = 100;
        int HitBox = 2;
    };

    namespace NoRecoil{
        bool Enabled = true;
        float PitchPower = 30;
        float YawPower = 30;
    };

    namespace Sense {
        bool Enabled = true;
        bool ItemGlow = true;
        bool DrawBox = true;
        bool DrawSkeleton = true;
        bool DrawTracers = true;
        bool DrawDistance = true;
        bool DrawSeer = true;
        bool AimedAtOnly = false;
        float MaxDistance = 200;
        float SeerMaxDistance = 200;
        bool ShowSpectators = true;
        bool SpectatorDisablesAA = true;
        bool DrawFOVCircle = true;
        float GameFOV = 120;
    };

    namespace Radar {
        bool miniRadar = true;
        bool bigRadar = true;
        bool MiniMapGuides = true;
        float miniMapRange = 100;
        int minimapradardotsize1 = 7;
        int minimapradardotsize2 = 1;
        int bigMapHotKey = 23;
    };

    namespace Triggerbot {
        bool Enabled = true;
        bool TurboFireEnabled = true;
        float TriggerbotMinRange = 10;
        float TriggerbotMaxRange = 200;
        bool wSentinel = true;
        bool wLongbow = true;
        bool wChargeRfile = false;
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
    };

    namespace TurboFire {
        bool Enabled = true;
        bool wtP2020 = true;
        bool wtG7 = true;
        bool wtHemlock = true;
    };

    namespace ScreenPref {
        unsigned int ScreenOffset_X = 1920;
        unsigned int ScreenOffset_Y = 0;
    };

    namespace GameMode {
        bool threeManSquad = true;
    };
};

void UpdateConfig() {
    std::ofstream conf(ConfigFile);
    if (conf.is_open()) {
        WriteSection(Aimbot);
        WritePair(Aimbot, Enabled);
        WritePair(Aimbot, PredictMovement);
        WritePair(Aimbot, PredictBulletDrop);
        WritePair(Aimbot, AllowTargetSwitch);
        WritePair(Aimbot, aimHotKey);
        WritePair(Aimbot, Speed);
        WritePair(Aimbot, Smooth);
        WritePair(Aimbot, FOV);
        WritePair(Aimbot, ZoomScale);
        WritePair(Aimbot, MinDistance);
        WritePair(Aimbot, MaxDistance);
        WritePair(Aimbot, HitBox);
        WriteSectionEnd();

        WriteSection(NoRecoil);
        WritePair(NoRecoil, Enabled);
        WritePair(NoRecoil, PitchPower);
        WritePair(NoRecoil, YawPower);
        WriteSectionEnd();

        WriteSection(Sense);
        WritePair(Sense, Enabled);
        WritePair(Sense, ItemGlow);
        WritePair(Sense, DrawBox);
        WritePair(Sense, DrawSkeleton);
        WritePair(Sense, DrawTracers);
        WritePair(Sense, DrawDistance);
        WritePair(Sense, DrawSeer);
        WritePair(Sense, AimedAtOnly);
        WritePair(Sense, MaxDistance);
        WritePair(Sense, SeerMaxDistance);
        WritePair(Sense, ShowSpectators);
        WritePair(Sense, SpectatorDisablesAA);
        WritePair(Sense, DrawFOVCircle);
        WritePair(Sense, GameFOV);
        WriteSectionEnd();

        WriteSection(Triggerbot);
        WritePair(Triggerbot, Enabled);
        WritePair(Triggerbot, TriggerbotMinRange);
        WritePair(Triggerbot, TriggerbotMaxRange);
        WritePair(Triggerbot, wSentinel);
        WritePair(Triggerbot, wLongbow);
        WritePair(Triggerbot, wChargeRfile);
        WritePair(Triggerbot, wMozambique);
        WritePair(Triggerbot, wEva8);
        WritePair(Triggerbot, wPeaceKeeper);
        WritePair(Triggerbot, wMastiff);
        WritePair(Triggerbot, wP2020);
        WritePair(Triggerbot, wRE45);
        WritePair(Triggerbot, wAlternator);
        WritePair(Triggerbot, wR99);
        WritePair(Triggerbot, wR301);
        WritePair(Triggerbot, wSpitfire);
        WritePair(Triggerbot, wG7);
        WritePair(Triggerbot, wCar);
        WritePair(Triggerbot, wRampage);
        WritePair(Triggerbot, w3030);
        WritePair(Triggerbot, wHemlock);
        WritePair(Triggerbot, wFlatline);
        WritePair(Triggerbot, wNemesis);
        WritePair(Triggerbot, wVolt);
        WritePair(Triggerbot, wTripleTake);
        WritePair(Triggerbot, wLStar);
        WritePair(Triggerbot, wDevotion);
        WritePair(Triggerbot, wHavoc);
        WritePair(Triggerbot, wWingman);
        WritePair(Triggerbot, wProwler);
        WritePair(Triggerbot, wBocek);
        WritePair(Triggerbot, wKraber);
        WritePair(Triggerbot, wThrowingKnife);
        WriteSectionEnd();

        WriteSection(TurboFire);
        WritePair(TurboFire, wtP2020);
        WritePair(TurboFire, wtG7);
        WritePair(TurboFire, wtHemlock);
        WriteSectionEnd();

        WriteSection(Radar);
        WritePair(Radar, miniRadar);
        WritePair(Radar, bigRadar);
        WritePair(Radar, MiniMapGuides);
        WritePair(Radar, miniMapRange);
        WritePair(Radar, minimapradardotsize1);
        WritePair(Radar, minimapradardotsize2);
        WritePair(Radar, bigMapHotKey);
        WriteSectionEnd();

        WriteSection(GameMode);
        WritePair(GameMode, threeManSquad);
        WriteSectionEnd();

        WriteSection(ScreenPref);
        WritePair(ScreenPref, ScreenOffset_X);
        WritePair(ScreenPref, ScreenOffset_Y);
        WriteSectionEnd();

        conf.close();
    }
}

bool ReadConfig(const std::string &configFile) {
    INIReader reader(configFile);
    if (reader.ParseError() < 0) {
        UpdateConfig();
        return false;
    }
    
    ReadPairBool(Aimbot, Enabled);
    ReadPairBool(Aimbot, PredictMovement);
    ReadPairBool(Aimbot, PredictBulletDrop);
    ReadPairBool(Aimbot, AllowTargetSwitch);
    ReadPairInt(Aimbot, aimHotKey);
    ReadPairBool(Aimbot, FinalDistance);
    ReadPairFloat(Aimbot, Speed);
    ReadPairFloat(Aimbot, Smooth);
    ReadPairFloat(Aimbot, FOV);
    ReadPairFloat(Aimbot, ZoomScale);
    ReadPairFloat(Aimbot, MinDistance);
    ReadPairFloat(Aimbot, MaxDistance);
    ReadPairFloat(Aimbot, MaxDistance);
    ReadPairInt(Aimbot, HitBox);

    ReadPairBool(NoRecoil, Enabled);
    ReadPairFloat(NoRecoil, PitchPower);
    ReadPairFloat(NoRecoil, YawPower);

    ReadPairBool(Sense, Enabled);
    ReadPairBool(Sense, ItemGlow);
    ReadPairBool(Sense, DrawBox);
    ReadPairBool(Sense, DrawSkeleton);
    ReadPairBool(Sense, DrawTracers);
    ReadPairBool(Sense, DrawDistance);
    ReadPairBool(Sense, DrawSeer);
    ReadPairBool(Sense, AimedAtOnly);
    ReadPairFloat(Sense, MaxDistance);
    ReadPairFloat(Sense, SeerMaxDistance);
    ReadPairBool(Sense, ShowSpectators);
    ReadPairBool(Sense, SpectatorDisablesAA);
    ReadPairBool(Sense, DrawFOVCircle);
    ReadPairFloat(Sense, GameFOV);

    ReadPairBool(Triggerbot, Enabled);
    ReadPairFloat(Triggerbot, TriggerbotMinRange);
    ReadPairFloat(Triggerbot, TriggerbotMaxRange);
    ReadPairBool(Triggerbot, wSentinel);
    ReadPairBool(Triggerbot, wLongbow);
    ReadPairBool(Triggerbot, wChargeRfile);
    ReadPairBool(Triggerbot, wMozambique);
    ReadPairBool(Triggerbot, wEva8);
    ReadPairBool(Triggerbot, wPeaceKeeper);
    ReadPairBool(Triggerbot, wMastiff);
    ReadPairBool(Triggerbot, wP2020);
    ReadPairBool(Triggerbot, wRE45);
    ReadPairBool(Triggerbot, wAlternator);
    ReadPairBool(Triggerbot, wR99);
    ReadPairBool(Triggerbot, wR301);
    ReadPairBool(Triggerbot, wSpitfire);
    ReadPairBool(Triggerbot, wG7);
    ReadPairBool(Triggerbot, wCar);
    ReadPairBool(Triggerbot, wRampage);
    ReadPairBool(Triggerbot, w3030);
    ReadPairBool(Triggerbot, wHemlock);
    ReadPairBool(Triggerbot, wFlatline);
    ReadPairBool(Triggerbot, wNemesis);
    ReadPairBool(Triggerbot, wVolt);
    ReadPairBool(Triggerbot, wTripleTake);
    ReadPairBool(Triggerbot, wLStar);
    ReadPairBool(Triggerbot, wDevotion);
    ReadPairBool(Triggerbot, wHavoc);
    ReadPairBool(Triggerbot, wWingman);
    ReadPairBool(Triggerbot, wProwler);
    ReadPairBool(Triggerbot, wBocek);
    ReadPairBool(Triggerbot, wKraber);
    ReadPairBool(Triggerbot, wThrowingKnife);

    ReadPairBool(TurboFire, Enabled);
    ReadPairBool(TurboFire, wtP2020);
    ReadPairBool(TurboFire, wtG7);
    ReadPairBool(TurboFire, wtHemlock);

    ReadPairBool(Radar, miniRadar);
    ReadPairBool(Radar, bigRadar);
    ReadPairBool(Radar, MiniMapGuides);
    ReadPairFloat(Radar, miniMapRange);
    ReadPairInt(Radar, minimapradardotsize1);
    ReadPairInt(Radar, minimapradardotsize2);
    ReadPairInt(Radar, bigMapHotKey);

    ReadPairFloat(ScreenPref, ScreenOffset_X);
    ReadPairFloat(ScreenPref, ScreenOffset_Y);

    UpdateConfig();
    return true;
}