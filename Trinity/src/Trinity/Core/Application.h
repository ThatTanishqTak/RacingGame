#pragma once

#include "Trinity/Core/Core.h"
#include "Trinity/Vulkan/VulkanContext.h"

namespace Trinity
{
    struct ApplicationCommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;
    };

    struct ApplicationSpecification
    {
        std::string Name = "Trinity";
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

        }

    protected:
        ApplicationSpecification m_Specification;
        std::unique_ptr<VulkanContext> m_VulkanContext;
    };
}