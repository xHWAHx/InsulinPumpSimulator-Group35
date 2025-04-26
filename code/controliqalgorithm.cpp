#include "controliqalgorithm.h"
#include "profile.h"
#include "datalogger.h"
#include "pumpcontroller.h"

double ControlIQAlgorithm::currentRate = 0;

void ControlIQAlgorithm::analyzeGlucoseData(double glucose, DataLogger* logger, PumpController* pump) {

    // Loads active profile data
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
        logger->logEvent("Info", "Profile basal rate set manually to " + QString::number(profileRate) + ".");
    }
}

void ControlIQAlgorithm::adjustBasalRate(PumpController* pump, double rate) {
    if (pump) {
        currentRate = rate;
        pump->adjustBasalRate(rate);
    }
}
