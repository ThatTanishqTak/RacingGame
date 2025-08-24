#include "Circuit.h"
#include "Car.h"
#include "Driver.h"

#include <cassert>
#include <cmath>
#include <filesystem>

int main()
{
    std::filesystem::path path = std::filesystem::path("Game/Assets/Tracks/SampleTrack.json");
    auto circuit = Circuit::FromJson(path.string());

    Car car("ModelX");
    Driver driver("Test Driver", 30);

    double lap = circuit.ComputeBaselineLap(car, driver);
    double target = 83.462; // Expected baseline
    assert(std::fabs(lap - target) / target <= 0.015);

    return 0;
}