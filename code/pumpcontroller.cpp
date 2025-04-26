#include "pumpcontroller.h"

// Holds the current basal rate applied by ControlIQ algorithm
// (If used externally) but here just resets to 0 by default.

PumpController::PumpController(InsulinReserve *insulin, DataLogger *logger, QCheckBox *errorCheckBox, QObject *parent)
    : QObject(parent),
      currentBasalRate(0.0),
      activeBolusAmount(0.0),
      activeBolusRate(0.0),
      bolusSuspended(false),
      emergencyStopped(false),
      insulinReserve(insulin),
      logger(logger),
      errorCheckBox(errorCheckBox)
{
    // No additional setup needed
}

void PumpController::deliverBolus(double amount, double rate, bool suppressTime)
{
    // Guard against unsafe conditions
    if (emergencyStopped || bolusSuspended) {
        logger->logEvent("Error", "Bolus blocked due to unsafe condition.");
        return;
    }
    // Determine how much insulin can actually be delivered
    double delivered = std::min(insulinReserve->getInsulinRemaining(), amount);
    activeBolusAmount = delivered;
    activeBolusRate = rate;
    suppressTimeUpdate = suppressTime;
    // Log the delivered bolus initiation
    logger->logEvent("Info", "Delivered " + QString::number(delivered) + " units at rate " + QString::number(rate));
}

void PumpController::adjustBasalRate(double rate)
{// Updates basal rate without immediate insulin injection.
    currentBasalRate = rate;
}

void PumpController::suspendBolus() // Cancels any ongoing bolus, emits cancellation, and logs warning.
{
    bolusSuspended = true;
    emit bolusCancelled(activeBolusAmount);
    logger-> logEvent("Warning", QString("Bolus cancelled with %1 units remaining to deliver").arg(activeBolusAmount, 0, 'f', 2));
    activeBolusAmount= 0;
}

void PumpController::resumeBolus()// Resumes bolus delivery if no emergency is present.
{
    if (!emergencyStopped) {
        bolusSuspended = false;
        logger->logEvent("Info", "Bolus delivery resumed.");
    }
}

int PumpController::checkDeviceStatus() // Returns pump status: 0=OK, 1=Suspended, 2=Emergency.
{
    if (emergencyStopped) return 2;
    if (bolusSuspended) return 1;
    return 0;
}

void PumpController::triggerEmergencyStop() // Immediately stops all insulin delivery and logs the event.
{
    emergencyStopped = true;
    logger->logEvent("Warning", "Emergency stop activated.");
}

void PumpController::pump(Bloodstream *blood) // Called on each simulation tick to deliver basal and bolus insulin.
{
    // Update emergency state from hardware error checkbox
    emergencyStopped = errorCheckBox->isChecked();

    // only pump if delivery is active + not blocked
    if (emergencyStopped) {
        //halts
        return;
    }

    if (not (bolusSuspended || activeBolusAmount <= 0)) { // If a bolus is active and not suspended, deliver a fraction this tick
        double unitsPerTick = activeBolusRate / 12.0; // Calculate insulin units delivered per 5-minute tick
        double deliveredThisTick = (activeBolusAmount < unitsPerTick) ? activeBolusAmount : unitsPerTick;
        activeBolusAmount -= deliveredThisTick;

        emit bolusDeliveryProgress(activeBolusAmount); // Notify UI

        blood->injectUnits(deliveredThisTick); // Inject bolus units into bloodstream and deduct from reserve
        insulinReserve->useInsulin(deliveredThisTick);
    }
    // Basal delivery: inject steady rate each tick
    blood->injectUnits(currentBasalRate/12);
    insulinReserve->useInsulin(currentBasalRate/12);
}
