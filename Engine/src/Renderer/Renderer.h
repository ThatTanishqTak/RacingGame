#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    class Mesh;
    class Shader;

    class Renderer
    {
    public:
        bool Initialize();
        void Shutdown();

        void BeginFrame();
        void EndFrame();

        void DrawMesh(const Mesh& mesh, const Shader& shader, const glm::mat4& transform);
    };
}