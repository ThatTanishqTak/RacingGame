#pragma once

#ifdef MANAGEMENT_MODE

#include <imgui.h>
#include <string>
#include <vector>

#include "UIManager.h"

class PitCrewPanel : public HUDPanel
{
public:
    explicit PitCrewPanel(const std::vector<std::string>& instructions);
    void Render() override;

private:
    std::vector<std::string> m_Instructions;
};

#endif