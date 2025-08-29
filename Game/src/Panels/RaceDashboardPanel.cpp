#include "RaceDashboardPanel.h"

#include "Core/PaletteManager.h"
#include "Core/EventBus.h"
#include "Renderer/Renderer.h"
#include "Renderer/StateStream.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include <algorithm>

RaceDashboardPanel::RaceDashboardPanel(EventBus& eventBus, Engine::Renderer& renderer) : m_EventBus(eventBus), m_Renderer(renderer)
{
    m_EventBus.Subscribe<PitIn>([this](const PitIn& e) { m_Toasts.push_back(e.DriverName + " entered pit"); });
    m_EventBus.Subscribe<PitOut>([this](const PitOut& e) { m_Toasts.push_back(e.DriverName + " exited pit"); });
    m_EventBus.Subscribe<DNF>([this](const DNF& e) { m_Toasts.push_back(e.DriverName + " DNF: " + e.Reason); });
    m_EventBus.Subscribe<ViewModeToggle>([this](const ViewModeToggle& e) { m_TopDownView = e.TopDown; });
}

RaceDashboardPanel::~RaceDashboardPanel()
{
    g_EventBus.Unsubscribe(m_PitInToken);
    g_EventBus.Unsubscribe(m_PitOutToken);
    g_EventBus.Unsubscribe(m_DNFToken);
    g_EventBus.Unsubscribe(m_ViewModeToken);
}

void RaceDashboardPanel::Render(const RaceState& state)
{
    RenderPitCrewPanel(state);
    RenderRaceInfoPanel(state);
    RenderTrackViewPanel(state);
    RenderDriverPanels(state);
    RenderScoreboardPanel(state);
    RenderToasts();
    RenderSettingsPanel();
}

void RaceDashboardPanel::RenderPitCrewPanel(const RaceState& state)
{
    if (ImGui::Begin("Pit Crew"))
    {
        ImGui::Text("Pit Crew Panel");
    }
    ImGui::End();
}

void RaceDashboardPanel::RenderRaceInfoPanel(const RaceState& state)
{
    if (ImGui::Begin("Race Info"))
    {
        ImGui::Text("Date: %s", state.Date.c_str());
        ImGui::SameLine();
        ImGui::Text("Time: %s", state.Time.c_str());
        ImGui::SameLine();
        ImGui::Text("Weather: %s", state.Weather.c_str());
    }
    ImGui::End();
}

