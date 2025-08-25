#include "GameLayer.h"

#include "Core/SessionManager.h"
#include "Core/Circuit.h"
#include "Core/Driver.h"
#include "Core/Team.h"

#include <memory>
#include <vector>
#ifndef MANAGEMENT_MODE
#include <glm/glm.hpp>
#endif

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

#ifndef MANAGEMENT_MODE
    std::vector<glm::vec2> carPositions{ {0.0f, 0.0f}, {50.0f, 20.0f} };
    m_HUD.SetCarPositions(carPositions);
#endif

    m_HUD.Render();
}