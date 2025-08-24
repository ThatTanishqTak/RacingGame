#include "Circuit.h"
#include "Car.h"
#include "Driver.h"
#include "TyreModel.h"

#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<TrackSegment> segments = { {5.0, 0.1, 0.0, false} };
    Circuit track("Test", 5.0, segments);
    Driver driver("Tester", 30);
    Car car("ModelY");
    car.SetTyreCompound(TyreCompound::Soft);

    // Fuel mass impact
    car.SetFuelMass(0.0);
    car.SetLapsCompleted(0);
    double baseline = car.CalculateLapTime(track, driver);

    car.SetFuelMass(5.0);
    double light = car.CalculateLapTime(track, driver);

    car.SetFuelMass(50.0);
    double heavy = car.CalculateLapTime(track, driver);

    double expectedLight = baseline * (1.0 + 5.0 * 0.001);
    double expectedHeavy = baseline * (1.0 + 50.0 * 0.001);
    assert(std::fabs(light - expectedLight) / expectedLight <= 0.01);
    assert(std::fabs(heavy - expectedHeavy) / expectedHeavy <= 0.01);

    // Tyre wear impact
    car.SetFuelMass(0.0);
    car.SetLapsCompleted(0);
    double freshLap = car.CalculateLapTime(track, driver);
    car.SetLapsCompleted(10);
    double wornLap = car.CalculateLapTime(track, driver);
    TyreState state = TyreModel::Predict(TyreCompound::Soft, 10);
    double expectedWorn = baseline * (1.0 + (1.0 - state.Wear));
    assert(std::fabs(wornLap - expectedWorn) / expectedWorn <= 0.01);
    assert(wornLap > freshLap);

    // TyreModel wear expectation
    double expectedWear = std::max(0.0, 1.0 - 0.06 * 10);
    assert(std::fabs(state.Wear - expectedWear) <= 0.05);

    return 0;
}