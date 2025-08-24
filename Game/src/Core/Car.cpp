#include "Car.h"
#include "Circuit.h"
#include "Driver.h"
#include "TyreModel.h"

Car::Car(const std::string& model) : Model(model)
{

}

const std::string& Car::GetModel() const { return Model; }

void Car::SetModel(const std::string& model) { Model = model; }

const std::optional<std::string>& Car::GetSponsor() const { return Sponsor; }

void Car::SetSponsor(const std::optional<std::string>& sponsor) { Sponsor = sponsor; }

std::shared_ptr<Driver> Car::GetAssignedDriver() const { return AssignedDriver; }

void Car::SetAssignedDriver(const std::shared_ptr<Driver>& driver) { AssignedDriver = driver; }

double Car::GetDownforceLow() const { return DownforceLow; }
void Car::SetDownforceLow(double value) { DownforceLow = value; }

double Car::GetDownforceHigh() const { return DownforceHigh; }
void Car::SetDownforceHigh(double value) { DownforceHigh = value; }

double Car::GetDrag() const { return Drag; }
void Car::SetDrag(double value) { Drag = value; }

double Car::GetPower() const { return Power; }
void Car::SetPower(double value) { Power = value; }

double Car::GetERSCapacity() const { return ERSCapacity; }
void Car::SetERSCapacity(double value) { ERSCapacity = value; }

const std::vector<double>& Car::GetGearRatios() const { return GearRatios; }
void Car::SetGearRatios(const std::vector<double>& ratios) { GearRatios = ratios; }

double Car::GetFuelMass() const { return FuelMass; }
void Car::SetFuelMass(double value) { FuelMass = value; }

double Car::GetReliability() const { return Reliability; }
void Car::SetReliability(double value) { Reliability = value; }

TyreCompound Car::GetTyreCompound() const { return Compound; }
void Car::SetTyreCompound(TyreCompound compound) { Compound = compound; }

int Car::GetLapsCompleted() const { return LapsCompleted; }
void Car::SetLapsCompleted(int laps) { LapsCompleted = laps; }

double Car::CalculateLapTime(const Circuit& track, const Driver& driver) const
{
    double base = track.ComputeBaselineLap(*this, driver);
    double fuelFactor = 1.0 + FuelMass * 0.001;
    TyreState state = TyreModel::Predict(Compound, LapsCompleted);
    double tyreFactor = 1.0 + (1.0 - state.Wear);

    return base * fuelFactor * tyreFactor;
}