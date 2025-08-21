#include "VertexArray.h"

#include <glad/glad.h>

namespace Engine
{
    VertexArray::VertexArray() { glGenVertexArrays(1, &m_ArrayID); }

    VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_ArrayID); }

    void VertexArray::Bind() const { glBindVertexArray(m_ArrayID); }

    void VertexArray::Unbind() const { glBindVertexArray(0); }

    std::unique_ptr<VertexArray> VertexArray::Create() { return std::make_unique<VertexArray>(); }
}