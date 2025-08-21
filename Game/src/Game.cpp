#include "Core/EntryPoint.h"

class Game : public Engine::Application
{
public:
	Game() = default;
	~Game() = default;
};

Engine::Application* Engine::CreateApplication()
{
	return new Game();
}