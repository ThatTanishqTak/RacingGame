#include "RaceSimulation.h"

#include <algorithm>
#include <cmath>

namespace
{
    constexpr float kWheelBase = 2.5f;
    constexpr float kMaxLongitudinalAccel = 5.0f;
    constexpr float kMaxBrakeAccel = 10.0f;
    constexpr float kMaxLateralAccel = 10.0f;
    constexpr float kPitSpeedLimit = 20.0f;
}

RaceSimulation::RaceSimulation(const Track& track, double startTime, RaceController& controller) : m_Track(track), m_SimulationTime(startTime), m_Controller(controller)
{
    m_Vehicles.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 2.0f);
    m_Vehicles.emplace_back(glm::vec3(2.0f, 0.0f, 0.0f), 0.0f, 2.0f);

    m_Cars.resize(m_Vehicles.size());
    m_Controllers.reserve(m_Vehicles.size());
    m_Progress.resize(m_Vehicles.size(), 0.0f);
    m_InPitLane.resize(m_Vehicles.size(), false);
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

// Determine car ahead distance and relative speed
RaceSimulation::AheadInfo RaceSimulation::DetermineCarAhead(size_t a_Index, float a_Position) const
{
    AheadInfo l_Info;
    l_Info.m_Distance = m_TrackLength;
    const Vehicle& l_Vehicle = m_Vehicles[a_Index];
    float l_SpeedAhead = l_Vehicle.GetSpeed();

    for (size_t it_J = 0; it_J < m_Vehicles.size(); ++it_J)
    {
        if (it_J == a_Index)
        {
            continue;
        }

        float l_Distance = m_Progress[it_J] - m_Progress[a_Index];
        if (l_Distance <= 0.0f)
        {
            l_Distance += m_TrackLength;
        }

        if (l_Distance < l_Info.m_Distance)
        {
            l_Info.m_Distance = l_Distance;
            l_SpeedAhead = m_Vehicles[it_J].GetSpeed();
            l_Info.m_Index = static_cast<int>(it_J);
            l_Info.m_Vehicle = &m_Vehicles[it_J];
        }
    }

    l_Info.m_RelativeSpeed = l_Vehicle.GetSpeed() - l_SpeedAhead;
    return l_Info;
}

// Compute steering based on controller and traffic conditions
float RaceSimulation::ComputeSteering(size_t a_Index, Vehicle& a_Vehicle, float a_Position, const AheadInfo& a_Info)
{
    float l_LookAhead = a_Info.m_Distance < 20.0f ? 5.0f : 10.0f;
    m_Controllers[a_Index].SetLookAhead(l_LookAhead);
    if (a_Info.m_Distance < 10.0f)
    {
        m_Controllers[a_Index].SetSideOffset((a_Index % 2 == 0) ? 1.5f : -1.5f);
    }
    else
    {
        m_Controllers[a_Index].SetSideOffset(0.0f);
    }

    float l_Steering = m_Controllers[a_Index].ComputeSteering(a_Vehicle.GetPosition(), a_Vehicle.GetYaw(), a_Position);
    if (a_Info.m_Distance < 5.0f)
    {
        float l_Penalty = a_Info.m_Distance / 5.0f;
        l_Steering *= l_Penalty;
    }

    a_Vehicle.SetSteering(l_Steering);
    return l_Steering;
}

// Compute target speed and apply throttle and brake
void RaceSimulation::ApplyThrottleAndBrake(Vehicle& a_Vehicle, float a_Position, float a_DistanceToAhead, const std::vector<float>& a_Arc)
{
    auto it = std::upper_bound(a_Arc.begin(), a_Arc.end(), a_Position);
    size_t l_Index = std::distance(a_Arc.begin(), it);
    if (l_Index >= m_SpeedTable.size())
    {
        l_Index = m_SpeedTable.size() - 1;
    }

    float l_TargetSpeed = m_SpeedTable[l_Index];
    for (size_t it_J = 1; it_J <= 10 && it_J < m_SpeedTable.size(); ++it_J)
    {
        size_t l_IndexAhead = (l_Index + it_J) % m_SpeedTable.size();
        l_TargetSpeed = std::min(l_TargetSpeed, m_SpeedTable[l_IndexAhead]);
    }

    if (a_DistanceToAhead < 15.0f)
    {
        l_TargetSpeed *= 0.9f;
    }

    float l_SpeedError = l_TargetSpeed - a_Vehicle.GetSpeed();
    if (l_SpeedError > 0.0f)
    {
        a_Vehicle.SetThrottle(std::clamp(l_SpeedError / std::max(1.0f, l_TargetSpeed), 0.0f, 1.0f));
        a_Vehicle.SetBrake(0.0f);
    }
    else
    {
        a_Vehicle.SetThrottle(0.0f);
        a_Vehicle.SetBrake(std::clamp(-l_SpeedError / std::max(1.0f, l_TargetSpeed), 0.0f, 1.0f));
    }

    if (a_DistanceToAhead < 5.0f)
    {
        float l_Penalty = a_DistanceToAhead / 5.0f;
        a_Vehicle.SetThrottle(a_Vehicle.GetThrottle() * l_Penalty);
    }
}

