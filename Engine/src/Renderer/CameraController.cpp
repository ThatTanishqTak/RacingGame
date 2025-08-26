#include "CameraController.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    bool CameraController::Initialize(Camera* camera)
    {
        m_Camera = camera;
        glm::vec3 l_Pos = m_Camera->GetPosition();
        m_TargetPan = { l_Pos.x, l_Pos.y };
        m_TargetZoom = l_Pos.z;

        return true;
    }

    void CameraController::Update(float deltaTime)
    {
        if (!m_Camera) return;

        if (m_Mode == Mode::FollowSelected && m_CarPositionProvider && m_SelectedCarId >= 0)
        {
            m_TargetPan = m_CarPositionProvider(m_SelectedCarId);
        }

        glm::vec3 l_Pos = m_Camera->GetPosition();
        glm::vec2 l_Pan{ l_Pos.x, l_Pos.y };
        l_Pan = SmoothDamp(l_Pan, m_TargetPan, m_PanVelocity, m_PanSmoothTime, deltaTime);
        l_Pos.x = l_Pan.x;
        l_Pos.y = l_Pan.y;

        l_Pos.z = SmoothDamp(l_Pos.z, m_TargetZoom, m_ZoomVelocity, m_ZoomSmoothTime, deltaTime);
        m_Camera->SetPosition(l_Pos);

        ClampToTrack();
    }

    void CameraController::SetMode(Mode mode)
    {
        m_Mode = mode;
        if (m_Mode == Mode::FitAll && m_Camera)
        {
            glm::mat4 l_Proj = m_Camera->GetProjectionMatrix();
            float l_Fov = 2.0f * glm::atan(1.0f / l_Proj[1][1]);
            float l_Aspect = l_Proj[1][1] / l_Proj[0][0];

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
        float l_Exp = 1.0f / (1.0f + l_X + 0.48f * l_X * l_X + 0.235f * l_X * l_X * l_X);
        glm::vec2 l_Change = current - target;
        glm::vec2 l_Temp = (velocity + l_Omega * l_Change) * deltaTime;
        velocity = (velocity - l_Omega * l_Temp) * l_Exp;

        return target + (l_Change + l_Temp) * l_Exp;
    }

    float CameraController::SmoothDamp(float current, float target, float& velocity, float smoothTime, float deltaTime)
    {
        float l_Omega = 2.0f / smoothTime;
        float l_X = l_Omega * deltaTime;
        float l_Exp = 1.0f / (1.0f + l_X + 0.48f * l_X * l_X + 0.235f * l_X * l_X * l_X);
        float l_Change = current - target;
        float l_Temp = (velocity + l_Omega * l_Change) * deltaTime;
        velocity = (velocity - l_Omega * l_Temp) * l_Exp;

        return target + (l_Change + l_Temp) * l_Exp;
    }

    void CameraController::ClampToTrack()
    {
        if (!m_Camera)
        {
            return;
        }

        glm::vec3 l_Pos = m_Camera->GetPosition();
        glm::mat4 l_Proj = m_Camera->GetProjectionMatrix();

        float l_Fov = 2.0f * glm::atan(1.0f / l_Proj[1][1]);
        float l_Aspect = l_Proj[1][1] / l_Proj[0][0];
        float l_HalfHeight = glm::tan(l_Fov * 0.5f) * l_Pos.z;
        float l_HalfWidth = l_HalfHeight * l_Aspect;

        l_Pos.x = glm::clamp(l_Pos.x, m_TrackMin.x + l_HalfWidth, m_TrackMax.x - l_HalfWidth);
        l_Pos.y = glm::clamp(l_Pos.y, m_TrackMin.y + l_HalfHeight, m_TrackMax.y - l_HalfHeight);

        m_Camera->SetPosition(l_Pos);
    }
}