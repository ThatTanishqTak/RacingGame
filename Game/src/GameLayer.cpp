#include "GameLayer.h"

#include "Panels/RaceDashboard.h"
#include "Panels/TimingTowerPanel.h"
#include "Panels/FlagPanel.h"
#include "Core/RaceState.h"
#include "Core/Circuit.h"

void GameLayer::Update()
{

}

void GameLayer::Render()
{
    static RaceDashboard Dashboard;
    static TimingTowerPanel TimingTower;
    static FlagPanel Flags;
    static Circuit Track("Sample Track", 5.0);
    static bool LayoutLoaded = false;

    if (!LayoutLoaded)
    {
        Track.LoadLayout("Game/Assets/Tracks/SampleTrack.txt");
        LayoutLoaded = true;
    }

    RaceState State;
    State.Date = "18-10-2025";
    State.Time = "12:00";
    State.Weather = "Clear";
    State.Drivers.push_back({ "Alice", 1, 0 });
    State.Drivers.push_back({ "Bob", 2, 1 });
    State.Positions = { 1, 2 };
    State.Intervals = { 0.0f, 1.2f };
    State.FlagStatus = "Green";
    State.TrackLayout = Track.GetLayout();

    Dashboard.Render(State);
    TimingTower.Render(State);
    Flags.Render(State);
}