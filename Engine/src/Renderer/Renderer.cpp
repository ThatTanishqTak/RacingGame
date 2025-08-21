#include "Renderer.h"

#include <glad/glad.h>

namespace Engine
{
    bool Renderer::Initialize()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        return true;
    }

    void Renderer::Shutdown()
    {

    }

    void Renderer::BeginFrame()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::EndFrame()
    {

    }

    void Renderer::DrawMesh(const Mesh& mesh, const Shader& shader, const glm::mat4& transform)
    {
        (void)mesh;
        (void)shader;
        (void)transform;
    }
}