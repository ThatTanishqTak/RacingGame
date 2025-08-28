#pragma once

#include <string>
#include <vector>

struct DriverInfo
{
    std::string Name;
    int Number;
    int TeamID;
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

struct GreenFlag
{
};

struct YellowFlag
{
};

struct BlueFlag
{
    int CarID;
};

struct ViewModeToggle
{
    bool TopDown;
};

struct RaceState
{
    std::string Date;
    std::string Time;
    std::string Weather;
    std::vector<DriverInfo> Drivers;
    std::vector<int> Positions;
    std::vector<float> Intervals;
    std::vector<int> Laps;
    std::string FlagStatus;
    std::vector<std::string> TrackLayout;
};