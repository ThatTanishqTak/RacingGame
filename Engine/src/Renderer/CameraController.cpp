#include "CameraController.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    bool CameraController::Initialize(Camera* camera)
    {
        m_Camera = camera;

        glm::vec3 l_Position = m_Camera->GetPosition();
        m_TargetPan = { l_Position.x, l_Position.z };
        m_TargetZoom = l_Position.y;

        return true;
    }

    void CameraController::Update(float deltaTime)
    {
        if (!m_Camera)
        {
            return;
        }

        if (m_Mode == Mode::FollowSelected && m_CarPositionProvider && m_SelectedCarId >= 0)
        {
            m_TargetPan = m_CarPositionProvider(m_SelectedCarId);
        }

        glm::vec3 l_Position = m_Camera->GetPosition();
        glm::vec2 l_Pan{ l_Position.x, l_Position.z };

        l_Pan = SmoothDamp(l_Pan, m_TargetPan, m_PanVelocity, m_PanSmoothTime, deltaTime);
        l_Position.x = l_Pan.x;
        l_Position.z = l_Pan.y;

        l_Position.y = SmoothDamp(l_Position.y, m_TargetZoom, m_ZoomVelocity, m_ZoomSmoothTime, deltaTime);
        m_Camera->SetPosition(l_Position);

        ClampToTrack();
    }

    void CameraController::SetMode(Mode mode)
    {
        m_Mode = mode;
        if (m_Mode == Mode::FitAll && m_Camera)
        {
            glm::mat4 l_Projection = m_Camera->GetProjectionMatrix();
            float l_Fov = 2.0f * glm::atan(1.0f / l_Projection[1][1]);
            float l_Aspect = l_Projection[1][1] / l_Projection[0][0];

            glm::vec2 l_Center = (m_TrackMin + m_TrackMax) * 0.5f;
            float l_Width = m_TrackMax.x - m_TrackMin.x;
            float l_Height = m_TrackMax.y - m_TrackMin.y;
            float l_RequiredHalfHeight = glm::max(l_Height * 0.5f, (l_Width * 0.5f) / l_Aspect);
            float l_Distance = l_RequiredHalfHeight / glm::tan(l_Fov * 0.5f);

            m_TargetPan = l_Center;
            m_TargetZoom = l_Distance;
        }
    }

    void CameraController::SelectCar(int id)
    {
        m_SelectedCarId = id;
        m_Mode = Mode::FollowSelected;
    }

    void CameraController::SetPanTarget(const glm::vec2& target)
    {
        m_TargetPan = target;
    }

    void CameraController::SetZoomTarget(float target)
    {
        m_TargetZoom = target;
    }

    void CameraController::SetTrackBounds(const glm::vec2& minBounds, const glm::vec2& maxBounds)
    {
        m_TrackMin = minBounds;
        m_TrackMax = maxBounds;
    }

    void CameraController::SetCarPositionProvider(const std::function<glm::vec2(int)>& provider)
    {
        m_CarPositionProvider = provider;
    }

    glm::vec2 CameraController::SmoothDamp(const glm::vec2& current, const glm::vec2& target, glm::vec2& velocity, float smoothTime, float deltaTime)
    {
        float l_Omega = 2.0f / smoothTime;
        float l_X = l_Omega * deltaTime;
        float l_Exponent = 1.0f / (1.0f + l_X + 0.48f * l_X * l_X + 0.235f * l_X * l_X * l_X);

        glm::vec2 l_Change = current - target;
        glm::vec2 l_Temp = (velocity + l_Omega * l_Change) * deltaTime;
        
        velocity = (velocity - l_Omega * l_Temp) * l_Exponent;

        return target + (l_Change + l_Temp) * l_Exponent;
    }

    float CameraController::SmoothDamp(float current, float target, float& velocity, float smoothTime, float deltaTime)
    {
        float l_Omega = 2.0f / smoothTime;
        float l_X = l_Omega * deltaTime;
        float l_Exponent = 1.0f / (1.0f + l_X + 0.48f * l_X * l_X + 0.235f * l_X * l_X * l_X);
        float l_Change = current - target;
        float l_Temp = (velocity + l_Omega * l_Change) * deltaTime;

        velocity = (velocity - l_Omega * l_Temp) * l_Exponent;

        return target + (l_Change + l_Temp) * l_Exponent;
    }

    void CameraController::ClampToTrack()
    {
        if (!m_Camera)
        {
            return;
        }

        glm::vec3 l_Position = m_Camera->GetPosition();
        glm::mat4 l_Projection = m_Camera->GetProjectionMatrix();

        float l_HalfWidth = 0.0f;
        float l_HalfHeight = 0.0f;

        bool l_IsPerspective = glm::abs(l_Projection[3][3]) < 0.5f;
        if (l_IsPerspective)
        {
            float l_Fov = 2.0f * glm::atan(1.0f / l_Projection[1][1]);
            float l_Aspect = l_Projection[1][1] / l_Projection[0][0];
            l_HalfHeight = glm::tan(l_Fov * 0.5f) * l_Position.y;
            l_HalfWidth = l_HalfHeight * l_Aspect;
        }

        else
        {
            l_HalfWidth = 1.0f / l_Projection[0][0];
            l_HalfHeight = 1.0f / l_Projection[1][1];
        }

        l_Position.x = glm::clamp(l_Position.x, m_TrackMin.x + l_HalfWidth, m_TrackMax.x - l_HalfWidth);
        l_Position.z = glm::clamp(l_Position.z, m_TrackMin.y + l_HalfHeight, m_TrackMax.y - l_HalfHeight);

        m_Camera->SetPosition(l_Position);
    }
}