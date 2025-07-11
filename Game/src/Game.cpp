#include "Trinity.h"
#include "Trinity/Core/EntryPoint.h"

#include "GameLayer.h"

namespace Trinity
{
	class Game : public Application
	{
	public:
		Game(const ApplicationSpecification& specs) : Application(specs)
		{

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification specs;
		specs.Name = "Game";
		specs.CommandLineArgs = args;

		return new Game(specs);
	}
}