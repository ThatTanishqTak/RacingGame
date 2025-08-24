#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

struct CarState
{
    double OvertakeProbability{};
    double DefendProbability{};
    double ErrorRate{};
    double Pace{ 1.0 };
    bool IsWet{};
};

class Driver
{
private:
    std::string Name;
    int Age;
    std::optional<std::string> PersonalSponsor;
    std::variant<std::string, std::vector<std::string>> Roles;
    double Aggression{};
    double Consistency{};
    double WetSkill{};
    double ConfidenceLevel{};

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

    double GetAggression() const;
    void SetAggression(double value);

    double GetConsistency() const;
    void SetConsistency(double value);

    double GetWetSkill() const;
    void SetWetSkill(double value);

    double GetConfidenceLevel() const;
    void SetConfidenceLevel(double value);

    void ApplyDriverModifiers(CarState& state) const;
};