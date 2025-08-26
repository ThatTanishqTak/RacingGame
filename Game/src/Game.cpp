#include "GameLayer.h"
#include "Core/EntryPoint.h"
#include "Core/PaletteManager.h"
#include "Renderer/StateStream.h"

#include <GLFW/glfw3.h>

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
        double l_Start = glfwGetTime();
        Engine::GlobalStateBuffer.SubmitSnapshot({ l_Start, { { 0, { 0.0f, 0.0f, 0.0f } }, { 1, { 2.0f, 0.0f, 0.0f } } } });
        Engine::GlobalStateBuffer.SubmitSnapshot({ l_Start + 5.0, { { 0, { 0.0f, 0.0f, -10.0f } }, { 1, { 2.0f, 0.0f, -10.0f } } } });

        GameLayer layer;
        while (!m_Window->WindowShouldClose())
        {
            m_ImGuiLayer->BeginFrame();
            m_Renderer->BeginFrame();

            layer.Update();
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