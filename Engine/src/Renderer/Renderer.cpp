#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "StateStream.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <sstream>

namespace Engine
{
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

        double time = glfwGetTime();
        auto states = GlobalStateBuffer.Interpolate(time);
        for (const auto& car : states)
        {
            glm::mat4 l_Transform = glm::translate(glm::mat4(1.0f), car.Position);
            glm::vec4 l_CarColour(1.0f, 0.5f, 0.2f, 1.0f);

            DrawMesh(*m_CarMesh, *m_Shader, l_Transform, l_CarColour);
        }
    }

    void Renderer::EndFrame()
    {

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
}