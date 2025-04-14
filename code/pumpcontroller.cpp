#include "pumpcontroller.h"
#include <iostream>
#include <cmath>

PumpController::PumpController(InsulinReserve *insulin, DataLogger *log, QObject *parent)
    : QObject(parent),
      currentBasalRate(0.0),
      activeBolusAmount(0.0),
      activeBolusRate(0.0),
      bolusSuspended(false),
      emergencyStopped(false),
      insulinReserve(insulin),
      logger(log)
{
}

void PumpController::deliverBolus(double amount, double rate)
{
    if (emergencyStopped || bolusSuspended) {
        //logger.logEvent("Error", "Bolus blocked due to unsafe condition.");
        return;
    }

    double delivered = insulinReserve->useInsulin(amount);
    activeBolusAmount = delivered;
    activeBolusRate = rate;

    //logger.logEvent("Bolus", "Delivered " + QString::number(delivered) + " units at rate " + QString::number(rate));
}

void PumpController::adjustBasalRate(double rate)
{
    currentBasalRate = rate;
    //logger.logEvent("Basal", "Adjusted to " + QString::number(rate) + " U/hr");
}

void PumpController::suspendBolus()
{
    bolusSuspended = true;
    //logger.logEvent("System", "Bolus delivery suspended.");
}

void PumpController::resumeBolus()
{
    if (!emergencyStopped) {
        bolusSuspended = false;
        //logger.logEvent("System", "Bolus delivery resumed.");
    }
}

int PumpController::checkDeviceStatus()
{
    if (emergencyStopped) return 2;
    if (bolusSuspended) return 1;
    return 0;
}

void PumpController::triggerEmergencyStop()
{
    emergencyStopped = true;
    //logger.logEvent("Critical", "Emergency stop activated.");
}

void PumpController::pump()
{
    //only pump if delivery is active + not blocked
    if (emergencyStopped || bolusSuspended || activeBolusAmount <= 0) {
           return;
    }
    const double tickIntervalSec = 1.0; //tick interval
    double unitsPerTick = activeBolusRate / 3600.0 * tickIntervalSec;
    double deliveredThisTick = (activeBolusAmount < unitsPerTick) ? activeBolusAmount : unitsPerTick;
    activeBolusAmount -= deliveredThisTick;

    emit bolusDeliveryProgress(activeBolusAmount, activeBolusRate, deliveredThisTick);

    std::cout << "Pumping " << deliveredThisTick << " units this tick at rate " << activeBolusRate << " U/hr... ";
    if (activeBolusAmount > 0) {
            // Estimate the remaining time (in seconds) required for the remaining bolus.
            double estimatedTimeRemaining = activeBolusAmount / (activeBolusRate / 3600.0);
            std::cout << "Remaining bolus: " << activeBolusAmount << " units. Estimated time remaining: " << estimatedTimeRemaining << " seconds." << std::endl;
            emit bolusTimeRemainingUpdated(estimatedTimeRemaining);
        } else {
            std::cout << "Bolus delivery complete." << std::endl;
            emit bolusTimeRemainingUpdated(0.0);
        }

}

//try
