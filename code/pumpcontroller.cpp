#include "pumpcontroller.h"
#include "iobtracker.h"
#include <iostream>
#include <cmath>

PumpController::PumpController(InsulinReserve *insulin, DataLogger *logger, IOBTracker* iob, QObject *parent)
    : QObject(parent),
      currentBasalRate(0.0),
      activeBolusAmount(0.0),
      activeBolusRate(0.0),
      bolusSuspended(false),
      emergencyStopped(false),
      insulinReserve(insulin),
      logger(logger),
      iobTracker(iob)
{
}

void PumpController::deliverBolus(double amount, double rate, bool suppressTime)
{
    if (emergencyStopped || bolusSuspended) {
        logger->logEvent("Error", "Bolus blocked due to unsafe condition.");
        return;
    }

    double delivered = std::min(insulinReserve->getInsulinRemaining(), amount);
    activeBolusAmount = delivered;
    activeBolusRate = rate;
    suppressTimeUpdate = suppressTime;

    //if (iobTracker){
    //    iobTracker-> addBolus(delivered, QDateTime::currentDateTime());
    //}

    logger->logEvent("Bolus", "Delivered " + QString::number(delivered) + " units at rate " + QString::number(rate));
}

void PumpController::adjustBasalRate(double rate)
{
    currentBasalRate = rate;
    //logger.logEvent("Basal", "Adjusted to " + QString::number(rate) + " U/hr");
}

void PumpController::suspendBolus()
{
    bolusSuspended = true;
    emit bolusCancelled(activeBolusAmount);
    activeBolusAmount= 0;
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

void PumpController::pump(Bloodstream *blood)
{
    //only pump if delivery is active + not blocked
    if (not (emergencyStopped || bolusSuspended || activeBolusAmount <= 0)) {
        //const double tickIntervalSec = 300.0; //tick interval
        double unitsPerTick = activeBolusRate / 12.0;
        double deliveredThisTick = (activeBolusAmount < unitsPerTick) ? activeBolusAmount : unitsPerTick;
        activeBolusAmount -= deliveredThisTick;

        emit bolusDeliveryProgress(activeBolusAmount, activeBolusRate, deliveredThisTick);

        //if (!suppressTimeUpdate) {
        //    double estimatedTimeRemaining = activeBolusAmount / (activeBolusRate / 12.0);
        //    emit bolusTimeRemainingUpdated(estimatedTimeRemaining);
        //}

        if (activeBolusAmount > 0){
            double estimatedTimeRemaining = activeBolusAmount / (activeBolusRate/ 12.0);
            emit bolusTimeRemainingUpdated(estimatedTimeRemaining);
        } else {
            emit bolusTimeRemainingUpdated(0.0);
            emit bolusCancelled(activeBolusAmount);
        }

        blood->injectUnits(deliveredThisTick);
        insulinReserve->useInsulin(deliveredThisTick);
    }
    blood->injectUnits(currentBasalRate/12);
    insulinReserve->useInsulin(currentBasalRate/12);

    /*std::cout << "Pumping " << deliveredThisTick << " units this tick at rate " << activeBolusRate << " U/hr... ";
    if (activeBolusAmount > 0) {
            // Estimate the remaining time (in seconds) required for the remaining bolus.
        if (!suppressTimeUpdate){
            double estimatedTimeRemaining = activeBolusAmount / (activeBolusRate / 3600.0);
            std::cout << "Remaining bolus: " << activeBolusAmount << " units. Estimated time remaining: " << estimatedTimeRemaining << " seconds." << std::endl;
            emit bolusTimeRemainingUpdated(estimatedTimeRemaining);
        } else {
            std::cout << "Bolus delivery complete." << std::endl;
            emit bolusTimeRemainingUpdated(0.0);
        }
    }*/
}
