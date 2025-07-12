#include "trpch.h"

#include "Trinity/Renderer/Renderer.h"
#include "Trinity/Core/Log.h"

namespace Trinity
{
    bool Renderer::Initialize(VulkanContext* context)
    {
        m_Context = context;

        m_SwapChain = std::make_unique<VulkanSwapChain>();
        if (!m_SwapChain->Initialize(context))
        {
            TR_CORE_ERROR("Failed to initialize swap chain");
            return false;
        }

        TR_CORE_INFO("Renderer initialized successfully");

        return true;
    }

    void Renderer::Shutdown()
    {
        if (m_SwapChain)
        {
            m_SwapChain->Shutdown();
            m_SwapChain.reset();
        }

        TR_CORE_INFO("Renderer shutdown successfully");
    }

    void Renderer::BeginFrame()
    {

    }

    void Renderer::EndFrame()
    {

    }
}