void RaceDashboardPanel::RenderTrackViewPanel(const RaceState& state)
{
    if (ImGui::Begin("Live"))
    {
        ImVec2 l_Canvas = ImGui::GetContentRegionAvail();
        l_Canvas.x = std::max(50.0f, l_Canvas.x);
        l_Canvas.y = std::max(50.0f, l_Canvas.y);

        ImVec2 l_Origin = ImGui::GetCursorScreenPos();
        ImDrawList* l_DrawList = ImGui::GetWindowDrawList();

        // Background + border
        l_DrawList->AddRectFilled(l_Origin, ImVec2(l_Origin.x + l_Canvas.x, l_Origin.y + l_Canvas.y), IM_COL32(30, 30, 30, 255));
        l_DrawList->AddRect(l_Origin, ImVec2(l_Origin.x + l_Canvas.x, l_Origin.y + l_Canvas.y), IM_COL32(255, 255, 255, 255));

        // Nothing to l_Drawable?
        if (!state.TrackLayout.empty())
        {
            const float l_LayoutWidth = static_cast<float>(state.TrackLayout[0].size());
            const float l_LayoutHeight = static_cast<float>(state.TrackLayout.size());

            // Scale the track to the l_Canvas with zoom
            const float l_TileWidth = (l_Canvas.x / l_LayoutWidth) * m_TrackZoom;
            const float l_TileHeight = (l_Canvas.y / l_LayoutHeight) * m_TrackZoom;

            ImVec2 l_TrackSize{ l_TileWidth * l_LayoutWidth, l_TileHeight * l_LayoutHeight };
            ImVec2 l_TrackMinimum{ l_Origin.x + (l_Canvas.x - l_TrackSize.x) * 0.5f, l_Origin.y + (l_Canvas.y - l_TrackSize.y) * 0.5f };
            ImVec2 l_TrackMaximum{ l_TrackMinimum.x + l_TrackSize.x, l_TrackMinimum.y + l_TrackSize.y };

            auto a_CellMininum = [&](int x, int y) { return ImVec2(l_TrackMinimum.x + x * l_TileWidth, l_TrackMinimum.y + y * l_TileHeight); };
            auto a_CellMaximum = [&](int x, int y) { return ImVec2(l_TrackMinimum.x + (x + 1) * l_TileWidth, l_TrackMinimum.y + (y + 1) * l_TileHeight); };
            auto a_CellCenter = [&](int x, int y) { ImVec2 l_Minimum = a_CellMininum(x, y), l_Maximum = a_CellMaximum(x, y); return ImVec2((l_Minimum.x + l_Maximum.x) * 0.5f, (l_Minimum.y + l_Maximum.y) * 0.5f); };

            // Stroke thickness and padding in pixels (based on the *smaller* tile dimension)
            const float l_Base = std::min(l_TileWidth, l_TileHeight);
            const float l_Padding = l_Base * m_TrackPadding;
            const float l_Thickness = std::max(1.0f, l_Base * m_TrackThickness);

            // Draw each tile as l_Minimum *thin* line/rect instead of l_Minimum full block
            for (int y = 0; y < (int)state.TrackLayout.size(); ++y)
            {
                const std::string& l_Row = state.TrackLayout[y];
                for (int x = 0; x < (int)l_Row.size(); ++x)
                {
                    const char l_Type = l_Row[x];
                    ImU32 l_Column = 0;
                    bool l_Drawable = true;

                    switch (l_Type)
                    {
                        case '-': l_Column = IM_COL32(130, 130, 130, 255); break;  // road (horizontal)
                        case '|': l_Column = IM_COL32(130, 130, 130, 255); break;  // road (vertical)
                        case '\\': l_Column = IM_COL32(130, 130, 130, 255); break; // road (diag)
                        case '/':  l_Column = IM_COL32(130, 130, 130, 255); break; // road (diag)
                        case '*':  l_Column = IM_COL32(255, 255, 255, 255); break; // S/F
                        case '&':  l_Column = IM_COL32(0, 0, 255, 255); break; // pitlane
                        
                        case '[':
                        case ']':  l_Column = IM_COL32(0, 255, 0, 255); break; // curbs/markers
                        
                        case ' ':  l_Drawable = false; break;
                        default:   l_Drawable = false; break;
                    }
                    
                    if (!l_Drawable)
                    {
                        continue;
                    }

                    ImVec2 l_Minimum = a_CellMininum(x, y);
                    ImVec2 l_Maximum = a_CellMaximum(x, y);
                    const float l_X = (l_Minimum.x + l_Maximum.x) * 0.5f;
                    const float l_Y = (l_Minimum.y + l_Maximum.y) * 0.5f;

                    if (l_Type == '-')
                    {
                        // Horizontal stroke
                        l_DrawList->AddRectFilled(ImVec2(l_Minimum.x + l_Padding, l_Y - l_Thickness * 0.5f), ImVec2(l_Maximum.x - l_Padding, l_Y + l_Thickness * 0.5f), l_Column);
                    }

                    else if (l_Type == '|')
                    {
                        // Vertical stroke
                        l_DrawList->AddRectFilled(ImVec2(l_X - l_Thickness * 0.5f, l_Minimum.y + l_Padding), ImVec2(l_X + l_Thickness * 0.5f, l_Maximum.y - l_Padding), l_Column);
                    }

                    else if (l_Type == '\\')
                    {
                        // Diagonal bottom-left -> top-right
                        l_DrawList->AddLine(ImVec2(l_Minimum.x + l_Padding, l_Maximum.y - l_Padding), ImVec2(l_Maximum.x - l_Padding, l_Minimum.y + l_Padding), l_Column, l_Thickness);
                    }

                    else if (l_Type == '/')
                    {
                        // Diagonal top-left -> bottom-right
                        l_DrawList->AddLine(ImVec2(l_Minimum.x + l_Padding, l_Minimum.y + l_Padding), ImVec2(l_Maximum.x - l_Padding, l_Maximum.y - l_Padding), l_Column, l_Thickness);
                    }

                    else if (l_Type == '*')
                    {
                        // Thin S/F stripe
                        l_DrawList->AddRectFilled(ImVec2(l_Minimum.x + l_Padding, l_Y - l_Thickness * 0.35f), ImVec2(l_Maximum.x - l_Padding, l_Y + l_Thickness * 0.35f), l_Column);
                    }

                    else if (l_Type == '[' || l_Type == ']')
                    {
                        // Small curb marker on cell edge
                        const float curbW = std::max(1.0f, l_Thickness * 0.8f);
                        if (l_Type == '[')
                        {
                            l_DrawList->AddRectFilled(ImVec2(l_Minimum.x + l_Padding, l_Minimum.y + l_Padding), ImVec2(l_Minimum.x + l_Padding + curbW, l_Maximum.y - l_Padding), l_Column);
                        }

                        else
                        {
                            l_DrawList->AddRectFilled(ImVec2(l_Maximum.x - l_Padding - curbW, l_Minimum.y + l_Padding), ImVec2(l_Maximum.x - l_Padding, l_Maximum.y - l_Padding), l_Column);
                        }
                    }

                    else if (l_Type == '&')
                    {
                        // Pitlane "slot"
                        l_DrawList->AddRect(ImVec2(l_Minimum.x + l_Padding, l_Minimum.y + l_Padding), ImVec2(l_Maximum.x - l_Padding, l_Maximum.y - l_Padding), l_Column, 0.0f, 0, l_Thickness * 0.5f);
                    }
                }
            }

            // --- Car overlay (live) ---
            if (m_ShowCars)
            {
                glm::vec2 l_Min{ -5.0f, -10.0f };
                glm::vec2 l_Max{ 5.0f, 10.0f };
                l_Min = m_Renderer.GetTrackMin();
                l_Max = m_Renderer.GetTrackMax();

                const float l_WorldMinimumX = l_Min.x;
                const float l_WorldMaximumX = l_Max.x;
                const float l_WorldMinimumZ = l_Min.y;
                const float l_WorldMaximumZ = l_Max.y;

                auto a_WorldToScreen = [&](const glm::vec3& l_Position)
                    {
                        const float nx = (l_Position.x - l_WorldMinimumX) / (l_WorldMaximumX - l_WorldMinimumX);
                        const float ny = (l_Position.z - l_WorldMinimumZ) / (l_WorldMaximumZ - l_WorldMinimumZ);

                        return ImVec2(l_TrackMinimum.x + nx * l_TrackSize.x, l_TrackMinimum.y + ny * l_TrackSize.y);
                    };

                // Map it_Car ID -> team colour using RaceState (ID 0->Number 1, etc.)
                auto a_IDToColour = [&](int carId)
                    {
                        int l_NumberGuess = carId + 1;
                        int l_TeamID = 0;
                        for (const auto& it_Driver : state.Drivers)
                        {
                            if (it_Driver.Number == l_NumberGuess)
                            { 
                                l_TeamID = it_Driver.TeamID; 
                                break; 
                            }
                        }
                        ImVec4 l_Color = g_PaletteManager.GetTeamColour((size_t)l_TeamID, m_ColourBlindMode);
                        
                        return IM_COL32((int)(l_Color.x * 255), (int)(l_Color.y * 255), (int)(l_Color.z * 255), 255);
                    };

                // Live positions
                double l_Time = glfwGetTime();
                auto a_Cars = Engine::g_StateBuffer.Interpolate(l_Time);

                for (const auto& it_Car : a_Cars)
                {
                    ImU32 l_Column = a_IDToColour(it_Car.ID);
                    ImVec2 l_Position = a_WorldToScreen(it_Car.Position);
                    l_DrawList->AddCircleFilled(l_Position, m_CarRadiusPx, l_Column);

                    if (m_LabelCars)
                    {
                        char l_Buffer[8];
                        snprintf(l_Buffer, sizeof(l_Buffer), "%d", it_Car.ID + 1);
                        l_DrawList->AddText(ImVec2(l_Position.x + m_CarRadiusPx + 2.0f, l_Position.y - 7.0f), IM_COL32(255, 255, 255, 220), l_Buffer);
                    }
                }
            }
        }

        ImGui::InvisibleButton("##track_canvas", l_Canvas, ImGuiButtonFlags_MouseButtonLeft);
        if (ImGui::IsItemHovered())
        {
            const float l_Zoom = ImGui::GetIO().MouseWheel;
            if (l_Zoom != 0.0f)
            {
                m_TrackZoom = std::clamp(m_TrackZoom + l_Zoom * 0.1f, 0.25f, 4.0f);
            }
        }
    }
    ImGui::End();
}

