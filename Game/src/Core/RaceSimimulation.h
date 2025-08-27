#pragma once

#include <vector>
#include "Renderer/StateStream.h"

class RaceSimimulation
{
public:
    explicit RaceSimimulation(double startTime);

    void Update(double deltaTime);
    double GetTime() const { return m_SimulationTime; }
    const std::vector<Engine::CarState>& GetCars() const { return m_Cars; }

private:
    double m_SimulationTime;
    std::vector<Engine::CarState> m_Cars;
};