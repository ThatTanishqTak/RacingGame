#pragma once

#ifdef MANAGEMENT_MODE

#include <imgui.h>
#include <string>

class HudPanel
{
public:
    virtual ~HudPanel() = default;
    virtual void Render() = 0;
};

class BudgetPanel : public HudPanel
{
public:
    explicit BudgetPanel(float budget) : m_Budget(budget) {}
    void Render() override
    {
        ImGui::Begin("Budget");
        ImGui::Text("Budget: $%.2f", m_Budget);
        ImGui::End();
    }
private:
    float m_Budget;
};

class StaffPanel : public HudPanel
{
public:
    explicit StaffPanel(int staff) : m_StaffCount(staff) {}
    void Render() override
    {
        ImGui::Begin("Staff");
        ImGui::Text("Staff: %d", m_StaffCount);
        ImGui::End();
    }
private:
    int m_StaffCount;
};

class RaceCalendarPanel : public HudPanel
{
public:
    explicit RaceCalendarPanel(const std::string& nextRace) : m_NextRace(nextRace) {}
    void Render() override
    {
        ImGui::Begin("Race Calendar");
        ImGui::Text("Next Race: %s", m_NextRace.c_str());
        ImGui::End();
    }
private:
    std::string m_NextRace;
};

class LiveSessionPanel : public HudPanel
{
public:
    explicit LiveSessionPanel(int lap) : m_CurrentLap(lap) {}
    void Render() override
    {
        ImGui::Begin("Live Session");
        ImGui::Text("Lap: %d", m_CurrentLap);
        ImGui::End();
    }
private:
    int m_CurrentLap;
};

class HUD
{
public:
    HUD();
    void Render();
private:
    BudgetPanel m_Budget;
    StaffPanel m_Staff;
    RaceCalendarPanel m_RaceCalendar;
    LiveSessionPanel m_LiveSession;
};

#else

class HUD
{
public:
    HUD() = default;
    void Render() {}
};

#endif