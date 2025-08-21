#pragma once

#include <memory>

namespace Engine
{
    class VertexBuffer
    {
    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

        static std::unique_ptr<VertexBuffer> Create(const void* data, unsigned int size);

    private:
        unsigned int m_BufferID = 0;
    };
}