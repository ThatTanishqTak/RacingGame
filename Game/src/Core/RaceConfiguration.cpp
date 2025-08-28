#include "RaceConfiguration.h"

RaceConfiguration g_RaceConfig;
static uint32_t s_CurrentSeed = 0;

void ResetRaceSeed()
{
    s_CurrentSeed = g_RaceConfig.Seed;
}

std::mt19937 CreateRandomGenerator()
{
    return std::mt19937(s_CurrentSeed++);
}

bool SaveRaceConfig(std::ofstream& stream, const RaceConfiguration& config)
{
    stream.write(reinterpret_cast<const char*>(&config.Seed), sizeof(config.Seed));
    
    return stream.good();
}

bool LoadRaceConfig(std::ifstream& stream, RaceConfiguration& config)
{
    stream.read(reinterpret_cast<char*>(&config.Seed), sizeof(config.Seed));

    return stream.good();
}