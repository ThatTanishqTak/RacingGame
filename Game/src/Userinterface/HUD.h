#pragma once

#ifdef MANAGEMENT_MODE

#include "UIManager.h"

class HUD
{
public:
    HUD();
    void Render();

private:
    UIManager m_UiManager;
};

#else

class HUD
{
public:
    HUD() = default;
    void Render() {}
};

#endif