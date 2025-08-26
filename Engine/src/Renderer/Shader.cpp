#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace Engine
{
    namespace Shaders
    {
        bool CheckCompileErrors(GLuint shader)
        {
            GLint l_Success = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &l_Success);
            if (l_Success == GL_FALSE)
            {
                GLint l_Length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &l_Length);
                
                std::string l_Message(l_Length, '\0');
                glGetShaderInfoLog(shader, l_Length, &l_Length, &l_Message[0]);

                std::cout << "Shader compilation failed: " << l_Message << std::endl;

                return false;
            }

            return true;
        }

        bool CheckLinkErrors(GLuint program)
        {
            GLint l_Success = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &l_Success);
            if (l_Success == GL_FALSE)
            {
                GLint l_Length = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &l_Length);
                
                std::string l_Message(l_Length, '\0');
                glGetProgramInfoLog(program, l_Length, &l_Length, &l_Message[0]);

                std::cout << "Program link failed: " << l_Message << std::endl;

                return false;
            }

            return true;
        }
    }

    Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
    {
        GLuint l_VertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* l_VertexSource = vertexSource.c_str();
        glShaderSource(l_VertexShader, 1, &l_VertexSource, nullptr);
        glCompileShader(l_VertexShader);
        Shaders::CheckCompileErrors(l_VertexShader);

        GLuint l_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* l_FragmentSource = fragmentSource.c_str();
        glShaderSource(l_FragmentShader, 1, &l_FragmentSource, nullptr);
        glCompileShader(l_FragmentShader);
        Shaders::CheckCompileErrors(l_FragmentShader);

        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, l_VertexShader);
        glAttachShader(m_RendererID, l_FragmentShader);
        glLinkProgram(m_RendererID);
        Shaders::CheckLinkErrors(m_RendererID);

        glDeleteShader(l_VertexShader);
        glDeleteShader(l_FragmentShader);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }

    void Shader::SetUniformMat2(const std::string& name, const glm::mat2& matrix) const
    {
        glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    void Shader::SetUniformMat3(const std::string& name, const glm::mat3& matrix) const
    {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) const
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::SetUniformVec2(const std::string& name, const glm::vec2& vector) const
    {
        glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(vector));
    }

    void Shader::SetUniformVec3(const std::string& name, const glm::vec3& vector) const
    {
        glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vector));
    }

    void Shader::SetUniformVec4(const std::string& name, const glm::vec4& vector) const
    {
        glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(vector));
    }

    void Shader::SetUniformFloat(const std::string& name, float value) const
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    int Shader::GetUniformLocation(const std::string& name) const
    {
        auto it = m_UniformLocationCache.find(name);
        if (it != m_UniformLocationCache.end())
        {
            return it->second;
        }

        GLint l_Location = glGetUniformLocation(m_RendererID, name.c_str());
        m_UniformLocationCache[name] = l_Location;

        return l_Location;
    }

    ShaderLibrary& ShaderLibrary::Get()
    {
        static ShaderLibrary s_Library;

        return s_Library;
    }

    std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
    {
        auto a_Shader = std::make_shared<Shader>(vertexSource, fragmentSource);
        m_Shaders[name] = a_Shader;

        return a_Shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name) const
    {
        auto it = m_Shaders.find(name);
        if (it != m_Shaders.end())
        {
            return it->second;
        }
        
        return nullptr;
    }
}