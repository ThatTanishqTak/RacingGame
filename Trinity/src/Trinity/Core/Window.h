#pragma once

#include <string>
#include <GLFW/glfw3.h>

namespace Trinity
{
    struct WindowSpecification
    {
        std::string Title = "Trinity";
        uint32_t Width = 1920;
        uint32_t Height = 1080;
    };

    class Window
    {
    public:
        Window() = default;
        Window(const WindowSpecification& specification);
        ~Window();

        bool Initialize();
        void Shutdown();

        void PollEvents();
        bool ShouldClose() const;

        GLFWwindow* GetNativeWindow() const { return m_Window; }

    private:
        WindowSpecification m_Specification;
        GLFWwindow* m_Window = nullptr;
    };
}