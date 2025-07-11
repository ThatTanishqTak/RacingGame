#pragma once

#include <vulkan/vulkan.h>

namespace Trinity
{
    class VulkanContext
    {
    public:
        VulkanContext() = default;
        ~VulkanContext();

        bool Initialize();
        void Shutdown();

    private:
        VkInstance m_Instance = VK_NULL_HANDLE;
    };
}