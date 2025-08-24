#include "TyreModel.h"

#include <algorithm>

TyreState TyreModel::Predict(TyreCompound compound, int laps)
{
    double wearRate = 0.0;
    double tempBase = 0.0;
    double tempRate = 0.0;

    switch (compound)
    {
    case TyreCompound::Soft:
        wearRate = 0.06; 
        tempBase = 95.0; 
        tempRate = 0.8; 
        break;
    case TyreCompound::Medium:
        wearRate = 0.045; 
        tempBase = 90.0; 
        tempRate = 0.6; 
        break;
    case TyreCompound::Hard:
        wearRate = 0.03; 
        tempBase = 85.0; 
        tempRate = 0.4; 
        break;
    }

    double wear = std::max(0.0, 1.0 - wearRate * laps);
    double temperature = tempBase + tempRate * laps;

    return { wear, temperature };
}