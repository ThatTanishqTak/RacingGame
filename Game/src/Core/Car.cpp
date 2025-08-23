#include "Car.h"
#include "Driver.h"

Car::Car(const std::string& model) : Model(model)
{

}

const std::string& Car::GetModel() const { return Model; }

void Car::SetModel(const std::string& model) { Model = model; }

const std::optional<std::string>& Car::GetSponsor() const { return Sponsor; }

void Car::SetSponsor(const std::optional<std::string>& sponsor) { Sponsor = sponsor; }

std::shared_ptr<Driver> Car::GetAssignedDriver() const { return AssignedDriver; }

void Car::SetAssignedDriver(const std::shared_ptr<Driver>& driver) { AssignedDriver = driver; }