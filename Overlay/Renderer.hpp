#pragma once
#include <GL/gl.h>
#include <algorithm>
#include "../imgui/imgui.h"
#include "../Utils/Color.hpp"
#include "../Math/Vector2D.hpp"
#include "../Math/Vector3D.hpp"
#include "../Math/Vector4D.hpp"

ImU32 HSVToRGB(float h, float s, float v) {
    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    float m = v - c;

    float r, g, b;
    if (h < 60) {
        r = c;
        g = x;
        b = 0;
    } else if (h < 120) {
        r = x;
        g = c;
        b = 0;
    } else if (h < 180) {
        r = 0;
        g = c;
        b = x;
    } else if (h < 240) {
        r = 0;
        g = x;
        b = c;
    } else if (h < 300) {
        r = x;
        g = 0;
        b = c;
    } else {
        r = c;
        g = 0;
        b = x;
    }

    return IM_COL32(
        static_cast<int>((r + m) * 255),
        static_cast<int>((g + m) * 255),
        static_cast<int>((b + m) * 255),
        255
    );
}

class Renderer {
public:
    static Vector3D RotatePoint(Vector3D EntityPos, Vector3D LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool* viewCheck)
    {
    	float r_1, r_2;
    	float x_1, y_1;
     
    	r_1 = -(EntityPos.y - LocalPlayerPos.y);
    	r_2 = EntityPos.x - LocalPlayerPos.x;
    	
    	float yawToRadian = angle * (float)(M_PI / 180.0F);
    	x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
    	y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;
     
    	*viewCheck = y_1 < 0;
     
    	x_1 *= zoom;
    	y_1 *= zoom;
     
    	int sizX = sizeX / 2;
    	int sizY = sizeY / 2;
     
    	x_1 += sizX;
    	y_1 += sizY;
     
    	if (x_1 < 5)
    		x_1 = 5;
     
    	if (x_1 > sizeX - 5)
    		x_1 = sizeX - 5;
     
    	if (y_1 < 5)
    		y_1 = 5;
     
    	if (y_1 > sizeY - 5)
    		y_1 = sizeY - 5;
     
     
    	x_1 += posX;
    	y_1 += posY;
     
     
    	return Vector3D(x_1, y_1, 0);
    }

    static void TeamMiniMap(int x, int y, int radius, int teamID, float targetyaw)
    {
        RGBA2 color;
        auto it = teamColors.find(teamID);
        if (it == teamColors.end()) {
            // Define the minimum sum of RGB values for a color to be considered "light"
            const int MIN_SUM_RGB = 500;

            // Generate a new random color for this team, discarding colors with a low sum of RGB values
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 255);
            RGBA2 color;
            do {
                color = { dis(gen), dis(gen), dis(gen), 255 };
            } while (color.R + color.G + color.B < MIN_SUM_RGB);

            // Store the color in the teamColors map
            teamColors[teamID] = color;
        }
        else {
            // Use the previously generated color for this team
            color = it->second;
        }

