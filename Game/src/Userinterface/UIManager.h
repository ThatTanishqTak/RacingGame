#pragma once

#ifdef MANAGEMENT_MODE

#include <imgui.h>
#include <string>

#include "UIState.h"

class HUDPanel
{
public:
    virtual ~HUDPanel() = default;
    virtual void Render() = 0;
};

class BudgetPanel : public HUDPanel
{
public:
    explicit BudgetPanel(float budget);
    void Render() override;

private:
    float m_Budget;
};

class StaffPanel : public HUDPanel
{
public:
    explicit StaffPanel(int staff);
    void Render() override;

private:
    int m_StaffCount;
};

class RaceCalendarPanel : public HUDPanel
{
public:
    explicit RaceCalendarPanel(const std::string& nextRace);
    void Render() override;

private:
    std::string m_NextRace;
};

class LiveSessionPanel : public HUDPanel
{
public:
    explicit LiveSessionPanel(int lap);
    void Render() override;

private:
    int m_CurrentLap;
};

class UIManager
{
public:
    UIManager();
    void Render();

private:
    UIState m_CurrentState;
    BudgetPanel m_Budget;
    StaffPanel m_Staff;
    RaceCalendarPanel m_RaceCalendar;
    LiveSessionPanel m_LiveSession;
};

#endif