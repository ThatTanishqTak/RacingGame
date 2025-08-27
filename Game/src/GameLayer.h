#pragma once

#include <string>
#include <vector>

class GameLayer
{
public:
    void Update();
    void Render(const std::vector<std::string>& trackLayout);
};