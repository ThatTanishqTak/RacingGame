#include "UIManager.h"

#ifdef MANAGEMENT_MODE

BudgetPanel::BudgetPanel(float budget) : m_Budget(budget)
{

}

void BudgetPanel::Render()
{
    ImGui::Begin("Budget");
    ImGui::Text("Budget: $%.2f", m_Budget);
    ImGui::End();
}

StaffPanel::StaffPanel(int staff) : m_StaffCount(staff)
{

}

void StaffPanel::Render()
{
    ImGui::Begin("Staff");
    ImGui::Text("Staff: %d", m_StaffCount);
    ImGui::End();
}

RaceCalendarPanel::RaceCalendarPanel(const std::string& nextRace) : m_NextRace(nextRace)
{

}

void RaceCalendarPanel::Render()
{
    ImGui::Begin("Race Calendar");
    ImGui::Text("Next Race: %s", m_NextRace.c_str());
    ImGui::End();
}

LiveSessionPanel::LiveSessionPanel(int lap) : m_CurrentLap(lap)
{

}

void LiveSessionPanel::Render()
{
    ImGui::Begin("Live Session");
    ImGui::Text("Lap: %d", m_CurrentLap);
    ImGui::End();
}

UIManager::UIManager() : m_CurrentState(UIState::Budget), m_Budget(1000000.0f), m_Staff(50), m_RaceCalendar("Monaco GP"), m_LiveSession(1)
{

}

void UIManager::Render()
{
    ImGuiIO& io = ImGui::GetIO();
    const float sideBarWidth = 200.0f;

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(sideBarWidth, io.DisplaySize.y));
    ImGui::Begin("Navigation", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if (ImGui::Button("Budget", ImVec2(sideBarWidth - 10.0f, 40.0f)))
    {
        m_CurrentState = UIState::Budget;
    }
    
    if (ImGui::Button("Staff", ImVec2(sideBarWidth - 10.0f, 40.0f)))
    {
        m_CurrentState = UIState::Staff;
    }
    
    if (ImGui::Button("Calendar", ImVec2(sideBarWidth - 10.0f, 40.0f)))
    {
        m_CurrentState = UIState::Calendar;
    }
    
    if (ImGui::Button("Live", ImVec2(sideBarWidth - 10.0f, 40.0f)))
    {
        m_CurrentState = UIState::LiveSession;
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(sideBarWidth, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - sideBarWidth, io.DisplaySize.y));

    switch (m_CurrentState)
    {
    case UIState::Budget:
        m_Budget.Render();
        break;
    case UIState::Staff:
        m_Staff.Render();
        break;
    case UIState::Calendar:
        m_RaceCalendar.Render();
        break;
    case UIState::LiveSession:
        m_LiveSession.Render();
        break;
    }
}

#endif