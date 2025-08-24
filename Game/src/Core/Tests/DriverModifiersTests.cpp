#include "Driver.h"

#include <cassert>

int main()
{
    CarState base{ 0.5, 0.5, 0.1, 1.0, false };

    Driver calm("Calm", 25);
    calm.SetAggression(0.1);
    calm.SetConsistency(0.3);
    calm.SetWetSkill(0.5);
    calm.SetConfidenceLevel(0.0);

    Driver aggressive("Aggressive", 25);
    aggressive.SetAggression(0.9);
    aggressive.SetConsistency(0.9);
    aggressive.SetWetSkill(0.5);
    aggressive.SetConfidenceLevel(0.5);

    CarState calmState = base;
    CarState aggressiveState = base;

    calm.ApplyDriverModifiers(calmState);
    aggressive.ApplyDriverModifiers(aggressiveState);

    assert(aggressiveState.OvertakeProbability > calmState.OvertakeProbability);
    assert(aggressiveState.DefendProbability > calmState.DefendProbability);
    assert(aggressiveState.ErrorRate < calmState.ErrorRate);
    assert(aggressiveState.Pace > calmState.Pace);

    CarState wetBase{ 0.5, 0.5, 0.1, 1.0, true };
    Driver novice("Novice", 20);
    novice.SetWetSkill(0.2);

    Driver ace("Ace", 20);
    ace.SetWetSkill(0.8);

    CarState noviceState = wetBase;
    CarState aceState = wetBase;

    novice.ApplyDriverModifiers(noviceState);
    ace.ApplyDriverModifiers(aceState);

    assert(aceState.ErrorRate < noviceState.ErrorRate);
    assert(aceState.Pace > noviceState.Pace);

    return 0;
}