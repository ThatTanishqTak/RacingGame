#include "HUD.h"

HUD::HUD() : m_Budget(1000000.0f), m_Staff(50), m_RaceCalendar("Monaco GP"), m_LiveSession(1)
{

}

void HUD::Render()
{
    m_Budget.Render();
    m_Staff.Render();
    m_RaceCalendar.Render();
    m_LiveSession.Render();
}