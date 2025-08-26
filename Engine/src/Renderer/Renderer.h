#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace Engine
{
    class Mesh;
    class Shader;
    class Camera;

    class Renderer
    {
    public:
        bool Initialize(Camera* camera);
        void Shutdown();

        void BeginFrame();
        void EndFrame();

        void DrawMesh(const Mesh& mesh, const Shader& shader, const glm::mat4& transform, const glm::vec4& colour);

#ifndef MANAGEMENT_MODE
        void DrawTrack();
#endif

    private:
        Camera* m_Camera{};
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<Mesh> m_CarMesh;
        std::shared_ptr<Mesh> m_TrackMesh;
    };
}