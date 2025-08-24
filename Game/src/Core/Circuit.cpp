#include "Circuit.h"
#include "Car.h"
#include "Driver.h"

#include <fstream>
#include <nlohmann/json.hpp>

Circuit::Circuit(const std::string& name, double length, const std::vector<TrackSegment>& segments) : Name(name), Length(length), Segments(segments)
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

const std::vector<TrackSegment>& Circuit::GetSegments() const
{
    return Segments;
}

Circuit Circuit::FromJson(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open track file: " + filePath);
    }

    nlohmann::json data;
    file >> data;

    std::string name = data.at("name").get<std::string>();
    double length = data.at("length").get<double>();
    std::vector<TrackSegment> segments;

    for (const auto& seg : data.at("segments"))
    {
        segments.push_back({
            seg.at("length").get<double>(),
            seg.at("curvature").get<double>(),
            seg.at("elevation").get<double>(),
            seg.at("drs").get<bool>()
            });
    }

    return Circuit(name, length, segments);
}

double Circuit::ComputeBaselineLap(const Car& /*car*/, const Driver& /*driver*/) const
{
    double baseSpeed = 70.0;
    double totalTime = 0.0;

    for (const auto& segment : Segments)
    {
        double speed = baseSpeed;
        speed *= (1.0 - segment.Curvature);
        speed *= (1.0 - segment.Elevation * 0.01);
        if (segment.HasDRS)
        {
            speed *= 1.05;
        }

        totalTime += segment.Length / speed;
    }

    return totalTime;
}