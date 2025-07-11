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
        bool PickPhysicalDevice();
        bool CreateLogicalDevice();

        VkInstance m_Instance = VK_NULL_HANDLE;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_Device = VK_NULL_HANDLE;
        VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
    };
}