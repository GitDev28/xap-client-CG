#pragma once
#include <iostream>
#include <vector>
#include "../Core/Player.hpp"
#include "../Core/LocalPlayer.hpp"
#include "../Core/Offsets.hpp"
#include "../Core/GlowMode.hpp"
#include "../Core/Camera.hpp"

#include "../Features/Aimbot.hpp"

#include "../Overlay/Overlay.hpp"
#include "../Overlay/Renderer.hpp"

#include "../Utils/Memory.hpp"
#include "../Utils/Color.hpp"
#include "../Utils/Conversion.hpp"
#include "../Utils/Config.hpp"
#include "../Utils/HitboxType.hpp"

// UI //
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

// Geometry
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI / 180.f) )

struct Sense {
    // Glow
    bool GlowEnabled = true;
    float GlowMaxDistance = 200;

    // Item Glow
    std::vector<GlowMode>* StoredGlowMode = new std::vector<GlowMode>;
    bool ItemGlow = true;

    // Health and Armor, etc...
    bool DrawBox = true;
    bool DrawSkeleton = true;
    float SkeletonThickness = 1.5f;
    bool DrawTracers = true;
    bool DrawDistance = true;
    bool DrawSeer = true;
    bool DrawFOVCircle = true;
    
    bool AimedAtOnly = false;

    float SeerMaxDistance = 200;
    float GameFOV = 120;

    bool ShowSpectators = true;
    bool SpectatorDisablesAA = true;
    
    // Variables
    Camera* GameCamera;
    LocalPlayer* Myself;
    std::vector<Player*>* Players;
    std::chrono::milliseconds LastUpdateTime;
    int TotalSpectators = 0;
    std::vector<std::string> Spectators;


    Sense(std::vector<Player*>* Players, Camera* GameCamera, LocalPlayer* Myself) {
        this->Players = Players;
        this->GameCamera = GameCamera;
        this->Myself = Myself;
    }

