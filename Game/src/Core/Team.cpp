#include "Team.h"
#include "Driver.h"

Team::Team(const std::string& name) : Name(name)
{

}

const std::string& Team::GetName() const { return Name; }

void Team::SetName(const std::string& name) { Name = name; }

const std::optional<std::string>& Team::GetPrimarySponsor() const { return PrimarySponsor; }

void Team::SetPrimarySponsor(const std::optional<std::string>& sponsor) { PrimarySponsor = sponsor; }

void Team::AddDriver(const std::shared_ptr<Driver>& driver) { Drivers.push_back(driver); }

const std::vector<std::shared_ptr<Driver>>& Team::GetDrivers() const { return Drivers; }