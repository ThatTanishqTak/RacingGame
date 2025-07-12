#include "trpch.h"

#include "Trinity/Core/Window.h"
#include "Trinity/Core/Log.h"

namespace Trinity
{
    Window::Window(const WindowSpecification& specification) : m_Specification(specification)
    {

    }

    bool Window::Initialize()
    {
        if (!glfwInit())
        {
            TR_CORE_ERROR("Failed to initialize GLFW");
            
            return false;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_Window = glfwCreateWindow(static_cast<int>(m_Specification.Width), static_cast<int>(m_Specification.Height), m_Specification.Title.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            TR_CORE_ERROR("Failed to create window");
            glfwTerminate();
            
            return false;
        }

        TR_CORE_INFO("Window initialized successfully");

        return true;
    }

    void Window::Shutdown()
    {
        if (m_Window)
        {
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;

            glfwTerminate();
        }

        TR_CORE_INFO("Window shutdown successfully");
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    bool Window::ShouldClose() const
    {
        return m_Window && glfwWindowShouldClose(m_Window);
    }
}