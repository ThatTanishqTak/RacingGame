#pragma once

#include <memory>
#include <optional>
#include <string>

class Driver;

class Car
{
private:
    std::string Model;
    std::optional<std::string> Sponsor;
    std::shared_ptr<Driver> AssignedDriver;

public:
    explicit Car(const std::string& model);

    const std::string& GetModel() const;
    void SetModel(const std::string& model);

    const std::optional<std::string>& GetSponsor() const;
    void SetSponsor(const std::optional<std::string>& sponsor);

    std::shared_ptr<Driver> GetAssignedDriver() const;
    void SetAssignedDriver(const std::shared_ptr<Driver>& driver);
};