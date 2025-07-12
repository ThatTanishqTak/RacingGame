#pragma once

#include "Trinity/Core/Core.h"
#include "Trinity/Vulkan/VulkanContext.h"

namespace Trinity
{
    class Renderer
    {
    public:
        Renderer() = default;
        ~Renderer() = default;

        bool Initialize(VulkanContext* context);
        void Shutdown();

        void BeginFrame();
        void EndFrame();

    private:
        VulkanContext* m_Context = nullptr;
    };
}