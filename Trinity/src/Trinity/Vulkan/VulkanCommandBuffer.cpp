#include "trpch.h"

#include "Trinity/Vulkan/VulkanCommandBuffer.h"
#include "Trinity/Core/Log.h"

namespace Trinity
{
    bool VulkanCommandBuffer::Initialize(VulkanContext* context, uint32_t count)
    {
        m_Context = context;

        if (!CreateCommandPool())
        {
            return false;
        }

        if (!AllocateCommandBuffers(count))
        {
            return false;
        }

        TR_CORE_TRACE("Command buffers created ({})", m_CommandBuffers.size());
        
        return true;
    }

    void VulkanCommandBuffer::Shutdown()
    {
        if (m_CommandPool != VK_NULL_HANDLE)
        {
            vkDestroyCommandPool(m_Context->GetDevice(), m_CommandPool, nullptr);
            m_CommandPool = VK_NULL_HANDLE;
            m_CommandBuffers.clear();

            TR_CORE_INFO("Command buffers destroyed");
        }
    }

    bool VulkanCommandBuffer::CreateCommandPool()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = m_Context->GetGraphicsFamilyIndex();

        if (vkCreateCommandPool(m_Context->GetDevice(), &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
        {
            TR_CORE_ERROR("Failed to create command pool");
            
            return false;
        }

        return true;
    }

    bool VulkanCommandBuffer::AllocateCommandBuffers(uint32_t count)
    {
        m_CommandBuffers.resize(count);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_CommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = count;

        if (vkAllocateCommandBuffers(m_Context->GetDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
        {
            TR_CORE_ERROR("Failed to allocate command buffers");
            
            return false;
        }

        return true;
    }
}