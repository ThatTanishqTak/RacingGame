#include "Mesh.h"

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine
{
    Mesh::Mesh(const std::vector<float>& l_Vertices, const std::vector<uint32_t>& l_Indices) : m_Vertices(l_Vertices), m_Indices(l_Indices), m_VBO(0), m_EBO(0), 
            m_VAO(0), m_Transform(1.0f)
    {
        SetupMesh();
    }

    Mesh::~Mesh()
    {
        if (m_VBO)
        { 
            glDeleteBuffers(1, &m_VBO); 
        }

        if (m_EBO)
        {
            glDeleteBuffers(1, &m_EBO);
        }

        if (m_VAO)
        {
            glDeleteVertexArrays(1, &m_VAO);
        }
    }

    void Mesh::Draw() const
    {
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, GetIndexCount(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    std::shared_ptr<Mesh> Mesh::CreateCube()
    {
        std::vector<float> l_Vertices =
        {
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f
        };

        std::vector<uint32_t> l_Indices =
        {
            0, 1, 2, 2, 3, 0,
            1, 5, 6, 6, 2, 1,
            5, 4, 7, 7, 6, 5,
            4, 0, 3, 3, 7, 4,
            3, 2, 6, 6, 7, 3,
            4, 5, 1, 1, 0, 4
        };

        return std::make_shared<Mesh>(l_Vertices, l_Indices);
    }

    std::shared_ptr<Mesh> Mesh::LoadFromFile(const std::string& l_Path)
    {
        Assimp::Importer l_Importer;
        // Import the FBX file using Assimp
        const aiScene* l_Scene = l_Importer.ReadFile(l_Path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
        if (!l_Scene || !l_Scene->HasMeshes())
        {
            return nullptr;
        }

        // Retrieve the first mesh from the scene
        aiMesh* l_Mesh = l_Scene->mMeshes[0];

        std::vector<float> l_Vertices;
        // Extract vertex positions
        l_Vertices.reserve(l_Mesh->mNumVertices * 3);
        for (unsigned int it_Vertex = 0u; it_Vertex < l_Mesh->mNumVertices; ++it_Vertex)
        {
            const aiVector3D& l_Pos = l_Mesh->mVertices[it_Vertex];
            l_Vertices.push_back(l_Pos.x);
            l_Vertices.push_back(l_Pos.y);
            l_Vertices.push_back(l_Pos.z);
        }

        std::vector<uint32_t> l_Indices;
        // Extract indices
        l_Indices.reserve(l_Mesh->mNumFaces * 3);
        for (unsigned int it_Face = 0u; it_Face < l_Mesh->mNumFaces; ++it_Face)
        {
            const aiFace& l_Face = l_Mesh->mFaces[it_Face];
            for (unsigned int it_Index = 0u; it_Index < l_Face.mNumIndices; ++it_Index)
            {
                l_Indices.push_back(l_Face.mIndices[it_Index]);
            }
        }

        // Create and initialize the mesh with the extracted data
        return std::make_shared<Mesh>(l_Vertices, l_Indices);
    }

    void Mesh::SetTransform(const glm::mat4& transform)
    {
        m_Transform = transform;
    }

    const glm::mat4& Mesh::GetTransform() const
    {
        return m_Transform;
    }

    void Mesh::SetupMesh()
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t), m_Indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindVertexArray(0);
    }
}