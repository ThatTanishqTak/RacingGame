#include "RaceDashboard.h"
#include "Core/PaletteManager.h"
#include "Renderer/StateStream.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include <algorithm>

RaceDashboard::RaceDashboard()
{
    g_EventBus.Subscribe<PitIn>([this](const PitIn& e) { Toasts.push_back(e.DriverName + " entered pit"); });
    g_EventBus.Subscribe<PitOut>([this](const PitOut& e) { Toasts.push_back(e.DriverName + " exited pit"); });
    g_EventBus.Subscribe<DNF>([this](const DNF& e) { Toasts.push_back(e.DriverName + " DNF: " + e.Reason); });
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
        ImVec2 canvasSize = ImGui::GetContentRegionAvail();
        if (canvasSize.x < 50.0f)
        {
            canvasSize.x = 50.0f;
        }

        if (canvasSize.y < 50.0f)
        {
            canvasSize.y = 50.0f;
        }

        ImVec2 canvasPos = ImGui::GetCursorScreenPos();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(30, 30, 30, 255));
        drawList->AddRect(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(255, 255, 255, 255));

        if (!state.TrackLayout.empty())
        {
            float tileWidth = canvasSize.x / static_cast<float>(state.TrackLayout[0].size());
            float tileHeight = canvasSize.y / static_cast<float>(state.TrackLayout.size());

            for (size_t y = 0; y < state.TrackLayout.size(); ++y)
            {
                for (size_t x = 0; x < state.TrackLayout[y].size(); ++x)
                {
                    char tile = state.TrackLayout[y][x];
                    ImU32 colour;
                    bool draw = true;
                    switch (tile)
                    {
                    case '-':
                    case '|':
                    case '\\':
                    case '/':
                        colour = IM_COL32(100, 100, 100, 255);
                        break;
                    case '*':
                        colour = IM_COL32(255, 255, 255, 255);
                        break;
                    case '&':
                        colour = IM_COL32(0, 0, 255, 255);
                        break;
                    case '[':
                    case ']':
                        colour = IM_COL32(0, 255, 0, 255);
                        break;
                    default:
                        draw = false;
                        break;
                    }

                    if (draw)
                    {
                        ImVec2 min = ImVec2(canvasPos.x + x * tileWidth, canvasPos.y + y * tileHeight);
                        ImVec2 max = ImVec2(min.x + tileWidth, min.y + tileHeight);
                        drawList->AddRectFilled(min, max, colour);
                    }
                }
            }
        }

        ImGui::InvisibleButton("canvas", canvasSize);
    }
    ImGui::End();
}

void RaceDashboard::RenderDriverPanels(const RaceState& state)
{
    if (ImGui::Begin("Drivers"))
    {
        for (const auto& driver : state.Drivers) 
        {
            ImVec4 colour = g_PaletteManager.GetTeamColour(driver.TeamId, ColourBlindMode);
            ImGui::TextColored(colour, "Driver %d: %s", driver.Number, driver.Name.c_str());
        }
    }
    ImGui::End();
}

void RaceDashboard::RenderScoreboardPanel(const RaceState& state) 
{
    if (ImGui::Begin("Positions")) 
    {
        int position = 1;
        for (int pos : state.Positions) 
        {
            auto it = std::find_if(state.Drivers.begin(), state.Drivers.end(), [pos](const DriverInfo& d) { return d.Number == pos; });
            ImVec4 colour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            if (it != state.Drivers.end()) 
            {
                colour = g_PaletteManager.GetTeamColour(it->TeamId, ColourBlindMode);
            }
            ImGui::TextColored(colour, "%d: %d", position++, pos);
        }
    }
    ImGui::End();
}

void RaceDashboard::RenderToasts() 
{
    if (ImGui::Begin("HUD Toasts")) 
    {
        for (const auto& msg : Toasts) 
        {
            ImGui::Text("%s", msg.c_str());
        }
    }
    ImGui::End();
}

void RaceDashboard::RenderSettingsPanel() 
{
    if (ImGui::Begin("Settings")) 
    {
        ImGui::Checkbox("Colour Blind Mode", &ColourBlindMode);
    }
    ImGui::End();
}