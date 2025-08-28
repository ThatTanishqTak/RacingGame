#include "Replay.h"

bool ReplayLogger::Open(const std::string& filePath, const RaceConfiguration& config)
{
    m_Stream.open(filePath, std::ios::binary);
    if (!m_Stream)
    {
        return false;
    }

    return SaveRaceConfig(m_Stream, config);
}

void ReplayLogger::WriteSnapshot(const Engine::Snapshot& snapshot)
{
    if (!m_Stream)
    {
        return;
    }

    m_Stream.write(reinterpret_cast<const char*>(&snapshot.Time), sizeof(snapshot.Time));
    uint32_t l_Count = static_cast<uint32_t>(snapshot.Cars.size());
    m_Stream.write(reinterpret_cast<const char*>(&l_Count), sizeof(l_Count));
    for (const auto& it_Car : snapshot.Cars)
    {
        m_Stream.write(reinterpret_cast<const char*>(&it_Car.ID), sizeof(it_Car.ID));
        m_Stream.write(reinterpret_cast<const char*>(&it_Car.Position), sizeof(it_Car.Position));
        m_Stream.write(reinterpret_cast<const char*>(&it_Car.Yaw), sizeof(it_Car.Yaw));
        m_Stream.write(reinterpret_cast<const char*>(&it_Car.Speed), sizeof(it_Car.Speed));
        m_Stream.write(reinterpret_cast<const char*>(&it_Car.DistanceToCarAhead), sizeof(it_Car.DistanceToCarAhead));
        m_Stream.write(reinterpret_cast<const char*>(&it_Car.RelativeSpeedToCarAhead), sizeof(it_Car.RelativeSpeedToCarAhead));
        m_Stream.write(reinterpret_cast<const char*>(&it_Car.InPitLane), sizeof(it_Car.InPitLane));
    }
}

void ReplayLogger::Close()
{
    if (m_Stream)
    {
        m_Stream.close();
    }
}

bool LoadReplay(const std::string& filePath, double baseTime)
{
    std::ifstream l_Stream(filePath, std::ios::binary);
    if (!l_Stream)
    {
        return false;
    }

    if (!LoadRaceConfig(l_Stream, g_RaceConfig))
    {
        return false;
    }
    ResetRaceSeed();

    while (l_Stream)
    {
        Engine::Snapshot l_Snapshot;
        l_Stream.read(reinterpret_cast<char*>(&l_Snapshot.Time), sizeof(l_Snapshot.Time));
        if (!l_Stream)
        {
            break;
        }

        uint32_t l_Count = 0;
        l_Stream.read(reinterpret_cast<char*>(&l_Count), sizeof(l_Count));
        l_Snapshot.Cars.resize(l_Count);
        for (auto& it_Car : l_Snapshot.Cars)
        {
            l_Stream.read(reinterpret_cast<char*>(&it_Car.ID), sizeof(it_Car.ID));
            l_Stream.read(reinterpret_cast<char*>(&it_Car.Position), sizeof(it_Car.Position));
            l_Stream.read(reinterpret_cast<char*>(&it_Car.Yaw), sizeof(it_Car.Yaw));
            l_Stream.read(reinterpret_cast<char*>(&it_Car.Speed), sizeof(it_Car.Speed));
            l_Stream.read(reinterpret_cast<char*>(&it_Car.DistanceToCarAhead), sizeof(it_Car.DistanceToCarAhead));
            l_Stream.read(reinterpret_cast<char*>(&it_Car.RelativeSpeedToCarAhead), sizeof(it_Car.RelativeSpeedToCarAhead));
            l_Stream.read(reinterpret_cast<char*>(&it_Car.InPitLane), sizeof(it_Car.InPitLane));
        }

        l_Snapshot.Time += baseTime;
        Engine::g_StateBuffer.SubmitSnapshot(l_Snapshot);
    }

    return true;
}