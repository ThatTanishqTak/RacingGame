#include "TrackRenderer.h"

#include <glad/glad.h>
#include <fstream>
#include <regex>
#include <sstream>
#include <cmath>

namespace Engine
{
    bool TrackRenderer::LoadFromSVG(const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (!file)
        {
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();

        std::regex polyline(R"(<polyline[^>]*points=\"([^\"]+)\"[^>]*/>)");
        std::smatch match;
        if (!std::regex_search(content, match, polyline))
        {
            return false;
        }

        std::string points = match[1];
        std::stringstream ss(points);
        std::string pair;
        m_Points.clear();
        while (ss >> pair)
        {
            auto comma = pair.find(',');
            if (comma == std::string::npos)
                continue;
            float x = std::stof(pair.substr(0, comma));
            float y = std::stof(pair.substr(comma + 1));
            m_Points.emplace_back(x, y);
        }

        return !m_Points.empty();
    }

    glm::vec2 TrackRenderer::WorldToScreen(const glm::vec2& world) const
    {
        glm::vec2 screen = (world + m_Pan) * m_Zoom;
        screen.x = std::round(screen.x);
        screen.y = std::round(screen.y);
        return screen;
    }

    void TrackRenderer::Render() const
    {
        if (m_Points.empty())
        {
            return;
        }

        glBegin(GL_LINE_STRIP);
        for (const auto& p : m_Points)
        {
            glm::vec2 s = WorldToScreen(p);
            glVertex2f(s.x, s.y);
        }
        glEnd();
    }
}