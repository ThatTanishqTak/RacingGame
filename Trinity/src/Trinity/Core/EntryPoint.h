#pragma once

#include "Trinity/Core/Core.h"
#include "Trinity/Core/Application.h"

extern Trinity::Allication* Trinity::CreateApplication(ApplicationCommandLineArgs args);

int main(int args, char** argv)
{
	Trinity::Log::Init();

	auto app = Trinity::CreateApplication({ args, argv });

	app->Run();

	delete app;

	return 0;
}