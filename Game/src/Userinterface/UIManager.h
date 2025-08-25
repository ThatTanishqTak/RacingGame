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

class PitCrewPanel : public HUDPanel
{
public:
    PitCrewPanel();
    void Render() override;
};

class RaceViewPanel : public HUDPanel
{
public:
    RaceViewPanel();
    void Render() override;
};

class StandingsPanel : public HUDPanel
{
public:
    StandingsPanel();
    void Render() override;
};

class DriverInfoPanel : public HUDPanel
{
public:
    explicit DriverInfoPanel(const std::string& driverName);
    void Render() override;

private:
    std::string m_DriverName;
};

class LiveSessionPanel : public HUDPanel
{
public:
    LiveSessionPanel();
    void Render() override;

private:
    PitCrewPanel m_PitCrew;
    RaceViewPanel m_RaceView;
    StandingsPanel m_Standings;
    DriverInfoPanel m_DriverOne;
    DriverInfoPanel m_DriverTwo;
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