#pragma once

#include <string>
#include <vector>

struct DriverInfo
{
    std::string Name;
    int Number;
};

struct RaceState
{
    std::string Date;
    std::string Time;
    std::string Weather;
    std::vector<DriverInfo> DriverInfo;
    std::vector<int> Positions;
};