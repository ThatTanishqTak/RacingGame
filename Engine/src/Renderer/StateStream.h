#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace Engine
{
    struct CarState
    {
        int ID;
        glm::vec3 Position;
    };

    struct Snapshot
    {
        double Time;
        std::vector<CarState> Cars;
    };

    class StateBuffer
    {
    private:
        std::vector<Snapshot> m_Snapshots;

    public:
        void SubmitSnapshot(const Snapshot& snapshot);
        std::vector<CarState> Interpolate(double tRender) const;
    };

    extern StateBuffer g_StateBuffer;
}