    void RenderUI() {
        if (ImGui::BeginTabItem("ESP", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton | ImGuiTabItemFlags_NoReorder)) {
            // Glow //
            ImGui::Checkbox("Glow##ESP", &GlowEnabled);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Toggle Glowing");
            ImGui::SameLine();
            ImGui::Checkbox("Item Glow", &ItemGlow);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("You will have Loba's ability to see rare items");
            ImGui::SliderFloat("Max Distance", &GlowMaxDistance, 0, 1000, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Only those in range will glow");

            ImGui::Separator();

            ImGui::Checkbox("Draw box", &DrawBox);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Draw Box on enemy");
            ImGui::SameLine();
            ImGui::Checkbox("Draw Skeleton", &DrawSkeleton);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Draw Skeleton on enemy");

            ImGui::Separator();

            // Drawings
            ImGui::Checkbox("Draw Tracer##ESP", &DrawTracers);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Draw lines to enemies");
            ImGui::SameLine();
            ImGui::Checkbox("Draw Distance##ESP", &DrawDistance);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Show how far the enemy is");
            ImGui::Checkbox("Draw Health and Armor##ESP", &DrawSeer);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Draw Health Bar and Armor");
            ImGui::Checkbox("Aimed At Only##ESP", &AimedAtOnly);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Only draw those who are locked on by Aim-Assist");
            ImGui::SliderFloat("Draw Distance", &SeerMaxDistance, 0, 1000, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Only draw those in range.");

            ImGui::Separator();

            ImGui::Checkbox("Draw FOV Circle", &DrawFOVCircle);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Draw FOV Circle");
            ImGui::SliderFloat("Game's FOV", &GameFOV, 70, 120, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Your current FOV in Settings");

            ImGui::Separator();

            ImGui::Checkbox("Show Spectators", &ShowSpectators);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Show spectators");

/*            ImGui::Checkbox("Disable Aim Assist if spectator", &SpectatorDisablesAA);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("If someone watches your game, the Aim Assist is automatically disabled. \nIt will re-engage when the spectator leaves");
*/
            ImGui::EndTabItem();
        }
    }
    
    bool Save() {
        try {
            Config::Sense::Enabled = GlowEnabled;
            Config::Sense::ItemGlow = ItemGlow;
            Config::Sense::MaxDistance = GlowMaxDistance;
            Config::Sense::DrawBox = DrawBox;
            Config::Sense::DrawSkeleton = DrawSkeleton;
            Config::Sense::DrawTracers = DrawTracers;
            Config::Sense::DrawDistance = DrawDistance;
            Config::Sense::DrawSeer = DrawSeer;
            Config::Sense::SeerMaxDistance = SeerMaxDistance;
            Config::Sense::AimedAtOnly = AimedAtOnly;
            Config::Sense::ShowSpectators = ShowSpectators;
            Config::Sense::SpectatorDisablesAA = SpectatorDisablesAA;
            Config::Sense::DrawFOVCircle = DrawFOVCircle;
            Config::Sense::GameFOV = GameFOV;
            return true;
        } catch (...) {
            return false;
        }
    }

    void Initialize() {
        //CG Adding more vectors
        for (int placebo = 0; placebo < 71; placebo++) {
            const GlowMode Ehh = { 135, 132, 35, 127 };
            StoredGlowMode->push_back(Ehh);
        }

        const GlowMode FirstStyle = { 135, 135, 128, 64 };
        const GlowMode SecondStyle = { 135, 135, 160, 64 };
        const GlowMode ThirdStyle = { 135, 135, 255, 64 };
        const GlowMode FourthStyle = { 135, 135, 32, 64 };

        StoredGlowMode->push_back(FirstStyle);
        StoredGlowMode->push_back(SecondStyle);
        StoredGlowMode->push_back(ThirdStyle);
        StoredGlowMode->push_back(FourthStyle);
    }

    void RenderDrawings(ImDrawList* Canvas, Aimbot* AimAssistState, LocalPlayer* Myself, Overlay OverlayWindow) {
        int ScreenWidth;
        int ScreenHeight;
        int posX;
        int posY;
        int scOffset;

        OverlayWindow.GetScreenResolution(ScreenWidth, ScreenHeight, posX, posY, scOffset);

        if (ShowSpectators) {
            ImVec2 Center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(ImVec2(0.0f, Center.y), ImGuiCond_Once, ImVec2(0.02f, 0.5f));
            ImGui::SetNextWindowBgAlpha(0.3f);
            ImGui::Begin("Spectators", nullptr, ImGuiWindowFlags_AlwaysAutoResize | 
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoSavedSettings | 
                ImGuiWindowFlags_NoMove | 
                ImGuiWindowFlags_NoInputs | 
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoScrollbar);

            std::chrono::milliseconds Now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
            if (Now >= LastUpdateTime + std::chrono::milliseconds(1500)) {
                int TempTotalSpectators = 0;
                std::vector<std::string> TempSpectators;

                for (int i = 0; i < Players->size(); i++) {
                    Player* p = Players->at(i);
                    if (p->BasePointer == Myself->BasePointer) continue;
                    if (p->GetViewYaw() == Myself->ViewYaw && p->IsDead) {
                        TempTotalSpectators++;
                        TempSpectators.push_back(p->GetPlayerName());
                    }
                }

                TotalSpectators = TempTotalSpectators;
                Spectators = TempSpectators;
                LastUpdateTime = Now;
            }
            ImGui::Text("Spectators: ");
            ImGui::SameLine(); ImGui::TextColored(TotalSpectators > 0 ? ImVec4(1, 0.343, 0.475, 1) : ImVec4(0.4, 1, 0.343, 1), "%d", TotalSpectators);
            if (static_cast<int>(Spectators.size()) > 0) {
                ImGui::Separator();
                for (int i = 0; i < static_cast<int>(Spectators.size()); i++) {
                    ImGui::TextColored(ImVec4(1, 0.343, 0.475, 1), "> %s", Spectators.at(i).c_str());
                }
            }
            ImGui::End();
        }

        // Draw FOV Circle
        if (DrawFOVCircle && Myself->IsCombatReady()) {
            float FOV = std::min(AimAssistState->FOV, AimAssistState->FOV * (AimAssistState->GetFOVScale() * AimAssistState->ZoomScale));
            float Radius = tanf(DEG2RAD(FOV) / 2) / tanf(DEG2RAD(GameFOV) / 2) * ScreenWidth;
            Renderer::DrawCircle(Canvas, Vector2D(ScreenWidth / 2, ScreenHeight / 2), Radius, 40, ImColor(255, 255, 255), 2);
        }

        // Draw lot of things
        for (int i = 0; i < Players->size(); i++) {
            Player* p = Players->at(i);
            if (!p->IsCombatReady() || !p->IsVisible || !p->IsHostile || p->DistanceToLocalPlayer > (Conversion::ToGameUnits(SeerMaxDistance)) || Myself->BasePointer == p->BasePointer) continue;

            if (DrawBox)
	    	{	
		        Vector2D Head, Neck, LowerChest, RightLeg, LeftLeg, boxUL, boxBR;
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Head), Head);
                GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Neck), Neck);
                GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::LowerChest), LowerChest);
                GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::RightLeg), RightLeg);
                GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::LeftLeg), LeftLeg);

                (LeftLeg.x-RightLeg.x) > 0 ? boxUL.x = RightLeg.x: boxUL.x = LeftLeg.x;
                boxUL.y = Head.y-3*(Neck.y-Head.y);
                (LeftLeg.x-RightLeg.x) > 0 ? boxBR.x = LeftLeg.x: boxBR.x = RightLeg.x;
                boxBR.y = LeftLeg.y;

		        if (p->IsLockedOn) 
                    Renderer::DrawBox(Canvas, boxUL, boxBR, ImColor(255,0,0), 2);
		        else if (!p->IsLockedOn)
                    Renderer::DrawBox(Canvas, boxUL, boxBR, ImColor(255, 255, 255), 2);
            }

            //CG REF https://www.unknowncheats.me/forum/apex-legends/606842-xap-client-aimbot-esp-triggerbot-26.html#post3955327
            if (DrawSkeleton)
	    	{	
			 ImColor SkeletonColor;
			 SkeletonColor = ImColor(255, 255, 255);
	    		
	    		if (p->IsLockedOn) 
		        {
		    	    SkeletonColor = ImColor(255, 0, 0);
		        } 
	    		
	    		Vector2D Head,Neck,UpperChest,LowerChest,Stomach,LeftShoulder,LeftElbow,LeftHand,RightShoulder,RightElbow,RightHand,LeftThigh,LeftKnee,LeftLeg,RightThigh,RightKnee,RightLeg;
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Head), Head);
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Neck), Neck);
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::UpperChest), UpperChest);
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::LowerChest), LowerChest);
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Stomach), Stomach);
		        
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::LeftShoulder), LeftShoulder);
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::LeftElbow), LeftElbow);
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::LeftHand), LeftHand);
		        
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::RightShoulder), RightShoulder);
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::RightElbow), RightElbow);
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::RightHand), RightHand);
		        
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::LeftThigh), LeftThigh);
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::LeftKnee), LeftKnee);
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::LeftLeg), LeftLeg);
		        
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::RightThigh), RightThigh);
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::RightKnee), RightKnee);
		        GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::RightLeg), RightLeg);
		
		       
		        //Renderer::DrawCircle(Canvas, Head, 10.0f, 0, SkeletonColor, SkeletonThickness);
		        
	    		Renderer::DrawLine(Canvas, Head, Neck, SkeletonThickness, SkeletonColor);
	     		Renderer::DrawLine(Canvas, Neck, UpperChest, SkeletonThickness, SkeletonColor);
	      		Renderer::DrawLine(Canvas, UpperChest, LowerChest, SkeletonThickness, SkeletonColor);
		        Renderer::DrawLine(Canvas, LowerChest, Stomach, SkeletonThickness, SkeletonColor);
		        
		        Renderer::DrawLine(Canvas, Neck, LeftShoulder, SkeletonThickness, SkeletonColor);
		        Renderer::DrawLine(Canvas, LeftShoulder, LeftElbow, SkeletonThickness, SkeletonColor);
		        Renderer::DrawLine(Canvas, LeftElbow, LeftHand, SkeletonThickness, SkeletonColor);
		        
		        Renderer::DrawLine(Canvas, Neck, RightShoulder, SkeletonThickness, SkeletonColor);
		        Renderer::DrawLine(Canvas, RightShoulder, RightElbow, SkeletonThickness, SkeletonColor);
		        Renderer::DrawLine(Canvas, RightElbow, RightHand, SkeletonThickness, SkeletonColor);
		        
		        Renderer::DrawLine(Canvas, Stomach, LeftThigh, SkeletonThickness, SkeletonColor);
		        Renderer::DrawLine(Canvas, LeftThigh, LeftKnee, SkeletonThickness, SkeletonColor);
		        Renderer::DrawLine(Canvas, LeftKnee, LeftLeg, SkeletonThickness, SkeletonColor);
		        
		        Renderer::DrawLine(Canvas, Stomach, RightThigh, SkeletonThickness, SkeletonColor);
		        Renderer::DrawLine(Canvas, RightThigh, RightKnee, SkeletonThickness, SkeletonColor);
		        Renderer::DrawLine(Canvas, RightKnee, RightLeg, SkeletonThickness, SkeletonColor);
	    	}

            // Tracer [//CG Added Tracer in AimedAtOnly]
            if (DrawTracers && !AimedAtOnly) {
                Vector2D chestScreenPosition;
                GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::UpperChest), chestScreenPosition);
                if (!chestScreenPosition.IsZeroVector()) {
                    int x = (int)(ScreenWidth * 0.5f);
                    Renderer::DrawLine(Canvas, Vector2D(x, ScreenHeight), chestScreenPosition, 1.5f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    Renderer::DrawCircleFilled(Canvas, chestScreenPosition, 2, 10, ImColor(255, 255, 255));
                }
            }

            // Distance [//CG Added Distance in AimedAtOnly]
            if (DrawDistance && !AimedAtOnly) {
                Vector2D originScreenPosition;
                GameCamera->WorldToScreen(p->LocalOrigin.Add(Vector3D(0, 15, 0)), originScreenPosition);
                if (!originScreenPosition.IsZeroVector()) {
                    Renderer::DrawText(Canvas, originScreenPosition.Add(Vector2D(5, 0)), std::to_string((int)Conversion::ToMeters(p->DistanceToLocalPlayer)).c_str(), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), true, true, false);
                }
            }

            // Seer
            if (DrawSeer && !AimedAtOnly) {
                Vector2D headScreenPosition;
                GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Head), headScreenPosition);
                if (!headScreenPosition.IsZeroVector())
                    Renderer::DrawSeer(Canvas, headScreenPosition.x, headScreenPosition.y - 20, p->Shield, p->MaxShield, p->Health);
            }
        }

        // Draw Seer, Distance and Tracer on locked target
        if (AimAssistState->TargetSelected && AimAssistState->CurrentTarget) {

            //CG Added Tracer in AimedAtOnly
            Vector2D chestScreenPosition;
            GameCamera->WorldToScreen(AimAssistState->CurrentTarget->GetBonePosition(HitboxType::UpperChest), chestScreenPosition);
            if (!chestScreenPosition.IsZeroVector()) {
                int x = (int)(ScreenWidth * 0.5f);
                Renderer::DrawLine(Canvas, Vector2D(x, ScreenHeight), chestScreenPosition, 1.5f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                Renderer::DrawCircleFilled(Canvas, chestScreenPosition, 2, 10, ImColor(255, 255, 255));
            }

            if (DrawDistance && !AimedAtOnly) {
                Vector2D originScreenPosition;
                GameCamera->WorldToScreen(AimAssistState->CurrentTarget->LocalOrigin.Add(Vector3D(0, 15, 0)), originScreenPosition);
                if (!originScreenPosition.IsZeroVector()) {
                    Renderer::DrawText(Canvas, originScreenPosition.Add(Vector2D(5, 0)), std::to_string((int)Conversion::ToMeters(AimAssistState->CurrentTarget->DistanceToLocalPlayer)).c_str(), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), true, true, false);
                }
            }

            // Seer
            Vector2D headScreenPosition;
            GameCamera->WorldToScreen(AimAssistState->CurrentTarget->GetBonePosition(HitboxType::Head), headScreenPosition);
            if (headScreenPosition.IsZeroVector())
                return;

            Renderer::DrawSeer(Canvas, headScreenPosition.x, headScreenPosition.y - 20, AimAssistState->CurrentTarget->Shield, AimAssistState->CurrentTarget->MaxShield, AimAssistState->CurrentTarget->Health);
            return;
        }
    }

    void SetGlowState(long HighlightSettingsPointer, long HighlightSize, int HighlightID, GlowMode NewGlowMode) {
        const GlowMode oldGlowMode = Memory::Read<GlowMode>(HighlightSettingsPointer + (HighlightSize * HighlightID) + 0);
        if (NewGlowMode != oldGlowMode)
            Memory::Write<GlowMode>(HighlightSettingsPointer + (HighlightSize * HighlightID) + 0, NewGlowMode);
    }

    void SetColorState(long HighlightSettingsPointer, long HighlightSize, int HighlightID, Color NewColor) {
        const Color oldColor = Memory::Read<Color>(HighlightSettingsPointer + (HighlightSize * HighlightID) + 4);
        if (oldColor != NewColor)
            Memory::Write<Color>(HighlightSettingsPointer + (HighlightSize * HighlightID) + 4, NewColor);
    }

    void SetGlow(Player* Target, int GlowEnabled, int GlowThroughWall, int HighlightID) {
        if (Target->GlowEnable != GlowEnabled) Memory::Write<int>(Target->BasePointer + OFF_GLOW_ENABLE, GlowEnabled);
        if (Target->GlowThroughWall != GlowThroughWall) {
            Memory::Write<int>(Target->BasePointer + OFF_GLOW_THROUGH_WALL, GlowThroughWall);
            Memory::Write<int>(Target->BasePointer + OFF_GLOW_FIX, 2);
        }
        if (Target->HighlightID != HighlightID) Memory::Write<int>(Target->BasePointer + OFF_GLOW_HIGHLIGHT_ID + 1, HighlightID);
    }

    void Update() {
        const long HighlightSettingsPointer = Memory::Read<long>(OFF_REGION + OFF_GLOW_HIGHLIGHTS);
        const long HighlightSize = 0x34;

        // Item Glow //
        if (ItemGlow) {
            //for (int highlightId = 34; highlightId < 39; highlightId++) {
            for (int highlightId = 34; highlightId < 39; highlightId++) {
                const GlowMode newGlowMode = { 137, 138, 35, 127 };
                SetGlowState(HighlightSettingsPointer, HighlightSize, highlightId, newGlowMode);
            }
        } else {
            //for (int highlightId = 34; highlightId < 39; highlightId++) {
            for (int highlightId = 34; highlightId < 39; highlightId++) {
                const GlowMode newGlowMode = StoredGlowMode->at(highlightId);
                SetGlowState(HighlightSettingsPointer, HighlightSize, highlightId, newGlowMode);
            }
        }

        // Player Glow //
        // -> Visible
        const GlowMode VisibleMode = { 2, 6, 32, 127 };
        const Color VisibleColor = { 0.6, 3, 2.04 };
        SetGlowState(HighlightSettingsPointer, HighlightSize, 0, VisibleMode);
        SetColorState(HighlightSettingsPointer, HighlightSize, 0, VisibleColor);

        // -> Invisible
        const GlowMode InvisibleMode = { 2, 6, 32, 100 };
        const Color InvisibleColor = { 4.5, 0.6, 0.6 };
        SetGlowState(HighlightSettingsPointer, HighlightSize, 1, InvisibleMode);
        SetColorState(HighlightSettingsPointer, HighlightSize, 1, InvisibleColor);

        // -> Knocked
        const GlowMode KnockedMode = { 2, 6, 32, 127 };
        const Color KnockedColor = { 1, 1, 0.35 };
        SetGlowState(HighlightSettingsPointer, HighlightSize, 90, KnockedMode);
        SetColorState(HighlightSettingsPointer, HighlightSize, 90, KnockedColor);

        // -> Disabled
        const GlowMode DisabledMode = { 0, 0, 0, 0 };
        const Color DisabledColor = { 1, 1, 1 };
        SetGlowState(HighlightSettingsPointer, HighlightSize, 91, DisabledMode);
        SetColorState(HighlightSettingsPointer, HighlightSize, 91, DisabledColor);

        // -> Locked On
        const GlowMode LockedOnMode = { 136, 6, 32, 127 };
        const Color LockedOnColor = { 0, 0.75, 0.75 };
        SetGlowState(HighlightSettingsPointer, HighlightSize, 92, LockedOnMode);
        SetColorState(HighlightSettingsPointer, HighlightSize, 92, LockedOnColor);


        for (int i = 0; i < Players->size(); i++) {
            Player* Target = Players->at(i);
            if (!Target->IsValid()) continue;
            if (!Target->IsHostile) continue;

            if (GlowEnabled) {
                if (Target->IsLockedOn) {
                    SetGlow(Target, 1, 2, 92);
                    continue;
                }

                if (Target->DistanceToLocalPlayer < Conversion::ToGameUnits(GlowMaxDistance)) {
                    if (Target->IsKnocked) {
                        SetGlow(Target, 1, 2, 90);
                        continue;
                    }

                    int Highlight = (Target->IsVisible) ? 0 : 1;
                    SetGlow(Target, 1, 2, Highlight);
                    continue;
                }
            }

            SetGlow(Target, 0, 0, 91);
        }
    }
};