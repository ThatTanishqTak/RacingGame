#pragma once

#include "Window/Window.h"
#include "Renderer/Renderer.h"

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
                m_Renderer->BeginFrame();
                m_Renderer->EndFrame();

                m_Window->SwapBuffers();
                m_Window->PollEvents();
            }
        }

    private:
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<Renderer> m_Renderer;
    };
}