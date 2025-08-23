#pragma once

#include <string>
#include <unordered_map>
#include <random>

class FinanceManager
{
public:
    explicit FinanceManager(double initialBudget = 0.0);

    void AllocateBudget(const std::string& department, double amount);
    void RecordExpense(const std::string& department, double amount);

    double GetDepartmentBudget(const std::string& department) const;
    double GetRemainingBudget() const;
    double GetTotalExpenses() const;

    void ProcessRandomEvent();

private:
    double m_TotalBudget;
    double m_Expenses;
    std::unordered_map<std::string, double> m_DepartmentBudgets;
    std::mt19937 m_Rng;
};