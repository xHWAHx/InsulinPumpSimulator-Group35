#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#include "insulinreserve.h"
#include "datalogger.h"
#include "iobtracker.h"
#include "bloodstream.h"
#include <QObject>
#include <QCheckBox>

class PumpController: public QObject
{
    Q_OBJECT
public:
    explicit PumpController(InsulinReserve *insulin, DataLogger *logger, IOBTracker *iob, QCheckBox *errorCheckBox, QObject *parent = nullptr);

    void deliverBolus(double amount, double rate, bool suppressTime = false);
    void adjustBasalRate(double rate);
    void suspendBolus();
    void resumeBolus();
    int checkDeviceStatus(); //0 = OK, 1 = Suspended, 2 = Emergency
    void triggerEmergencyStop();
    void pump(Bloodstream *blood); //simulation loop or single "tick"

signals:
      void bolusDeliveryProgress(double remainingBolus);
      void bolusTimeRemainingUpdated(double timeRemaining);
      void bolusCancelled(double amountDelivered);

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

    bool isSafeToDeliver();
};

#endif // PUMPCONTROLLER_H

//removed internal instances + pass pointers to shared objects 4/6/2025
