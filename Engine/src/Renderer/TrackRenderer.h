#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Engine
{
    class TrackRenderer
    {
    public:
        bool LoadFromSVG(const std::string& filePath);

        void SetZoom(float zoom) { m_Zoom = zoom; }
        void SetPan(const glm::vec2& pan) { m_Pan = pan; }
        glm::vec2 WorldToScreen(const glm::vec2& world) const;
        void Render() const;

    private:
        std::vector<glm::vec2> m_Points;
        float m_Zoom{ 1.0f };
        glm::vec2 m_Pan{ 0.0f, 0.0f };
    };
}