#include "GameLayer.h"

#include "Panels/RaceDashboardPanel.h"
#include "Panels/TimingTowerPanel.h"
#include "Panels/FlagPanel.h"
#include "Panels/LeaderboardPanel.h"
#include "Panels/CarInspectorPanel.h"
#include "Core/RaceState.h"
#include "Core/EventBus.h"
#include "Renderer/Renderer.h"
#include "Renderer/StateStream.h"
#include "Core/PaletteManager.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

GameLayer::GameLayer(EventBus& eventBus, Engine::Renderer& renderer) : m_EventBus(eventBus), m_Renderer(renderer)
{

}

void GameLayer::Update()
{
    static bool s_TogglePressed = false;
    bool l_TabDown = !ImGui::GetIO().WantCaptureKeyboard && ImGui::IsKeyDown(ImGuiKey_Tab);
    if (l_TabDown && !s_TogglePressed)
    {
        bool l_TopDown = m_Renderer.GetViewMode() == Engine::Renderer::ViewMode::View2DTopDown;
        m_EventBus.Publish(ViewModeToggle{ !l_TopDown });
    }
    s_TogglePressed = l_TabDown;
}

void GameLayer::Render(const std::vector<std::string>& trackLayout)
{
    static RaceDashboardPanel s_Dashboard(m_EventBus, m_Renderer);
    static TimingTowerPanel s_TimingTower;
    static FlagPanel s_Flags;
    static LeaderboardPanel s_Leaderboard;
    static CarInspectorPanel s_Inspector(m_Renderer);

    RaceState l_State;
    l_State.Date = "18-10-2025";
    l_State.Time = "12:00";
    l_State.Weather = "Clear";
    l_State.Drivers.push_back({ "Alice", 1, 0 });
    l_State.Drivers.push_back({ "Bob", 2, 1 });
    l_State.Positions = { 1, 2 };
    l_State.Intervals = { 0.0f, 1.2f };
    l_State.Laps = { 3, 3 };
    l_State.FlagStatus = "Green";
    l_State.TrackLayout = trackLayout;

#ifdef MANAGEMENT_MODE
    ImDrawList* l_DrawList = ImGui::GetForegroundDrawList();
    ImVec2 l_Size = ImGui::GetIO().DisplaySize;

    if (!trackLayout.empty())
    {
        float l_LayoutW = static_cast<float>(trackLayout[0].size());
        float l_LayoutH = static_cast<float>(trackLayout.size());
        float l_TileW = l_Size.x / l_LayoutW;
        float l_TileH = l_Size.y / l_LayoutH;

        float l_Base = std::min(l_TileW, l_TileH);
        float l_Padding = l_Base * 0.1f;
        float l_Thickness = std::max(1.0f, l_Base * 0.1f);

        for (int y = 0; y < (int)trackLayout.size(); ++y)
        {
            const std::string& l_Row = trackLayout[y];
            for (int x = 0; x < (int)l_Row.size(); ++x)
            {
                char l_Type = l_Row[x];
                ImVec2 l_Min{ x * l_TileW, y * l_TileH };
                ImVec2 l_Max{ (x + 1) * l_TileW, (y + 1) * l_TileH };
                float l_X = (l_Min.x + l_Max.x) * 0.5f;
                float l_Y = (l_Min.y + l_Max.y) * 0.5f;
                ImU32 l_Col = IM_COL32(130, 130, 130, 255);

                if (l_Type == '-')
                {
                    l_DrawList->AddRectFilled(ImVec2(l_Min.x + l_Padding, l_Y - l_Thickness * 0.5f), ImVec2(l_Max.x - l_Padding, l_Y + l_Thickness * 0.5f), l_Col);
                }

                else if (l_Type == '|')
                {
                    l_DrawList->AddRectFilled(ImVec2(l_X - l_Thickness * 0.5f, l_Min.y + l_Padding), ImVec2(l_X + l_Thickness * 0.5f, l_Max.y - l_Padding), l_Col);
                }

                else if (l_Type == '\\')
                {
                    l_DrawList->AddLine(ImVec2(l_Min.x + l_Padding, l_Max.y - l_Padding), ImVec2(l_Max.x - l_Padding, l_Min.y + l_Padding), l_Col, l_Thickness);
                }

                else if (l_Type == '/')
                {
                    l_DrawList->AddLine(ImVec2(l_Min.x + l_Padding, l_Min.y + l_Padding), ImVec2(l_Max.x - l_Padding, l_Max.y - l_Padding), l_Col, l_Thickness);
                }
            }
        }

        glm::vec2 l_MinBounds = m_Renderer.GetTrackMin();
        glm::vec2 l_MaxBounds = m_Renderer.GetTrackMax();

        auto a_WorldToScreen = [&](const glm::vec3& p)
            {
                float nx = (p.x - l_MinBounds.x) / (l_MaxBounds.x - l_MinBounds.x);
                float ny = (p.z - l_MinBounds.y) / (l_MaxBounds.y - l_MinBounds.y);
                return ImVec2(nx * l_Size.x, ny * l_Size.y);
            };

        double l_Time = glfwGetTime();
        auto a_Cars = Engine::g_StateBuffer.Interpolate(l_Time);
        for (const auto& it_Car : a_Cars)
        {
            ImVec4 l_Color = g_PaletteManager.GetTeamColour(static_cast<size_t>(it_Car.ID), false);
            ImU32 l_DrawCol = IM_COL32((int)(l_Color.x * 255.0f), (int)(l_Color.y * 255.0f), (int)(l_Color.z * 255.0f), 255);
            ImVec2 l_Pos = a_WorldToScreen(it_Car.Position);
            l_DrawList->AddCircleFilled(l_Pos, 5.0f, l_DrawCol);
        }
    }
#endif

    s_Dashboard.Render(l_State);
    s_TimingTower.Render(l_State);
    s_Flags.Render(l_State);
    s_Leaderboard.Render(l_State);
    s_Inspector.Render(l_State);
}