#pragma once

#ifdef MANAGEMENT_MODE

#include "UIManager.h"

class HUD
{
public:
    HUD();
    void Render();

private:
    UIManager m_UiManager;
};

#else

#include <vector>
#include <glm/glm.hpp>
#include "Renderer/TrackRenderer.h"

class HUD
{
public:
    HUD();
    void SetCarPositions(const std::vector<glm::vec2>& positions);
    void Render();

private:
    Engine::TrackRenderer m_TrackRenderer;
};

#endif