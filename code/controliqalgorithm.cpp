#include "controliqalgorithm.h"
#include "profile.h"
#include "datalogger.h"
#include "pumpcontroller.h"
#include <numeric>
#include <iostream>

double ControlIQAlgorithm::currentRate = 0;

void ControlIQAlgorithm::analyzeGlucoseData(double glucose, DataLogger* logger, PumpController* pump) {

    // loads active profile data
    Profile profile = Profile::getActiveProfile();
    double target = profile.getTargetGlucose();
    double profileRate = profile.getBasalRate();

    if (glucose <= 3.9) {
        adjustBasalRate(pump, 0);
        logger->logEvent("Warning", "Low glucose detected. Basal rate pumping suspended.");
    } else if ((glucose > target) and (currentRate == 0)) {
        adjustBasalRate(pump, profileRate);
        logger->logEvent("Info", "Glucose stable. Resumed basal rate pumping.");
    } else if ((currentRate != 0) and (currentRate != profileRate)) {
        adjustBasalRate(pump, profileRate);
        logger->logEvent("Info", "Basal rate changed manually to " + QString::number(profileRate) + ".");
    }
}

void ControlIQAlgorithm::adjustBasalRate(PumpController* pump, double rate) {
    if (pump) {
        currentRate = rate;
        pump->adjustBasalRate(rate);
    }
}

//void ControlIQAlgorithm::suspendForLowGlucose(PumpController* pump) {
//    if (pump) {
//        pump->suspendBolus();
//    }
//}

// checks if glucose is within stable range 
//bool ControlIQAlgorithm::isGlucoseLevelStable(double glucose, double target) {
//    return (glucose >= target - 0.5 && glucose <= target + 0.5);
//}


