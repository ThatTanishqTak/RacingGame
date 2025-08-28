#pragma once

#include "Core/RaceState.h"
#include "Core/EventBus.h"
#include <string>
#include <vector>

class RaceDashboardPanel
{
public:
    RaceDashboardPanel();
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
    bool m_TopDownView = false;
    bool  m_ShowCars = true;
    bool  m_LabelCars = true;
    bool  m_ShowLaneLines = false;
    float m_TrackZoom = 0.8f;
    float m_TrackThickness = 0.22f;
    float m_TrackPadding = 0.12f;
    float m_CarRadiusPx = 7.0f;
};