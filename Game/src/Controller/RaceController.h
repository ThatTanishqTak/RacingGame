#pragma once

#include <array>
#include <vector>

#include "Renderer/StateStream.h"

class RaceController
{
public:
    enum class State
    {
        PreRace,
        Formation,
        Green,
        Yellow,
        Finish
    };

    struct ClassificationEntry
    {
        int CarID;
        int Lap;
        float Progress;
    };

public:
    RaceController(float trackLength, int carCount, double startTime = 0.0);

    void Update(double currentTime, const std::vector<float>& progress);

    State GetState() const { return m_State; }
    const std::vector<ClassificationEntry>& GetClassification() const { return m_Classification; }

private:
    struct DriverTiming
    {
        int Lap{ 0 };
        int Sector{ 0 };
        double LapStart{ 0.0 };
        double SectorStart{ 0.0 };
        std::array<double, 3> LastSectorTimes{ 0.0, 0.0, 0.0 };
        double LastLapTime{ 0.0 };
        float LastProgress{ 0.0f };
    };

private:
    void ChangeState(State newState);
    void HandleProgress(int carId, float newProgress, double currentTime);
    void CheckBlueFlags();

private:
    State m_State{ State::PreRace };
    float m_TrackLength;
    std::vector<DriverTiming> m_Timing;
    std::vector<ClassificationEntry> m_Classification;
};