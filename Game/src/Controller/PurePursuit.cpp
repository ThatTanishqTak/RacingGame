#include "PurePursuit.h"

#include <algorithm>
#include <cmath>

PurePursuit::PurePursuit(const Track& track, float lookAhead) : m_Track(track), m_LookAhead(lookAhead)
{

}

float PurePursuit::ComputeSteering(const glm::vec3& position, float yaw, float progress) const
{
    glm::vec2 pos2D(position.x, position.z);
    glm::vec2 target = m_Track.PositionAt(progress + m_LookAhead);
    glm::vec2 toTarget = target - pos2D;
    float distance = glm::length(toTarget);
    if (distance < 1e-3f)
    {
        return 0.0f;
    }

    glm::vec2 forward(std::sin(yaw), -std::cos(yaw));
    glm::vec2 right(std::cos(yaw), std::sin(yaw));
    float x = glm::dot(toTarget, right);

    constexpr float kWheelBase = 2.5f;
    
    return std::atan2(2.0f * kWheelBase * x, distance * distance);
}