#include "IndexBuffer.h"

#include <glad/glad.h>

namespace Engine
{
    IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count)
        : m_Count(count)
    {
        glGenBuffers(1, &m_BufferID);
        
        Bind();

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_BufferID); }

    void IndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID); }

    void IndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    std::unique_ptr<IndexBuffer> IndexBuffer::Create(const unsigned int* indices, unsigned int count) { return std::make_unique<IndexBuffer>(indices, count); }
}