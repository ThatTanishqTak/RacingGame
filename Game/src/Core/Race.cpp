#include "Race.h"
#include "Circuit.h"
#include "Driver.h"
#include "RaceEventManager.h"
#include "Team.h"
#include "StrategyPlanner.h"

#include <algorithm>
#include <limits>
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

std::vector<SessionResult> Race::ConductRace(const std::vector<std::shared_ptr<Team>>& teams)
{
    RaceEventManager manager;
    manager.LoadFromJson("Game/Assets/Events.json");

    const int laps = 5;

    std::vector<std::shared_ptr<Driver>> drivers;
    for (const auto& team : teams)
    {
        const auto& teamDrivers = team->GetDrivers();
        drivers.insert(drivers.end(), teamDrivers.begin(), teamDrivers.end());
    }

    std::vector<CarEventState> carStates(drivers.size());
    std::vector<double> totalTimes(drivers.size(), 0.0);

    std::mt19937 rng{ std::random_device{}() };
    std::uniform_real_distribution<double> dist(60.0, 120.0);

    StrategyPlanner planner;

    for (int lap = 0; lap < laps; ++lap)
    {
        TrackFlag flag = TrackFlag::Green;
        manager.EvaluateLap(carStates, flag);

        for (const auto& team : teams)
        {
            planner.UpdateERSMode(*team);
            if (flag == TrackFlag::Green)
            {
                planner.PlanPitStops(*team, lap, laps, rng);
            }
            else
            {
                planner.ReplanForSafetyCar(*team, flag, lap, laps);
            }
        }

        for (size_t i = 0; i < drivers.size(); ++i)
        {
            if (!carStates[i].Retired)
            {
                double lapTime = dist(rng);
                if (flag == TrackFlag::SafetyCar)
                {
                    lapTime *= 1.1;
                }
                else if (flag == TrackFlag::VirtualSafetyCar)
                {
                    lapTime *= 1.05;
                }
                totalTimes[i] += lapTime;
            }
        }
    }

    RaceResults.clear();
    for (size_t i = 0; i < drivers.size(); ++i)
    {
        double t = carStates[i].Retired ? std::numeric_limits<double>::infinity() : totalTimes[i];
        RaceResults.push_back({ drivers[i], t });
    }

    std::sort(RaceResults.begin(), RaceResults.end(), [](const SessionResult& a, const SessionResult& b)
        {
            return a.Time < b.Time;
        });

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