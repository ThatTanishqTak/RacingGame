#pragma once

#include <fstream>
#include <string>

#include "RaceConfiguration.h"
#include "Renderer/StateStream.h"

class ReplayLogger
{
public:
    bool Open(const std::string& filePath, const RaceConfiguration& config);
    void WriteSnapshot(const Engine::Snapshot& snapshot);
    void Close();

private:
    std::ofstream m_Stream;
};

bool LoadReplay(const std::string& filePath, double baseTime);