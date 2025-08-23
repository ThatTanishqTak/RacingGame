#pragma once

#include <map>
#include <memory>
#include <vector>

class Driver;
class Race;

class Season
{
private:
    std::vector<Race> Races;

public:
    void AddRace(const Race& race);
    const std::vector<Race>& GetRaces() const;

    std::map<std::shared_ptr<Driver>, int> GetLeaderboard() const;
};