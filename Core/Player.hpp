#pragma once
#include <string>
#include "Offsets.hpp"
#include "LocalPlayer.hpp"
#include "../Utils/Memory.hpp"
#include "../Utils/HitboxType.hpp"
#include "../Utils/Config.hpp"
#include "../Math/Vector2D.hpp"
#include "../Math/Vector3D.hpp"
#include "../Math/Matrix.hpp"

struct Player {
    LocalPlayer* Myself;

    int Index;
    long BasePointer;

    std::string Name;
    int Team;

    int GlowEnable;
    int GlowThroughWall;
    int HighlightID;

    bool IsDead;
    bool IsKnocked;

    Vector2D ViewAngles;
    Vector3D LocalOrigin;
    Vector3D AbsoluteVelocity;

    int Health;
    int MaxHealth;
    int Shield;
    int MaxShield;

    int LastTimeAimedAt;
    int LastTimeAimedAtPrevious;
    bool IsAimedAt;

    bool nonBR;
    bool friendly;
    int LastVisibleTime;
    int LastTimeVisiblePrevious;
    bool IsVisible;

    bool IsLocal;
    bool IsAlly;
    bool IsHostile;

    float DistanceToLocalPlayer;
    float Distance2DToLocalPlayer;

    float ViewYaw;

    bool IsLockedOn;

    Player(int PlayerIndex, LocalPlayer* Me) {
        this->Index = PlayerIndex;
        this->Myself = Me;
    }

    void Read() {
        BasePointer = Memory::Read<long>(OFF_REGION + OFF_ENTITY_LIST + ((Index + 1) << 5));
        if (BasePointer == 0) return;

        Name = Memory::ReadString(BasePointer + OFF_NAME);
        Team = Memory::Read<int>(BasePointer + OFF_TEAM_NUMBER);

        if (!IsPlayer() && !IsDummy()) { BasePointer = 0; return; }
        IsDead = (IsDummy()) ? false : Memory::Read<short>(BasePointer + OFF_LIFE_STATE) > 0;
        IsKnocked = (IsDummy()) ? false : Memory::Read<short>(BasePointer + OFF_BLEEDOUT_STATE) > 0;

        LocalOrigin = Memory::Read<Vector3D>(BasePointer + OFF_LOCAL_ORIGIN);
        AbsoluteVelocity = Memory::Read<Vector3D>(BasePointer + OFF_ABSVELOCITY);

        GlowEnable = Memory::Read<int>(BasePointer + OFF_GLOW_ENABLE);
        GlowThroughWall = Memory::Read<int>(BasePointer + OFF_GLOW_THROUGH_WALL);
        HighlightID = Memory::Read<int>(BasePointer + OFF_GLOW_HIGHLIGHT_ID + 1);

        ViewAngles = Memory::Read<Vector2D>(BasePointer + OFF_VIEW_ANGLES);
        ViewYaw = Memory::Read<float>(BasePointer + OFF_YAW);

        LastTimeAimedAt = Memory::Read<int>(BasePointer + OFF_LAST_AIMEDAT_TIME);
        IsAimedAt = LastTimeAimedAtPrevious < LastTimeAimedAt;
        LastTimeAimedAtPrevious = LastTimeAimedAt;

        LastVisibleTime = Memory::Read<int>(BasePointer + OFF_LAST_VISIBLE_TIME);
        IsVisible = IsDummy() || IsAimedAt || LastTimeVisiblePrevious < LastVisibleTime;
        LastTimeVisiblePrevious = LastVisibleTime;

        Health = Memory::Read<int>(BasePointer + OFF_HEALTH);
        MaxHealth = Memory::Read<int>(BasePointer + OFF_MAXHEALTH);
        Shield = Memory::Read<int>(BasePointer + OFF_SHIELD);
        MaxShield = Memory::Read<int>(BasePointer + OFF_MAXSHIELD);

        if (Myself->IsValid()) {
            IsLocal = Myself->BasePointer == BasePointer;

            nonBR = !Config::GameMode::threeManSquad;
            friendly = (nonBR)
                ? (Myself->Team % 2 == 0 && Team % 2 == 0) || (Myself->Team % 2 != 0 && Team % 2 != 0)
                : Myself->Team == Team;

            IsAlly = Myself->Team == Team;
            IsHostile = !IsAlly;
            DistanceToLocalPlayer = Myself->LocalOrigin.Distance(LocalOrigin);
            Distance2DToLocalPlayer = Myself->LocalOrigin.To2D().Distance(LocalOrigin.To2D());
        }
    }

