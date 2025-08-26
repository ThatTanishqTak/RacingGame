#include "VertexBuffer.h"

#include <glad/glad.h>

namespace Engine
{
    VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    {
        glGenBuffers(1, &m_BufferID);
        
        Bind();

        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_BufferID);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
    }

    void VertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    std::unique_ptr<VertexBuffer> VertexBuffer::Create(const void* data, unsigned int size)
    {
        return std::make_unique<VertexBuffer>(data, size);
    }
}