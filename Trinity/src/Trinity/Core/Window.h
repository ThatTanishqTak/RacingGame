#pragma once

#include <string>
#include <GLFW/glfw3.h>

namespace Trinity
{
    struct WindowSpecification
    {
        std::string Title{};
        uint32_t Width{};
        uint32_t Height{};
    };

    class Window
    {
    public:
        Window() = default;
        Window(const WindowSpecification& specification);
        ~Window() = default;

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