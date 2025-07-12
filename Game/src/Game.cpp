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
		specs.Name = "Racing Game";
		specs.Width = 1920;
		specs.Height = 1080;
		specs.CommandLineArgs = args;

		return new Game(specs);
	}
}