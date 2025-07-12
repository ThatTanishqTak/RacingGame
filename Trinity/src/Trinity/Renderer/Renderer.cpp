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

        if (m_Framebuffer)
        {
            m_Framebuffer->Shutdown();
            m_Framebuffer.reset();
        }

        m_Pipeline = std::make_unique<VulkanPipeline>();
        if (!m_Pipeline->Initialize(context, m_RenderPass->GetRenderPass(), "assets/shaders/simple.vert.spv", "assets/shaders/simple.frag.spv"))
        {
            TR_CORE_ERROR("Failed to create graphics pipeline");

            return false;
        }

        m_Framebuffer = std::make_unique<VulkanFramebuffer>();
        if (!m_Framebuffer->Initialize(context, m_RenderPass->GetRenderPass(), m_SwapChain->GetImageViews(), m_SwapChain->GetExtent()))
        {
            TR_CORE_ERROR("Failed to create framebuffers");

            return false;
        }

        m_CommandBuffers = std::make_unique<VulkanCommandBuffer>();
        if (!m_CommandBuffers->Initialize(context, static_cast<uint32_t>(m_Framebuffer->GetFramebuffers().size())))
        {
            TR_CORE_ERROR("Failed to create command buffers");

            return false;
        }

        const std::array<Vertex, 3> vertices{ { { { 0.0f, -0.5f, 0.0f } }, { { 0.5f,  0.5f, 0.0f } }, { { -0.5f, 0.5f, 0.0f } } } };

        m_VertexBuffer = std::make_unique<VulkanBuffer>();
        if (!m_VertexBuffer->Initialize(context, sizeof(vertices), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertices.data()))
        {
            TR_CORE_ERROR("Failed to create vertex buffer");
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

        if (m_CommandBuffers)
        {
            m_CommandBuffers->Shutdown();
            m_CommandBuffers.reset();
        }

        if (m_VertexBuffer)
        {
            m_VertexBuffer->Shutdown();
            m_VertexBuffer.reset();
        }

        TR_CORE_INFO("Renderer shutdown successfully");
    }

    void Renderer::BeginFrame()
    {
        const auto& buffers = m_CommandBuffers->GetCommandBuffers();
        VkCommandBuffer commandBuffer = buffers[m_CurrentFrame];

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_RenderPass->GetRenderPass();
        renderPassInfo.framebuffer = m_Framebuffer->GetFramebuffers()[m_CurrentFrame];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = m_SwapChain->GetExtent();
        VkClearValue clearColor{};
        clearColor.color = { {0.0f, 0.0f, 0.0f, 1.0f} };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->GetPipeline());

        VkBuffer vertexBuffers[] = { m_VertexBuffer->GetBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    }

    void Renderer::EndFrame()
    {
        const auto& buffers = m_CommandBuffers->GetCommandBuffers();
        VkCommandBuffer commandBuffer = buffers[m_CurrentFrame];

        vkCmdEndRenderPass(commandBuffer);
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        vkQueueSubmit(m_Context->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_Context->GetGraphicsQueue());

        m_CurrentFrame = (m_CurrentFrame + 1) % static_cast<uint32_t>(buffers.size());
    }
}