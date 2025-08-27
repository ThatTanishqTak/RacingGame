#include "RaceSimimulation.h"

#include <cmath>

RaceSimimulation::RaceSimimulation(double startTime) : m_SimulationTime(startTime)
{
    m_Cars.push_back({ 0, { 0.0f, 0.0f, 0.0f }, 0.0f, 2.0f });
    m_Cars.push_back({ 1, { 2.0f, 0.0f, 0.0f }, 0.0f, 2.0f });
}

void RaceSimimulation::Update(double deltaTime)
{
    m_SimulationTime += deltaTime;
    for (auto& it_Car : m_Cars)
    {
        float l_Distance = it_Car.Speed * static_cast<float>(deltaTime);

        it_Car.Position.x += std::sin(it_Car.Yaw) * l_Distance;
        it_Car.Position.z -= std::cos(it_Car.Yaw) * l_Distance;
    }
}