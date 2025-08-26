#pragma once

#include "Camera.h"

#include <glm/glm.hpp>
#include <functional>
#include <limits>

namespace Engine
{
    class CameraController
    {
    public:
        enum class Mode
        {
            FitAll,
            FollowSelected,
            Free
        };

        bool Initialize(Camera* camera);
        void Update(float deltaTime);

        void SetMode(Mode mode);
        void SelectCar(int id);

        void SetPanTarget(const glm::vec2& target);
        void SetZoomTarget(float target);
        void SetTrackBounds(const glm::vec2& minBounds, const glm::vec2& maxBounds);

        void SetCarPositionProvider(const std::function<glm::vec2(int)>& provider);

    private:
        glm::vec2 SmoothDamp(const glm::vec2& current, const glm::vec2& target, glm::vec2& velocity, float smoothTime, float deltaTime);
        float SmoothDamp(float current, float target, float& velocity, float smoothTime, float deltaTime);
        void ClampToTrack();

    private:
        Camera* m_Camera{ nullptr };
        Mode m_Mode{ Mode::FitAll };

        glm::vec2 m_TargetPan{ 0.0f };
        glm::vec2 m_PanVelocity{ 0.0f };
        float m_PanSmoothTime{ 0.15f };

        float m_TargetZoom{ 3.0f };
        float m_ZoomVelocity{ 0.0f };
        float m_ZoomSmoothTime{ 0.15f };

        glm::vec2 m_TrackMin{ -std::numeric_limits<float>::max() };
        glm::vec2 m_TrackMax{ std::numeric_limits<float>::max() };

        int m_SelectedCarId{ -1 };
        std::function<glm::vec2(int)> m_CarPositionProvider;
    };
}