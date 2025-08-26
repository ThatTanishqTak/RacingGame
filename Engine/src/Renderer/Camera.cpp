#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

namespace Engine
{
    bool Camera::Initialize(float fov, float aspectRatio, float nearClip, float farClip)
    {
        m_Position = glm::vec3{ 0.0f, 0.0f, 3.0f };
        m_Yaw = -90.0f;
        m_Pitch = 0.0f;
        m_Fov = fov;
        m_AspectRatio = aspectRatio;
        m_NearClip = nearClip;
        m_FarClip = farClip;

        return true;
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        glm::vec3 l_Front;
        l_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        l_Front.y = sin(glm::radians(m_Pitch));
        l_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

        glm::vec3 l_Direction = glm::normalize(l_Front);

        return glm::lookAt(m_Position, m_Position + l_Direction, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 Camera::GetProjectionMatrix() const { return glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip); }
}