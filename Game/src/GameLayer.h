#pragma once

#include <string>
#include <vector>

class EventBus;
namespace Engine { class Renderer; }

class GameLayer
{
public:
    GameLayer(EventBus& eventBus, Engine::Renderer& renderer);
    void Update();
    void Render(const std::vector<std::string>& trackLayout);

private:
    EventBus& m_EventBus;
    Engine::Renderer& m_Renderer;
};