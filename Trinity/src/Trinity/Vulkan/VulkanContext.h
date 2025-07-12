#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace Trinity
{
    class VulkanContext
    {
    public:
        VulkanContext() = default;
        ~VulkanContext() = default;

        bool Initialize(GLFWwindow* window);
        void Shutdown();

        VkDevice GetDevice() const { return m_Device; }
        VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
        VkSurfaceKHR GetSurface() const { return m_Surface; }
        VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
        uint32_t GetGraphicsFamilyIndex() const { return m_GraphicsFamilyIndex; }

    private:
        bool CreateSurface(GLFWwindow* window);
        bool CreateInstance();
        bool SetupDebugMessenger();
        bool PickPhysicalDevice();
        bool HasGraphicsAndPresent(VkPhysicalDevice device) const;
        bool CreateLogicalDevice();
        bool CheckValidationLayerSupport();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    private:
        VkInstance m_Instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_Device = VK_NULL_HANDLE;
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
        VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
        uint32_t m_GraphicsFamilyIndex = UINT32_MAX;
    };
}