#pragma once

#include <memory>

namespace Engine
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        static std::unique_ptr<VertexArray> Create();

    private:
        unsigned int m_ArrayID = 0;
    };
}