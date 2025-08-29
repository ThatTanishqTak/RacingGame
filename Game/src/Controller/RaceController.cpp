#include "RaceController.h"

#include <algorithm>
#include "Core/EventBus.h"
#include "Core/RaceState.h"

RaceController::RaceController(EventBus& eventBus, float trackLength, int carCount, double startTime) : m_EventBus(eventBus), m_TrackLength(trackLength)
{
    m_Timing.resize(carCount);
    for (auto& it_Time : m_Timing)
    {
        it_Time.LapStart = startTime;
        it_Time.SectorStart = startTime;
    }
    m_Classification.resize(carCount);
}

void RaceController::Update(double currentTime, double deltaTime, const std::vector<float>& progress, const std::vector<bool>& inPitLane, const std::vector<float>& speeds)
{
    if (m_State == State::PreRace)
    {
        ChangeState(State::Formation);
    }

    if (m_State == State::Formation)
    {
        ChangeState(State::Green);
    }

    for (size_t i = 0; i < progress.size() && i < m_Timing.size(); ++i)
    {
        HandleProgress(static_cast<int>(i), progress[i], currentTime);

        auto& a_Time = m_Timing[i];
        bool l_InPit = (i < inPitLane.size()) ? inPitLane[i] : false;
        if (l_InPit)
        {
            if (!a_Time.InPitLane)
            {
                a_Time.InPitLane = true;
                a_Time.PitEntryTime = currentTime;
                a_Time.ServiceTime = 0.0;
            }

            if (i < speeds.size() && speeds[i] < 0.1f)
            {
                a_Time.ServiceTime += deltaTime;
            }
        }

        else if (a_Time.InPitLane)
        {
            a_Time.InPitLane = false;
            double l_Total = currentTime - a_Time.PitEntryTime;
            a_Time.PitExitDelta = l_Total - a_Time.ServiceTime;
            a_Time.StopsDone++;
            if (a_Time.StopsDone >= a_Time.MandatoryStops)
            {
                a_Time.PitStopCompleted = true;
            }
        }
    }

    m_Classification.clear();
    for (size_t i = 0; i < m_Timing.size(); ++i)
    {
        m_Classification.push_back({ static_cast<int>(i), m_Timing[i].Lap, m_Timing[i].LastProgress });
    }

    std::sort(m_Classification.begin(), m_Classification.end(), [](const ClassificationEntry& a, const ClassificationEntry& b)
        {
            if (a.Lap != b.Lap)
            {
                return a.Lap > b.Lap;
            }

            return a.Progress > b.Progress;
        });

    CheckBlueFlags();
}

void RaceController::SetPitStrategy(int carId, int mandatoryStops)
{
    if (carId >= 0 && carId < static_cast<int>(m_Timing.size()))
    {
        m_Timing[carId].MandatoryStops = std::max(0, mandatoryStops);
    }
}

void RaceController::ChangeState(State newState)
{
    if (m_State == newState)
    {
        return;
    }

    m_State = newState;
    switch (m_State)
    {
    case State::Green:
        m_EventBus.Publish(GreenFlag{});
        break;
    case State::Yellow:
        m_EventBus.Publish(YellowFlag{});
        break;
    default:
        break;
    }
}

void RaceController::HandleProgress(int carId, float newProgress, double currentTime)
{
    auto& a_Time = m_Timing[carId];
    float l_Previous = a_Time.LastProgress;
    float l_SectorLength = m_TrackLength / 3.0f;

    double l_ABSPrevious = a_Time.Lap * m_TrackLength + l_Previous;
    double l_ABSCurrent = a_Time.Lap * m_TrackLength + newProgress;
    if (newProgress < l_Previous)
    {
        l_ABSCurrent += m_TrackLength;
    }

    double l_NextBoundary = a_Time.Lap * m_TrackLength + (a_Time.Sector + 1) * l_SectorLength;
    while (l_ABSCurrent >= l_NextBoundary)
    {
        double l_SectorTime = currentTime - a_Time.SectorStart;
        a_Time.LastSectorTimes[a_Time.Sector] = l_SectorTime;
        a_Time.SectorStart = currentTime;
        a_Time.Sector++;
        if (a_Time.Sector == 3)
        {
            a_Time.Sector = 0;
            a_Time.Lap++;
            a_Time.LastLapTime = currentTime - a_Time.LapStart;
            a_Time.LapStart = currentTime;
        }
        l_NextBoundary = a_Time.Lap * m_TrackLength + (a_Time.Sector + 1) * l_SectorLength;
    }

    a_Time.LastProgress = newProgress;
}

void RaceController::CheckBlueFlags()
{
    if (m_Classification.empty())
    {
        return;
    }

    int l_Leader = m_Classification.front().CarID;
    float l_LeaderProgress = m_Timing[l_Leader].LastProgress;
    for (const auto& it_Entry : m_Classification)
    {
        if (it_Entry.CarID == l_Leader)
        {
            continue;
        }

        int l_LapDifference = m_Timing[l_Leader].Lap - m_Timing[it_Entry.CarID].Lap;
        float l_Distance = l_LeaderProgress - m_Timing[it_Entry.CarID].LastProgress;
        if (l_Distance < 0.0f)
        {
            l_Distance += m_TrackLength;
        }

        if (l_LapDifference > 0 && l_Distance < 50.0f)
        {
            m_EventBus.Publish(BlueFlag{ it_Entry.CarID });
        }
    }
}