#include "FinanceManager.h"
#include "RaceConfiguration.h"

FinanceManager::FinanceManager(double initialBudget) : m_TotalBudget(initialBudget), m_Expenses(0.0), m_Rng(CreateRandomGenerator())
{

}

void FinanceManager::AllocateBudget(const std::string& department, double amount)
{
    if (amount <= m_TotalBudget)
    {
        m_DepartmentBudgets[department] += amount;
        m_TotalBudget -= amount;
    }
}

void FinanceManager::RecordExpense(const std::string& department, double amount)
{
    double& budget = m_DepartmentBudgets[department];
    if (amount <= budget)
    {
        budget -= amount;
        m_Expenses += amount;
    }
}

double FinanceManager::GetDepartmentBudget(const std::string& department) const
{
    if (auto it = m_DepartmentBudgets.find(department); it != m_DepartmentBudgets.end())
    {
        return it->second;
    }
    
    return 0.0;
}

double FinanceManager::GetRemainingBudget() const
{
    return m_TotalBudget;
}

double FinanceManager::GetTotalExpenses() const
{
    return m_Expenses;
}

void FinanceManager::ProcessRandomEvent()
{
    std::uniform_int_distribution<int> eventDist(0, 1);
    if (eventDist(m_Rng) == 0)
    {
        std::uniform_real_distribution<double> expenseDist(10000.0, 50000.0);
        double unexpectedCost = expenseDist(m_Rng);
        if (unexpectedCost <= m_TotalBudget)
        {
            m_TotalBudget -= unexpectedCost;
        }

        m_Expenses += unexpectedCost;
    }
}