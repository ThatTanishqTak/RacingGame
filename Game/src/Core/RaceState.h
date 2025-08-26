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

struct PitIn
{
    std::string DriverName;
};

struct PitOut
{
    std::string DriverName;
};

struct DNF
{
    std::string DriverName;
    std::string Reason;
};

struct Overtake
{
    std::string Overtaker;
    std::string Overtaken;
};