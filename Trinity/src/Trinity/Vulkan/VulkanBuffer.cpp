#include "trpch.h"

#include "Trinity/Vulkan/VulkanBuffer.h"
#include "Trinity/Core/Log.h"

namespace Trinity
{
    bool VulkanBuffer::Initialize(VulkanContext* context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, const void* data)
    {
        m_Context = context;

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_Context->GetDevice(), &bufferInfo, nullptr, &m_Buffer) != VK_SUCCESS)
        {
            TR_CORE_ERROR("Failed to create buffer");
            
            return false;
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_Context->GetDevice(), m_Buffer, &memRequirements);

        uint32_t memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);
        if (memoryTypeIndex == UINT32_MAX)
        {
            return false;
        }

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = memoryTypeIndex;

        if (vkAllocateMemory(m_Context->GetDevice(), &allocInfo, nullptr, &m_BufferMemory) != VK_SUCCESS)
        {
            TR_CORE_ERROR("Failed to allocate buffer memory");
            
            return false;
        }

        if (vkBindBufferMemory(m_Context->GetDevice(), m_Buffer, m_BufferMemory, 0) != VK_SUCCESS)
        {
            TR_CORE_ERROR("Failed to bind buffer memory");

            return false;
        }

        if (data)
        {
            void* mapped = nullptr;
            vkMapMemory(m_Context->GetDevice(), m_BufferMemory, 0, size, 0, &mapped);
            std::memcpy(mapped, data, static_cast<size_t>(size));
            vkUnmapMemory(m_Context->GetDevice(), m_BufferMemory);
        }

        return true;
    }

    void VulkanBuffer::Shutdown()
    {
        if (m_Buffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(m_Context->GetDevice(), m_Buffer, nullptr);
            m_Buffer = VK_NULL_HANDLE;
        }

        if (m_BufferMemory != VK_NULL_HANDLE)
        {
            vkFreeMemory(m_Context->GetDevice(), m_BufferMemory, nullptr);
            m_BufferMemory = VK_NULL_HANDLE;
        }

        m_Context = nullptr;

        TR_CORE_INFO("Buffer destroyed");
    }

    uint32_t VulkanBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_Context->GetPhysicalDevice(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        TR_CORE_CRITICAL("Failed to find suitable memory type");

        return UINT32_MAX;
    }
}