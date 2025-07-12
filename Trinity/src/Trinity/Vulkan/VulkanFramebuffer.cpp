#include "trpch.h"

#include "Trinity/Vulkan/VulkanFramebuffer.h"
#include "Trinity/Core/Log.h"

namespace Trinity
{
    bool VulkanFramebuffer::Initialize(VulkanContext* context, VkRenderPass renderPass, const std::vector<VkImageView>& imageViews, VkExtent2D extent)
    {
        m_Context = context;
        m_Framebuffers.resize(imageViews.size());

        for (size_t i = 0; i < imageViews.size(); ++i)
        {
            VkImageView attachments[] = { imageViews[i] };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = extent.width;
            framebufferInfo.height = extent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(m_Context->GetDevice(), &framebufferInfo, nullptr, &m_Framebuffers[i]) != VK_SUCCESS)
            {
                TR_CORE_ERROR("Failed to create framebuffer");
                return false;
            }
        }

        TR_CORE_TRACE("Framebuffers created ({})", m_Framebuffers.size());

        return true;
    }

    void VulkanFramebuffer::Shutdown()
    {
        for (VkFramebuffer framebuffer : m_Framebuffers)
        {
            vkDestroyFramebuffer(m_Context->GetDevice(), framebuffer, nullptr);
        }
        m_Framebuffers.clear();

        TR_CORE_INFO("Framebuffers destroyed");
    }
}