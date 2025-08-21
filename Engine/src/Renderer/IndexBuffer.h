#pragma once

#include <memory>

namespace Engine
{
    class IndexBuffer
    {
    public:
        IndexBuffer(const unsigned int* indices, unsigned int count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        unsigned int GetCount() const { return m_Count; }

        static std::unique_ptr<IndexBuffer> Create(const unsigned int* indices, unsigned int count);

    private:
        unsigned int m_BufferID = 0;
        unsigned int m_Count = 0;
    };
}