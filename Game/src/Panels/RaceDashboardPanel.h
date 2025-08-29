#pragma once

#include "Core/RaceState.h"
#include <string>
#include <vector>

class EventBus;
namespace Engine { class Renderer; }

class RaceDashboardPanel
{
public:
    RaceDashboardPanel(EventBus& eventBus, Engine::Renderer& renderer);
    ~RaceDashboardPanel();
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
    EventBus& m_EventBus;
    Engine::Renderer& m_Renderer;
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
    int m_PitInToken = 0;
    int m_PitOutToken = 0;
    int m_DNFToken = 0;
    int m_ViewModeToken = 0;
};