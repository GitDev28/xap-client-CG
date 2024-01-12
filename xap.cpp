// Externals
#include <iostream>
#include <string>
#include <unistd.h>
#include <atomic>
#include <vector>
#include <chrono>
#include <thread>
#include <GLFW/glfw3.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XInput2.h>

// Internals
#include "Core/Level.hpp"
#include "Core/Player.hpp"
#include "Core/LocalPlayer.hpp"
#include "Core/Camera.hpp"

#include "Features/Aimbot.hpp"
#include "Features/Sense.hpp"
#include "Features/Triggerbot.hpp"
#include "Features/Radar.hpp"
#include "Features/NoRecoil.hpp"

#include "Overlay/Overlay.hpp"

#include "Utils/Config.hpp"
#include "Utils/Memory.hpp"
#include "Utils/XDisplay.hpp"

// Objects
XDisplay* X11Display = new XDisplay();
Overlay OverlayWindow = Overlay();
ImDrawList* Canvas;

// Game Objects
Level* Map = new Level();
LocalPlayer* Myself = new LocalPlayer();
Camera* GameCamera = new Camera();

// Players
std::vector<Player*>* HumanPlayers = new std::vector<Player*>;
std::vector<Player*>* Dummies = new std::vector<Player*>;
std::vector<Player*>* Players = new std::vector<Player*>;

// Features
Sense* ESP = new Sense(Players, GameCamera, Myself);
Aimbot* AimAssist = new Aimbot(X11Display, Myself, Players);
Triggerbot* Trigger = new Triggerbot(X11Display, Myself, Players);
Radar *MapRadar = new Radar(X11Display, Players, GameCamera, Map, Myself);
NoRecoil *Rec = new NoRecoil(X11Display,Myself);

// Booleans and Variables
bool IsMenuOpened = true;
bool multiScreen = true;
float ScreenOffset_X = 0;
float ScreenOffset_Y = 0;

// Thread
std::atomic_bool StopThread(false);

// Inputs
void HandleKeyEvent(Display* display, XEvent* Event) {
    if (Event->type == KeyPress) {
        if (IsMenuOpened) {
            IsMenuOpened = false;
            OverlayWindow.CaptureInput(false);
        } else {
            IsMenuOpened = true;
            OverlayWindow.CaptureInput(true);
        }
    }
}

