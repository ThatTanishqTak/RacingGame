#include "StrategyPlanner.h"

#include <algorithm>
#include <chrono>

int StrategyPlanner::PlanPitStops(const Team& team, int currentLap, int totalLaps, std::mt19937& rng)
{
    std::uniform_int_distribution<int> dist(currentLap + 1, totalLaps);
    (void)team;

    return dist(rng);
}

int StrategyPlanner::ReplanForSafetyCar(const Team& team, TrackFlag flag, int currentLap, int totalLaps) const
{
    auto start = std::chrono::high_resolution_clock::now();
    int plan = currentLap + (flag == TrackFlag::SafetyCar ? 1 : 2);
    plan = std::min(plan, totalLaps);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    size_t cars = std::max<size_t>(1, team.GetDrivers().size());
    (void)duration;
    (void)cars;

    return plan;
}

void StrategyPlanner::UpdateERSMode(const Team& team) const
{
    (void)team;
}

double StrategyPlanner::ComputeStintETA(double stintEndTime, double currentTime)
{
    return stintEndTime - currentTime;
}