#pragma once

#include "Trinity/Vulkan/VulkanContext.h"
#include <vector>

namespace Trinity
{
    class VulkanCommandBuffer
    {
    public:
        VulkanCommandBuffer() = default;
        ~VulkanCommandBuffer() = default;

        bool Initialize(VulkanContext* context, uint32_t count);
        void Shutdown();

        const std::vector<VkCommandBuffer>& GetCommandBuffers() const { return m_CommandBuffers; }

    private:
        bool CreateCommandPool();
        bool AllocateCommandBuffers(uint32_t count);

    private:
        VulkanContext* m_Context = nullptr;
        VkCommandPool m_CommandPool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> m_CommandBuffers{};
    };
}