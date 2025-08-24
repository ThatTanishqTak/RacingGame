#include "StrategyPlanner.h"
#include "Team.h"
#include "Driver.h"

#include <cassert>
#include <chrono>
#include <random>
#include <memory>

int main()
{
    StrategyPlanner planner;
    Team team("Test");
    team.AddDriver(std::make_shared<Driver>("One", 30));
    team.AddDriver(std::make_shared<Driver>("Two", 28));

    std::mt19937 rngA(123);
    int planA = planner.PlanPitStops(team, 0, 10, rngA);
    std::mt19937 rngB(123);
    int planB = planner.PlanPitStops(team, 0, 10, rngB);
    assert(planA == planB);

    auto start = std::chrono::high_resolution_clock::now();
    int scPlan = planner.ReplanForSafetyCar(team, TrackFlag::SafetyCar, 2, 10);
    auto end = std::chrono::high_resolution_clock::now();
    auto perCar = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / team.GetDrivers().size();
    assert(perCar < 50);
    assert(scPlan == 3);

    int vscPlan = planner.ReplanForSafetyCar(team, TrackFlag::VirtualSafetyCar, 2, 10);
    assert(vscPlan == 4);

    double eta = StrategyPlanner::ComputeStintETA(90.0, 80.0);
    assert(eta == 10.0);

    return 0;
}