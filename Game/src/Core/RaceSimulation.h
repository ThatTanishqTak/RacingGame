#pragma once

#include "Renderer/StateStream.h"
#include "Vehicle.h"

#include <vector>

class RaceSimulation
{
public:
    explicit RaceSimulation(double startTime);

    void Update(double deltaTime);
    double GetTime() const { return m_SimulationTime; }
    const std::vector<Engine::CarState>& GetCars() const { return m_Cars; }

private:
    double m_SimulationTime;
    std::vector<Vehicle> m_Vehicles;
    std::vector<Engine::CarState> m_Cars;
};