#pragma once

#include <cstdint>
#include <random>
#include <fstream>

struct RaceConfiguration
{
    uint32_t Seed{ 0 };
};

extern RaceConfiguration g_RaceConfig;

void ResetRaceSeed();
std::mt19937 CreateRandomGenerator();

bool SaveRaceConfig(std::ofstream& stream, const RaceConfiguration& config);
bool LoadRaceConfig(std::ifstream& stream, RaceConfiguration& config);