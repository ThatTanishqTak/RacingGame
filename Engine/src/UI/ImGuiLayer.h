#pragma once

struct GLFWwindow;

namespace Engine
{
	class ImGuiLayer
	{
	public:
		bool Initialize(GLFWwindow* window);
		void Shutdown();

		void BeginFrame();
		void EndFrame();
	};
}