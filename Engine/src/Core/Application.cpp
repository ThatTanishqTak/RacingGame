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

        m_Camera = std::make_unique<Camera>();
        if (!m_Camera->Initialize(45.0f, 16.0f / 9.0f, 0.1f, 100.0f))
        {
            throw std::runtime_error("Failed to initialize the camera");
        }

        m_Renderer = std::make_unique<Renderer>();
        if (!m_Renderer->Initialize(m_Camera.get()))
        {
            throw std::runtime_error("Failed to initialize the renderer");
        }

        m_ImGuiLayer = std::make_unique<ImGuiLayer>();
        if (!m_ImGuiLayer->Initialize(m_Window->GetNativeWindow()))
        {
            throw std::runtime_error("Failed to initialize the imgui layer");
        }
    }

    Application::~Application()
    {
        m_ImGuiLayer->Shutdown();
        m_Renderer->Shutdown();
        m_Window->Shutdown();
    }

    void Application::SetViewMode(Renderer::ViewMode mode)
    {
        if (m_Renderer)
        {
            m_Renderer->SetViewMode(mode);
        }
    }

    Renderer::ViewMode Application::GetViewMode() const
    {
        return m_Renderer ? m_Renderer->GetViewMode() : Renderer::ViewMode::View3D;
    }
}