    void setCustomGlow(int health, bool isVisible, bool isSameTeam)
    {
        static const int contextId = 1;
        long basePointer = BasePointer;
        int settingIndex = 65;
        std::array<unsigned char, 4> highlightFunctionBits = {
            0,   // InsideFunction
            125, // OutlineFunction: HIGHLIGHT_OUTLINE_OBJECTIVE
            64,  // OutlineRadius: size * 255 / 8
            64   // (EntityVisible << 6) | State & 0x3F | (AfterPostProcess << 7)
        };
        std::array<float, 4> glowColorRGB = { 0, 0, 0, 0 };
        if (isSameTeam) {
            settingIndex = 20;
        } else if (!isVisible) {
            settingIndex = 65;
            glowColorRGB = { 0.5, 0.5, 0.5, 1 }; // knocked enemies // gray
        } else if (health >= 205) {
            settingIndex = 66;
            glowColorRGB = { 1, 0, 0, 1 }; // red shield
        } else if (health >= 190) {
            settingIndex = 67;
            glowColorRGB = { 0.5, 0, 0.5, 1 }; // purple shield
        } else if (health >= 170) {
            settingIndex = 68;
            glowColorRGB = { 0, 0.5, 1, 1 }; // blue shield
        } else if (health >= 95) {
            settingIndex = 69;
            glowColorRGB = { 0, 1, 0.5, 1 }; // gray shield // cyan 
        } else {
            settingIndex = 70;
            glowColorRGB = { 0, 0.5, 0, 1 }; // low health enemies // green
        }
        Memory::Write<unsigned char>(basePointer + OFF_GLOW_HIGHLIGHT_ID + contextId, settingIndex);
        if (!isSameTeam) {
            Memory::Write<typeof(highlightFunctionBits)>(
                Myself->highlightSettingsPtr + HIGHLIGHT_TYPE_SIZE * settingIndex + 4, highlightFunctionBits);
            Memory::Write<typeof(glowColorRGB)>(
                Myself->highlightSettingsPtr + HIGHLIGHT_TYPE_SIZE * settingIndex + 8, glowColorRGB);
            Memory::Write<int>(basePointer + OFF_GLOW_FIX, 1);
        }   
    }

    std::string GetPlayerName(){
        uintptr_t NameIndex = Memory::Read<uintptr_t>(BasePointer + OFF_NAME_INDEX);
        uintptr_t NameOffset = Memory::Read<uintptr_t>(OFF_REGION + OFF_NAME_LIST + ((NameIndex - 1) << 4 ));
        std::string PlayerName = Memory::ReadPlayerName(NameOffset, 64);
        return PlayerName;
    }

