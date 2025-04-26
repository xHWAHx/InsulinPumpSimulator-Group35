#ifndef DEVICE_H
#define DEVICE_H

#include <QMainWindow>
//#include <unistd.h>
#include <batterymanager.h>
#include <datalogger.h>
#include <insulinreserve.h>
#include <cgmreader.h>
#include <profile.h>
#include <pumpcontroller.h>
#include <userinterface.h>
#include <alert.h>
#include <bloodstream.h>

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
    void togglePaused();
    void simCarbIntake();

private:
    int simulationRate; // a rate of 1 means 1 second represents 5 minutes

    bool poweredOn;
    bool monitoring;
    bool paused;
    BatteryManager *battery;
    DataLogger *logger;
    InsulinReserve *insulin;
    CGMReader *cgm;
    Bloodstream *bloodstream;
    ControlIQAlgorithm *controlIQ;
    Profile *profiles;
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
    void setSimRate(int rate);
};

#endif // DEVICE_H
