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
        bool CreateInstance();
        bool SetupDebugMessenger();
        bool PickPhysicalDevice();
        bool CreateLogicalDevice();
        bool CheckValidationLayerSupport();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

        VkInstance m_Instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_Device = VK_NULL_HANDLE;
        VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
    };
}