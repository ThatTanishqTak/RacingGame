#pragma once

#include "Core/RaceState.h"

namespace Engine { class Renderer; }

class CarInspectorPanel
{
public:
    CarInspectorPanel(Engine::Renderer& renderer);
    void Render(const RaceState& state);

private:
    Engine::Renderer& m_Renderer;
};