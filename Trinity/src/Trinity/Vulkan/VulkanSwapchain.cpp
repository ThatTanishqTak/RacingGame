#include "trpch.h"

#include "Trinity/Vulkan/VulkanSwapchain.h"
#include "Trinity/Core/Log.h"

namespace Trinity
{
    bool VulkanSwapChain::Initialize(VulkanContext* context)
    {
        m_Context = context;

        if (!CreateSwapChain())
        {
            return false;
        }

        if (!CreateImageViews())
        {
            return false;
        }

        VkDevice device = m_Context->GetDevice();

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        size_t imageCount = m_Images.size();
        m_ImageAvailableSemaphores.resize(imageCount);
        m_RenderFinishedSemaphores.resize(imageCount);
        m_InFlightFences.resize(imageCount);
        m_ImagesInFlight.resize(imageCount, VK_NULL_HANDLE);

        for (size_t i = 0; i < imageCount; ++i)
        {
            if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(device, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
            {
                TR_CORE_ERROR("Failed to create synchronization objects for frame {}", i);

                return false;
            }
        }

        TR_CORE_INFO("Swap chain initialized successfully");

        return true;
    }

    void VulkanSwapChain::Shutdown()
    {
        VkDevice device = m_Context->GetDevice();

        for (VkImageView imageView : m_ImageViews)
        {
            vkDestroyImageView(device, imageView, nullptr);
        }
        m_ImageViews.clear();

        if (m_SwapChain != VK_NULL_HANDLE)
        {
            vkDestroySwapchainKHR(device, m_SwapChain, nullptr);
            m_SwapChain = VK_NULL_HANDLE;
        }

        for (size_t i = 0; i < m_ImageAvailableSemaphores.size(); ++i)
        {
            if (m_ImageAvailableSemaphores[i] != VK_NULL_HANDLE)
            {
                vkDestroySemaphore(device, m_ImageAvailableSemaphores[i], nullptr);
            }
            
            if (m_RenderFinishedSemaphores[i] != VK_NULL_HANDLE)
            {
                vkDestroySemaphore(device, m_RenderFinishedSemaphores[i], nullptr);
            }

            if (m_InFlightFences[i] != VK_NULL_HANDLE)
            {
                vkDestroyFence(device, m_InFlightFences[i], nullptr);
            }
        }

        m_ImageAvailableSemaphores.clear();
        m_RenderFinishedSemaphores.clear();
        m_InFlightFences.clear();

        TR_CORE_INFO("Swap chain shutdown successfully");
    }

    bool VulkanSwapChain::CreateSwapChain()
    {
        VkPhysicalDevice physicalDevice = m_Context->GetPhysicalDevice();
        VkSurfaceKHR surface = m_Context->GetSurface();

        VkSurfaceCapabilitiesKHR capabilities{};
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
        std::vector<VkSurfaceFormatKHR> formats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());

        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

        VkSurfaceFormatKHR surfaceFormat = formats[0];
        for (const auto& availableFormat : formats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                surfaceFormat = availableFormat;

                break;
            }
        }

        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        for (const auto& availablePresentMode : presentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                presentMode = availablePresentMode;

                break;
            }
        }

        VkExtent2D extent = capabilities.currentExtent;
        if (extent.width == UINT32_MAX)
        {
            extent.width = 1920;
            extent.height = 1080;
        }

        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
        {
            imageCount = capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;

        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_Context->GetDevice(), &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
        {
            TR_CORE_ERROR("Failed to create swap chain");

            return false;
        }

        vkGetSwapchainImagesKHR(m_Context->GetDevice(), m_SwapChain, &imageCount, nullptr);
        m_Images.resize(imageCount);
        vkGetSwapchainImagesKHR(m_Context->GetDevice(), m_SwapChain, &imageCount, m_Images.data());

        m_ImageFormat = surfaceFormat.format;
        m_Extent = extent;

        TR_CORE_TRACE("Swap chain created");

        return true;
    }

    bool VulkanSwapChain::CreateImageViews()
    {
        m_ImageViews.resize(m_Images.size());

        for (size_t i = 0; i < m_Images.size(); ++i)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_Images[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_ImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_Context->GetDevice(), &createInfo, nullptr, &m_ImageViews[i]) != VK_SUCCESS)
            {
                TR_CORE_ERROR("Failed to create swap chain image view");

                return false;
            }
        }

        TR_CORE_TRACE("Swap chain image views created ({})", m_ImageViews.size());

        return true;
    }

    bool VulkanSwapChain::AcquireNextImage(uint32_t* imageIndex, uint32_t frameIndex)
    {
        VkDevice device = m_Context->GetDevice();

        vkWaitForFences(device, 1, &m_InFlightFences[frameIndex], VK_TRUE, UINT64_MAX);
        vkResetFences(device, 1, &m_InFlightFences[frameIndex]);

        VkResult result = vkAcquireNextImageKHR(device, m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[frameIndex], VK_NULL_HANDLE, imageIndex);
        if (result != VK_SUCCESS)
        {
            TR_CORE_ERROR("Failed to acquire swap chain image");

            return false;
        }

        if (m_ImagesInFlight[*imageIndex] != VK_NULL_HANDLE)
        {
            vkWaitForFences(device, 1, &m_ImagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
        }

        m_ImagesInFlight[*imageIndex] = m_InFlightFences[frameIndex];

        return true;
    }

    bool VulkanSwapChain::PresentImage(uint32_t imageIndex, uint32_t frameIndex)
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &m_RenderFinishedSemaphores[frameIndex];
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &m_SwapChain;
        presentInfo.pImageIndices = &imageIndex;

        VkResult result = vkQueuePresentKHR(m_Context->GetGraphicsQueue(), &presentInfo);
        if (result != VK_SUCCESS)
        {
            TR_CORE_ERROR("Failed to present swap chain image");

            return false;
        }
        return true;
    }
}