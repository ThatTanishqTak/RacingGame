#pragma once

#include <iostream>
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
        std::cout << "Budget: $" << m_Budget << '\n';
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
        std::cout << "Staff: " << m_StaffCount << '\n';
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
        std::cout << "Next Race: " << m_NextRace << '\n';
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
        std::cout << "Lap: " << m_CurrentLap << '\n';
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