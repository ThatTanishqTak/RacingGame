#pragma once

#include <array>
#include <vector>

#include "Renderer/StateStream.h"

class EventBus;

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
    RaceController(EventBus& eventBus, float trackLength, int carCount, double startTime = 0.0);

    void Update(double currentTime, double deltaTime, const std::vector<float>& progress, const std::vector<bool>& inPitLane, const std::vector<float>& speeds);
    void SetPitStrategy(int carId, int mandatoryStops);

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
        bool InPitLane{ false };
        int StopsDone{ 0 };
        int MandatoryStops{ 1 };
        bool PitStopCompleted{ false };
        double PitEntryTime{ 0.0 };
        double ServiceTime{ 0.0 };
        double PitExitDelta{ 0.0 };
    };

private:
    void ChangeState(State newState);
    void HandleProgress(int carId, float newProgress, double currentTime);
    void CheckBlueFlags();

private:
    EventBus& m_EventBus;
    State m_State{ State::PreRace };
    float m_TrackLength;
    std::vector<DriverTiming> m_Timing;
    std::vector<ClassificationEntry> m_Classification;
};