#pragma once

#include <glm/glm.hpp>

class Vehicle
{
public:
    Vehicle(glm::vec3 position = glm::vec3(0.0f), float yaw = 0.0f, float speed = 0.0f);

    const glm::vec3& GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec3& position) { m_Position = position; }

    float GetYaw() const { return m_Yaw; }
    void SetYaw(float yaw) { m_Yaw = yaw; }

    float GetSpeed() const { return m_Speed; }
    void SetSpeed(float speed) { m_Speed = speed; }

    float GetThrottle() const { return m_Throttle; }
    void SetThrottle(float throttle) { m_Throttle = throttle; }

    float GetBrake() const { return m_Brake; }
    void SetBrake(float brake) { m_Brake = brake; }

    float GetSteering() const { return m_Steering; }
    void SetSteering(float steering) { m_Steering = steering; }

private:
    glm::vec3 m_Position;
    float m_Yaw;
    float m_Speed;
    float m_Throttle;
    float m_Brake;
    float m_Steering;
};