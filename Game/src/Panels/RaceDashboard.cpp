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
        ImVec2 l_CanvasSize = ImGui::GetContentRegionAvail();
        if (l_CanvasSize.x < 50.0f)
        {
            l_CanvasSize.x = 50.0f;
        }
        
        if (l_CanvasSize.y < 50.0f)
        {
            l_CanvasSize.y = 50.0f;
        }

        ImVec2 l_CanvasPos = ImGui::GetCursorScreenPos();
        ImDrawList* l_DrawList = ImGui::GetWindowDrawList();
        l_DrawList->AddRectFilled(l_CanvasPos, ImVec2(l_CanvasPos.x + l_CanvasSize.x, l_CanvasPos.y + l_CanvasSize.y), IM_COL32(30, 30, 30, 255));
        l_DrawList->AddRect(l_CanvasPos, ImVec2(l_CanvasPos.x + l_CanvasSize.x, l_CanvasPos.y + l_CanvasSize.y), IM_COL32(255, 255, 255, 255));

        auto l_ToScreen = [&](const glm::vec2& p)
            {
                float l_U = (p.x + 5.0f) / 10.0f;
                float l_V = (p.y + 10.0f) / 20.0f;
                
                return ImVec2(l_CanvasPos.x + l_U * l_CanvasSize.x, l_CanvasPos.y + (1.0f - l_V) * l_CanvasSize.y);
            };

        l_DrawList->AddRect(l_ToScreen({ -5.0f, -10.0f }), l_ToScreen({ 5.0f, 10.0f }), IM_COL32(200, 200, 200, 255));

        double l_Time = glfwGetTime();
        auto l_States = Engine::GlobalStateBuffer.Interpolate(l_Time);
        for (const auto& l_Car : l_States)
        {
            ImVec2 l_Pos = l_ToScreen({ l_Car.Position.x, l_Car.Position.z });
            l_DrawList->AddCircleFilled(l_Pos, 5.0f, IM_COL32(255, 0, 0, 255));
        }

        ImGui::InvisibleButton("canvas", l_CanvasSize);
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