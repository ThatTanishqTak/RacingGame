#include "RaceDashboard.h"

#include <imgui.h>

void RaceDashboard::Render(const RaceState& state)
{
    RenderPitCrewPanel(state);
    RenderRaceInfoPanel(state);
    RenderTrackViewPanel(state);
    RenderDriverPanels(state);
    RenderScoreboardPanel(state);
}

void RaceDashboard::RenderPitCrewPanel(const RaceState& state)
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(200.0f, 600.0f));
    if (ImGui::Begin("Pit Crew"))
    {
        ImGui::Text("Pit Crew Panel");
    }
    ImGui::End();
}

void RaceDashboard::RenderRaceInfoPanel(const RaceState& state)
{
    ImGui::SetNextWindowPos(ImVec2(200.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(800.0f, 80.0f));
    if (ImGui::Begin("Race Info"))
    {
        ImGui::Text("Date: %s", state.Date.c_str());
        ImGui::Text("Time: %s", state.Time.c_str());
        ImGui::Text("Weather: %s", state.Weather.c_str());
    }
    ImGui::End();
}

void RaceDashboard::RenderTrackViewPanel(const RaceState& state)
{
    ImGui::SetNextWindowPos(ImVec2(200.0f, 80.0f));
    ImGui::SetNextWindowSize(ImVec2(800.0f, 400.0f));
    if (ImGui::Begin("Track View"))
    {
        ImGui::Text("Track View Panel");
    }
    ImGui::End();
}

void RaceDashboard::RenderDriverPanels(const RaceState& state)
{
    ImGui::SetNextWindowPos(ImVec2(200.0f, 480.0f));
    ImGui::SetNextWindowSize(ImVec2(800.0f, 120.0f));
    if (ImGui::Begin("Drivers"))
    {
        for (const auto& driver : state.DriverInfo)
        {
            ImGui::Text("Driver %d: %s", driver.Number, driver.Name.c_str());
        }
    }
    ImGui::End();
}

void RaceDashboard::RenderScoreboardPanel(const RaceState& state)
{
    ImGui::SetNextWindowPos(ImVec2(1000.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(200.0f, 600.0f));
    if (ImGui::Begin("Scoreboard"))
    {
        int position = 1;
        for (int pos : state.Positions)
        {
            ImGui::Text("%d: %d", position++, pos);
        }
    }
    ImGui::End();
}