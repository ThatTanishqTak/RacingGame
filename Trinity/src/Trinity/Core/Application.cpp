#include "trpch.h"

#include "Application.h"

#include "Trinity/Vulkan/VulkanContext.h"

#include "Trinity/Core/Log.h"
#include "Trinity/Core/Window.h"
#include "Trinity/Renderer/Renderer.h"

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

        m_Renderer = std::make_unique<Renderer>();
        if (!m_Renderer->Initialize(m_VulkanContext.get()))
        {
            TR_CORE_ERROR("Failed to initialize renderer");
        }
    }

    Application::~Application()
    {
        if (m_Renderer)
        {
            m_Renderer->Shutdown();
        }


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