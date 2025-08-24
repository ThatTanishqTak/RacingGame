#include "HUD.h"

#ifdef MANAGEMENT_MODE

HUD::HUD() : m_Budget(1000000.0f), m_Staff(50), m_RaceCalendar("Monaco GP"), m_LiveSession(1)
{

}

void HUD::Render()
{
    ImGuiIO& io = ImGui::GetIO();
    const float sideBarWidth = 200.0f;
    const float topBarHeight = 50.0f;

    ImGui::SetNextWindowPos(ImVec2(sideBarWidth, 0.0f));
    ImGui::SetNextWindowSize(ImVec2((io.DisplaySize.x - sideBarWidth) * 0.5f, topBarHeight));
    m_Budget.Render();

    ImGui::SetNextWindowPos(ImVec2(sideBarWidth + (io.DisplaySize.x - sideBarWidth) * 0.5f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2((io.DisplaySize.x - sideBarWidth) * 0.5f, topBarHeight));
    m_LiveSession.Render();

    ImGui::SetNextWindowPos(ImVec2(0.0f, topBarHeight));
    ImGui::SetNextWindowSize(ImVec2(sideBarWidth, (io.DisplaySize.y - topBarHeight) * 0.5f));
    m_Staff.Render();

    ImGui::SetNextWindowPos(ImVec2(0.0f, topBarHeight + (io.DisplaySize.y - topBarHeight) * 0.5f));
    ImGui::SetNextWindowSize(ImVec2(sideBarWidth, (io.DisplaySize.y - topBarHeight) * 0.5f));
    m_RaceCalendar.Render();
}

#else

HUD::HUD()
{

}

void HUD::Render()
{

}

#endif 