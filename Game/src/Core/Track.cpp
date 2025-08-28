#include "Track.h"

#include <algorithm>
#include <cmath>

void Track::SetCenterline(const std::vector<glm::vec2>& points)
{
    Centerline = points;

    ComputeRacingLine();
    BuildArcLengthTable();
    ComputeCurvature();
    UpdateBounds();
}

void Track::SetWidthProfile(const std::vector<float>& widths)
{
    Widths = widths;
    if (Widths.size() < Centerline.size())
    {
        Widths.resize(Centerline.size(), Widths.empty() ? 0.0f : Widths.back());
    }
    UpdateBounds();
}

void Track::BuildArcLengthTable()
{
    ArcLengthTable.clear();
    ArcLengthTable.push_back(0.0f);
    float l_Total = 0.0f;
    for (size_t i = 1; i < RacingLine.size(); ++i)
    {
        l_Total += glm::length(RacingLine[i] - RacingLine[i - 1]);
        ArcLengthTable.push_back(l_Total);
    }
}

glm::vec2 Track::PositionAt(float positionAt) const
{
    if (RacingLine.size() < 2 || ArcLengthTable.empty())
    {
        return {};
    }

    float l_Total = ArcLengthTable.back();
    if (l_Total <= 0.0f)
    {
        return RacingLine.front();
    }

    positionAt = std::fmod(positionAt, l_Total);
    if (positionAt < 0.0f)
    {
        positionAt += l_Total;
    }

    auto it = std::upper_bound(ArcLengthTable.begin(), ArcLengthTable.end(), positionAt);
    size_t l_Index = std::distance(ArcLengthTable.begin(), it);
    if (l_Index == 0)
    {
        return RacingLine.front();
    }

    float l_SegmentLength = ArcLengthTable[l_Index] - ArcLengthTable[l_Index - 1];
    float l_Temp = l_SegmentLength > 0.0f ? (positionAt - ArcLengthTable[l_Index - 1]) / l_SegmentLength : 0.0f;

    return glm::mix(RacingLine[l_Index - 1], RacingLine[l_Index], l_Temp);
}

void Track::ComputeRacingLine()
{
    RacingLine = Centerline;
}

void Track::ComputeCurvature()
{
    Curvature.assign(RacingLine.size(), 0.0f);
    for (size_t i = 1; i + 1 < RacingLine.size(); ++i)
    {
        glm::vec2 p0 = RacingLine[i - 1];
        glm::vec2 p1 = RacingLine[i];
        glm::vec2 p2 = RacingLine[i + 1];
        glm::vec2 v1 = p1 - p0;
        glm::vec2 v2 = p2 - p1;
        float a = glm::length(v1);
        float b = glm::length(v2);
        if (a > 0.0f && b > 0.0f)
        {
            float cross = v1.x * v2.y - v1.y * v2.x;
            float dot = glm::dot(v1, v2);
            float angle = std::atan2(cross, dot);
            float avgLen = 0.5f * (a + b);
            if (avgLen > 0.0f)
            {
                Curvature[i] = angle / avgLen;
            }
        }
    }
}

void Track::UpdateBounds()
{
    if (RacingLine.empty())
    {
        Min = Max = { 0.0f, 0.0f };

        return;
    }

    Min = Max = RacingLine.front();
    for (size_t i = 0; i < RacingLine.size(); ++i)
    {
        glm::vec2 P = RacingLine[i];
        float W = (i < Widths.size()) ? Widths[i] * 0.5f : 0.0f;

        glm::vec2 Dir{ 0.0f };
        if (i + 1 < RacingLine.size())
        {
            Dir = glm::normalize(RacingLine[i + 1] - P);
        }

        else if (i > 0)
        {
            Dir = glm::normalize(P - RacingLine[i - 1]);
        }

        glm::vec2 Normal(-Dir.y, Dir.x);
        glm::vec2 Left = P + Normal * W;
        glm::vec2 Right = P - Normal * W;

        Min = glm::min(Min, glm::min(Left, Right));
        Max = glm::max(Max, glm::max(Left, Right));
    }
}