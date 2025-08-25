#pragma once

#ifdef MANAGEMENT_MODE

#include <imgui.h>
#include <string>
#include <vector>

class StandingsPanel
{
public:
    StandingsPanel() = default;
    explicit StandingsPanel(std::vector<std::string> positions);
    void Render();

private:
    std::vector<std::string> m_Positions;
};

#endif