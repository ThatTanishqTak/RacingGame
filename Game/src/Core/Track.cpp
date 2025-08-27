#include "Track.h"

#include <algorithm>
#include <cmath>

void Track::SetCenterline(const std::vector<glm::vec2>& points)
{
    Centerline = points;

    BuildArcLengthTable();
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
    for (size_t i = 1; i < Centerline.size(); ++i)
    {
        l_Total += glm::length(Centerline[i] - Centerline[i - 1]);
        ArcLengthTable.push_back(l_Total);
    }
}

glm::vec2 Track::PositionAt(float positionAt) const
{
    if (Centerline.size() < 2 || ArcLengthTable.empty())
    {
        return {};
    }

    float l_Total = ArcLengthTable.back();
    if (l_Total <= 0.0f)
    {
        return Centerline.front();
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
        return Centerline.front();
    }

    float l_SegmentLength = ArcLengthTable[l_Index] - ArcLengthTable[l_Index - 1];
    float l_Temp = l_SegmentLength > 0.0f ? (positionAt - ArcLengthTable[l_Index - 1]) / l_SegmentLength : 0.0f;

    return glm::mix(Centerline[l_Index - 1], Centerline[l_Index], l_Temp);
}

void Track::UpdateBounds()
{
    if (Centerline.empty())
    {
        Min = Max = { 0.0f, 0.0f };

        return;
    }

    Min = Max = Centerline.front();
    for (size_t i = 0; i < Centerline.size(); ++i)
    {
        glm::vec2 P = Centerline[i];
        float W = (i < Widths.size()) ? Widths[i] * 0.5f : 0.0f;

        glm::vec2 Dir{ 0.0f };
        if (i + 1 < Centerline.size())
        {
            Dir = glm::normalize(Centerline[i + 1] - P);
        }
        
        else if (i > 0)
        {
            Dir = glm::normalize(P - Centerline[i - 1]);
        }

        glm::vec2 Normal(-Dir.y, Dir.x);
        glm::vec2 Left = P + Normal * W;
        glm::vec2 Right = P - Normal * W;

        Min = glm::min(Min, glm::min(Left, Right));
        Max = glm::max(Max, glm::max(Left, Right));
    }
}