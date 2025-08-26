#include "StateStream.h"

#include <algorithm>

namespace Engine
{
    StateBuffer g_StateBuffer;

    void StateBuffer::SubmitSnapshot(const Snapshot& snapshot)
    {
        auto it = std::lower_bound(m_Snapshots.begin(), m_Snapshots.end(), snapshot.Time, [](const Snapshot& s, double time) { return s.Time < time; });
        m_Snapshots.insert(it, snapshot);
    }

    std::vector<CarState> StateBuffer::Interpolate(double tRender) const
    {
        if (m_Snapshots.empty())
        {
            return {};
        }

        if (m_Snapshots.size() == 1)
        {
            return m_Snapshots.front().Cars;
        }

        if (tRender <= m_Snapshots.front().Time)
        {
            const Snapshot& l_Snapshot0 = m_Snapshots.front();
            const Snapshot& l_Snapshot1 = m_Snapshots[1];
            double l_DeltaTime = l_Snapshot1.Time - l_Snapshot0.Time;
            std::vector<CarState> l_Result = l_Snapshot0.Cars;
            if (l_DeltaTime > 0.0)
            {
                double l_Alpha = (tRender - l_Snapshot0.Time) / l_DeltaTime;
                for (std::size_t i = 0; i < l_Result.size() && i < l_Snapshot1.Cars.size(); ++i)
                {
                    l_Result[i].Position += (l_Snapshot1.Cars[i].Position - l_Result[i].Position) * static_cast<float>(l_Alpha);
                }
            }

            return l_Result;
        }

        if (tRender >= m_Snapshots.back().Time)
        {
            const Snapshot& l_Snapshot0 = m_Snapshots[m_Snapshots.size() - 2];
            const Snapshot& l_Snapshot1 = m_Snapshots.back();
            double l_DeltaTime = l_Snapshot1.Time - l_Snapshot0.Time;
            std::vector<CarState> l_Result = l_Snapshot1.Cars;
            if (l_DeltaTime > 0.0)
            {
                double l_Alpha = (tRender - l_Snapshot1.Time) / l_DeltaTime;
                for (std::size_t i = 0; i < l_Result.size() && i < l_Snapshot0.Cars.size(); ++i)
                {
                    l_Result[i].Position += (l_Snapshot1.Cars[i].Position - l_Snapshot0.Cars[i].Position) * static_cast<float>(l_Alpha);
                }
            }

            return l_Result;
        }

        auto it = std::upper_bound(m_Snapshots.begin(), m_Snapshots.end(), tRender, [](double time, const Snapshot& s) { return time < s.Time; });
        const Snapshot& l_Snapshot0 = *(it - 1);
        const Snapshot& l_Snapshot1 = *it;
        double l_DeltaTime = l_Snapshot1.Time - l_Snapshot0.Time;
        double l_Alpha = (tRender - l_Snapshot0.Time) / l_DeltaTime;

        std::vector<CarState> l_Result = l_Snapshot0.Cars;
        for (std::size_t i = 0; i < l_Result.size() && i < l_Snapshot1.Cars.size(); ++i)
        {
            l_Result[i].Position += (l_Snapshot1.Cars[i].Position - l_Result[i].Position) * static_cast<float>(l_Alpha);
        }

        return l_Result;
    }
}