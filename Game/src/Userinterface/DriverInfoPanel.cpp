#include "DriverInfoPanel.h"

#ifdef MANAGEMENT_MODE

#include <imgui.h>

DriverInfoPanel::DriverInfoPanel(const std::string& name, float lapTime, const std::string& tireCompound, float tireWear, float fuelLevel, int position)
        : m_Name(name), m_LapTime(lapTime), m_TireCompound(tireCompound), m_TireWear(tireWear), m_FuelLevel(fuelLevel), m_Position(position)
{

}

void DriverInfoPanel::Render()
{
    ImGui::Begin(m_Name.c_str());
    ImGui::Text("Position: %d", m_Position);
    ImGui::Text("Lap Time: %.3f", m_LapTime);
    ImGui::Text("Tire: %s", m_TireCompound.c_str());
    ImGui::Text("Tire Wear: %.1f%%", m_TireWear);
    ImGui::Text("Fuel Level: %.1f L", m_FuelLevel);
    ImGui::End();
}

#endif