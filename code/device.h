#ifndef DEVICE_H
#define DEVICE_H

#include <QMainWindow>
#include <unistd.h>
#include <batterymanager.h>
#include <datalogger.h>
#include <insulinreserve.h>
#include <cgmreader.h>
#include <profile.h>
#include <pumpcontroller.h>
#include "iobtracker.h"
#include <userinterface.h>
#include "alert.h"
#include "bloodstream.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Device; }
QT_END_NAMESPACE

class Device : public QMainWindow
{
    Q_OBJECT
public:
    explicit Device(QWidget *parent = nullptr);

public slots:
	void power();
    void noPower();
    void tick();
    void startMonitoring();

private:
    int static const simulationRate = 1; // a rate of 1 means 1 second represents 5 minutes

    bool poweredOn;
    bool monitoring;
    BatteryManager *battery;
    DataLogger *logger;
    InsulinReserve *insulin;
    CGMReader *cgm;
    Bloodstream *bloodstream;
    ControlIQAlgorithm *controlIQ;
    Profile *profiles;
    IOBTracker *iobTracker;
    PumpController *pump;
    UserInterface *interface;
    Alert *alerts;
    Ui::Device *window;
    QTimer *tickClock;
    bool batteryAlertShown= false;
    bool insulinAlertShown= false;
    bool cgmAlertShown= false;
    bool lowGlucoseAlertShown= false;
    bool highGlucoseAlertShown= false;

    void monitor();
    void safetyChecks(double glucose, double target);
};

#endif // DEVICE_H
