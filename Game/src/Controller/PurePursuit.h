#pragma once

#include "Core/Track.h"
#include <glm/glm.hpp>

class PurePursuit
{
public:
    PurePursuit(const Track& track, float lookAhead);
    float ComputeSteering(const glm::vec3& position, float yaw, float progress) const;
    void SetLookAhead(float lookAhead);
    void SetSideOffset(float offset);

private:
    const Track& m_Track;
    float m_LookAhead;
    float m_SideOffset{ 0.0f };
};