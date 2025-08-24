#pragma once

enum class TyreCompound
{
    Soft,
    Medium,
    Hard
};

struct TyreState
{
    double Wear;
    double Temperature;
};

class TyreModel
{
public:
    static TyreState Predict(TyreCompound compound, int laps);
};