#include "HUD.h"

#ifdef MANAGEMENT_MODE

HUD::HUD() = default;

void HUD::Render()
{
    m_UiManager.Render();
}

#else

#include <algorithm>

HUD::HUD()
{
    m_TrackRenderer.LoadFromSVG("Game/Assets/Tracks/SampleTrack.svg");
}

void HUD::SetCarPositions(const std::vector<glm::vec2>& positions)
{
    if (positions.empty())
        return;

    glm::vec2 min = positions[0];
    glm::vec2 max = positions[0];
    for (const auto& p : positions)
    {
        min = glm::min(min, p);
        max = glm::max(max, p);
    }

    glm::vec2 size = max - min;
    if (size.x <= 0.0f) size.x = 1.0f;
    if (size.y <= 0.0f) size.y = 1.0f;

    const glm::vec2 screen{ 800.0f, 600.0f };
    float zoom = std::min(screen.x / size.x, screen.y / size.y);
    glm::vec2 center = (min + max) * 0.5f;
    glm::vec2 pan = screen / (2.0f * zoom) - center;
    m_TrackRenderer.SetZoom(zoom);
    m_TrackRenderer.SetPan(pan);
}

void HUD::Render()
{
    m_TrackRenderer.Render();
}

#endif