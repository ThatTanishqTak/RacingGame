#pragma once

#include "Renderer/StateStream.h"
#include "Vehicle.h"
#include "Track.h"
#include "Controller/PurePursuit.h"
#include "Controller/RaceController.h"

#include <vector>

class RaceSimulation
{
public:
    RaceSimulation(const Track& track, double startTime, RaceController& controller);

    void Update(double deltaTime);
    double GetTime() const { return m_SimulationTime; }
    const std::vector<Engine::CarState>& GetCars() const { return m_Cars; }

private:
    void BuildSpeedTable();

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