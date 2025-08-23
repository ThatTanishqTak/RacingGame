#include "Contract.h"
#include "Driver.h"
#include "Team.h"

Contract::Contract(const std::shared_ptr<Team>& team, const std::shared_ptr<Driver>& driver) : TeamRef(team), DriverRef(driver)
{

}

std::shared_ptr<Team> Contract::GetTeam() const { return TeamRef; }

void Contract::SetTeam(const std::shared_ptr<Team>& team) { TeamRef = team; }

std::shared_ptr<Driver> Contract::GetDriver() const { return DriverRef; }

void Contract::SetDriver(const std::shared_ptr<Driver>& driver) { DriverRef = driver; }

const std::variant<double, std::pair<double, double>>& Contract::GetPayment() const { return Payment; }

void Contract::SetPayment(const std::variant<double, std::pair<double, double>>& payment) { Payment = payment; }