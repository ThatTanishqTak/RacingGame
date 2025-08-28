#include "GameLayer.h"

#include "Renderer/StateStream.h"
#include "Renderer/CameraController.h"

#include "Core/EntryPoint.h"
#include "Core/PaletteManager.h"
#include "Core/EventBus.h"
#include "Core/RaceState.h"
#include "Core/RaceSimulation.h"
#include "Core/Circuit.h"
#include "Core/Track.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <vector>

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

        Circuit l_Circuit("Sample track", 5.0);
        l_Circuit.LoadLayout("Assets/Tracks/SampleTrack.txt");

        Track l_Track;
        l_Track.SetCenterline(l_Circuit.GetCenterline());
        std::vector<float> l_Widths(l_Circuit.GetCenterline().size(), l_Circuit.GetHalfWidth() * 2.0f);
        l_Track.SetWidthProfile(l_Widths);

        RaceSimulation l_Sim(l_Track, l_Start);
        const double l_FixedTimeStep = 1.0 / 60.0;
        double l_LastTime = l_Start;
        double l_Accumulator = 0.0;

        Engine::g_Renderer->SetTrackCenterline(l_Track.GetCenterline(), l_Circuit.GetHalfWidth());

        Engine::CameraController l_CameraController;
        l_CameraController.Initialize(m_Camera.get());
        
        auto [l_MinBounds, l_MaxBounds] = l_Track.Bounds();
        l_CameraController.SetTrackBounds(l_MinBounds, l_MaxBounds);
        l_CameraController.SetCarPositionProvider([&l_Sim](int id) 
            {
                const auto& l_Cars = l_Sim.GetCars();
                if (id >= 0 && id < (int)l_Cars.size())
                {
                    return glm::vec2(l_Cars[id].Position.x, l_Cars[id].Position.z);
                }

                return glm::vec2(0.0f);
            });
        l_CameraController.SetMode(Engine::CameraController::Mode::FitAll);
        bool l_SwitchedToFollow = false;

        GameLayer l_Layer;
        while (!m_Window->WindowShouldClose())
        {
            double l_CurrentTime = glfwGetTime();
            double l_FrameTime = l_CurrentTime - l_LastTime;
            l_LastTime = l_CurrentTime;
            l_Accumulator += l_FrameTime;

            while (l_Accumulator >= l_FixedTimeStep)
            {
                l_Sim.Update(l_FixedTimeStep);
                Engine::g_StateBuffer.SubmitSnapshot({ l_Sim.GetTime(), l_Sim.GetCars() });
                l_Accumulator -= l_FixedTimeStep;
            }

            l_CameraController.Update(static_cast<float>(l_FrameTime));
            if (!l_SwitchedToFollow)
            {
                l_CameraController.SelectCar(0);
                l_SwitchedToFollow = true;
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
            l_Layer.Render(l_Circuit.GetLayout());

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