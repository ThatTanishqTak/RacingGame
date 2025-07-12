#pragma once

#include "Trinity/Vulkan/VulkanContext.h"

namespace Trinity
{
    class VulkanBuffer
    {
    public:
        VulkanBuffer() = default;
        ~VulkanBuffer() = default;

        bool Initialize(VulkanContext* context, VkDeviceSize size, VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties, const void* data = nullptr);
        void Shutdown();

        VkBuffer GetBuffer() const { return m_Buffer; }

    private:
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    private:
        VulkanContext* m_Context = nullptr;
        VkBuffer m_Buffer = VK_NULL_HANDLE;
        VkDeviceMemory m_BufferMemory = VK_NULL_HANDLE;
    };
}