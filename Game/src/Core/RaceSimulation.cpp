#include "RaceSimulation.h"

#include <algorithm>
#include <cmath>

namespace
{
    constexpr float kWheelBase = 2.5f;
    constexpr float kMaxLongitudinalAccel = 5.0f;
    constexpr float kMaxBrakeAccel = 10.0f;
    constexpr float kMaxLateralAccel = 10.0f;
}

RaceSimulation::RaceSimulation(double startTime) : m_SimulationTime(startTime)
{
    m_Vehicles.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 2.0f);
    m_Vehicles.emplace_back(glm::vec3(2.0f, 0.0f, 0.0f), 0.0f, 2.0f);

    m_Cars.resize(m_Vehicles.size());
    for (size_t i = 0; i < m_Vehicles.size(); ++i)
    {
        m_Cars[i].ID = static_cast<int>(i);
        m_Cars[i].Position = m_Vehicles[i].GetPosition();
        m_Cars[i].Yaw = m_Vehicles[i].GetYaw();
        m_Cars[i].Speed = m_Vehicles[i].GetSpeed();
    }
}

void RaceSimulation::Update(double deltaTime)
{
    m_SimulationTime += deltaTime;

    for (size_t i = 0; i < m_Vehicles.size(); ++i)
    {
        Vehicle& l_Vehicle = m_Vehicles[i];

        float l_Accel = l_Vehicle.GetThrottle() * kMaxLongitudinalAccel - l_Vehicle.GetBrake() * kMaxBrakeAccel;
        l_Accel = std::clamp(l_Accel, -kMaxBrakeAccel, kMaxLongitudinalAccel);
        l_Vehicle.SetSpeed(std::max(0.0f, l_Vehicle.GetSpeed() + l_Accel * static_cast<float>(deltaTime)));

        float l_YawRate = 0.0f;
        if (std::abs(l_Vehicle.GetSteering()) > 0.0f)
        {
            l_YawRate = l_Vehicle.GetSpeed() / kWheelBase * std::tan(l_Vehicle.GetSteering());
            float l_LatAccel = l_Vehicle.GetSpeed() * l_YawRate;
            if (std::abs(l_LatAccel) > kMaxLateralAccel)
            {
                l_YawRate = std::copysign(kMaxLateralAccel / std::max(0.1f, l_Vehicle.GetSpeed()), l_LatAccel);
            }
        }

        l_Vehicle.SetYaw(l_Vehicle.GetYaw() + l_YawRate * static_cast<float>(deltaTime));
        l_Vehicle.SetPosition(l_Vehicle.GetPosition() + glm::vec3(std::sin(l_Vehicle.GetYaw()), 0.0f, -std::cos(l_Vehicle.GetYaw())) * l_Vehicle.GetSpeed() * static_cast<float>(deltaTime));

        m_Cars[i].ID = static_cast<int>(i);
        m_Cars[i].Position = l_Vehicle.GetPosition();
        m_Cars[i].Yaw = l_Vehicle.GetYaw();
        m_Cars[i].Speed = l_Vehicle.GetSpeed();
    }
}