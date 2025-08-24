#pragma once

#include "RaceEventManager.h"
#include "Team.h"

#include <random>

class StrategyPlanner
{
public:
    int PlanPitStops(const Team& team, int currentLap, int totalLaps, std::mt19937& rng);
    int ReplanForSafetyCar(const Team& team, TrackFlag flag, int currentLap, int totalLaps) const;
    void UpdateERSMode(const Team& team) const;
    static double ComputeStintETA(double stintEndTime, double currentTime);
};