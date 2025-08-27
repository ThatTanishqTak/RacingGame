#include "GameLayer.h"
#include "Core/EntryPoint.h"
#include "Core/PaletteManager.h"
#include "Renderer/StateStream.h"
#include "Core/EventBus.h"
#include "Core/RaceState.h"
#include "Core/RaceSimulation.h"

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
        RaceSimulation l_Sim(l_Start);
        const double l_FixedTimeStep = 1.0 / 60.0;
        double l_LastTime = l_Start;
        double l_Accumulator = 0.0;

        GameLayer l_Layer;
        while (!m_Window->WindowShouldClose())
        {
            double l_CurrentTime = glfwGetTime();
            double l_FrameTime = l_CurrentTime - l_LastTime;
            l_LastTime = l_CurrentTime;
            l_Accumulator += l_FrameTime;

            while (l_Accumulator >= l_FixedTimeStep)
            {
                l_Sim.Step(l_FixedTimeStep);
                Engine::g_StateBuffer.SubmitSnapshot({ l_Sim.GetTime(), l_Sim.GetCars() });
                l_Accumulator -= l_FixedTimeStep;
            }

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