    std::string getPlayerModelName(){
        uintptr_t modelOffset = Memory::Read<uintptr_t>(BasePointer + OFF_MODELNAME);
        std::string modelName = Memory::ReadString(modelOffset, 1024);
        // Check for different player names
        if (modelName.find("dummie") != std::string::npos) modelName = "DUMMIE";
        else if (modelName.find("ash") != std::string::npos) modelName = "ASH";
        else if (modelName.find("ballistic") != std::string::npos) modelName = "BALLISTIC";
        else if (modelName.find("bangalore") != std::string::npos) modelName = "BANGALORE";
        else if (modelName.find("bloodhound") != std::string::npos) modelName = "BLOODHOUND";
        else if (modelName.find("catalyst") != std::string::npos) modelName = "CATALYST";
        else if (modelName.find("caustic") != std::string::npos) modelName = "CAUSTIC";
        else if (modelName.find("conduit") != std::string::npos) modelName = "CONDUIT";
        else if (modelName.find("crypto") != std::string::npos) modelName = "CRYPTO";
        else if (modelName.find("fuse") != std::string::npos) modelName = "FUSE";
        else if (modelName.find("gibraltar") != std::string::npos) modelName = "GIBRALTAR";
        else if (modelName.find("horizon") != std::string::npos) modelName = "HORIZON";
        else if (modelName.find("nova") != std::string::npos) modelName = "HORIZON";
        else if (modelName.find("holo") != std::string::npos) modelName = "MIRAGE";
        else if (modelName.find("mirage") != std::string::npos) modelName = "MIRAGE";
        else if (modelName.find("lifeline") != std::string::npos) modelName = "LIFELINE";
        else if (modelName.find("loba") != std::string::npos) modelName = "LOBA";
        else if (modelName.find("madmaggie") != std::string::npos) modelName = "MADMAGGIE";
        else if (modelName.find("newcastle") != std::string::npos) modelName = "NEWCASTLE";
        else if (modelName.find("octane") != std::string::npos) modelName = "OCTANE";
        else if (modelName.find("pathfinder") != std::string::npos) modelName = "PATHFINDER";
        else if (modelName.find("rampart") != std::string::npos) modelName = "RAMPART";
        else if (modelName.find("revenant") != std::string::npos) modelName = "REVENANT";
        else if (modelName.find("seer") != std::string::npos) modelName = "SEER";
        else if (modelName.find("stim") != std::string::npos) modelName = "OCTANE";
        else if (modelName.find("valkyrie") != std::string::npos) modelName = "VALKYRIE";
        else if (modelName.find("vantage") != std::string::npos) modelName = "VANTAGE";
        else if (modelName.find("wattson") != std::string::npos) modelName = "WATTSON";
        else if (modelName.find("wraith") != std::string::npos) modelName = "WRAITH";
        
        return modelName;
    }
    
    float GetViewYaw() {
        if (!IsDummy() || IsPlayer()) {
            return Memory::Read<float>(BasePointer + OFF_YAW);
        }
        return 0.0f;
    }

    bool IsValid() {
        return BasePointer != 0 && Health > 0 && (IsPlayer() || IsDummy());
    }

    bool IsCombatReady() {
        if (!IsValid())return false;
        if (IsDummy()) return true;
        if (IsDead) return false;
        if (IsKnocked) return false;
        return true;
    }

    bool IsPlayer() {
        return Name == "player";
    }

    bool IsDummy() {
        return Team == 97;
    }

    // Bones //
    int GetBoneFromHitbox(HitboxType HitBox) const {
        long ModelPointer = Memory::Read<long>(BasePointer + OFF_STUDIOHDR);
        if (!Memory::IsValidPointer(ModelPointer))
            return -1;

        long StudioHDR = Memory::Read<long>(ModelPointer + 0x8);
        if (!Memory::IsValidPointer(StudioHDR + 0x34))
            return -1;

        auto HitboxCache = Memory::Read<uint16_t>(StudioHDR + 0x34);
        auto HitboxArray = StudioHDR + ((uint16_t)(HitboxCache & 0xFFFE) << (4 * (HitboxCache & 1))); 
        if (!Memory::IsValidPointer(HitboxArray + 0x4))
            return -1;

        auto IndexCache = Memory::Read<uint16_t>(HitboxArray + 0x4);
        auto HitboxIndex = ((uint16_t)(IndexCache & 0xFFFE) << (4 * (IndexCache & 1)));
        auto BonePointer = HitboxIndex + HitboxArray + (static_cast<int>(HitBox) * 0x20);
        if (!Memory::IsValidPointer(BonePointer))
            return -1;
        
        return Memory::Read<uint16_t>(BonePointer);
    }

    Vector3D GetBonePosition(HitboxType HitBox) const {
        Vector3D Offset = Vector3D(0.0f, 0.0f, 0.0f);

        int Bone = GetBoneFromHitbox(HitBox);
        if (Bone < 0 || Bone > 255)
            return LocalOrigin.Add(Offset);

        long BonePtr = Memory::Read<long>(BasePointer + OFF_BONES);
        BonePtr += (Bone * sizeof(Matrix3x4));
        if (!Memory::IsValidPointer(BonePtr))
            return LocalOrigin.Add(Offset);

        Matrix3x4 BoneMatrix = Memory::Read<Matrix3x4>(BonePtr);
        Vector3D BonePosition = BoneMatrix.GetPosition();

        if (!BonePosition.IsValid())
            return LocalOrigin.Add(Vector3D(0, 0, 0));

        BonePosition += LocalOrigin;
        return BonePosition;
    }
};