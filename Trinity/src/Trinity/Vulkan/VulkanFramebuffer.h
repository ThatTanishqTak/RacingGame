#pragma once

#include "Trinity/Vulkan/VulkanContext.h"

#include <vector>

namespace Trinity
{
    class VulkanFramebuffer
    {
    public:
        VulkanFramebuffer() = default;
        ~VulkanFramebuffer() = default;

        bool Initialize(VulkanContext* context, VkRenderPass renderPass, const std::vector<VkImageView>& imageViews, VkExtent2D extent);
        void Shutdown();

        const std::vector<VkFramebuffer>& GetFramebuffers() const { return m_Framebuffers; }

    private:
        VulkanContext* m_Context = nullptr;
        std::vector<VkFramebuffer> m_Framebuffers;
    };
}