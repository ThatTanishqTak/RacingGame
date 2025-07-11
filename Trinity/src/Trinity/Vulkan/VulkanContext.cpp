#include "trpch.h"

#include "Trinity/Vulkan/VulkanContext.h"
#include "Trinity/Core/Log.h"

namespace Trinity
{
    VulkanContext::~VulkanContext()
    {
        Shutdown();
    }

    bool VulkanContext::Initialize()
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Trinity";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Trinity";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
        {
            TR_CORE_ERROR("Failed to create Vulkan instance");
        
            return false;
        }

        TR_CORE_INFO("Vulkan initialized successfully");
        
        return true;
    }

    void VulkanContext::Shutdown()
    {
        if (m_Instance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(m_Instance, nullptr);
            m_Instance = VK_NULL_HANDLE;
        }
    }
}