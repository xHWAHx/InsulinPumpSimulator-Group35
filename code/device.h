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
    void tick();
    void startMonitoring();

signals:

private:
	bool poweredOn;
    BatteryManager *battery;
    DataLogger *logger;
    InsulinReserve *insulin;
    CGMReader *cgm;
    Profile *profiles;
    IOBTracker *iobTracker;
    PumpController *pump;
    UserInterface *interface;
    Ui::Device *window;
    QTimer *tickClock;
    bool batteryAlertShown= false;
    bool insulinAlertShown= false;
    bool cgmAlertShown= false;
    bool lowGlucoseAlertShown= false;
    bool highGlucoseAlertShown= false;


};

#endif // DEVICE_H
