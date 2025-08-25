#include "HUD.h"

#ifdef MANAGEMENT_MODE

HUD::HUD() = default;

void HUD::Render()
{
    m_UiManager.Render();
}

#else

HUD::HUD() = default;

void HUD::Render() {}

#endif