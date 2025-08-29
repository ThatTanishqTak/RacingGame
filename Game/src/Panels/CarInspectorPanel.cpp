#include "CarInspectorPanel.h"

#include "Renderer/Renderer.h"
#include "Renderer/StateStream.h"
#include "Core/PaletteManager.h"

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <algorithm>

CarInspectorPanel::CarInspectorPanel(Engine::Renderer& renderer) : m_Renderer(renderer)
{

}

void CarInspectorPanel::Render(const RaceState& state)
{
    if (ImGui::Begin("Car Inspector"))
    {
        double l_Time = glfwGetTime();
        auto a_Cars = Engine::g_StateBuffer.Interpolate(l_Time);

        for (const auto& it_Car : a_Cars)
        {
            ImVec4 l_Colour = g_PaletteManager.GetTeamColour(static_cast<size_t>(it_Car.ID), false);
            ImGui::TextColored(l_Colour, "Car %d Speed %.1f", it_Car.ID + 1, it_Car.Speed);
        }

        bool l_ShowLabels = m_Renderer.m_ShowCarLabels;
        if (ImGui::Checkbox("Car Labels", &l_ShowLabels))
        {
            m_Renderer.m_ShowCarLabels = l_ShowLabels;
        }

        bool l_ShowLanes = m_Renderer.m_ShowLaneLines;
        if (ImGui::Checkbox("Lane Lines", &l_ShowLanes))
        {
            m_Renderer.m_ShowLaneLines = l_ShowLanes;
        }
    }
    ImGui::End();
}