#include "FlagPanel.h"

#include <imgui.h>

void FlagPanel::Render(const RaceState& state)
{
    if (ImGui::Begin("Flags"))
    {
        ImFont* l_Font = ImGui::GetFont();
        float l_Scale = 1.0f;
        ImGui::SetWindowFontScale(l_Scale);

        ImGui::Text("Current Flag: %s", state.FlagStatus.c_str());

        ImGui::SetWindowFontScale(1.0f);
    }
    ImGui::End();
}