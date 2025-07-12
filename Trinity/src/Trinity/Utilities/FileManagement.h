#pragma once

#include "Trinity/Core/Log.h"

#include <fstream>

namespace Trinity
{
    namespace Utilities
    {
        static std::vector<char> ReadFile(const std::string& filepath)
        {
            std::ifstream file(filepath, std::ios::ate | std::ios::binary);
            if (!file.is_open())
            {
                TR_CORE_ERROR("Failed to open shader file: {}", filepath);

                return {};
            }

            size_t fileSize = static_cast<size_t>(file.tellg());
            std::vector<char> buffer(fileSize);
            file.seekg(0);
            file.read(buffer.data(), fileSize);

            return buffer;
        }
    }
}