#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "TyreModel.h"

class Driver;
class Circuit;

class Car
{
private:
    std::string Model;
    std::optional<std::string> Sponsor;
    std::shared_ptr<Driver> AssignedDriver;

    double DownforceLow{};
    double DownforceHigh{};
    double Drag{};
    double Power{};
    double ERSCapacity{};
    std::vector<double> GearRatios;
    double FuelMass{};
    double Reliability{};
    TyreCompound Compound{ TyreCompound::Medium };
    int LapsCompleted{};

public:
    explicit Car(const std::string& model);

    const std::string& GetModel() const;
    void SetModel(const std::string& model);

    const std::optional<std::string>& GetSponsor() const;
    void SetSponsor(const std::optional<std::string>& sponsor);

    std::shared_ptr<Driver> GetAssignedDriver() const;
    void SetAssignedDriver(const std::shared_ptr<Driver>& driver);

    double GetDownforceLow() const;
    void SetDownforceLow(double value);

    double GetDownforceHigh() const;
    void SetDownforceHigh(double value);

    double GetDrag() const;
    void SetDrag(double value);

    double GetPower() const;
    void SetPower(double value);

    double GetERSCapacity() const;
    void SetERSCapacity(double value);

    const std::vector<double>& GetGearRatios() const;
    void SetGearRatios(const std::vector<double>& ratios);

    double GetFuelMass() const;
    void SetFuelMass(double value);

    double GetReliability() const;
    void SetReliability(double value);

    TyreCompound GetTyreCompound() const;
    void SetTyreCompound(TyreCompound compound);

    int GetLapsCompleted() const;
    void SetLapsCompleted(int laps);

    double CalculateLapTime(const Circuit& track, const Driver& driver) const;
};