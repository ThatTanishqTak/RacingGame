#pragma once

#include <memory>
#include <optional>
#include <string>

class Driver;

class Car
{
private:
    std::string m_Model;
    std::optional<std::string> m_Sponsor;
    std::weak_ptr<Driver> m_AssignedDriver;

public:
    explicit Car(const std::string& model);

    const std::string& GetModel() const;
    void SetModel(const std::string& model);

    const std::optional<std::string>& GetSponsor() const;
    void SetSponsor(const std::optional<std::string>& sponsor);

    std::shared_ptr<Driver> GetAssignedDriver() const;
    void SetAssignedDriver(const std::shared_ptr<Driver>& driver);
};