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
        g_PaletteManager.LoadPalettes("Assets/Palettes/primary.json", "Assets/Palettes/colourblind.json");
    }
    ~Game() = default;

    void Run() override
    {
        double l_Start = glfwGetTime();
        Engine::g_StateBuffer.SubmitSnapshot({ l_Start, { { 0, { 0.0f, 0.0f, 0.0f } }, { 1, { 2.0f, 0.0f, 0.0f } } } });
        Engine::g_StateBuffer.SubmitSnapshot({ l_Start + 5.0, { { 0, { 0.0f, 0.0f, -10.0f } }, { 1, { 2.0f, 0.0f, -10.0f } } } });

        GameLayer l_Layer;
        bool l_LastV = false;

        while (!m_Window->WindowShouldClose())
        {
            bool l_CurrentV = m_Window->IsKeyPressed(GLFW_KEY_V);
            if (l_CurrentV && !l_LastV)
            {
                if (GetViewMode() == Engine::Renderer::ViewMode::View3D)
                {
                    SetViewMode(Engine::Renderer::ViewMode::View2DTopDown);
                }

                else
                {
                    SetViewMode(Engine::Renderer::ViewMode::View3D);
                }
            }
            l_LastV = l_CurrentV;

            m_ImGuiLayer->BeginFrame();
            m_Renderer->BeginFrame();

            l_Layer.Update();
            l_Layer.Render();

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