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

RaceSimulation::RaceSimulation(const Track& track, double startTime)
    : m_Track(track), m_SimulationTime(startTime)
{
    m_Vehicles.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 2.0f);
    m_Vehicles.emplace_back(glm::vec3(2.0f, 0.0f, 0.0f), 0.0f, 2.0f);

    m_Cars.resize(m_Vehicles.size());
    m_Controllers.reserve(m_Vehicles.size());
    m_Progress.resize(m_Vehicles.size(), 0.0f);
    for (size_t i = 0; i < m_Vehicles.size(); ++i)
    {
        m_Cars[i].ID = static_cast<int>(i);
        m_Cars[i].Position = m_Vehicles[i].GetPosition();
        m_Cars[i].Yaw = m_Vehicles[i].GetYaw();
        m_Cars[i].Speed = m_Vehicles[i].GetSpeed();
        m_Controllers.emplace_back(track, 10.0f);
    }

    m_TrackLength = track.GetLength();
    BuildSpeedTable();
}

void RaceSimulation::BuildSpeedTable()
{
    const auto& curvature = m_Track.GetCurvature();
    m_SpeedTable.resize(curvature.size());
    for (size_t i = 0; i < curvature.size(); ++i)
    {
        float kappa = std::abs(curvature[i]);
        if (kappa < 1e-3f)
        {
            m_SpeedTable[i] = 50.0f;
        }
        else
        {
            m_SpeedTable[i] = std::sqrt(kMaxLateralAccel / kappa);
        }
    }
}

void RaceSimulation::Update(double deltaTime)
{
    m_SimulationTime += deltaTime;

    const auto& arc = m_Track.GetArcLengthTable();
    for (size_t i = 0; i < m_Vehicles.size(); ++i)
    {
        Vehicle& l_Vehicle = m_Vehicles[i];

        float position = std::fmod(m_Progress[i], m_TrackLength);

        // Steering from controller
        float steering = m_Controllers[i].ComputeSteering(l_Vehicle.GetPosition(), l_Vehicle.GetYaw(), position);
        l_Vehicle.SetSteering(steering);

        // Determine speed target from curvature table with simple look-ahead
        auto it = std::upper_bound(arc.begin(), arc.end(), position);
        size_t idx = std::distance(arc.begin(), it);
        if (idx >= m_SpeedTable.size())
        {
            idx = m_SpeedTable.size() - 1;
        }

        float targetSpeed = m_SpeedTable[idx];
        for (size_t j = 1; j <= 10 && j < m_SpeedTable.size(); ++j)
        {
            size_t idxAhead = (idx + j) % m_SpeedTable.size();
            targetSpeed = std::min(targetSpeed, m_SpeedTable[idxAhead]);
        }

        float speedError = targetSpeed - l_Vehicle.GetSpeed();
        if (speedError > 0.0f)
        {
            l_Vehicle.SetThrottle(std::clamp(speedError / std::max(1.0f, targetSpeed), 0.0f, 1.0f));
            l_Vehicle.SetBrake(0.0f);
        }
        else
        {
            l_Vehicle.SetThrottle(0.0f);
            l_Vehicle.SetBrake(std::clamp(-speedError / std::max(1.0f, targetSpeed), 0.0f, 1.0f));
        }

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
        l_Vehicle.SetPosition(l_Vehicle.GetPosition() + glm::vec3(std::sin(l_Vehicle.GetYaw()), 0.0f, -std::cos(l_Vehicle.GetYaw())) 
            * l_Vehicle.GetSpeed() * static_cast<float>(deltaTime));

        m_Progress[i] = position + l_Vehicle.GetSpeed() * static_cast<float>(deltaTime);

        m_Cars[i].ID = static_cast<int>(i);
        m_Cars[i].Position = l_Vehicle.GetPosition();
        m_Cars[i].Yaw = l_Vehicle.GetYaw();
        m_Cars[i].Speed = l_Vehicle.GetSpeed();
    }
}