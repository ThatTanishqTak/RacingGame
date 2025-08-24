#include "SessionManager.h"
#include "Driver.h"

SessionManager::SessionManager(const Race& race, const std::vector<std::shared_ptr<Team>>& teams) : Event(race), Teams(teams)
{

}

void SessionManager::RunPractice()
{
    std::vector<std::shared_ptr<Driver>> drivers;
    for (const auto& team : Teams)
    {
        const auto& teamDrivers = team->GetDrivers();
        drivers.insert(drivers.end(), teamDrivers.begin(), teamDrivers.end());
    }
    Event.ConductPractice(drivers);
}

void SessionManager::RunQualifying()
{
    std::vector<std::shared_ptr<Driver>> drivers;
    for (const auto& team : Teams)
    {
        const auto& teamDrivers = team->GetDrivers();
        drivers.insert(drivers.end(), teamDrivers.begin(), teamDrivers.end());
    }
    Event.ConductQualifying(drivers);
}

void SessionManager::RunRace()
{
    Event.ConductRace(Teams);
}

Race& SessionManager::GetRace()
{
    return Event;
}

const Race& SessionManager::GetRace() const
{
    return Event;
}

const std::vector<SessionResult>& SessionManager::GetGrid() const
{
    return Event.GetQualifyingResults();
}