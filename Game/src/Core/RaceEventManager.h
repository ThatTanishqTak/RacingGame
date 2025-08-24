#pragma once

#include <memory>
#include <random>
#include <string>
#include <vector>

enum class TrackFlag
{
    Green,
    SafetyCar,
    VirtualSafetyCar
};

struct CarEventState
{
    bool Retired{ false };
};

class RaceEvent
{
public:
    virtual ~RaceEvent() = default;
    virtual void Execute(std::mt19937& rng, TrackFlag& flag, std::vector<CarEventState>& cars) = 0;
};

class SafetyCarEvent : public RaceEvent
{
private:
    double Probability;

public:
    explicit SafetyCarEvent(double probability);
    void Execute(std::mt19937& rng, TrackFlag& flag, std::vector<CarEventState>& cars) override;
};

class MechanicalFailureEvent : public RaceEvent
{
private:
    double Probability;

public:
    explicit MechanicalFailureEvent(double probability);
    void Execute(std::mt19937& rng, TrackFlag& flag, std::vector<CarEventState>& cars) override;
};

class RaceEventManager
{
private:
    std::mt19937 Rng;
    std::vector<std::unique_ptr<RaceEvent>> Events;

public:
    explicit RaceEventManager(unsigned int seed = std::random_device{}());
    void LoadFromJson(const std::string& filePath);
    void RegisterEvent(std::unique_ptr<RaceEvent> event);
    void EvaluateLap(std::vector<CarEventState>& cars, TrackFlag& flag);
};