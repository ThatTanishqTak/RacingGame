#include "PurePursuit.h"

#include <algorithm>
#include <cmath>

PurePursuit::PurePursuit(const Track& track, float lookAhead) : m_Track(track), m_LookAhead(lookAhead)
{

}

void PurePursuit::SetLookAhead(float lookAhead)
{
    m_LookAhead = lookAhead;
}

void PurePursuit::SetSideOffset(float offset)
{
    m_SideOffset = offset;
}

float PurePursuit::ComputeSteering(const glm::vec3& position, float yaw, float progress) const
{
    glm::vec2 l_Position2D(position.x, position.z);
    glm::vec2 l_Target = m_Track.PositionAt(progress + m_LookAhead);
    if (std::abs(m_SideOffset) > 1e-3f)
    {
        glm::vec2 l_Ahead = m_Track.PositionAt(progress + m_LookAhead + 1.0f);
        glm::vec2 l_Tangent = glm::normalize(l_Ahead - l_Target);
        glm::vec2 l_Normal(l_Tangent.y, -l_Tangent.x);
        l_Target += l_Normal * m_SideOffset;
    }

    glm::vec2 l_ToTarget = l_Target - l_Position2D;
    float l_Distance = glm::length(l_ToTarget);
    if (l_Distance < 1e-3f)
    {
        return 0.0f;
    }

    glm::vec2 l_Forward(std::sin(yaw), -std::cos(yaw));
    glm::vec2 l_Right(std::cos(yaw), std::sin(yaw));
    float l_X = glm::dot(l_ToTarget, l_Right);

    constexpr float l_KWheelBase = 2.5f;
    
    return std::atan2(2.0f * l_KWheelBase * l_X, l_Distance * l_Distance);
}