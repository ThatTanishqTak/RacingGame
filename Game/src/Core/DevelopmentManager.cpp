#include "DevelopmentManager.h"

DevelopmentManager::DevelopmentManager(FinanceManager& finance) : m_Finance(finance), m_Rng(std::random_device{}())
{

}

void DevelopmentManager::AllocateChassis(double amount)
{
    m_Finance.AllocateBudget("Chassis", amount);
    m_Upgrades.push({ "Chassis", amount });
}

void DevelopmentManager::AllocateAerodynamics(double amount)
{
    m_Finance.AllocateBudget("Aerodynamics", amount);
    m_Upgrades.push({ "Aerodynamics", amount });
}

void DevelopmentManager::AllocatePowerUnit(double amount)
{
    m_Finance.AllocateBudget("PowerUnit", amount);
    m_Upgrades.push({ "PowerUnit", amount });
}

void DevelopmentManager::ProcessUpgrades()
{
    while (!m_Upgrades.empty())
    {
        UpgradeTask task = m_Upgrades.front();
        m_Upgrades.pop();
        m_Finance.RecordExpense(task.type, task.budget);
    }
}

void DevelopmentManager::TriggerRandomEvents()
{
    std::uniform_int_distribution<int> eventDist(0, 1);
    if (eventDist(m_Rng) == 0)
    {
        // Part failure: drop the next upgrade
        if (!m_Upgrades.empty())
        {
            m_Upgrades.pop();
        }
    }
    else
    {
        // Wind-tunnel limit: reduce aerodynamics budget
        std::uniform_real_distribution<double> cutDist(10000.0, 30000.0);
        double cut = cutDist(m_Rng);

        m_Finance.RecordExpense("Aerodynamics", cut);
    }
}