void RaceDashboardPanel::RenderDriverPanels(const RaceState& state)
{
    if (ImGui::Begin("Drivers"))
    {
        for (const auto& it_driver : state.Drivers) 
        {
            ImVec4 l_Colour = g_PaletteManager.GetTeamColour(it_driver.TeamID, m_ColourBlindMode);
            ImGui::TextColored(l_Colour, "Driver %d: %s", it_driver.Number, it_driver.Name.c_str());
        }
    }
    ImGui::End();
}

void RaceDashboardPanel::RenderScoreboardPanel(const RaceState& state) 
{
    if (ImGui::Begin("Positions")) 
    {
        int l_Position = 1;
        for (int it_Position : state.Positions) 
        {
            auto it = std::find_if(state.Drivers.begin(), state.Drivers.end(), [it_Position](const DriverInfo& it_Driver) { return it_Driver.Number == it_Position; });
            ImVec4 l_Colour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            if (it != state.Drivers.end()) 
            {
                l_Colour = g_PaletteManager.GetTeamColour(it->TeamID, m_ColourBlindMode);
            }
            ImGui::TextColored(l_Colour, "%d: %d", l_Position++, it_Position);
        }
    }
    ImGui::End();
}

void RaceDashboardPanel::RenderToasts() 
{
    if (ImGui::Begin("HUD Toasts")) 
    {
        for (const auto& it_message : m_Toasts) 
        {
            ImGui::Text("%s", it_message.c_str());
        }
    }
    ImGui::End();
}

