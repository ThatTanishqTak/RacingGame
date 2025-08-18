#include "Application.h"

namespace Engine
{
	Application::Application()
	{
		m_Window = std::make_unique<Window>();
		if (!m_Window->Initialize(1920, 1080, "Game"))
		{
			return;
		}
	}

	Application::~Application()
	{
		m_Window->Shutdown();
	}
}