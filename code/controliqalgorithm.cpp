#include "controliqalgorithm.h"
#include "profile.h"
#include "datalogger.h"
#include "pumpcontroller.h"
#include <numeric>
#include <iostream>

void ControlIQAlgorithm::analyzeGlucoseData(double data, DataLogger* logger, PumpController* pump) {

    double avgGlucose = data;

    // loads active profile data
    Profile profile = Profile::getActiveProfile();  
    double target = profile.getTargetGlucose();
    double currentRate = profile.getBasalRate();

    if (avgGlucose < 3.9) {
        suspendForLowGlucose(pump); // low glucose
        logger->logEvent("Warning", "Low glucose detected. Bolus suspended.");
        adjustBasalRate(pump, 0);
    } else {
        adjustBasalRate(pump, currentRate); // fine-tunes basal rate for stable glucose
        logger->logEvent("Info", "Glucose stable. Adjusted basal rate.");
    }
}

void ControlIQAlgorithm::adjustBasalRate(PumpController* pump, double rate) {
    if (pump) {
        pump->adjustBasalRate(rate);
    }
}

void ControlIQAlgorithm::suspendForLowGlucose(PumpController* pump) {
    if (pump) {
        pump->suspendBolus();
    }
}

// checks if glucose is within stable range 
bool ControlIQAlgorithm::isGlucoseLevelStable(double glucose, double target) {
    return (glucose >= target - 0.5 && glucose <= target + 0.5);
}


