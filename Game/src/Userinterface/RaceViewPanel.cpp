#include "RaceViewPanel.h"

#ifdef MANAGEMENT_MODE

#include <imgui.h>

RaceViewPanel::RaceViewPanel(const std::string& date, const std::string& time, const std::string& weather) : m_Date(date), m_Time(time), m_Weather(weather)
{

}

void RaceViewPanel::Render()
{
    ImGui::Begin("Race View");
    ImGui::Text("Date: %s", m_Date.c_str());
    ImGui::Text("Time: %s", m_Time.c_str());
    ImGui::Text("Weather: %s", m_Weather.c_str());

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    const ImVec2 center = ImGui::GetCursorScreenPos();
    const ImVec2 ovalCenter(center.x + 200.0f, center.y + 150.0f);
    drawList->AddEllipse(ovalCenter, ImVec2(150.0f, 75.0f), IM_COL32(255, 255, 255, 255));

    ImGui::End();
}

#else

RaceViewPanel::RaceViewPanel(const std::string&, const std::string&, const std::string&)
{

}

void RaceViewPanel::Render()
{

}

#endif