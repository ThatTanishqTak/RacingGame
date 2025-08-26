#include "TimingTowerPanel.h"

#include <imgui.h>
#include <algorithm>

void TimingTowerPanel::Render(const RaceState& state)
{
    if (ImGui::Begin("Timing Tower"))
    {
        ImFont* font = ImGui::GetFont();
        float scale = /*font && font->FontSize < 12.0f ? 12.0f / font->FontSize :*/ 1.0f;
        ImGui::SetWindowFontScale(scale);

        int position = 1;
        for (size_t i = 0; i < state.Positions.size(); ++i)
        {
            int carNumber = state.Positions[i];
            const DriverInfo* driver = nullptr;
            for (const auto& d : state.Drivers)
            {
                if (d.Number == carNumber)
                {
                    driver = &d;
                    break;
                }
            }

            std::string driverCode = driver ? driver->Name.substr(0, 3) : std::to_string(carNumber);
            float interval = i < state.Intervals.size() ? state.Intervals[i] : 0.0f;
            ImGui::Text("%d %s %.3fs", position++, driverCode.c_str(), interval);
        }

        ImGui::SetWindowFontScale(1.0f);
    }
    ImGui::End();
}