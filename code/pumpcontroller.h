#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#include "insulinreserve.h"
#include "datalogger.h"
#include "iobtracker.h"
#include "bloodstream.h"
#include <QObject>
#include <QCheckBox>

/**
 * controller for insulin pump operations
 */
class PumpController: public QObject
{
    Q_OBJECT
public:
/**
     *  Constructs a PumpController.
     *  insulin Pointer to shared InsulinReserve object.
     *  logger Pointer to shared DataLogger for event logging.
     *  iob Pointer to IOBTracker to track insulin on board.
     *  errorCheckBox UI checkbox used to signal occlusion/emergency.
     * parent Parent QObject.
     */
    explicit PumpController(InsulinReserve *insulin, DataLogger *logger, IOBTracker *iob, QCheckBox *errorCheckBox, QObject *parent = nullptr);
/**
 * initiates a bolus delivery
 */
    void deliverBolus(double amount, double rate, bool suppressTime = false);
    /**
     * adjusts basal rate insulin delivery rate. basal rate in units/hour
     */
    void adjustBasalRate(double rate);
    /**
     * suspends the current bolus delivery and emits a cancellation signal
     */
    void suspendBolus();
    /**
     * resumes a suspended bolus if emergency stop is not active
     */
    void resumeBolus();
    int checkDeviceStatus(); //0 = OK, 1 = Suspended, 2 = Emergency
    void triggerEmergencyStop(); //activates an emergency stop, halting all insulin delivery
    void pump(Bloodstream *blood); //simulation loop or single "tick"

signals:
      void bolusDeliveryProgress(double remainingBolus); //Emitted during bolus delivery to report remaining units.
      void bolusTimeRemainingUpdated(double timeRemaining); //update estimated time remaining for bolus
      void bolusCancelled(double amountDelivered); //when a bolus delivery is cancelled.

private:
    double currentBasalRate; 
    double activeBolusAmount;
    double activeBolusRate;
    bool bolusSuspended;
    bool emergencyStopped;
    bool suppressTimeUpdate = false;

    InsulinReserve *insulinReserve;
    DataLogger *logger;
    IOBTracker* iobTracker;
    QCheckBox *errorCheckBox;

    bool isSafeToDeliver(); //checks if conditions are safe for insulin delivery. true if safe
                            //false otherwise
};

#endif // PUMPCONTROLLER_H

