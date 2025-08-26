#pragma once

#include "Core/RaceState.h"
#include "Core/EventBus.h"
#include <string>
#include <vector>

class RaceDashboard
{
public:
    RaceDashboard();
    void Render(const RaceState& state);

private:
    void RenderPitCrewPanel(const RaceState& state);
    void RenderRaceInfoPanel(const RaceState& state);
    void RenderTrackViewPanel(const RaceState& state);
    void RenderDriverPanels(const RaceState& state);
    void RenderScoreboardPanel(const RaceState& state);
    void RenderToasts();
    void RenderSettingsPanel();

private:
    std::vector<std::string> m_Toasts;
    bool m_ColourBlindMode = false;
};