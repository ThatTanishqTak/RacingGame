#include "RaceDashboard.h"
#include "Core/PaletteManager.h"

#include <algorithm>
#include <imgui.h>

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
        ImGui::Text("Track View Panel");
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