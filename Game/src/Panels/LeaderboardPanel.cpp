#include "LeaderboardPanel.h"

#include <imgui.h>

void LeaderboardPanel::Render(const RaceState& state)
{
    if (ImGui::Begin("Leaderboard"))
    {
        for (size_t i = 0; i < state.Positions.size(); ++i)
        {
            int l_Car = state.Positions[i];
            int l_Lap = i < state.Laps.size() ? state.Laps[i] : 0;
            float l_Gap = i < state.Intervals.size() ? state.Intervals[i] : 0.0f;

            ImGui::Text("#%d Lap %d +%.3fs", l_Car, l_Lap, l_Gap);
        }
    }
    ImGui::End();
}