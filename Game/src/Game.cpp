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
#include "Core/RaceConfiguration.h"
#include "Core/Replay.h"

#include "Controller/RaceController.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <vector>
#include <random>
#include <string>

class Game : public Engine::Application
{
public:
    Game(const std::string& saveFile, const std::string& replayFile) : m_SaveFile(saveFile), m_ReplayFile(replayFile)
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

        std::vector<glm::vec2> l_PitLane = l_Circuit.GetCenterline();
        l_Track.SetPitLane(l_PitLane, 0.0f, 0.0f);

        RaceController l_Controller(l_Track.GetLength(), 2, l_Start);
        l_Controller.SetPitStrategy(0, 1);
        l_Controller.SetPitStrategy(1, 1);

        RaceSimulation l_Simulation(l_Track, l_Start, l_Controller);
        const double l_FixedTimeStep = 1.0 / 60.0;
        double l_LastTime = l_Start;
        double l_Accumulator = 0.0;

        if (!m_ReplayFile.empty())
        {
            LoadReplay(m_ReplayFile, l_Start);
        }

        else
        {
            g_RaceConfig.Seed = static_cast<uint32_t>(std::random_device{}());
            ResetRaceSeed();
            if (!m_SaveFile.empty())
            {
                m_Logger.Open(m_SaveFile, g_RaceConfig);
            }
        }

        Engine::g_Renderer->SetTrackCenterline(l_Track.GetCenterline(), l_Circuit.GetHalfWidth());

        Engine::CameraController l_CameraController;
        l_CameraController.Initialize(m_Camera.get());

        auto [l_MinBounds, l_MaxBounds] = l_Track.Bounds();
        l_CameraController.SetTrackBounds(l_MinBounds, l_MaxBounds);
        l_CameraController.SetCarPositionProvider([&l_Simulation](int id)
            {
                const auto& l_Cars = l_Simulation.GetCars();
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

            if (m_ReplayFile.empty())
            {
                l_Accumulator += l_FrameTime;
                while (l_Accumulator >= l_FixedTimeStep)
                {
                    l_Simulation.Update(l_FixedTimeStep);
                    Engine::Snapshot l_BufferSnapshot{ l_Simulation.GetTime(), l_Simulation.GetCars() };
                    Engine::g_StateBuffer.SubmitSnapshot(l_BufferSnapshot);
                    if (!m_SaveFile.empty())
                    {
                        Engine::Snapshot l_LogSnapshot{ l_Simulation.GetTime() - l_Start, l_Simulation.GetCars() };
                        m_Logger.WriteSnapshot(l_LogSnapshot);
                    }
                    l_Accumulator -= l_FixedTimeStep;
                }
            }

            l_CameraController.Update(static_cast<float>(l_FrameTime));
            if (!l_SwitchedToFollow)
            {
                l_CameraController.SelectCar(0);
                l_SwitchedToFollow = true;
            }

            m_ImGuiLayer->BeginFrame();
            m_Renderer->BeginFrame();

            l_Layer.Update();
            l_Layer.Render(l_Circuit.GetLayout());

            m_Renderer->EndFrame();
            m_ImGuiLayer->EndFrame();

            m_Window->SwapBuffers();
            m_Window->PollEvents();
        }

        if (!m_SaveFile.empty())
        {
            m_Logger.Close();
        }
    }

private:
    std::string m_SaveFile;
    std::string m_ReplayFile;
    ReplayLogger m_Logger;
};

Engine::Application* Engine::CreateApplication(int argc, char** argv)
{
    std::string l_SaveFile;
    std::string l_ReplayFile;
    for (int it_I = 1; it_I < argc; ++it_I)
    {
        std::string l_Arg = argv[it_I];
        if (l_Arg == "--save" && it_I + 1 < argc)
        {
            l_SaveFile = argv[++it_I];
        }

        else if (l_Arg == "--replay" && it_I + 1 < argc)
        {
            l_ReplayFile = argv[++it_I];
        }
    }
    return new Game(l_SaveFile, l_ReplayFile);
}