// Integrate vehicle motion using current controls
float RaceSimulation::IntegrateMotion(Vehicle& a_Vehicle, const AheadInfo& a_Info, float a_Position, float a_DeltaTime)
{
    float l_Acceleration = a_Vehicle.GetThrottle() * kMaxLongitudinalAccel - a_Vehicle.GetBrake() * kMaxBrakeAccel;
    float l_MaxLateralAcceleration = kMaxLateralAccel;
    if (a_Info.m_Index >= 0 && a_Info.m_Distance < 15.0f)
    {
        glm::vec2 l_Difference(a_Info.m_Vehicle->GetPosition().x - a_Vehicle.GetPosition().x, a_Info.m_Vehicle->GetPosition().z - a_Vehicle.GetPosition().z);
        glm::vec2 l_Forward(std::sin(a_Vehicle.GetYaw()), -std::cos(a_Vehicle.GetYaw()));
        glm::vec2 l_Right(std::cos(a_Vehicle.GetYaw()), std::sin(a_Vehicle.GetYaw()));
        float l_LateralSep = glm::dot(l_Difference, l_Right);
        if (std::abs(l_LateralSep) < 1.0f)
        {
            l_Acceleration *= 1.1f;
        }

        l_MaxLateralAcceleration *= 0.8f;
    }

    l_Acceleration = std::clamp(l_Acceleration, -kMaxBrakeAccel, kMaxLongitudinalAccel);
    a_Vehicle.SetSpeed(std::max(0.0f, a_Vehicle.GetSpeed() + l_Acceleration * a_DeltaTime));
    if (m_Track.IsInPitLane(a_Position))
    {
        a_Vehicle.SetSpeed(std::min(a_Vehicle.GetSpeed(), kPitSpeedLimit));
    }

    float l_YawRate = 0.0f;
    if (std::abs(a_Vehicle.GetSteering()) > 0.0f)
    {
        l_YawRate = a_Vehicle.GetSpeed() / kWheelBase * std::tan(a_Vehicle.GetSteering());
        float l_LateralAcceleration = a_Vehicle.GetSpeed() * l_YawRate;
        if (std::abs(l_LateralAcceleration) > l_MaxLateralAcceleration)
        {
            l_YawRate = std::copysign(l_MaxLateralAcceleration / std::max(0.1f, a_Vehicle.GetSpeed()), l_LateralAcceleration);
        }
    }

    a_Vehicle.SetYaw(a_Vehicle.GetYaw() + l_YawRate * a_DeltaTime);
    a_Vehicle.SetPosition(a_Vehicle.GetPosition() + glm::vec3(std::sin(a_Vehicle.GetYaw()), 0.0f, -std::cos(a_Vehicle.GetYaw())) * a_Vehicle.GetSpeed() * a_DeltaTime);

    float l_NewProgress = a_Position + a_Vehicle.GetSpeed() * a_DeltaTime;
    if (l_NewProgress >= m_TrackLength)
    {
        l_NewProgress -= m_TrackLength;
    }
    return l_NewProgress;
}

void RaceSimulation::Update(double deltaTime)
{
    m_SimulationTime += deltaTime;

    const auto& a_Arc = m_Track.GetArcLengthTable();
    std::vector<float> l_Speeds(m_Vehicles.size(), 0.0f);
    for (size_t it_I = 0; it_I < m_Vehicles.size(); ++it_I)
    {
        Vehicle& l_Vehicle = m_Vehicles[it_I];
        float l_Position = std::fmod(m_Progress[it_I], m_TrackLength);

        AheadInfo l_Info = DetermineCarAhead(it_I, l_Position);
        m_Cars[it_I].DistanceToCarAhead = l_Info.m_Distance;
        m_Cars[it_I].RelativeSpeedToCarAhead = l_Info.m_RelativeSpeed;

        ComputeSteering(it_I, l_Vehicle, l_Position, l_Info);
        ApplyThrottleAndBrake(l_Vehicle, l_Position, l_Info.m_Distance, a_Arc);

        float l_NewProgress = IntegrateMotion(l_Vehicle, l_Info, l_Position, static_cast<float>(deltaTime));

        m_Progress[it_I] = l_NewProgress;
        m_InPitLane[it_I] = m_Track.IsInPitLane(l_NewProgress);

        m_Cars[it_I].ID = static_cast<int>(it_I);
        m_Cars[it_I].Position = l_Vehicle.GetPosition();
        m_Cars[it_I].Yaw = l_Vehicle.GetYaw();
        m_Cars[it_I].Speed = l_Vehicle.GetSpeed();

        l_Speeds[it_I] = l_Vehicle.GetSpeed();
    }

    m_Controller.Update(m_SimulationTime, deltaTime, m_Progress, m_InPitLane, l_Speeds);
}