#include "RaceController.h"

#include <algorithm>
#include "Core/EventBus.h"
#include "Core/RaceState.h"

RaceController::RaceController(float trackLength, int carCount, double startTime) : m_TrackLength(trackLength)
{
    m_Timing.resize(carCount);
    for (auto& it_Time : m_Timing)
    {
        it_Time.LapStart = startTime;
        it_Time.SectorStart = startTime;
    }
    m_Classification.resize(carCount);
}

void RaceController::Update(double currentTime, const std::vector<float>& progress)
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
        g_EventBus.Publish(GreenFlag{});
        break;
    case State::Yellow:
        g_EventBus.Publish(YellowFlag{});
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
            g_EventBus.Publish(BlueFlag{ it_Entry.CarID });
        }
    }
}