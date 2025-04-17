#include "controliqalgorithm.h"
#include "profile.h"
#include "datalogger.h"
#include "pumpcontroller.h"
#include <numeric>
#include <iostream>

void ControlIQAlgorithm::analyzeGlucoseData(double data, DataLogger* logger, PumpController* pump) {
    //if (data.empty() || !pump) return; // skips processing if data / dependencies are missing

    //double avgGlucose = std::accumulate(data.begin(), data.end(), 0.0) / data.size(); // gets data from CGM
    double avgGlucose = data;

    // loads active profile data
    Profile profile = Profile::getActiveProfile();  
    double target = profile.getTargetGlucose();
    double currentRate = profile.getBasalRate();

    if (avgGlucose < 3.9) {
        suspendForLowGlucose(pump); // low glucose
        logger->logEvent("Warning", "Low glucose detected. Bolus suspended.");
        adjustBasalRate(pump, 0);
//    } else if (avgGlucose > target + 2.0) {
//        increaseInsulinForHighGlucose(pump, currentRate); // high glucose
//        logger->logEvent("Info", "High glucose detected. Increased basal rate.");
    } else {
        adjustBasalRate(pump, currentRate); // fine-tunes basal rate for stable glucose
        logger->logEvent("Info", "Glucose stable. Adjusted basal rate.");
    }
}

void ControlIQAlgorithm::adjustBasalRate(PumpController* pump, double rate) {
    if (pump) {
        pump->adjustBasalRate(rate);
        //std::cout << "[ControlIQ] Basal rate set to " << rate << " U/hr\n";
    }
}

void ControlIQAlgorithm::suspendForLowGlucose(PumpController* pump) {
    if (pump) {
        pump->suspendBolus();
        //std::cout << "[ControlIQ] Low glucose — bolus suspended.\n";
    }
}

//void ControlIQAlgorithm::increaseInsulinForHighGlucose(PumpController* pump, double currentRate) {
//    if (pump) {
//        double newRate = currentRate + 0.5;
//        pump->adjustBasalRate(newRate);
//        //std::cout << "[ControlIQ] High glucose — basal rate increased to " << newRate << " U/hr\n";
//    }
//}

// checks if glucose is within stable range 
bool ControlIQAlgorithm::isGlucoseLevelStable(double glucose, double target) {
    return (glucose >= target - 0.5 && glucose <= target + 0.5);
}


