#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace Engine
{
    class Shader
    {
    public:
        Shader(const std::string& vertexSource, const std::string& fragmentSource);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void SetUniformMat2(const std::string& name, const glm::mat2& matrix) const;
        void SetUniformMat3(const std::string& name, const glm::mat3& matrix) const;
        void SetUniformMat4(const std::string& name, const glm::mat4& matrix) const;
        void SetUniformVec2(const std::string& name, const glm::vec2& matrix) const;
        void SetUniformVec3(const std::string& name, const glm::vec3& vector) const;
        void SetUniformVec4(const std::string& name, const glm::vec4& vector) const;
        void SetUniformFloat(const std::string& name, float value) const;

    private:
        int GetUniformLocation(const std::string& name) const;

    private:
        uint32_t m_RendererID;
        mutable std::unordered_map<std::string, int> m_UniformLocationCache;
    };

    class ShaderLibrary
    {
    public:
        static ShaderLibrary& Get();

        std::shared_ptr<Shader> Load(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
        std::shared_ptr<Shader> Get(const std::string& name) const;

    private:
        std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
    };
}