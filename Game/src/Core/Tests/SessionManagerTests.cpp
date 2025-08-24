#include "Circuit.h"
#include "Driver.h"
#include "Race.h"
#include "SessionManager.h"
#include "Team.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

static bool EqualResults(const std::vector<SessionResult>& a, const std::vector<SessionResult>& b)
{
    if (a.size() != b.size())
    {
        return false;
    }

    for (size_t i = 0; i < a.size(); ++i)
    {
        if (a[i].Driver != b[i].Driver || a[i].Time != b[i].Time)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    auto circuit = std::make_shared<Circuit>("Test", 5.0);
    auto teamA = std::make_shared<Team>("Alpha");
    teamA->AddDriver(std::make_shared<Driver>("A1", 30));
    teamA->AddDriver(std::make_shared<Driver>("A2", 31));
    auto teamB = std::make_shared<Team>("Beta");
    teamB->AddDriver(std::make_shared<Driver>("B1", 32));
    teamB->AddDriver(std::make_shared<Driver>("B2", 33));
    std::vector<std::shared_ptr<Team>> teams{ teamA, teamB };

    Race race(circuit, "2024-01-01");
    SessionManager manager(race, teams);

    manager.RunPractice();
    std::vector<std::shared_ptr<Driver>> drivers;
    for (const auto& team : teams)
    {
        const auto& teamDrivers = team->GetDrivers();
        drivers.insert(drivers.end(), teamDrivers.begin(), teamDrivers.end());
    }
    auto initialPractice = manager.GetRace().GetPracticeResults();

    manager.RunQualifying();
    assert(manager.GetRace().IsParcFerme());
    auto rePractice = manager.GetRace().ConductPractice(drivers);
    assert(EqualResults(initialPractice, rePractice));

    manager.RunRace();
    const auto& grid = manager.GetGrid();
    const auto& raceResults = manager.GetRace().GetRaceResults();
    std::vector<std::shared_ptr<Driver>> gridDrivers;
    for (const auto& r : grid)
    {
        gridDrivers.push_back(r.Driver);
    }
    std::vector<std::shared_ptr<Driver>> raceDrivers;
    for (const auto& r : raceResults)
    {
        raceDrivers.push_back(r.Driver);
    }
    assert(std::is_permutation(gridDrivers.begin(), gridDrivers.end(), raceDrivers.begin()));

    return 0;
}