        auto colOutline = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0, 0.0, 0.0, 1.0));
        ImVec2 center(x, y);
        ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)));
        ImGui::GetWindowDrawList()->AddCircle(center, radius, colOutline, 12, Config::Radar::minimapradardotsize2);

        // Draw a line pointing in the direction of each player's aim
        const int numPlayers = 3;
        for (int i = 0; i < numPlayers; i++) {
            float angle = (360.0 - targetyaw) * (M_PI / 180.0); // Replace this with the actual yaw of the player, then convert it to radians.
            ImVec2 endpoint(center.x + radius * cos(angle), center.y + radius * sin(angle));
            ImGui::GetWindowDrawList()->AddLine(center, endpoint, colOutline);
        }
    }

    static void DrawText(ImDrawList* canvas, const Vector2D& pos, const char* text, ImColor color, bool outline, bool centered, bool adjustHeight) {
		const auto textColor = color;
        const auto outlineColor = ImColor(0, 0, 0);

        const auto textSize = ImGui::CalcTextSize(text);
        const auto horizontalOffset = centered ? textSize.x / 2 : 0.0f;
        const auto verticalOffset = adjustHeight ? textSize.y : 0.0f;

        if (outline) {
            canvas->AddText({pos.x - horizontalOffset, pos.y - verticalOffset - 1}, outlineColor, text);
            canvas->AddText({pos.x - horizontalOffset, pos.y - verticalOffset + 1}, outlineColor, text);
            canvas->AddText({pos.x - horizontalOffset - 1, pos.y - verticalOffset}, outlineColor, text);
            canvas->AddText({pos.x - horizontalOffset + 1, pos.y - verticalOffset}, outlineColor, text);
        }
        canvas->AddText({pos.x - horizontalOffset, pos.y - verticalOffset}, textColor, text);
	}

    static void DrawCircle(ImDrawList* canvas, const Vector2D& pos, float radius, int segments, const ImColor& color, float thickness) {
		canvas->AddCircle(ImVec2(pos.x, pos.y), radius, color, segments, thickness);
	}

    static void DrawCircleFilled(ImDrawList* canvas, const Vector2D& pos, float radius, int segments, const ImColor& color) {
		canvas->AddCircleFilled(ImVec2(pos.x, pos.y), radius, color, segments);
	}

    static void DrawLine(ImDrawList* canvas, const Vector2D& start, const Vector2D& end, float thickness, const ImVec4& color) {
		canvas->AddLine((const ImVec2&)start, (const ImVec2&)end, ImColor(color), thickness);
	}

    static void DrawQuadFilled(ImDrawList* canvas, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, ImColor color) {
	    canvas->AddQuadFilled(p1, p2, p3, p4, color);
    }

    static void DrawHexagon(ImDrawList* canvas, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p5, const ImVec2& p6, ImU32 col, float thickness) {
        canvas->AddHexagon(p1, p2, p3, p4, p5, p6, col, thickness);
    }

    static void DrawHexagonFilled(ImDrawList* canvas, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p5, const ImVec2& p6, ImU32 col) {
        canvas->AddHexagonFilled(p1, p2, p3, p4, p5, p6, col);
    }

    static void DrawBox(ImDrawList* canvas, const Vector2D& foot, const Vector2D& head, const ImColor& color, float thickness) {
        float height = head.y - foot.y;
    	float width = height / 2.0f;
        canvas->AddRect(ImVec2(foot.x - (width / 2), foot.y), ImVec2(head.x + (width/2), head.y+(height*0.2)), color, 0.0f, 0, thickness);
	}

    static void DrawSeer(ImDrawList* Canvas, float x, float y, int shield, int max_shield, int health) {
        int bg_offset = 3;
        int bar_width = 158;
        float max_health = 100.0f;
        float shield_step = 25.0f;
        ImVec2 bg1(x - bar_width / 2 - bg_offset, y);
        ImVec2 bg2(bg1.x - 10, bg1.y - 16);
        ImVec2 bg3(bg2.x + 5, bg2.y - 7);
        ImVec2 bg4(bg3.x + bar_width + bg_offset, bg3.y);
        ImVec2 bg5(bg4.x + 11, bg4.y + 18);
        ImVec2 bg6(x + bar_width / 2 + bg_offset, y);
        Renderer::DrawHexagonFilled(Canvas, bg1, bg2, bg3, bg4, bg5, bg6, ImColor(0, 0, 0, 120));

        ImVec2 h1(bg1.x + 3, bg1.y - 4);
        ImVec2 h2(h1.x - 5, h1.y - 8);
        ImVec2 h3(h2.x + (float)health / max_health * bar_width, h2.y);
        ImVec2 h4(h1.x + (float)health / max_health * bar_width, h1.y);
        ImVec2 h3m(h2.x + bar_width, h2.y);
        ImVec2 h4m(h1.x + bar_width, h1.y);
        Renderer::DrawQuadFilled(Canvas, h1, h2, h3m, h4m, ImColor(10,10,30,60));
        Renderer::DrawQuadFilled(Canvas, h1, h2, h3, h4, ImColor(255, 255, 255, 255));
    
        ImColor shieldCracked(97,97,97);
        ImColor shieldCrackedDark(67, 67, 67);

        ImColor shieldCol;
        ImColor shieldColDark;
        if (max_shield == 50) { //white
            shieldCol = ImColor(247, 247, 247);
            shieldColDark = ImColor(164, 164, 164);
        }
        else if (max_shield == 75) { //blue
            shieldCol = ImColor(39, 178, 255);
            shieldColDark = ImColor(27, 120, 210);
        }
        else if (max_shield == 100) { //purple
            shieldCol = ImColor(206, 59, 255);
            shieldColDark = ImColor(136, 36, 220);
        }
        else if (max_shield == 125) { //red
            shieldCol = ImColor(219, 2, 2);
            shieldColDark = ImColor(219, 2, 2);
        }
        else {
            shieldCol = ImColor(247, 247, 247);
            shieldColDark = ImColor(164, 164, 164);
        }

        int shield_25 = 30;
        int shield_tmp = shield;
        int shield1 =0;
        int shield2 =0;
        int shield3 =0;
        int shield4 =0;
        int shield5 =0;
        if (shield_tmp > 25) {
            shield1 = 25;
            shield_tmp -= 25;
            if (shield_tmp > 25) {
                shield2 = 25;
                shield_tmp -= 25;
                if (shield_tmp > 25) {
                    shield3 = 25;
                    shield_tmp -= 25;
                    if (shield_tmp > 25) {
                        shield4 = 25;
                        shield_tmp -= 25;
                        shield5 = shield_tmp;
                    }
                    else {
                        shield4 = shield_tmp;
                    }
                }
                else {
                    shield3 = shield_tmp;
                }
            }
            else {
                shield2 = shield_tmp;
            }
        }
        else {
            shield1 = shield_tmp;
        }

        ImVec2 s1(h2.x - 1, h2.y - 2);
        ImVec2 s2(s1.x - 3, s1.y - 5);
        ImVec2 s3(s2.x + shield1 / shield_step * shield_25, s2.y);
        ImVec2 s4(s1.x + shield1 / shield_step * shield_25, s1.y);
        ImVec2 s3m(s2.x + shield_25, s2.y);
        ImVec2 s4m(s1.x + shield_25, s1.y);
    
        ImVec2 ss1(s4m.x + 2, s1.y);
        ImVec2 ss2(s3m.x + 2, s2.y);
        ImVec2 ss3(ss2.x + shield2 / shield_step * shield_25, s2.y);
        ImVec2 ss4(ss1.x + shield2 / shield_step * shield_25, s1.y);
        ImVec2 ss3m(ss2.x + shield_25, s2.y);
        ImVec2 ss4m(ss1.x + shield_25, s1.y);
        
        ImVec2 sss1(ss4m.x + 2, s1.y);
        ImVec2 sss2(ss3m.x + 2, s2.y);
        ImVec2 sss3(sss2.x + shield3 / shield_step * shield_25, s2.y);
        ImVec2 sss4(sss1.x + shield3 / shield_step * shield_25, s1.y);
        ImVec2 sss3m(sss2.x + shield_25, s2.y);
        ImVec2 sss4m(sss1.x + shield_25, s1.y);
        
        ImVec2 ssss1(sss4m.x + 2, s1.y);
        ImVec2 ssss2(sss3m.x + 2, s2.y);
        ImVec2 ssss3(ssss2.x + shield4 / shield_step * shield_25, s2.y);
        ImVec2 ssss4(ssss1.x + shield4 / shield_step * shield_25, s1.y);
        ImVec2 ssss3m(ssss2.x + shield_25, s2.y);
        ImVec2 ssss4m(ssss1.x + shield_25, s1.y);
        
        ImVec2 sssss1(ssss4m.x + 2, s1.y);
        ImVec2 sssss2(ssss3m.x + 2, s2.y);
        ImVec2 sssss3(sssss2.x + shield5 / shield_step * shield_25, s2.y);
        ImVec2 sssss4(sssss1.x + shield5 / shield_step * shield_25, s1.y);
        ImVec2 sssss3m(sssss2.x + shield_25, s2.y);
        ImVec2 sssss4m(sssss1.x + shield_25, s1.y);
        if (max_shield == 50) {
            if (shield <= 25) {
                if (shield < 25) {
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3m, s4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                }
                if(shield != 0)
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
    
            }
            else if (shield <= 50) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                if (shield != 50) {
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
            }
        }
        else if (max_shield == 75) {
            if (shield <= 25) {
                if (shield < 25) {
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3m, s4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
    
            }
            else if (shield <= 50) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                if (shield < 50) {
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
            }
            else if (shield <= 75) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
                if (shield < 75) {
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3, sss4, shieldCol);
            }
        }
        else if (max_shield == 100) {
            if (shield <= 25) {
                if (shield < 25) {
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3m, s4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
    
            }
            else if (shield <= 50) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                if (shield < 50) {
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
            }
            else if (shield <= 75) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
                if (shield < 75) {
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3, sss4, shieldCol);
            }
            else if (shield <= 100) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3, sss4, shieldCol);
                if (shield < 100) {
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3, ssss4, shieldCol);
            }
        }
        else if (max_shield == 125) {
            if (shield <= 25) {
                if (shield < 25) {
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3m, s4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
    
            }
            else if (shield <= 50) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                if (shield < 50) {
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
            }
            else if (shield <= 75) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
                if (shield < 75) {
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3, sss4, shieldCol);
            }
            else if (shield <= 100) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3, sss4, shieldCol);
                if (shield < 100) {
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3, ssss4, shieldCol);
            }
            else if (shield <= 125) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3, sss4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3, ssss4, shieldCol);
                if (shield < 125) {
                    Renderer::DrawQuadFilled(Canvas, sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, sssss1, sssss2, sssss3, sssss4, shieldCol);
            }
        }
    }
};