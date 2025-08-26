#include "Race.h"

#include "Core/Circuit.h"
#include "Core/Driver.h"
#include "Core/EventBus.h"
#include "Core/RaceState.h"

#include <algorithm>
#include <random>

Race::Race(const std::shared_ptr<Circuit>& circuit, const std::string& date) : Track(circuit), Date(date)
{

}

const std::shared_ptr<Circuit>& Race::GetCircuit() const
{
    return Track;
}

const std::string& Race::GetDate() const
{
    return Date;
}

std::vector<SessionResult> Race::GenerateResults(const std::vector<std::shared_ptr<Driver>>& drivers)
{
    std::vector<SessionResult> results;
    std::mt19937 rng{ std::random_device{}() };
    std::uniform_real_distribution<double> dist(60.0, 120.0);

    for (const auto& driver : drivers)
    {
        results.push_back({ driver, dist(rng) });
    }

    std::sort(results.begin(), results.end(), [](const SessionResult& a, const SessionResult& b)
        {
            return a.Time < b.Time;
        });

    return results;
}

std::vector<SessionResult> Race::ConductPractice(const std::vector<std::shared_ptr<Driver>>& drivers)
{
    PracticeResults = GenerateResults(drivers);

    return PracticeResults;
}

std::vector<SessionResult> Race::ConductQualifying(const std::vector<std::shared_ptr<Driver>>& drivers)
{
    QualifyingResults = GenerateResults(drivers);

    return QualifyingResults;
}

std::vector<SessionResult> Race::ConductRace(const std::vector<std::shared_ptr<Driver>>& drivers)
{
    RaceResults = GenerateResults(drivers);

    std::mt19937 rng{ std::random_device{}() };
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (const auto& driver : drivers)
    {
        double chance = dist(rng);
        if (chance < 0.1)
        {
            g_EventBus.Publish(PitIn{ driver->GetName() });
            g_EventBus.Publish(PitOut{ driver->GetName() });
        }
        else if (chance < 0.15)
        {
            g_EventBus.Publish(DNF{ driver->GetName(), "mechanical failure" });
        }
    }

    return RaceResults;
}

const std::vector<SessionResult>& Race::GetPracticeResults() const
{
    return PracticeResults;
}

const std::vector<SessionResult>& Race::GetQualifyingResults() const
{
    return QualifyingResults;
}

const std::vector<SessionResult>& Race::GetRaceResults() const
{
    return RaceResults;
}