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

RaceSimulation::RaceSimulation(const Track& track, double startTime, RaceController& controller) : m_Track(track), m_SimulationTime(startTime), m_Controller(controller)
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
    const auto& l_Curvature = m_Track.GetCurvature();
    m_SpeedTable.resize(l_Curvature.size());
    for (size_t i = 0; i < l_Curvature.size(); ++i)
    {
        float l_Kappa = std::abs(l_Curvature[i]);
        if (l_Kappa < 1e-3f)
        {
            m_SpeedTable[i] = 50.0f;
        }

        else
        {
            m_SpeedTable[i] = std::sqrt(kMaxLateralAccel / l_Kappa);
        }
    }
}

void RaceSimulation::Update(double deltaTime)
{
    m_SimulationTime += deltaTime;

    const auto& a_Arc = m_Track.GetArcLengthTable();
    for (size_t i = 0; i < m_Vehicles.size(); ++i)
    {
        Vehicle& l_Vehicle = m_Vehicles[i];

        float l_Position = std::fmod(m_Progress[i], m_TrackLength);

        // Determine car ahead distance and relative speed
        float l_DistanceToAhead = m_TrackLength;
        float l_SpeedAhead = l_Vehicle.GetSpeed();
        int l_AheadIndex = -1;
        for (size_t j = 0; j < m_Vehicles.size(); ++j)
        {
            if (j == i)
            {
                continue;
            }

            float l_Distance = m_Progress[j] - m_Progress[i];
            if (l_Distance <= 0.0f)
            {
                l_Distance += m_TrackLength;
            }
            
            if (l_Distance < l_DistanceToAhead)
            {
                l_DistanceToAhead = l_Distance;
                l_SpeedAhead = m_Vehicles[j].GetSpeed();
                l_AheadIndex = static_cast<int>(j);
            }
        }
        float l_RelativeSpeed = l_Vehicle.GetSpeed() - l_SpeedAhead;
        m_Cars[i].DistanceToCarAhead = l_DistanceToAhead;
        m_Cars[i].RelativeSpeedToCarAhead = l_RelativeSpeed;

        // Adjust controller in traffic
        float l_LookAhead = l_DistanceToAhead < 20.0f ? 5.0f : 10.0f;
        m_Controllers[i].SetLookAhead(l_LookAhead);
        if (l_DistanceToAhead < 10.0f)
        {
            m_Controllers[i].SetSideOffset((i % 2 == 0) ? 1.5f : -1.5f);
        }

        else
        {
            m_Controllers[i].SetSideOffset(0.0f);
        }

        // Steering from controller
        float l_Steering = m_Controllers[i].ComputeSteering(l_Vehicle.GetPosition(), l_Vehicle.GetYaw(), l_Position);
        if (l_DistanceToAhead < 5.0f)
        {
            float l_Penalty = l_DistanceToAhead / 5.0f;
            l_Steering *= l_Penalty;
        }

        l_Vehicle.SetSteering(l_Steering);

        // Determine speed target from l_Curvature table with simple look-ahead
        auto it = std::upper_bound(a_Arc.begin(), a_Arc.end(), l_Position);
        size_t l_Index = std::distance(a_Arc.begin(), it);
        if (l_Index >= m_SpeedTable.size())
        {
            l_Index = m_SpeedTable.size() - 1;
        }

        float l_TargetSpeed = m_SpeedTable[l_Index];
        for (size_t j = 1; j <= 10 && j < m_SpeedTable.size(); ++j)
        {
            size_t l_IndexAhead = (l_Index + j) % m_SpeedTable.size();
            l_TargetSpeed = std::min(l_TargetSpeed, m_SpeedTable[l_IndexAhead]);
        }

        if (l_DistanceToAhead < 15.0f)
        {
            l_TargetSpeed *= 0.9f;
        }

        float l_SpeedError = l_TargetSpeed - l_Vehicle.GetSpeed();
        if (l_SpeedError > 0.0f)
        {
            l_Vehicle.SetThrottle(std::clamp(l_SpeedError / std::max(1.0f, l_TargetSpeed), 0.0f, 1.0f));
            l_Vehicle.SetBrake(0.0f);
        }

        else
        {
            l_Vehicle.SetThrottle(0.0f);
            l_Vehicle.SetBrake(std::clamp(-l_SpeedError / std::max(1.0f, l_TargetSpeed), 0.0f, 1.0f));
        }

        if (l_DistanceToAhead < 5.0f)
        {
            float l_Penalty = l_DistanceToAhead / 5.0f;
            l_Vehicle.SetThrottle(l_Vehicle.GetThrottle() * l_Penalty);
        }

        float l_Acceleration = l_Vehicle.GetThrottle() * kMaxLongitudinalAccel - l_Vehicle.GetBrake() * kMaxBrakeAccel;
        float l_MaxLateralAcceleration = kMaxLateralAccel;
        if (l_AheadIndex >= 0 && l_DistanceToAhead < 15.0f)
        {
            glm::vec2 l_Difference(m_Vehicles[l_AheadIndex].GetPosition().x - l_Vehicle.GetPosition().x, m_Vehicles[l_AheadIndex].GetPosition().z - l_Vehicle.GetPosition().z);
            glm::vec2 l_Forward(std::sin(l_Vehicle.GetYaw()), -std::cos(l_Vehicle.GetYaw()));
            glm::vec2 l_Right(std::cos(l_Vehicle.GetYaw()), std::sin(l_Vehicle.GetYaw()));
            float l_LateralSep = glm::dot(l_Difference, l_Right);
            if (std::abs(l_LateralSep) < 1.0f)
            {
                l_Acceleration *= 1.1f;
            }

            l_MaxLateralAcceleration *= 0.8f;
        }

        l_Acceleration = std::clamp(l_Acceleration, -kMaxBrakeAccel, kMaxLongitudinalAccel);
        l_Vehicle.SetSpeed(std::max(0.0f, l_Vehicle.GetSpeed() + l_Acceleration * static_cast<float>(deltaTime)));

        float l_YawRate = 0.0f;
        if (std::abs(l_Vehicle.GetSteering()) > 0.0f)
        {
            l_YawRate = l_Vehicle.GetSpeed() / kWheelBase * std::tan(l_Vehicle.GetSteering());
            float l_LateralAcceleration = l_Vehicle.GetSpeed() * l_YawRate;
            if (std::abs(l_LateralAcceleration) > l_MaxLateralAcceleration)
            {
                l_YawRate = std::copysign(l_MaxLateralAcceleration / std::max(0.1f, l_Vehicle.GetSpeed()), l_LateralAcceleration);
            }
        }

        l_Vehicle.SetYaw(l_Vehicle.GetYaw() + l_YawRate * static_cast<float>(deltaTime));
        l_Vehicle.SetPosition(l_Vehicle.GetPosition() + glm::vec3(std::sin(l_Vehicle.GetYaw()), 0.0f, -std::cos(l_Vehicle.GetYaw())) 
            * l_Vehicle.GetSpeed() * static_cast<float>(deltaTime));

        float l_NewProgress = l_Position + l_Vehicle.GetSpeed() * static_cast<float>(deltaTime);
        if (l_NewProgress >= m_TrackLength)
        {
            l_NewProgress -= m_TrackLength;
        }

        m_Progress[i] = l_NewProgress;

        m_Cars[i].ID = static_cast<int>(i);
        m_Cars[i].Position = l_Vehicle.GetPosition();
        m_Cars[i].Yaw = l_Vehicle.GetYaw();
        m_Cars[i].Speed = l_Vehicle.GetSpeed();
    }

    m_Controller.Update(m_SimulationTime, m_Progress);
}