void RaceDashboardPanel::RenderSettingsPanel()
{
    if (ImGui::Begin("Settings"))
    {
        ImGui::Checkbox("Colour Blind Mode", &m_ColourBlindMode);
        if (ImGui::Checkbox("Top Down View", &m_TopDownView))
        {
            m_EventBus.Publish(ViewModeToggle{ m_TopDownView });
        }
        ImGui::SeparatorText("Track View");
        ImGui::SliderFloat("Zoom", &m_TrackZoom, 0.25f, 4.0f, "%.2f");
        ImGui::SliderFloat("Stroke Thickness", &m_TrackThickness, 0.05f, 0.60f, "%.2f");
        ImGui::SliderFloat("Stroke Padding", &m_TrackPadding, 0.00f, 0.40f, "%.2f");
        if (ImGui::Checkbox("Lane Lines", &m_ShowLaneLines))
        {
            m_Renderer.m_ShowLaneLines = m_ShowLaneLines;
        }

        ImGui::SeparatorText("Cars");
        ImGui::Checkbox("Show Cars", &m_ShowCars);
        ImGui::Checkbox("Label Cars", &m_LabelCars);
        ImGui::SliderFloat("Car Radius (px)", &m_CarRadiusPx, 3.0f, 16.0f, "%.0f");
    }
    ImGui::End();
}