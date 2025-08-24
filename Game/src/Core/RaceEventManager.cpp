#include "RaceEventManager.h"

#include <algorithm>
#include <fstream>

#include <nlohmann/json.hpp>

RaceEventManager::RaceEventManager(unsigned int seed) : Rng(seed)
{

}

void RaceEventManager::LoadFromJson(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        return;
    }

    nlohmann::json data;
    file >> data;

    if (data.contains("SafetyCar"))
    {
        double prob = data["SafetyCar"].value("probability", 0.0);
        RegisterEvent(std::make_unique<SafetyCarEvent>(prob));
    }

    if (data.contains("MechanicalFailure"))
    {
        double prob = data["MechanicalFailure"].value("probability", 0.0);
        RegisterEvent(std::make_unique<MechanicalFailureEvent>(prob));
    }
}

void RaceEventManager::RegisterEvent(std::unique_ptr<RaceEvent> event)
{
    Events.push_back(std::move(event));
}

void RaceEventManager::EvaluateLap(std::vector<CarEventState>& cars, TrackFlag& flag)
{
    for (auto& evt : Events)
    {
        evt->Execute(Rng, flag, cars);
    }
}

SafetyCarEvent::SafetyCarEvent(double probability) : Probability(probability)
{

}

void SafetyCarEvent::Execute(std::mt19937& rng, TrackFlag& flag, std::vector<CarEventState>& cars)
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    if (dist(rng) < Probability)
    {
        flag = TrackFlag::SafetyCar;
    }
}

MechanicalFailureEvent::MechanicalFailureEvent(double probability) : Probability(probability)
{

}

void MechanicalFailureEvent::Execute(std::mt19937& rng, TrackFlag& flag, std::vector<CarEventState>& cars)
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    if (dist(rng) < Probability)
    {
        std::vector<size_t> candidates;
        for (size_t i = 0; i < cars.size(); ++i)
        {
            if (!cars[i].Retired)
            {
                candidates.push_back(i);
            }
        }

        if (!candidates.empty())
        {
            std::uniform_int_distribution<size_t> pick(0, candidates.size() - 1);
            cars[candidates[pick(rng)]].Retired = true;
        }
    }
}