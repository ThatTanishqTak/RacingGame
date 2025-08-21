#include "Application.h"

#include <stdexcept>

namespace Engine
{
    Application::Application()
    {
        m_Window = std::make_unique<Window>();
        if (!m_Window->Initialize(1920, 1080, "Racing Game"))
        {
            throw std::runtime_error("Failed to initialize the window");
        }

        m_Renderer = std::make_unique<Renderer>();
        if (!m_Renderer->Initialize())
        {
            throw std::runtime_error("Failed to initialize the renderer");
        }
    }

    Application::~Application()
    {
        m_Renderer->Shutdown();
        m_Window->Shutdown();
    }
}