#ifndef CONTROLIQALGORITHM_H
#define CONTROLIQALGORITHM_H

#include <vector>

class DataLogger;
class PumpController;

class ControlIQAlgorithm {
public:
    static void analyzeGlucoseData(double data, DataLogger* logger, PumpController* pump);
    static void adjustBasalRate(PumpController* pump, double rate);
    static void suspendForLowGlucose(PumpController* pump);
    static void increaseInsulinForHighGlucose(PumpController* pump, double currentRate);
    static bool isGlucoseLevelStable(double glucose, double target);
};

#endif // CONTROLIQALGORITHM_H
