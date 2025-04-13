#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#include "insulinreserve.h"
#include "datalogger.h"
#include <QObject>

class PumpController: public QObject
{
    Q_OBJECT
public:
    explicit PumpController(InsulinReserve *insulin, DataLogger *log, QObject *parent = nullptr);

    void deliverBolus(double amount, double rate);
    void adjustBasalRate(double rate);
    void suspendBolus();
    void resumeBolus();
    int checkDeviceStatus(); //0 = OK, 1 = Suspended, 2 = Emergency
    void triggerEmergencyStop();
    void pump(); //simulation loop or single "tick"

signals:
      void bolusDeliveryProgress(double remainingBolus, double rate, double deliveredThisTick);
      // new signal added 13/04/2025
      void bolusTimeRemainingUpdated(double seconds);  

private:
    double currentBasalRate;
    double activeBolusAmount;
    double activeBolusRate;
    bool bolusSuspended;
    bool emergencyStopped;

    InsulinReserve *insulinReserve;
    DataLogger *logger;

    bool isSafeToDeliver();
};

#endif // PUMPCONTROLLER_H

//removed internal instances + pass pointers to shared objects 4/6/2025
