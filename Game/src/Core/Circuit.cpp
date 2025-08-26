#include "Circuit.h"

#include <fstream>
#include <limits>
#include <algorithm>
#include <glm/glm.hpp>

Circuit::Circuit(const std::string& name, double length) : Name(name), Length(length)
{

}

const std::string& Circuit::GetName() const
{
    return Name;
}

double Circuit::GetLength() const
{
    return Length;
}

void Circuit::SetLength(double length)
{
    Length = length;
}

bool Circuit::LoadLayout(const std::string& filePath)
{
    std::ifstream File(filePath);
    if (!File.is_open())
    {
        return false;
    }

    Layout.clear();
    std::string Line;
    while (std::getline(File, Line))
    {
        for (char Tile : Line)
        {
            switch (Tile)
            {
            case '-':
            case '|':
            case '\\':
            case '/':
            case '*':
            case '&':
            case '[':
            case ']':
            case ' ':
                break;
            default:
                return false;
            }
        }

        Layout.push_back(Line);
    }

    Centerline.clear();
    Centerline.emplace_back(0.0f, 0.0f);
    Centerline.emplace_back(static_cast<float>(Length), 0.0f);
    HalfWidth = 5.0f;
    BuildArcLengthTable();

    return true;
}

const std::vector<std::string>& Circuit::GetLayout() const
{
    return Layout;
}

bool Circuit::LoadCenterline(const std::string& filePath)
{
    std::ifstream File(filePath);
    if (!File.is_open())
    {
        return false;
    }

    Centerline.clear();
    float x = 0.0f, y = 0.0f;
    while (File >> x >> y)
    {
        Centerline.emplace_back(x, y);
    }

    if (Centerline.size() < 2)
    {
        return false;
    }

    BuildArcLengthTable();
    return true;
}

const std::vector<glm::vec2>& Circuit::GetCenterline() const
{
    return Centerline;
}

float Circuit::GetHalfWidth() const
{
    return HalfWidth;
}

glm::vec2 Circuit::ToWorld(float s, float d) const
{
    if (Centerline.size() < 2 || ArcLengthTable.empty())
    {
        return {};
    }

    if (Length > 0.0)
    {
        s = std::fmod(s, static_cast<float>(Length));
        if (s < 0.0f)
        {
            s += static_cast<float>(Length);
        }
    }

    for (size_t i = 1; i < ArcLengthTable.size(); ++i)
    {
        if (s <= ArcLengthTable[i])
        {
            float segmentLength = ArcLengthTable[i] - ArcLengthTable[i - 1];
            float t = segmentLength > 0.0f ? (s - ArcLengthTable[i - 1]) / segmentLength : 0.0f;
            glm::vec2 p0 = Centerline[i - 1];
            glm::vec2 p1 = Centerline[i];
            glm::vec2 dir = p1 - p0;
            if (segmentLength > 0.0f)
            {
                dir /= segmentLength;
            }
            glm::vec2 pos = p0 + dir * (s - ArcLengthTable[i - 1]);
            glm::vec2 normal(-dir.y, dir.x);
            return pos + normal * d;
        }
    }

    return Centerline.back();
}

glm::vec2 Circuit::ToSD(const glm::vec2& world) const
{
    if (Centerline.size() < 2 || ArcLengthTable.empty())
    {
        return {};
    }

    float bestDist = std::numeric_limits<float>::max();
    float bestS = 0.0f;
    float bestD = 0.0f;

    for (size_t i = 1; i < Centerline.size(); ++i)
    {
        glm::vec2 p0 = Centerline[i - 1];
        glm::vec2 p1 = Centerline[i];
        glm::vec2 seg = p1 - p0;
        float segLen = glm::length(seg);
        if (segLen == 0.0f)
        {
            continue;
        }

        glm::vec2 dir = seg / segLen;
        float t = glm::dot(world - p0, dir);
        t = std::clamp(t, 0.0f, segLen);
        glm::vec2 closest = p0 + dir * t;
        float dist = glm::length(world - closest);
        if (dist < bestDist)
        {
            bestDist = dist;
            bestS = ArcLengthTable[i - 1] + t;
            glm::vec2 normal(-dir.y, dir.x);
            bestD = glm::dot(world - closest, normal);
        }
    }

    return { bestS, bestD };
}

void Circuit::BuildArcLengthTable()
{
    ArcLengthTable.clear();
    ArcLengthTable.push_back(0.0f);
    float total = 0.0f;
    for (size_t i = 1; i < Centerline.size(); ++i)
    {
        total += glm::length(Centerline[i] - Centerline[i - 1]);
        ArcLengthTable.push_back(total);
    }
    Length = total;
}