#include "GameLayer.h"
#include "Core/EntryPoint.h"
#include "Core/PaletteManager.h"

class Game : public Engine::Application
{
public:
    Game()
    {
        g_PaletteManager.LoadPalettes("Game/Assets/Palettes/primary.json", "Game/Assets/Palettes/colourblind.json");
    }
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