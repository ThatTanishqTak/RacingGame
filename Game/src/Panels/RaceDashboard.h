#pragma once

#include "Core/RaceState.h"

class RaceDashboard
{
public:
    void Render(const RaceState& state);

private:
    void RenderPitCrewPanel(const RaceState& state);
    void RenderRaceInfoPanel(const RaceState& state);
    void RenderTrackViewPanel(const RaceState& state);
    void RenderDriverPanels(const RaceState& state);
    void RenderScoreboardPanel(const RaceState& state);
};