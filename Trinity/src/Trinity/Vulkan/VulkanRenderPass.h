#pragma once

#include "Trinity/Vulkan/VulkanContext.h"
#include <vulkan/vulkan.h>

namespace Trinity
{
    class VulkanRenderPass
    {
    public:
        VulkanRenderPass() = default;
        ~VulkanRenderPass() = default;

        bool Initialize(VulkanContext* context, VkFormat colorFormat);
        void Shutdown();

        VkRenderPass GetRenderPass() const { return m_RenderPass; }

    private:
        VulkanContext* m_Context = nullptr;
        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    };
}