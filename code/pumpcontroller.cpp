#include "pumpcontroller.h"
#include <iostream>

PumpController::PumpController(InsulinReserve *insulin, DataLogger *log)
    : insulinReserve(insulin),
      //batteryManager(battery),
      logger(log),
      currentBasalRate(0.0),
      activeBolusAmount(0.0),
      activeBolusRate(0.0),
      bolusSuspended(false),
      emergencyStopped(false)
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
    if (!emergencyStopped && !bolusSuspended && activeBolusAmount > 0) {
        //double glucose = cgmReader.getCurrentGlucoseLevel();
        //logger.logEvent("CGM", "Current glucose: " + QString::number(glucose));

        /**if (glucose < 3.9) {
            suspendBolus();
            //logger.logEvent("Safety", "Bolus auto-suspended (hypoglycemia).");
            return;
        }*/

        std::cout << "Pumping active bolus @ " << activeBolusRate << " U/hr...\n";
        //logger.logTick(LogEntry{});
    }
}
