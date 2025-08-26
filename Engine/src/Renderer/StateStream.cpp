#include "StateStream.h"

#include <algorithm>

namespace Engine
{
    StateBuffer GlobalStateBuffer;

    void StateBuffer::SubmitSnapshot(const Snapshot& snapshot)
    {
        auto it = std::lower_bound(Snapshots.begin(), Snapshots.end(), snapshot.Time,
            [](const Snapshot& s, double time) { return s.Time < time; });
        Snapshots.insert(it, snapshot);
    }

    std::vector<CarState> StateBuffer::Interpolate(double tRender) const
    {
        if (Snapshots.empty())
            return {};

        if (Snapshots.size() == 1)
            return Snapshots.front().Cars;

        if (tRender <= Snapshots.front().Time)
        {
            const Snapshot& s0 = Snapshots.front();
            const Snapshot& s1 = Snapshots[1];
            double dt = s1.Time - s0.Time;
            std::vector<CarState> result = s0.Cars;
            if (dt > 0.0)
            {
                double alpha = (tRender - s0.Time) / dt;
                for (std::size_t i = 0; i < result.size() && i < s1.Cars.size(); ++i)
                {
                    result[i].Position += (s1.Cars[i].Position - result[i].Position) * static_cast<float>(alpha);
                }
            }
            return result;
        }

        if (tRender >= Snapshots.back().Time)
        {
            const Snapshot& s0 = Snapshots[Snapshots.size() - 2];
            const Snapshot& s1 = Snapshots.back();
            double dt = s1.Time - s0.Time;
            std::vector<CarState> result = s1.Cars;
            if (dt > 0.0)
            {
                double alpha = (tRender - s1.Time) / dt;
                for (std::size_t i = 0; i < result.size() && i < s0.Cars.size(); ++i)
                {
                    result[i].Position += (s1.Cars[i].Position - s0.Cars[i].Position) * static_cast<float>(alpha);
                }
            }
            return result;
        }

        auto it = std::upper_bound(Snapshots.begin(), Snapshots.end(), tRender, [](double time, const Snapshot& s) { return time < s.Time; });
        const Snapshot& s0 = *(it - 1);
        const Snapshot& s1 = *it;
        double dt = s1.Time - s0.Time;
        double alpha = (tRender - s0.Time) / dt;

        std::vector<CarState> result = s0.Cars;
        for (std::size_t i = 0; i < result.size() && i < s1.Cars.size(); ++i)
        {
            result[i].Position += (s1.Cars[i].Position - result[i].Position) * static_cast<float>(alpha);
        }

        return result;
    }
}