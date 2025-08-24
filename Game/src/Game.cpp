#include "GameLayer.h"
#include "Core/EntryPoint.h"

class Game : public Engine::Application
{
public:
    Game() = default;
    ~Game() = default;

    void Run() override
    {
        GameLayer layer;
        while (!m_Window->WindowShouldClose())
        {
            m_ImGuiLayer->BeginFrame();
            m_Renderer->BeginFrame();

            layer.Render();

#ifndef MANAGEMENT_MODE
            m_Renderer->DrawTrack();
#endif

            m_Renderer->EndFrame();
            m_ImGuiLayer->EndFrame();

            m_Window->SwapBuffers();
            m_Window->PollEvents();
        }
    }
};

Engine::Application* Engine::CreateApplication()
{
    return new Game();
}