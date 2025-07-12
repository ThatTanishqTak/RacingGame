#pragma once

#include "Trinity/Vulkan/VulkanContext.h"

#include <string>
#include <vector>
#include <array>
#include <glm/glm.hpp>

namespace Trinity
{
    struct Vertex
    {
        glm::vec3 Position;

        static VkVertexInputBindingDescription GetBindingDescription()
        {
            VkVertexInputBindingDescription binding{};
            binding.binding = 0;
            binding.stride = sizeof(Vertex);
            binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return binding;
        }

        static std::array<VkVertexInputAttributeDescription, 1> GetAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 1> attributes{};
            attributes[0].binding = 0;
            attributes[0].location = 0;
            attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributes[0].offset = offsetof(Vertex, Position);

            return attributes;
        }
    };


    class VulkanPipeline
    {
    public:
        VulkanPipeline() = default;
        ~VulkanPipeline() = default;

        bool Initialize(VulkanContext* context, VkRenderPass renderPass, const std::string& vertPath, const std::string& fragPath);
        void Shutdown();

        VkPipeline GetPipeline() const { return m_Pipeline; }

    private:
        bool CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
        bool CreatePipelineLayout();
        bool CreateGraphicsPipeline(const std::string& vertPath, const std::string& fragPath);

    private:
        VulkanContext* m_Context = nullptr;
        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
        VkPipeline m_Pipeline = VK_NULL_HANDLE;
    };
}