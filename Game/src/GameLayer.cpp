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
    static RaceDashboard l_Dashboard;
    static TimingTowerPanel l_TimingTower;
    static FlagPanel l_Flags;
    static Circuit l_Track("Sample l_Track", 5.0);
    static bool l_LayoutLoaded = false;

    if (!l_LayoutLoaded)
    {
        l_Track.LoadLayout("Assets/Tracks/SampleTrack.txt");
        l_LayoutLoaded = true;
    }

    RaceState l_State;
    l_State.Date = "18-10-2025";
    l_State.Time = "12:00";
    l_State.Weather = "Clear";
    l_State.Drivers.push_back({ "Alice", 1, 0 });
    l_State.Drivers.push_back({ "Bob", 2, 1 });
    l_State.Positions = { 1, 2 };
    l_State.Intervals = { 0.0f, 1.2f };
    l_State.FlagStatus = "Green";
    l_State.TrackLayout = l_Track.GetLayout();

    l_Dashboard.Render(l_State);
    l_TimingTower.Render(l_State);
    l_Flags.Render(l_State);
}