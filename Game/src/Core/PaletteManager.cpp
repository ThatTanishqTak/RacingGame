#include "PaletteManager.h"

#include <fstream>
#include <regex>
#include <sstream>

static std::vector<ImVec4> ParsePalette(const std::string& path)
{
    std::vector<ImVec4> colours;
    std::ifstream file(path);
    if (!file.is_open())
    {
        return colours;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::regex pattern(R"(\[\s*([0-9.]+)\s*,\s*([0-9.]+)\s*,\s*([0-9.]+)\s*\])");
    auto begin = std::sregex_iterator(content.begin(), content.end(), pattern);
    auto end = std::sregex_iterator();
    for (auto it = begin; it != end; ++it)
    {
        float r = std::stof((*it)[1].str());
        float g = std::stof((*it)[2].str());
        float b = std::stof((*it)[3].str());
        colours.emplace_back(r, g, b, 1.0f);
    }

    return colours;
}

void PaletteManager::LoadPalettes(const std::string& primaryPath, const std::string& colourBlindPath)
{
    PrimaryPalette = ParsePalette(primaryPath);
    ColourBlindPalette = ParsePalette(colourBlindPath);
}

ImVec4 PaletteManager::GetTeamColour(size_t teamId, bool colourBlindMode) const
{
    const auto& palette = colourBlindMode ? ColourBlindPalette : PrimaryPalette;
    if (teamId < palette.size())
    {
        return palette[teamId];
    }
    
    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

PaletteManager g_PaletteManager;