#pragma once

#include <vector>
#include <memory>
#include <string>

#include <glm/glm.hpp>

namespace Engine
{
    class Mesh
    {
    public:
        Mesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices);
        ~Mesh();

        void Draw() const;

        static std::shared_ptr<Mesh> CreateCube();
        static std::shared_ptr<Mesh> LoadFromFile(const std::string& path);

        void SetTransform(const glm::mat4& transform);
        const glm::mat4& GetTransform() const;

        unsigned int GetIndexCount() const { return static_cast<unsigned int>(m_Indices.size()); }

    private:
        void SetupMesh();

        std::vector<float> m_Vertices;
        std::vector<uint32_t> m_Indices;
        unsigned int m_VBO;
        unsigned int m_EBO;
        unsigned int m_VAO;
        glm::mat4 m_Transform;
    };
}