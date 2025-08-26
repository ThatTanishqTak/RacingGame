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

        enum class ViewMode { View3D, View2DTopDown };

        void SetViewMode(ViewMode mode);
        ViewMode GetViewMode() const { return m_ViewMode; }
        const glm::vec2& GetTrackMin() const { return m_TrackMin; }
        const glm::vec2& GetTrackMax() const { return m_TrackMax; }

#ifndef MANAGEMENT_MODE
        void DrawTrack();
#endif
        void View2DTopDown();

    public:
        float m_CarMarkerRadius{ 5.0f };
        bool m_ShowCarLabels{ false };

    private:
        Camera* m_Camera{};
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<Mesh> m_CarMesh;
        std::shared_ptr<Mesh> m_TrackMesh;
        glm::vec2 m_TrackMin{};
        glm::vec2 m_TrackMax{};
        ViewMode m_ViewMode{ ViewMode::View3D };
    };

    extern Renderer* g_Renderer;
}