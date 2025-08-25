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

PitCrewPanel::PitCrewPanel()
{

}

void PitCrewPanel::Render()
{
    ImGui::Text("Pit crew ready");
}

RaceViewPanel::RaceViewPanel()
{

}

void RaceViewPanel::Render()
{
    ImGui::Text("Race view placeholder");
}

StandingsPanel::StandingsPanel()
{

}

void StandingsPanel::Render()
{
    ImGui::Text("1. Driver A");
    ImGui::Text("2. Driver B");
    ImGui::Text("3. Driver C");
}

DriverInfoPanel::DriverInfoPanel(const std::string& driverName) : m_DriverName(driverName)
{

}

void DriverInfoPanel::Render()
{
    ImGui::Text("%s", m_DriverName.c_str());
    ImGui::Text("Lap Time: --:--");
}

LiveSessionPanel::LiveSessionPanel() : m_DriverOne("Driver 1"), m_DriverTwo("Driver 2")
{

}

void LiveSessionPanel::Render()
{
    ImGuiIO& io = ImGui::GetIO();
    const float sideWidth = 200.0f;
    const float driverHeight = 150.0f;
    const float mainWidth = io.DisplaySize.x - 2.0f * sideWidth;
    const float mainHeight = io.DisplaySize.y - driverHeight;

    ImGui::Begin("Live Session", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
    ImGui::BeginChild("PitCrew", ImVec2(sideWidth, mainHeight), true);
    m_PitCrew.Render();
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(sideWidth, 0.0f));
    ImGui::BeginChild("RaceView", ImVec2(mainWidth, mainHeight), true);
    m_RaceView.Render();
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(sideWidth + mainWidth, 0.0f));
    ImGui::BeginChild("Standings", ImVec2(sideWidth, io.DisplaySize.y), true);
    m_Standings.Render();
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(sideWidth, mainHeight));
    ImGui::BeginChild("DriverOne", ImVec2(mainWidth / 2.0f, driverHeight), true);
    m_DriverOne.Render();
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(sideWidth + mainWidth / 2.0f, mainHeight));
    ImGui::BeginChild("DriverTwo", ImVec2(mainWidth / 2.0f, driverHeight), true);
    m_DriverTwo.Render();
    ImGui::EndChild();
    ImGui::End();
}

UIManager::UIManager() : m_CurrentState(UIState::Budget), m_Budget(1000000.0f), m_Staff(50), m_RaceCalendar("Monaco GP"), m_LiveSession()
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