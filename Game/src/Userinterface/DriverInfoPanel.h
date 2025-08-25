#pragma once

#ifdef MANAGEMENT_MODE

#include <string>

class DriverInfoPanel
{
public:
    DriverInfoPanel(const std::string& name, float lapTime, const std::string& tireCompound, float tireWear, float fuelLevel, int position);
    void Render();

private:
    std::string m_Name;
    float m_LapTime;
    std::string m_TireCompound;
    float m_TireWear;
    float m_FuelLevel;
    int m_Position;
};

#endif