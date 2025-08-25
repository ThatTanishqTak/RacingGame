#include "Driver.h"

Driver::Driver(const std::string& name, int age) : Name(name), Age(age)
{

}

const std::string& Driver::GetName() const { return Name; }

void Driver::SetName(const std::string& name) { Name = name; }

int Driver::GetAge() const { return Age; }

void Driver::SetAge(int age) { Age = age; }

const std::optional<std::string>& Driver::GetPersonalSponsor() const { return PersonalSponsor; }

void Driver::SetPersonalSponsor(const std::optional<std::string>& sponsor) { PersonalSponsor = sponsor; }

const std::variant<std::string, std::vector<std::string>>& Driver::GetRoles() const { return Roles; }

void Driver::SetRoles(const std::variant<std::string, std::vector<std::string>>& roles) { Roles = roles; }