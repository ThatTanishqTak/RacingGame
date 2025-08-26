#include "GameLayer.h"
#include "Panels/RaceDashboard.h"
#include "Core/RaceState.h"

void GameLayer::Render()
{
    static RaceDashboard Dashboard;
    RaceState State;
    State.Date = "2023-01-01";
    State.Time = "12:00";
    State.Weather = "Clear";
    State.Drivers.push_back({ "Alice", 1, 0 });
    State.Drivers.push_back({ "Bob", 2, 1 });
    State.Positions = { 1, 2 };
    Dashboard.Render(State);
}