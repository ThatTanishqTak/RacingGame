#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>

namespace Engine
{
    static void GLFWErrorCallback(int code, const char* desc)
    {
        fprintf(stderr, "GLFW error %d: %s\n", code, desc);
    }

    bool Window::Initialize(int width, int height, const char* title)
    {
        glfwSetErrorCallback(GLFWErrorCallback);

        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");

            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!m_Window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");

            return false;
        }

        m_Width = width;
        m_Height = height;

        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");

            return false;
        }

        glViewport(0, 0, width, height);

        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow*, int width, int height) { glViewport(0, 0, width, height); });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) { ImGui_ImplGlfw_CursorPosCallback(window, x, y); });
        
        glfwSetCursorEnterCallback(m_Window, [](GLFWwindow* window, int entered) { ImGui_ImplGlfw_CursorEnterCallback(window, entered); });
        
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) { ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods); });
        
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) { ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset); });
        
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) { ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods); });
        
        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int character) { ImGui_ImplGlfw_CharCallback(window, character); });

        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) { ImGui_ImplGlfw_WindowFocusCallback(window, focused); });

        return true;
    }

    void Window::Shutdown()
    {
        if (m_Window)
        {
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;
        }
        glfwTerminate();
    }

    bool Window::WindowShouldClose() const { return glfwWindowShouldClose(m_Window); }

    void Window::PollEvents() const { glfwPollEvents(); }

    void Window::SwapBuffers() const { glfwSwapBuffers(m_Window); }

    bool Window::IsKeyPressed(int key) const
    {
        ImGuiIO& IO = ImGui::GetIO();
        if (IO.WantCaptureKeyboard)
        {
            return false;
        }
        
        return glfwGetKey(m_Window, key) == GLFW_PRESS;
    }

    bool Window::IsMouseButtonPressed(int button) const
    {
        ImGuiIO& IO = ImGui::GetIO();
        if (IO.WantCaptureMouse)
        {
            return false;
        }
        
        return glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
    }
}