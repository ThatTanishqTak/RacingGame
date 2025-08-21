#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace Engine
{
	static void GLFWErrorCallback(int code, const char* desc)
	{
		fprintf(stderr, "GLFW error %d: %s\n", code, desc);
	}

	bool Window::Initialize(int width, int height, const char* title)
	{
		glfwSetErrorCallback(GLFWErrorCallback);

		if (!glfwInit())
		{
			throw std::runtime_error("Failed to initialize GLFW");

			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!m_Window)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");

			return false;
		}

		m_Width = width;
		m_Height = height;

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::runtime_error("Failed to initialize GLAD");

			return false;
		}

		glViewport(0, 0, width, height);

		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow*, int width, int height){ glViewport(0, 0, width, height); });

		return true;
	}

	void Window::Shutdown()
	{
		if (m_Window)
		{
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
		}
		glfwTerminate();
	}

	bool Window::WindowShouldClose() const { return glfwWindowShouldClose(m_Window); }

	void Window::PollEvents() const { glfwPollEvents(); }

	void Window::SwapBuffers() const { glfwSwapBuffers(m_Window); }
}
