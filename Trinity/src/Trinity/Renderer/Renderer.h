#pragma once

#include "Trinity/Core/Core.h"

#include "Trinity/Vulkan/VulkanContext.h"
#include "Trinity/Vulkan/VulkanSwapchain.h"
#include "Trinity/Vulkan/VulkanRenderPass.h"
#include "Trinity/Vulkan/VulkanPipeline.h"
#include "Trinity/Vulkan/VulkanFramebuffer.h"
#include "Trinity/Vulkan/VulkanCommandBuffer.h"

namespace Trinity
{
    class Renderer
    {
    public:
        Renderer() = default;
        ~Renderer() = default;

        bool Initialize(VulkanContext* context);
        void Shutdown();

        void BeginFrame();
        void EndFrame();

    private:
        VulkanContext* m_Context = nullptr;

        std::unique_ptr<VulkanSwapChain> m_SwapChain;
        std::unique_ptr<VulkanRenderPass> m_RenderPass;
        std::unique_ptr<VulkanPipeline> m_Pipeline;
        std::unique_ptr<VulkanFramebuffer> m_Framebuffer;
        std::unique_ptr<VulkanCommandBuffer> m_CommandBuffers;

        uint32_t m_CurrentFrame = 0;
    };
}