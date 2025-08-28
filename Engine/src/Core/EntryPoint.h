#include "Application.h"

namespace Engine
{
    Application* CreateApplication(int argc, char** argv);
}

int main(int argc, char** argv)
{
    auto a_App = Engine::CreateApplication(argc, argv);
    a_App->Run();

    delete a_App;

    return 0;
}