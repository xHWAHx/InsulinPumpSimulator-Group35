#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#include "insulinreserve.h"
#include "batterymanager.h"
#include "cgmreader.h"
#include "datalogger.h"

class PumpController
{
public:
    PumpController(InsulinReserve *insulin, BatteryManager *battery, CGMReader *cgm, DataLogger *log);

    void deliverBolus(double amount, double rate);
    void adjustBasalRate(double rate);
    void suspendBolus();
    void resumeBolus();
    int checkDeviceStatus(); //0 = OK, 1 = Suspended, 2 = Emergency
    void triggerEmergencyStop();
    void pump(); //simulation loop or single "tick"

private:
    double currentBasalRate;
    double activeBolusAmount;
    double activeBolusRate;
    bool bolusSuspended;
    bool emergencyStopped;

    InsulinReserve *insulinReserve;
    BatteryManager *batteryManager;
    CGMReader *cgmReader;
    DataLogger *logger;

    bool isSafeToDeliver();
};

#endif // PUMPCONTROLLER_H

//removed internal instances + pass pointers to shared objects 4/6/2025
