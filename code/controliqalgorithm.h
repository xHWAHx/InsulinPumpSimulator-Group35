#ifndef CONTROLIQALGORITHM_H
#define CONTROLIQALGORITHM_H

#include <vector>

class ControlIQAlgorithm
{
public:
    static void analyzeGlucoseData(const std::vector<double>& data);
    static void adjustBasalRate();
    static void suspendForLowGlucose();
    static void increaseInsulinForHighGlucose();
    static bool isGlucoseLevelStable();
};

#endif // CONTROLIQALGORITHM_H
