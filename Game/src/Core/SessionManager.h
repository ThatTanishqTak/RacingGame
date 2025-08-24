#pragma once

#include "Race.h"
#include "Team.h"

#include <memory>
#include <vector>

class SessionManager
{
private:
    Race Event;
    std::vector<std::shared_ptr<Team>> Teams;

public:
    SessionManager(const Race& race, const std::vector<std::shared_ptr<Team>>& teams);

    void RunPractice();
    void RunQualifying();
    void RunRace();

    Race& GetRace();
    const Race& GetRace() const;

    const std::vector<SessionResult>& GetGrid() const;
};