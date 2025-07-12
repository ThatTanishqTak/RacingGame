#pragma once

#include "Trinity/Vulkan/VulkanContext.h"

#include <string>
#include <vector>

namespace Trinity
{
    class VulkanPipeline
    {
    public:
        VulkanPipeline() = default;
        ~VulkanPipeline() = default;

        bool Initialize(VulkanContext* context, const std::string& vertPath, const std::string& fragPath);
        void Shutdown();

        VkPipeline GetPipeline() const { return m_Pipeline; }

    private:
        bool CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
        bool CreatePipelineLayout();
        bool CreateGraphicsPipeline(const std::string& vertPath, const std::string& fragPath);

        VulkanContext* m_Context = nullptr;
        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
        VkPipeline m_Pipeline = VK_NULL_HANDLE;
    };
}