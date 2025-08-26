#include "FlagPanel.h"

#include <imgui.h>

void FlagPanel::Render(const RaceState& state)
{
    if (ImGui::Begin("Flags"))
    {
        ImFont* font = ImGui::GetFont();
        float scale = /*font && font->FontSize < 12.0f ? 12.0f / font->FontSize :*/ 1.0f;
        ImGui::SetWindowFontScale(scale);

        ImGui::Text("Current Flag: %s", state.FlagStatus.c_str());

        ImGui::SetWindowFontScale(1.0f);
    }
    ImGui::End();
}