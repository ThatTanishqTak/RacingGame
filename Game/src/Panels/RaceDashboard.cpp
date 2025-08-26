#include "RaceDashboard.h"
#include "Core/PaletteManager.h"
#include "Renderer/StateStream.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include <algorithm>

RaceDashboard::RaceDashboard()
{
    g_EventBus.Subscribe<PitIn>([this](const PitIn& e) { m_Toasts.push_back(e.DriverName + " entered pit"); });
    g_EventBus.Subscribe<PitOut>([this](const PitOut& e) { m_Toasts.push_back(e.DriverName + " exited pit"); });
    g_EventBus.Subscribe<DNF>([this](const DNF& e) { m_Toasts.push_back(e.DriverName + " DNF: " + e.Reason); });
}

void RaceDashboard::Render(const RaceState& state)
{
    RenderPitCrewPanel(state);
    RenderRaceInfoPanel(state);
    RenderTrackViewPanel(state);
    RenderDriverPanels(state);
    RenderScoreboardPanel(state);
    RenderToasts();
    RenderSettingsPanel();
}

void RaceDashboard::RenderPitCrewPanel(const RaceState& state)
{
    if (ImGui::Begin("Pit Crew"))
    {
        ImGui::Text("Pit Crew Panel");
    }
    ImGui::End();
}

void RaceDashboard::RenderRaceInfoPanel(const RaceState& state)
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

void RaceDashboard::RenderTrackViewPanel(const RaceState& state)
{
    if (ImGui::Begin("Track View"))
    {
        ImVec2 l_CanvasSize = ImGui::GetContentRegionAvail();
        if (l_CanvasSize.x < 50.0f)
        {
            l_CanvasSize.x = 50.0f;
        }

        if (l_CanvasSize.y < 50.0f)
        {
            l_CanvasSize.y = 50.0f;
        }

        ImVec2 l_CanvasPosition = ImGui::GetCursorScreenPos();
        ImDrawList* l_DrawList = ImGui::GetWindowDrawList();
        l_DrawList->AddRectFilled(l_CanvasPosition, ImVec2(l_CanvasPosition.x + l_CanvasSize.x, l_CanvasPosition.y + l_CanvasSize.y), IM_COL32(30, 30, 30, 255));
        l_DrawList->AddRect(l_CanvasPosition, ImVec2(l_CanvasPosition.x + l_CanvasSize.x, l_CanvasPosition.y + l_CanvasSize.y), IM_COL32(255, 255, 255, 255));

        if (!state.TrackLayout.empty())
        {
            float l_TileWidth = l_CanvasSize.x / static_cast<float>(state.TrackLayout[0].size());
            float l_TileHeight = l_CanvasSize.y / static_cast<float>(state.TrackLayout.size());

            for (size_t y = 0; y < state.TrackLayout.size(); ++y)
            {
                for (size_t x = 0; x < state.TrackLayout[y].size(); ++x)
                {
                    char l_Tile = state.TrackLayout[y][x];
                    ImU32 l_Colour;
                    bool l_Draw = true;

                    switch (l_Tile)
                    {
                    case '-':
                    case '|':
                    case '\\':
                    case '/':
                        l_Colour = IM_COL32(100, 100, 100, 255);
                        break;
                    case '*':
                        l_Colour = IM_COL32(255, 255, 255, 255);
                        break;
                    case '&':
                        l_Colour = IM_COL32(0, 0, 255, 255);
                        break;
                    case '[':
                    case ']':
                        l_Colour = IM_COL32(0, 255, 0, 255);
                        break;
                    default:
                        l_Draw = false;
                        break;
                    }

                    if (l_Draw)
                    {
                        ImVec2 l_Minimum = ImVec2(l_CanvasPosition.x + x * l_TileWidth, l_CanvasPosition.y + y * l_TileHeight);
                        ImVec2 l_Maximum = ImVec2(l_Minimum.x + l_TileWidth, l_Minimum.y + l_TileHeight);

                        l_DrawList->AddRectFilled(l_Minimum, l_Maximum, l_Colour);
                    }
                }
            }
        }

        ImGui::InvisibleButton("canvas", l_CanvasSize);
    }
    ImGui::End();
}

void RaceDashboard::RenderDriverPanels(const RaceState& state)
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

void RaceDashboard::RenderScoreboardPanel(const RaceState& state) 
{
    if (ImGui::Begin("Positions")) 
    {
        int l_Position = 1;
        for (int it_Position : state.Positions) 
        {
            auto it = std::find_if(state.Drivers.begin(), state.Drivers.end(), [it_Position](const DriverInfo& d) { return d.Number == it_Position; });
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

void RaceDashboard::RenderToasts() 
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

void RaceDashboard::RenderSettingsPanel() 
{
    if (ImGui::Begin("Settings")) 
    {
        ImGui::Checkbox("Colour Blind Mode", &m_ColourBlindMode);
    }
    ImGui::End();
}