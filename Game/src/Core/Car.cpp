#include "Car.h"
#include "Driver.h"

Car::Car(const std::string& model) : m_Model(model)
{

}

const std::string& Car::GetModel() const
{
	return m_Model;
}

void Car::SetModel(const std::string& model)
{
	m_Model = model;
}

const std::optional<std::string>& Car::GetSponsor() const
{
	return m_Sponsor;
}

void Car::SetSponsor(const std::optional<std::string>& sponsor)
{
	m_Sponsor = sponsor;
}

std::shared_ptr<Driver> Car::GetAssignedDriver() const
{
	return m_AssignedDriver;
}

void Car::SetAssignedDriver(const std::shared_ptr<Driver>& driver)
{
	m_AssignedDriver = driver;
}