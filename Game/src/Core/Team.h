#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

class Driver;

class Team
{
private:
    std::string Name;
    std::optional<std::string> PrimarySponsor;
    std::vector<std::shared_ptr<Driver>> Drivers;

public:
    explicit Team(const std::string& name);

    const std::string& GetName() const;
    void SetName(const std::string& name);

    const std::optional<std::string>& GetPrimarySponsor() const;
    void SetPrimarySponsor(const std::optional<std::string>& sponsor);

    void AddDriver(const std::shared_ptr<Driver>& driver);
    const std::vector<std::shared_ptr<Driver>>& GetDrivers() const;
};