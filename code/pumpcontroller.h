/**
 * @file pumpcontroller.h
 * @brief Defines the PumpController class for controlling insulin delivery.
 *
 * The PumpController manages both basal and bolus insulin injections over time,
 * enforces safety through suspension and emergency stop mechanisms,
 * and emits status signals for UI updates.
 */
#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#include "insulinreserve.h"
#include "datalogger.h"
#include "bloodstream.h"
#include <QObject>
#include <QCheckBox>

/**
 * @class PumpController
 * @brief controller for insulin pump operations
 * 
 * Manages basal and bolus insulin delivery, emergency stop, and device status.
 */
class PumpController: public QObject
{
    Q_OBJECT
public:
/**
     *  @brief Constructs a PumpController.
     *  @param insulin Pointer to shared InsulinReserve object.
     *  @param logger Pointer to shared DataLogger for event logging.
     *  @param iob Pointer to IOBTracker to track insulin on board.
     *  @param errorCheckBox UI checkbox used to signal occlusion/emergency.
     * @param parent Parent QObject.
     */
    explicit PumpController(InsulinReserve *insulin, DataLogger *logger, QCheckBox *errorCheckBox, QObject *parent = nullptr);
/**
     * @brief Initiates a bolus delivery.
     * @param amount Amount of insulin units to deliver.
     * @param rate   Delivery rate in units per hour.
     * @param suppressTime If true, suppress time updates during delivery.
 */
    void deliverBolus(double amount, double rate, bool suppressTime = false);
    
    /**
     * @brief Adjusts basal rate insulin delivery.
     * @param rate Basal rate in units per hour.
     */
    void adjustBasalRate(double rate);
    
    /**
     *  @brief Suspends the current bolus delivery.
     * Emits a cancellation signal.
     */
    void suspendBolus();
    
    /**
     * @brief resumes a suspended bolus if emergency stop is not active
     */
    void resumeBolus();
    
    /**
     * @brief Checks the current device status.
     * @return 0 if OK, 1 if Suspended, 2 if Emergency.
     */
    int checkDeviceStatus(); //0 = OK, 1 = Suspended, 2 = Emergency
    
    /**
     * @brief Activates an emergency stop, halting all insulin delivery.
     */
    void triggerEmergencyStop(); //activates an emergency stop, halting all insulin delivery
    
    /**
     * @brief Simulation loop or single "tick" of pump operation.
     * @param blood Pointer to Bloodstream where insulin is delivered.
     */
    void pump(Bloodstream *blood); //simulation loop or single "tick"

signals:
    /**
     * @brief Emitted during bolus delivery to report remaining units.
     * @param remainingBolus Remaining insulin units.
     */
      void bolusDeliveryProgress(double remainingBolus); //Emitted during bolus delivery to report remaining units.
      
    /**
     * @brief Updates estimated time remaining for bolus.
     * @param timeRemaining Estimated time remaining.
     */  
      void bolusTimeRemainingUpdated(double timeRemaining); //update estimated time remaining for bolus
      
     /**
     * @brief Emitted when a bolus delivery is cancelled.
     * @param amountDelivered Amount delivered before cancellation.
     */ 
      void bolusCancelled(double amountDelivered); //when a bolus delivery is cancelled.

private:
    double currentBasalRate; ///< Current basal delivery rate (units/hour).
    double activeBolusAmount; ///< Active bolus amount remaining (units).
    double activeBolusRate; ///< Active bolus delivery rate (units/hour).
    bool bolusSuspended; ///< True if bolus delivery is suspended.
    bool emergencyStopped; ///< True if emergency stop is active.
    bool suppressTimeUpdate = false; ///< True if time updates are suppressed.
 
    InsulinReserve *insulinReserve; ///< Shared insulin reserve.
    DataLogger *logger; ///< Shared event logger.
    QCheckBox *errorCheckBox; ///< UI checkbox for error/emergency.

    bool isSafeToDeliver(); //checks if conditions are safe for insulin delivery. true if safe
                            //false otherwise
};

#endif // PUMPCONTROLLER_H

