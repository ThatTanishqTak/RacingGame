#pragma once

#include <memory>
#include <utility>
#include <variant>

class Team;
class Driver;

class Contract
{
private:
    std::shared_ptr<Team> TeamRef;
    std::shared_ptr<Driver> DriverRef;
    std::variant<double, std::pair<double, double>> Payment;

public:
    Contract(const std::shared_ptr<Team>& team, const std::shared_ptr<Driver>& driver);

    std::shared_ptr<Team> GetTeam() const;
    void SetTeam(const std::shared_ptr<Team>& team);

    std::shared_ptr<Driver> GetDriver() const;
    void SetDriver(const std::shared_ptr<Driver>& driver);

    const std::variant<double, std::pair<double, double>>& GetPayment() const;
    void SetPayment(const std::variant<double, std::pair<double, double>>& payment);
};