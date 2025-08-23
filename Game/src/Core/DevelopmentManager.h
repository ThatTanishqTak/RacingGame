#pragma once

#include <queue>
#include <string>
#include <random>
#include "FinanceManager.h"

struct UpgradeTask
{
    std::string type;
    double budget;
};

class DevelopmentManager
{
public:
    explicit DevelopmentManager(FinanceManager& finance);

    void AllocateChassis(double amount);
    void AllocateAerodynamics(double amount);
    void AllocatePowerUnit(double amount);

    void ProcessUpgrades();
    void TriggerRandomEvents();

private:
    FinanceManager& m_Finance;
    std::queue<UpgradeTask> m_Upgrades;
    std::mt19937 m_Rng;
};