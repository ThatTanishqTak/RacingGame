#pragma once

#include "Trinity/Core/Core.h"
#include "Trinity/Vulkan/VulkanContext.h"
#include "Trinity/Core/Window.h"

namespace Trinity
{
    struct ApplicationCommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;
    };

    struct ApplicationSpecification
    {
        std::string Name = "Trinity-Application";
        unsigned int Width = 1920;
        unsigned int Height = 1080;
        ApplicationCommandLineArgs CommandLineArgs;
    };

    class Application
    {
    public:
        Application() = default;
        Application(const ApplicationSpecification& specification);
        virtual ~Application();

        virtual void Run()
        {
            while (m_Window && !m_Window->ShouldClose())
            {
                m_Window->PollEvents();
            }
        }

    protected:
        ApplicationSpecification m_Specification;
        
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<VulkanContext> m_VulkanContext;
    };
}