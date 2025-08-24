#pragma once

#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "UI/ImGuiLayer.h"

#include <memory>

namespace Engine
{
    class Application
    {
    public:
        Application();
        ~Application();

        virtual void Run()
        {
            while (!m_Window->WindowShouldClose())
            {
                glm::mat4 l_View = m_Camera->GetViewMatrix(); (void)l_View;
                glm::mat4 l_Projection = m_Camera->GetProjectionMatrix(); (void)l_Projection;

                m_ImGuiLayer->BeginFrame();
                m_Renderer->BeginFrame();
                m_Renderer->EndFrame();
                m_ImGuiLayer->EndFrame();

                m_Window->SwapBuffers();
                m_Window->PollEvents();
            }
        }

    protected:
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<Renderer> m_Renderer;
        std::unique_ptr<Camera> m_Camera;
        std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
    };
}