#include "GameLayer.h"
#include "Core/EntryPoint.h"
#include "Core/PaletteManager.h"
#include "Renderer/StateStream.h"
#include "Core/EventBus.h"
#include "Core/RaceState.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

static bool s_TogglePressed = false;

class Game : public Engine::Application
{
public:
    Game()
    {
        g_PaletteManager.LoadPalettes("Assets/Palettes/primary.json", "Assets/Palettes/colourblind.json");
        g_EventBus.Subscribe<ViewModeToggle>([this](const ViewModeToggle& e)
            {
                SetViewMode(e.TopDown ? Engine::Renderer::ViewMode::View2DTopDown : Engine::Renderer::ViewMode::View3D);
            });
    }
    ~Game() = default;

    void Run() override
    {
        double l_Start = glfwGetTime();
        Engine::g_StateBuffer.SubmitSnapshot({ l_Start, { { 0, { 0.0f, 0.0f, 0.0f } }, { 1, { 2.0f, 0.0f, 0.0f } } } });
        Engine::g_StateBuffer.SubmitSnapshot({ l_Start + 5.0, { { 0, { 0.0f, 0.0f, -10.0f } }, { 1, { 2.0f, 0.0f, -10.0f } } } });

        GameLayer l_Layer;
        while (!m_Window->WindowShouldClose())
        {
            bool l_TabDown = !ImGui::GetIO().WantCaptureKeyboard && m_Window->IsKeyPressed(GLFW_KEY_TAB);
            if (l_TabDown && !s_TogglePressed)
            {
                auto a_Mode = GetViewMode() == Engine::Renderer::ViewMode::View3D ? Engine::Renderer::ViewMode::View2DTopDown : Engine::Renderer::ViewMode::View3D;
                SetViewMode(a_Mode);

                g_EventBus.Publish(ViewModeToggle{ a_Mode == Engine::Renderer::ViewMode::View2DTopDown });
            }
            s_TogglePressed = l_TabDown;

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