#pragma once

#include <memory>
#include <string>
#include <vector>

class Driver;
class Circuit;
class Team;

enum class SessionType
{
    Practice,
    Qualifying,
    Race
};

struct SessionResult
{
    std::shared_ptr<Driver> Driver;
    double Time;
};

class Race
{
private:
    std::shared_ptr<Circuit> Track;
    std::string Date;

    std::vector<SessionResult> PracticeResults;
    std::vector<SessionResult> QualifyingResults;
    std::vector<SessionResult> RaceResults;

    SessionType CurrentSession;
    bool ParcFerme;

    std::vector<SessionResult> GenerateResults(const std::vector<std::shared_ptr<Driver>>& drivers);

public:
    Race(const std::shared_ptr<Circuit>& circuit, const std::string& date);

    const std::shared_ptr<Circuit>& GetCircuit() const;
    const std::string& GetDate() const;
    SessionType GetCurrentSession() const;
    bool IsParcFerme() const;


    std::vector<SessionResult> ConductPractice(const std::vector<std::shared_ptr<Driver>>& drivers);
    std::vector<SessionResult> ConductQualifying(const std::vector<std::shared_ptr<Driver>>& drivers);
    std::vector<SessionResult> ConductRace(const std::vector<std::shared_ptr<Team>>& teams);

    const std::vector<SessionResult>& GetPracticeResults() const;
    const std::vector<SessionResult>& GetQualifyingResults() const;
    const std::vector<SessionResult>& GetRaceResults() const;
};