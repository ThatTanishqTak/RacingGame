#include "RaceDashboard.h"

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
}

void RaceDashboard::RenderPitCrewPanel(const RaceState& state)
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200.0f, 600.0f), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Pit Crew"))
    {
        ImGui::Text("Pit Crew Panel");
    }
    ImGui::End();
}

void RaceDashboard::RenderRaceInfoPanel(const RaceState& state)
{
    ImGui::SetNextWindowPos(ImVec2(200.0f, 0.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(800.0f, 80.0f), ImGuiCond_FirstUseEver);
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
    ImGui::SetNextWindowPos(ImVec2(200.0f, 80.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(800.0f, 400.0f), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Track View"))
    {
        ImGui::Text("Track View Panel");
    }
    ImGui::End();
}

void RaceDashboard::RenderDriverPanels(const RaceState& state)
{
    ImGui::SetNextWindowPos(ImVec2(200.0f, 480.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(800.0f, 120.0f), ImGuiCond_FirstUseEver);
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
    ImGui::SetNextWindowPos(ImVec2(1000.0f, 0.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200.0f, 600.0f), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Positions"))
    {
        int position = 1;
        for (int pos : state.Positions)
        {
            ImGui::Text("%d: %d", position++, pos);
        }
    }
    ImGui::End();
}

void RaceDashboard::RenderToasts()
{
    ImGui::SetNextWindowPos(ImVec2(1000.0f, 600.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200.0f, 100.0f), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("HUD Toasts"))
    {
        for (const auto& msg : Toasts)
        {
            ImGui::Text("%s", msg.c_str());
        }
    }
    ImGui::End();
}