#pragma once

#include <string>
#include <vector>

class Car;
class Driver;

struct TrackSegment
{
    double Length;
    double Curvature;
    double Elevation;
    bool HasDRS;
};

class Circuit
{
private:
    std::string Name;
    double Length;
    std::vector<TrackSegment> Segments;

public:
    Circuit(const std::string& name, double length, const std::vector<TrackSegment>& segments = {});

    const std::string& GetName() const;
    double GetLength() const;
    void SetLength(double length);

    const std::vector<TrackSegment>& GetSegments() const;

    static Circuit FromJson(const std::string& filePath);
    double ComputeBaselineLap(const Car& car, const Driver& driver) const;
};