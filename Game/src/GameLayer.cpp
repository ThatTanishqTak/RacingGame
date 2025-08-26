#include "GameLayer.h"
#include "panels/RaceDashboard.h"
#include "Core/RaceState.h"

void GameLayer::Render()
{
    static RaceDashboard Dashboard;
    RaceState State;
    State.Date = "2023-01-01";
    State.Time = "12:00";
    State.Weather = "Clear";
    Dashboard.Render(State);
}