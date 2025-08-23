#pragma once

#include "Userinterface/HUD.h"

class GameLayer
{
public:
    void Render();
private:
    HUD m_HUD;
};