void X11EventListener() {
    Display* display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(display);
    XGrabKey(display, XKeysymToKeycode(display, XK_Insert), AnyModifier, root, False, GrabModeAsync, GrabModeAsync);
    XEvent event;
    while (!StopThread) {
        XNextEvent(display, &event);
        HandleKeyEvent(display, &event);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    XCloseDisplay(display);
}

// Overlay
bool InitializeOverlayWindow() {
    if (!OverlayWindow.InitializeOverlay()) {
        OverlayWindow.DestroyOverlay();
        return false;
    }
    int ScreenWidth;
    int ScreenHeight;
    int posX;
    int posY;
    int ScOffset;

    OverlayWindow.GetScreenResolution(ScreenWidth, ScreenHeight, posX, posY, ScOffset);
    GameCamera->Initialize(ScreenWidth, ScreenHeight);
    std::cout << "Overlay initialized (" << ScreenWidth << " x " << ScreenHeight << ")" << std::endl;
    return true;
}

// Config
void LoadConfig() {
    bool success = ReadConfig("config.ini");
    if (success)
        std::cout << "Successfully read config" << std::endl;
    else
        std::cout << "Can't read/find config file. New config file created." << std::endl;

    // Aimbot //
    AimAssist->AimbotEnabled = Config::Aimbot::Enabled;
    AimAssist->PredictMovement = Config::Aimbot::PredictMovement;
    AimAssist->PredictBulletDrop = Config::Aimbot::PredictBulletDrop;
    AimAssist->AllowTargetSwitch = Config::Aimbot::AllowTargetSwitch;
    AimAssist->aimHotKey = static_cast<InputKeyType>(Config::Aimbot::aimHotKey);
    AimAssist->Speed = Config::Aimbot::Speed;
    AimAssist->Smooth = Config::Aimbot::Smooth;
    AimAssist->FOV = Config::Aimbot::FOV;
    AimAssist->ZoomScale = Config::Aimbot::ZoomScale;
    AimAssist->MinDistance = Config::Aimbot::MinDistance;
    AimAssist->MaxDistance = Config::Aimbot::MaxDistance;
    AimAssist->HitBox = static_cast<HitboxType>(Config::Aimbot::HitBox);


    // ESP //
    ESP->GlowEnabled = Config::Sense::Enabled;
    ESP->ItemGlow = Config::Sense::ItemGlow;
    ESP->GlowMaxDistance = Config::Sense::MaxDistance;
    ESP->DrawBox = Config::Sense::DrawBox;
    ESP->DrawSkeleton = Config::Sense::DrawSkeleton;
    ESP->DrawTracers = Config::Sense::DrawTracers;
    ESP->DrawDistance = Config::Sense::DrawDistance;
    ESP->DrawSeer = Config::Sense::DrawSeer;
    ESP->SeerMaxDistance = Config::Sense::SeerMaxDistance;
    ESP->AimedAtOnly = Config::Sense::AimedAtOnly;
    ESP->ShowSpectators = Config::Sense::ShowSpectators;
    ESP->SpectatorDisablesAA = Config::Sense::SpectatorDisablesAA;
    ESP->DrawFOVCircle = Config::Sense::DrawFOVCircle;
    ESP->GameFOV = Config::Sense::GameFOV;

    // Triggerbot //
    Trigger->TriggerbotEnabled = Config::Triggerbot::Enabled;
    Trigger->TurboFireEnabled = Config::Triggerbot::TurboFireEnabled;
    Trigger->TriggerbotMinRange = Config::Triggerbot::TriggerbotMinRange;
    Trigger->TriggerbotMaxRange = Config::Triggerbot::TriggerbotMaxRange;
    Trigger->wSentinel = Config::Triggerbot::wSentinel;
    Trigger->wLongbow = Config::Triggerbot::wLongbow;
    Trigger->wChargeRfile = Config::Triggerbot::wChargeRfile;
    Trigger->wMozambique = Config::Triggerbot::wMozambique;
    Trigger->wEva8 = Config::Triggerbot::wEva8;
    Trigger->wPeaceKeeper = Config::Triggerbot::wPeaceKeeper;
    Trigger->wMastiff = Config::Triggerbot::wMastiff;
    Trigger->wP2020 = Config::Triggerbot::wP2020;
    Trigger->wRE45 = Config::Triggerbot::wRE45;
    Trigger->wAlternator = Config::Triggerbot::wAlternator;
    Trigger->wR99 = Config::Triggerbot::wR99;
    Trigger->wR301 = Config::Triggerbot::wR301;
    Trigger->wSpitfire = Config::Triggerbot::wSpitfire;
    Trigger->wG7 = Config::Triggerbot::wG7;
    Trigger->wCar = Config::Triggerbot::wCar;
    Trigger->wRampage = Config::Triggerbot::wRampage;
    Trigger->w3030 = Config::Triggerbot::w3030;
    Trigger->wHemlock = Config::Triggerbot::wHemlock;
    Trigger->wFlatline = Config::Triggerbot::wFlatline;
    Trigger->wNemesis = Config::Triggerbot::wNemesis;
    Trigger->wVolt = Config::Triggerbot::wVolt;
    Trigger->wTripleTake = Config::Triggerbot::wTripleTake;
    Trigger->wLStar = Config::Triggerbot::wLStar;
    Trigger->wDevotion = Config::Triggerbot::wDevotion;
    Trigger->wHavoc = Config::Triggerbot::wHavoc;
    Trigger->wWingman = Config::Triggerbot::wWingman;
    Trigger->wProwler = Config::Triggerbot::wProwler;
    Trigger->wBocek = Config::Triggerbot::wBocek;
    Trigger->wKraber = Config::Triggerbot::wKraber;
    Trigger->wThrowingKnife = Config::Triggerbot::wThrowingKnife;

    // Screen Preferences
    ScreenOffset_X = Config::ScreenPref::ScreenOffset_X;
    ScreenOffset_Y = Config::ScreenPref::ScreenOffset_Y;

    // No Recoil
    Rec->RCSEnabled = Config::NoRecoil::Enabled;
    Rec->PitchPower = Config::NoRecoil::PitchPower;
    Rec->YawPower = Config::NoRecoil::YawPower;

    // Radar
    /*MapRadar->miniRadar = Config::Radar::miniRadar;
    MapRadar->bigRadar = Config::Radar::bigRadar;
    MapRadar->MiniMapGuides = Config::Radar::MiniMapGuides;
    MapRadar->miniMapRange = Config::Radar::miniMapRange;
    MapRadar->minimapradardotsize1 = Config::Radar::minimapradardotsize1;
    MapRadar->minimapradardotsize2 = Config::Radar::minimapradardotsize2;
    MapRadar->bigMapHotKey = static_cast<InputKeyType>(Config::Radar::bigMapHotKey);*/
}

void SaveConfig() {
    if (!AimAssist->Save()) std::cout << "something went wrong trying to save Aimbot settings" << std::endl;
    if (!ESP->Save()) std::cout << "something went wrong trying to save ESP settings" << std::endl;
    if (!Trigger->Save()) std::cout << "something went wrong trying to save Triggerbot settings" << std::endl;
    if (!MapRadar->Save()) std::cout << "something went wrong trying to save Radar settings" << std::endl;
    if (!Rec->Save()) std::cout << "something went wrong trying to save NoRecoil settings" << std::endl;
    UpdateConfig();
}

// Interface
ImVec4 ProcessingTimeColor;
void RenderUI() {
    auto io = ImGui::GetIO();
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("##Overlay", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoBackground
	);
	Canvas = ImGui::GetWindowDrawList();
    if (Map->IsPlayable)
        ESP->RenderDrawings(Canvas, AimAssist, Myself, OverlayWindow);
        MapRadar->RenderDrawings(Canvas, AimAssist, Myself, OverlayWindow);
	ImGui::End();

    if (!IsMenuOpened) return;

    // Menu
    int menuWidth = 440;
    int menuHeight = 500;
    ImGui::SetNextWindowSizeConstraints(ImVec2(menuWidth, menuHeight), ImVec2(menuWidth, menuHeight));
    ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight), ImGuiCond_FirstUseEver);
    ImGui::Begin("xap-client", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
    
    ProcessingTimeColor = OverlayWindow.ProcessingTime > 20 ? ProcessingTimeColor = ImVec4(1, 0.343, 0.475, 1) : ProcessingTimeColor = ImVec4(0.4, 1, 0.343, 1);
    ImGui::TextColored(ImVec4(0.90f, 0.90f, 0.90f, 0.90f), "Push INS (insert) key to open/close this menu");
    ImGui::TextColored(ProcessingTimeColor, "Processing Time: %02dms", OverlayWindow.ProcessingTime);
    ImGui::SameLine();

    if (OverlayWindow.AlignedButton("Save Config", 0.8f)) {
        SaveConfig();
        std::cout << "Config saved" << std::endl;
    }
    ImGui::SameLine();
    if (OverlayWindow.AlignedButton("Close", 1.0f))
    {
        OverlayWindow.DestroyOverlay();
        std::system("clear");
        std::cout << "Closing Overlay..." << std::endl;
        exit(0);
    }   

    if (ImGui::BeginTabBar("Menus"), ImGuiTabBarFlags_NoCloseWithMiddleMouseButton) {
        // Draw Settings //
        AimAssist->RenderUI();
        Rec->RenderUI();
        Trigger->RenderUI();
        ESP->RenderUI();
        MapRadar->RenderUI();

        /* Screen Preferences */
        if (ImGui::BeginTabItem("Config", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton | ImGuiTabItemFlags_NoReorder)) {
            ImGui::Checkbox("Trios, Gun run or Training Area", &Config::GameMode::threeManSquad);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Game Mode Settings");
            
            if (OverlayWindow.MonCount > 1) {
                ImGui::TextUnformatted("Multiple monitors detected");

                /*ImGui::SliderFloat("Screen Offset X", &ScreenOffset_X, 0, 3000, "%.0f");
                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                    ImGui::SetTooltip("Screen Offset X");

                ImGui::SliderFloat("Screen Offset Y", &ScreenOffset_Y, -1000, 1000, "%.0f");
                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                    ImGui::SetTooltip("Screen Offset Y");*/

                /*
                static char OffsetX[10] = "1920";
                static char OffsetY[10] = "0";
                ImGui::InputText("X Offset", OffsetX, IM_ARRAYSIZE(OffsetX));
                ImGui::InputText("Y Offset", OffsetY, IM_ARRAYSIZE(OffsetY));*/

                // Moving the overlay
                //OverlayWindow.moveOverlay(std::stoi(OffsetX), std::stoi(OffsetY));
            }
            ImGui::EndTabItem();
        }

        // Draw Credits //
        if (ImGui::BeginTabItem("Credits", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton | ImGuiTabItemFlags_NoReorder)) {
            ImGui::TextColored(ImVec4(0.65, 1, 0.95, 1), "Credits:");
            ImGui::TextColored(ImVec4(0.03, 1, 0.95, 1), "arturzxc: ESP, Triggerbot and X11Display's Move Mouse method");
            ImGui::TextColored(ImVec4(0.3, 1, 0.64, 1), "Koelion: ImGui Menu, AimbotResolver");
            ImGui::TextColored(ImVec4(0.03, 1, 0.95, 1), "rafaelstefany: Multiple monitors setup");
            ImGui::TextColored(ImVec4(0.3, 1, 0.64, 1), "unguhelvu: Seer-Styled ESP");
            ImGui::TextColored(ImVec4(0.03, 1, 0.95, 1), "Unknowndidi: Draw Box");
            ImGui::TextColored(ImVec4(0.3, 0.6, 0.9, 1), "unknowncheats: Offsets and ton of other things");
            ImGui::TextColored(ImVec4(0.6, 1, 0.64, 1), "Made by Azreol/Nexilist");
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

// Core
bool UpdateCore() {
    try {
        // Map Checking //
        Map->readFromMemory();
        if (!Map->IsPlayable) {
            return true;
        }

        // Read Local Player //
        Myself->Read();
        if (!Myself->IsValid()) {
            return true;
        }

        // Populate Players //
        Players->clear();
        if (Map->IsFiringRange) {
            for (int i = 0; i < Dummies->size(); i++) {
                Player* p = Dummies->at(i);
                p->Read();
                if (p->BasePointer != 0 && (p->IsPlayer() || p->IsDummy()))
                    Players->push_back(p);
            }
        } else {
            for (int i = 0; i < HumanPlayers->size(); i++) {
                Player* p = HumanPlayers->at(i);
                p->Read();
                if (p->BasePointer != 0 && (p->IsPlayer() || p->IsDummy()))
                    Players->push_back(p);
            }
        }

        // Updates //
        GameCamera->Update();
        ESP->Update();
        AimAssist->Update();
        Trigger->Update();
        Trigger->TurboFire();
        Rec->controlWeapon();
        return true;
    } catch(const std::exception& ex) {
        std::system("clear");
        std::cout << "Error: " << ex.what() << std::endl;
        return true;
    }

    return false;
}

// Main
int main(int argc, char *argv[]) {
    if (getuid()) { 
        std::cout << "Please run as sudo!" << std::endl;
        return -1; 
    }

    // Waiting for Apex Legends to be found //
    while (Memory::GetPID() == 0) {
        std::system("clear");
        std::cout << "Waiting for Apex Legends..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    std::system("clear");
    std::cout << "xap client ver 1.2 (CG modified version 01/05/2024)" << std::endl;

    // Initialize Overlay Window //
    if (!InitializeOverlayWindow()) return -1;

    // Theading //
    std::thread X11Thread(X11EventListener);
    X11Thread.detach();
    std::thread InputManagerThread(InputManager::run);
    InputManagerThread.detach();

    // Initialize the whole process //
    try {
        for (int i = 0; i < 70; i++) 
            HumanPlayers->push_back(new Player(i, Myself));

        for (int i = 0; i < 15000; i++) 
            Dummies->push_back(new Player(i, Myself));

        ESP->Initialize();
        
        std::cout << "Core initialized" << std::endl;
        std::cout << "-----------------------------" << std::endl;

        LoadConfig();
        // This is where the fun starts //
        OverlayWindow.Start(&UpdateCore, &RenderUI);
    }
    catch(...) {}

    StopThread = true;
    InputManager::StopThread = true;
    X11Thread.join();
    InputManagerThread.join();

    return 0;
}