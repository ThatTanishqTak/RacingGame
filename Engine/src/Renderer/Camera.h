#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    class Camera
    {
    public:
        bool Initialize(float fov, float aspectRatio, float nearClip, float farClip);
        void Shutdown();

        const glm::vec3& GetPosition() const { return m_Position; }
        void SetPosition(const glm::vec3& position) { m_Position = position; }

        float GetYaw() const { return m_Yaw; }
        void SetYaw(float yaw) { m_Yaw = yaw; }

        float GetPitch() const { return m_Pitch; }
        void SetPitch(float pitch) { m_Pitch = pitch; }

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;

    private:
        glm::vec3 m_Position;
        float m_Yaw;
        float m_Pitch;
        float m_Fov;
        float m_AspectRatio;
        float m_NearClip;
        float m_FarClip;
    };
}