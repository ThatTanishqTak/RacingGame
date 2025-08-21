#pragma once

struct GLFWwindow;

namespace Engine
{
	class Window
	{
	public:
		bool Initialize(int width, int height, const char* title);
		void Shutdown();

		bool WindowShouldClose() const;
		void PollEvents() const;
		void SwapBuffers() const;

		GLFWwindow* GetNativeWindow() const { return m_Window; }

	private:
		GLFWwindow* m_Window = nullptr;
		int m_Width = 0;
		int m_Height = 0;
	};
}
