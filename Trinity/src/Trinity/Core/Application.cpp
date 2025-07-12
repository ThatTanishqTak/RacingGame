#include "Application.h"

#include "Trinity/Vulkan/VulkanContext.h"

#include "Trinity/Core/Log.h"
#include "Trinity/Core/Window.h"

namespace Trinity
{
    Application::Application(const ApplicationSpecification& specification) : m_Specification(specification)
    {
        m_Window = std::make_unique<Window>(WindowSpecification{ m_Specification.Name, m_Specification.Width, m_Specification.Height });
        if (!m_Window->Initialize())
        {
            TR_CORE_ERROR("Failed to initialize window");
        }

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

        if (m_Window)
        {
            m_Window->Shutdown();
        }
    }
}