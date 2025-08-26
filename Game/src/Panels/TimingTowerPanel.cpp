#include "TimingTowerPanel.h"

#include <imgui.h>
#include <algorithm>

void TimingTowerPanel::Render(const RaceState& state)
{
    if (ImGui::Begin("Timing Tower"))
    {
        ImFont* l_Font = ImGui::GetFont();
        float l_Scale = /*l_Font && l_Font->SetWindowFontScale() < 12.0f ? 12.0f / l_Font->FontSize :*/ 1.0f;
        ImGui::SetWindowFontScale(l_Scale);

        int l_Position = 1;
        for (size_t i = 0; i < state.Positions.size(); ++i)
        {
            int l_CarNumber = state.Positions[i];
            const DriverInfo* l_Driver = nullptr;
            for (const auto& it_Driver : state.Drivers)
            {
                if (it_Driver.Number == l_CarNumber)
                {
                    l_Driver = &it_Driver;
                    break;
                }
            }

            std::string l_DriverCode = l_Driver ? l_Driver->Name.substr(0, 3) : std::to_string(l_CarNumber);
            float l_Interval = i < state.Intervals.size() ? state.Intervals[i] : 0.0f;

            ImGui::Text("%it_Driver %s %.3fs", l_Position++, l_DriverCode.c_str(), l_Interval);
        }

        ImGui::SetWindowFontScale(1.0f);
    }
    ImGui::End();
}