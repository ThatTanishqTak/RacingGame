#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

class Circuit
{
public:
    Circuit(const std::string& name, double length);

    const std::string& GetName() const;
    double GetLength() const;
    void SetLength(double length);

    bool LoadLayout(const std::string& filePath);
    const std::vector<std::string>& GetLayout() const;

    bool LoadCenterline(const std::string& filePath);
    const std::vector<glm::vec2>& GetCenterline() const;
    float GetHalfWidth() const;
    glm::vec2 ToWorld(float s, float d) const;
    glm::vec2 ToSD(const glm::vec2& world) const;

private:
    std::string Name;
    double Length;
    std::vector<std::string> Layout;
    std::vector<glm::vec2> Centerline;
    std::vector<float> ArcLengthTable;
    float HalfWidth{ 0.0f };

    void BuildArcLengthTable();
};