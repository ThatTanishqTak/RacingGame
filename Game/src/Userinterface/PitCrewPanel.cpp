#include "PitCrewPanel.h"

#ifdef MANAGEMENT_MODE

PitCrewPanel::PitCrewPanel(const std::vector<std::string>& instructions) : m_Instructions(instructions)
{

}

void PitCrewPanel::Render()
{
    ImGui::Begin("Pit Crew");
    for (const std::string& line : m_Instructions)
    {
        ImGui::TextUnformatted(line.c_str());
    }
    ImGui::End();
}

#endif