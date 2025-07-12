#pragma once

#include "Trinity/Vulkan/VulkanContext.h"
#include <vector>

namespace Trinity
{
    class VulkanSwapChain
    {
    public:
        VulkanSwapChain() = default;
        ~VulkanSwapChain() = default;

        bool Initialize(VulkanContext* context);
        void Shutdown();

        bool AcquireNextImage(uint32_t* imageIndex);
        bool PresentImage(uint32_t imageIndex);

        VkSemaphore GetImageAvailableSemaphore() const { return m_ImageAvailableSemaphore; }
        VkSemaphore GetRenderFinishedSemaphore() const { return m_RenderFinishedSemaphore; }

        VkSwapchainKHR GetSwapChain() const { return m_SwapChain; }
        VkFormat GetImageFormat() const { return m_ImageFormat; }
        VkExtent2D GetExtent() const { return m_Extent; }
        const std::vector<VkImageView>& GetImageViews() const { return m_ImageViews; }

    private:
        bool CreateSwapChain();
        bool CreateImageViews();

    private:
        VulkanContext* m_Context = nullptr;
        VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
        VkFormat m_ImageFormat = VK_FORMAT_UNDEFINED;
        VkExtent2D m_Extent{};
        std::vector<VkImage> m_Images;
        std::vector<VkImageView> m_ImageViews;

        VkSemaphore m_ImageAvailableSemaphore = VK_NULL_HANDLE;
        VkSemaphore m_RenderFinishedSemaphore = VK_NULL_HANDLE;
    };
}