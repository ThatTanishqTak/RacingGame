#pragma once

#include "Trinity/Core/Core.h"

namespace Trinity
{
    struct ApplicationCommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;
    };

    class Application
    {
    public:
        Application() = default;
        virtual ~Application() = default;

        virtual void Run()
        {

        }
    };
}