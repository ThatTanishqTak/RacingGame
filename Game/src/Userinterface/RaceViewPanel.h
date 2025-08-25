#pragma once

#ifdef MANAGEMENT_MODE

#include <string>

class RaceViewPanel
{
public:
    RaceViewPanel(const std::string& date, const std::string& time, const std::string& weather);
    void Render();

private:
    std::string m_Date;
    std::string m_Time;
    std::string m_Weather;
};

#else

class RaceViewPanel
{
public:
    RaceViewPanel(const std::string&, const std::string&, const std::string&) {}
    void Render() {}
};

#endif