#pragma once

#include "Window/Window.h"

#include <memory>

namespace Engine
{
	class Application
	{
	public:
		Application();
		~Application();

		virtual void Run()
		{
			while (!m_Window->ShouldClose())
			{
				m_Window->SwapBuffers();

				m_Window->PollEvents();
			}
		}

	private:
		std::unique_ptr<Window> m_Window;
	};
}