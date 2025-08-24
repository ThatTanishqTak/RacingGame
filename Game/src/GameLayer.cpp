#include "GameLayer.h"
#include "Core/SessionManager.h"
#include "Core/Circuit.h"
#include "Core/Driver.h"
#include "Core/Team.h"

#include <memory>

void GameLayer::Render()
{
    auto circuit = std::make_shared<Circuit>("Demo Circuit", 5.0);

    auto teamA = std::make_shared<Team>("Alpha");
    teamA->AddDriver(std::make_shared<Driver>("Driver1", 30));
    teamA->AddDriver(std::make_shared<Driver>("Driver2", 31));

    auto teamB = std::make_shared<Team>("Beta");
    teamB->AddDriver(std::make_shared<Driver>("Driver3", 32));
    teamB->AddDriver(std::make_shared<Driver>("Driver4", 33));

    std::vector<std::shared_ptr<Team>> teams{ teamA, teamB };
    Race race(circuit, "2024-01-01");
    SessionManager manager(race, teams);
    manager.RunPractice();
    manager.RunQualifying();
    manager.RunRace();

    m_HUD.Render();
}