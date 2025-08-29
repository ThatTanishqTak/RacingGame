#pragma once

#include "Renderer/StateStream.h"
#include "Vehicle.h"
#include "Track.h"
#include "Controller/PurePursuit.h"
#include "Controller/RaceController.h"

class RaceSimulationTestAccessor;

class RaceSimulation
{
public:
    RaceSimulation(const Track& track, double startTime, RaceController& controller);

    void Update(double deltaTime);
    double GetTime() const { return m_SimulationTime; }
    const std::vector<Engine::CarState>& GetCars() const { return m_Cars; }

private:
    void BuildSpeedTable();

    struct AheadInfo
    {
        float m_Distance{ 0.0f };        // Distance to car ahead along the track
        float m_RelativeSpeed{ 0.0f };   // Difference in speed to the car ahead
        int m_Index{ -1 };               // Index of the car ahead
        const Vehicle* m_Vehicle{ nullptr }; // Pointer to car ahead for position checks
    };

    AheadInfo DetermineCarAhead(size_t a_Index, float a_Position) const;                                       // Locate nearest car ahead
    float ComputeSteering(size_t a_Index, Vehicle& a_Vehicle, float a_Position, const AheadInfo& a_Info);        // Steering control
    void ApplyThrottleAndBrake(Vehicle& a_Vehicle, float a_Position, float a_DistanceToAhead, const std::vector<float>& a_Arc); // Throttle/brake logic
    float IntegrateMotion(Vehicle& a_Vehicle, const AheadInfo& a_Info, float a_Position, float a_DeltaTime);     // Integrate vehicle motion

    friend class RaceSimulationTestAccessor;

private:
    const Track& m_Track;
    double m_SimulationTime;
    RaceController& m_Controller;
    std::vector<Vehicle> m_Vehicles;
    std::vector<PurePursuit> m_Controllers;
    std::vector<Engine::CarState> m_Cars;
    std::vector<float> m_Progress;
    std::vector<bool> m_InPitLane;
    std::vector<float> m_SpeedTable;
    float m_TrackLength{ 0.0f };
};