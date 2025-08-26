#include "Season.h"
#include "Race.h"
#include "Driver.h"

void Season::AddRace(const Race& race)
{
    Races.push_back(race);
}

const std::vector<Race>& Season::GetRaces() const
{
    return Races;
}

std::map<std::shared_ptr<Driver>, int> Season::GetLeaderboard() const
{
    std::map<std::shared_ptr<Driver>, int> leaderboard;
    const std::vector<int> PointsTable = { 25, 18, 15, 12, 10, 8, 6, 4, 2, 1 };

    for (const auto& race : Races)
    {
        const auto& results = race.GetRaceResults();

        for (size_t i = 0; i < results.size(); ++i)
        {
            int points = 0;
            if (i < PointsTable.size())
            {
                points = PointsTable[i];
            }

            auto& driverPoints = leaderboard[results[i].DriverRef];
            driverPoints += points;
        }
    }

    return leaderboard;
}