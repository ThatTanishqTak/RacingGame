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

        m_RenderPass = std::make_unique<VulkanRenderPass>();
        if (!m_RenderPass->Initialize(context, m_SwapChain->GetImageFormat()))
        {
            TR_CORE_ERROR("Failed to create render pass");

            return false;
        }

        m_Pipeline = std::make_unique<VulkanPipeline>();
        if (!m_Pipeline->Initialize(context, m_RenderPass->GetRenderPass(), "assets/shaders/simple.vert.spv", "assets/shaders/simple.frag.spv"))
        {
            TR_CORE_ERROR("Failed to create graphics pipeline");
            
            return false;
        }

        TR_CORE_INFO("Renderer initialized successfully");

        return true;
    }

    void Renderer::Shutdown()
    {
        if (m_Pipeline)
        {
            m_Pipeline->Shutdown();
            m_Pipeline.reset();
        }

        if (m_RenderPass)
        {
            m_RenderPass->Shutdown();
            m_RenderPass.reset();
        }

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