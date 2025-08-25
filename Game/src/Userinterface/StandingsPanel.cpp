#include "StandingsPanel.h"

#ifdef MANAGEMENT_MODE

#include <utility>

StandingsPanel::StandingsPanel(std::vector<std::string> positions) : m_Positions(std::move(positions))
{

}

void StandingsPanel::Render()
{
    ImGui::Begin("Standings");
    for (size_t i = 0; i < m_Positions.size(); ++i)
    {
        ImGui::Text("P%zu %s", i + 1, m_Positions[i].c_str());
    }
    ImGui::End();
}

#endif