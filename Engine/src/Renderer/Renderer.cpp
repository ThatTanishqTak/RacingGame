#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "StateStream.h"
#include "Core/PaletteManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

namespace Engine
{
    Renderer* g_Renderer = nullptr;

    namespace
    {
        std::string ReadFile(const std::string& path)
        {
            std::ifstream l_File(path);
            std::stringstream l_Stream;
            l_Stream << l_File.rdbuf();

            return l_Stream.str();
        }
    }

    bool Renderer::Initialize(Camera* camera)
    {
        m_Camera = camera;

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        std::string l_Vertex = ReadFile("Assets/Shaders/Shader.vert");
        std::string l_Fragment = ReadFile("Assets/Shaders/Shader.frag");
        m_Shader = ShaderLibrary::Get().Load("Basic", l_Vertex, l_Fragment);

        m_CarMesh = Mesh::CreateCube();

        std::vector<float> l_TrackVertices =
        {
            -5.0f, 0.0f, -10.0f,
             5.0f, 0.0f, -10.0f,
             5.0f, 0.0f,  10.0f,
            -5.0f, 0.0f,  10.0f
        };

        std::vector<uint32_t> l_TrackIndices = { 0, 1, 2, 2, 3, 0 };
        m_TrackMesh = std::make_shared<Mesh>(l_TrackVertices, l_TrackIndices);

        m_TrackMin = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
        m_TrackMax = { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
        
        const auto& l_Vertices = m_TrackMesh->GetVertices();
        for (size_t i = 0; i < l_Vertices.size(); i += 3)
        {
            float x = l_Vertices[i];
            float z = l_Vertices[i + 2];

            m_TrackMin.x = std::min(m_TrackMin.x, x);
            m_TrackMin.y = std::min(m_TrackMin.y, z);
            m_TrackMax.x = std::max(m_TrackMax.x, x);
            m_TrackMax.y = std::max(m_TrackMax.y, z);
        }

        return true;
    }

    void Renderer::Shutdown()
    {

    }

    void Renderer::BeginFrame()
    {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifndef MANAGEMENT_MODE
        DrawTrack();
#endif

        if (m_ViewMode == ViewMode::View3D)
        {
            double l_Time = glfwGetTime();
            auto a_States = g_StateBuffer.Interpolate(l_Time);
            for (const auto& it_Car : a_States)
            {
                glm::mat4 l_Transform = glm::translate(glm::mat4(1.0f), it_Car.Position);
                glm::vec4 l_CarColour(1.0f, 0.5f, 0.2f, 1.0f);

                DrawMesh(*m_CarMesh, *m_Shader, l_Transform, l_CarColour);
            }
        }
    }

    void Renderer::EndFrame()
    {

    }

    void Renderer::View2DTopDown()
    {
#ifndef MANAGEMENT_MODE
        DrawTrack();
#endif

        ImDrawList* l_DrawList = ImGui::GetForegroundDrawList();

        double l_Time = glfwGetTime();
        auto a_States = g_StateBuffer.Interpolate(l_Time);

        glm::mat4 l_ViewProjection = m_Camera->GetProjectionMatrix() * m_Camera->GetViewMatrix();
        ImVec2 l_DisplaySize = ImGui::GetIO().DisplaySize;

        if (m_ShowLaneLines && m_TrackCenterline.size() > 1)
        {
            std::vector<ImVec2> l_Left;
            std::vector<ImVec2> l_Right;
            l_Left.reserve(m_TrackCenterline.size());
            l_Right.reserve(m_TrackCenterline.size());

            for (size_t i = 0; i < m_TrackCenterline.size(); ++i)
            {
                glm::vec2 p = m_TrackCenterline[i];
                glm::vec2 dir;
                if (i + 1 < m_TrackCenterline.size())
                {
                    dir = glm::normalize(m_TrackCenterline[i + 1] - p);
                }
                else
                {
                    dir = glm::normalize(p - m_TrackCenterline[i - 1]);
                }
                glm::vec2 normal(-dir.y, dir.x);
                glm::vec2 left = p + normal * m_TrackHalfWidth;
                glm::vec2 right = p - normal * m_TrackHalfWidth;

                auto toScreen = [&](const glm::vec2& pt) -> ImVec2
                    {
                        glm::vec4 clip = l_ViewProjection * glm::vec4(pt, 0.0f, 1.0f);
                        if (clip.w == 0.0f)
                        {
                            return ImVec2(0.0f, 0.0f);
                        }
                        glm::vec3 ndc = glm::vec3(clip) / clip.w;
                        return ImVec2((ndc.x * 0.5f + 0.5f) * l_DisplaySize.x,
                            (1.0f - (ndc.y * 0.5f + 0.5f)) * l_DisplaySize.y);
                    };

                l_Left.push_back(toScreen(left));
                l_Right.push_back(toScreen(right));
            }

            ImU32 l_Colour = IM_COL32(255, 255, 255, 80);
            for (size_t i = 1; i < l_Left.size(); ++i)
            {
                l_DrawList->AddLine(l_Left[i - 1], l_Left[i], l_Colour);
                l_DrawList->AddLine(l_Right[i - 1], l_Right[i], l_Colour);
            }
        }

        for (const auto& it_Car : a_States)
        {
            glm::vec4 l_Clip = l_ViewProjection * glm::vec4(it_Car.Position, 1.0f);
            if (l_Clip.w == 0.0f)
            {
                continue;
            }
            glm::vec3 l_NDC = glm::vec3(l_Clip) / l_Clip.w;

            ImVec2 l_Screen;
            l_Screen.x = (l_NDC.x * 0.5f + 0.5f) * l_DisplaySize.x;
            l_Screen.y = (1.0f - (l_NDC.y * 0.5f + 0.5f)) * l_DisplaySize.y;

            ImVec4 l_Colour = g_PaletteManager.GetTeamColour(static_cast<size_t>(it_Car.ID), false);
            ImU32 l_DrawColour = IM_COL32((int)(l_Colour.x * 255.0f), (int)(l_Colour.y * 255.0f), (int)(l_Colour.z * 255.0f), 255);

            l_DrawList->AddCircleFilled(l_Screen, m_CarMarkerRadius, l_DrawColour);

            if (m_ShowCarLabels)
            {
                char l_Buffer[8];
                snprintf(l_Buffer, sizeof(l_Buffer), "%i", it_Car.ID + 1);
                l_DrawList->AddText(ImVec2(l_Screen.x + m_CarMarkerRadius + 2.0f, l_Screen.y - 6.0f), IM_COL32(255, 255, 255, 255), l_Buffer);
            }
        }
    }

    void Renderer::DrawMesh(const Mesh& mesh, const Shader& shader, const glm::mat4& transform, const glm::vec4& colour)
    {
        if (!m_Camera)
        {
            return;
        }

        shader.Bind();

        glm::mat4 l_MVP = m_Camera->GetProjectionMatrix() * m_Camera->GetViewMatrix() * transform;
        shader.SetUniformMat4("u_MVP", l_MVP);
        shader.SetUniformVec4("u_Color", colour);

        mesh.Draw();
    }

#ifndef MANAGEMENT_MODE
    void Renderer::DrawTrack()
    {
        if (!m_Shader || !m_TrackMesh)
        {
            return;
        }

        glm::mat4 l_Transform = glm::mat4(1.0f);
        glm::vec4 l_TrackColour(1.0f);

        DrawMesh(*m_TrackMesh, *m_Shader, l_Transform, l_TrackColour);
    }
#endif

    void Renderer::SetViewMode(ViewMode mode)
    {
        if (!m_Camera || m_ViewMode == mode)
        {
            m_ViewMode = mode;
            return;
        }

        m_ViewMode = mode;

        if (m_ViewMode == ViewMode::View2DTopDown)
        {
            glm::vec2 l_Size = m_TrackMax - m_TrackMin;
            float l_Width = l_Size.x;
            float l_Depth = l_Size.y;
            float l_Aspect = m_Camera->GetAspectRatio();
            float l_Height = std::max(l_Depth * 0.5f, (l_Width * 0.5f) / l_Aspect);

            glm::vec3 l_Center((m_TrackMin.x + m_TrackMax.x) * 0.5f, 0.0f, (m_TrackMin.y + m_TrackMax.y) * 0.5f);
            m_Camera->LookTopDown(l_Center, l_Height);
        }

        else
        {
            m_Camera->LookPerspective();
        }
    }

    void Renderer::SetTrackCenterline(const std::vector<glm::vec2>& centerline, float halfWidth)
    {
        m_TrackCenterline = centerline;
        m_TrackHalfWidth = halfWidth;

        if (!m_TrackCenterline.empty())
        {
            m_TrackMin = m_TrackCenterline.front();
            m_TrackMax = m_TrackCenterline.front();
            for (const auto& p : m_TrackCenterline)
            {
                m_TrackMin = glm::min(m_TrackMin, p);
                m_TrackMax = glm::max(m_TrackMax, p);
            }
        }
    }
}