#include "trpch.h"

#include "Trinity/Renderer/Renderer.h"
#include "Trinity/Core/Log.h"

namespace Trinity
{
    bool Renderer::Initialize(VulkanContext* context)
    {
        m_Context = context;
        TR_CORE_INFO("Renderer initialized successfully");

        return true;
    }

    void Renderer::Shutdown()
    {
        TR_CORE_INFO("Renderer shutdown successfully");
    }

    void Renderer::BeginFrame()
    {

    }

    void Renderer::EndFrame()
    {

    }
}