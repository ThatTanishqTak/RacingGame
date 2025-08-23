#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

class Driver
{
private:
    std::string Name;
    int Age;
    std::optional<std::string> PersonalSponsor;
    std::variant<std::string, std::vector<std::string>> Roles;

public:
    Driver(const std::string& name, int age);

    const std::string& GetName() const;
    void SetName(const std::string& name);

    int GetAge() const;
    void SetAge(int age);

    const std::optional<std::string>& GetPersonalSponsor() const;
    void SetPersonalSponsor(const std::optional<std::string>& sponsor);

    const std::variant<std::string, std::vector<std::string>>& GetRoles() const;
    void SetRoles(const std::variant<std::string, std::vector<std::string>>& roles);
};