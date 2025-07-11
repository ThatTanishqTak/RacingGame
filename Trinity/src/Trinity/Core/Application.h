#pragma once

#include "Trinity/Core/Core.h"

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
        virtual ~Application() = default;

        virtual void Run()
        {

        }

    protected:
        ApplicationSpecification m_Specification;
    };
}