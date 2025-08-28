#pragma once

#include <vector>
#include <utility>
#include <glm/glm.hpp>

class Track
{
public:
    void SetCenterline(const std::vector<glm::vec2>& points);
    void SetWidthProfile(const std::vector<float>& widths);

    glm::vec2 PositionAt(float positionAt) const;
    std::pair<glm::vec2, glm::vec2> Bounds() const { return { Min, Max }; }
    const std::vector<glm::vec2>& GetCenterline() const { return Centerline; }
    const std::vector<glm::vec2>& GetRacingLine() const { return RacingLine; }
    const std::vector<float>& GetArcLengthTable() const { return ArcLengthTable; }
    const std::vector<float>& GetCurvature() const { return Curvature; }
    float GetLength() const { return ArcLengthTable.empty() ? 0.0f : ArcLengthTable.back(); }

private:
    void BuildArcLengthTable();
    void ComputeRacingLine();
    void ComputeCurvature();
    void UpdateBounds();

private:
    std::vector<glm::vec2> Centerline;
    std::vector<glm::vec2> RacingLine;
    std::vector<float> Widths;
    std::vector<float> ArcLengthTable;
    std::vector<float> Curvature;
    glm::vec2 Min{ 0.0f };
    glm::vec2 Max{ 0.0f };
};