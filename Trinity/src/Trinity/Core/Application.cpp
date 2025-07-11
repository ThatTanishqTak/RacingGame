#include "Application.h"

#include "Trinity/Vulkan/VulkanContext.h"
#include "Trinity/Core/Log.h"

namespace Trinity
{
    Application::Application(const ApplicationSpecification& specification) : m_Specification(specification)
    {
        m_VulkanContext = std::make_unique<VulkanContext>();
        if (!m_VulkanContext->Initialize())
        {
            TR_CORE_ERROR("Failed to initialize Vulkan context");
        }
    }

    Application::~Application()
    {
        if (m_VulkanContext)
        {
            m_VulkanContext->Shutdown();
        }
    }
}