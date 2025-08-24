#include "Driver.h"
#include <algorithm>

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

double Driver::GetAggression() const { return Aggression; }

void Driver::SetAggression(double value) { Aggression = value; }

double Driver::GetConsistency() const { return Consistency; }

void Driver::SetConsistency(double value) { Consistency = value; }

double Driver::GetWetSkill() const { return WetSkill; }

void Driver::SetWetSkill(double value) { WetSkill = value; }

double Driver::GetConfidenceLevel() const { return ConfidenceLevel; }

void Driver::SetConfidenceLevel(double value) { ConfidenceLevel = value; }

void Driver::ApplyDriverModifiers(CarState& state) const
{
    double confidenceFactor = 1.0 + ConfidenceLevel;

    state.Pace *= confidenceFactor;

    state.OvertakeProbability = std::clamp(state.OvertakeProbability * (1.0 + Aggression * confidenceFactor), 0.0, 1.0);
    state.DefendProbability = std::clamp(state.DefendProbability * (1.0 + 0.5 * Aggression * confidenceFactor), 0.0, 1.0);

    double consistencyEffect = 1.0 - Consistency * confidenceFactor;
    state.ErrorRate = std::max(0.0, state.ErrorRate * consistencyEffect);

    if (state.IsWet)
    {
        state.Pace *= WetSkill;
        state.ErrorRate *= (1.0 - WetSkill);
    }
}