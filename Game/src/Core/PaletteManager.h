#pragma once

#include <imgui.h>
#include <string>
#include <vector>

class PaletteManager
{
public:
    void LoadPalettes(const std::string& primaryPath, const std::string& colourBlindPath);
    ImVec4 GetTeamColour(size_t teamId, bool colourBlindMode) const;

private:
    std::vector<ImVec4> PrimaryPalette;
    std::vector<ImVec4> ColourBlindPalette;
};

extern PaletteManager g_PaletteManager;