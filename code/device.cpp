#include "device.h"
#include "ui_device.h"
#include "QTimer"
#include <iostream>
#include "controliqalgorithm.h"
#include "QDateTime"

Device::Device(QWidget *parent)
    : QMainWindow{parent}
    , poweredOn(false)
    , battery(new BatteryManager)
    , log(new DataLogger)
    , insulin(new InsulinReserve)
    , cgm(new CGMReader)
    , pump(new PumpController(insulin, log))
    , window(new Ui::Device)
    , tickClock(new QTimer(this))
{
    window->setupUi(this);
    interface = new UserInterface(window->uiWidget);

    connect(window->powerButton, &QPushButton::released, this, &Device::power);

    connect(interface, &UserInterface::deviceUnlocked, this, &Device::startMonitoring);
    connect(tickClock, &QTimer::timeout, this, &Device::tick);

    Profile::createProfile("Default", 2, 3, 4, 5);
    Profile::selectProfileById(1);

    interface->hide(); // because device starts powered off
}

void Device::power(){
    if (poweredOn || battery->getBatteryLevel() == 0){
        poweredOn = false;
        interface->hide();
        window->powerLabel->setText("Device is powered off");
        tickClock->stop();
    } else {
        poweredOn = true;
        interface->show();
        window->powerLabel->setText("Device is powered on");

        interface->showLoginScreen();
    }
}

void Device::startMonitoring(){
    std::cout << "Starting monitoring\n" << std::flush;
    interface->displayHomeScreen();
    tickClock->start(1000);
}

void Device::tick(){
    std::cout << "Ticking\n" << std::flush;
    battery->drainBattery();
    double batteryLevel = battery->getBatteryLevel();
    double glucose = cgm->getCurrentGlucoseLevel();
    double insulinReading = insulin->getInsulinRemaining();
    QDateTime time = QDateTime::currentDateTime();
    interface->refreshStatusBar(glucose, batteryLevel, insulinReading);

    // safety checks here

    //Profile currentProfile = Profile::getActiveProfile();
    //ControlIQAlgorithm::adjustBasalRate(currentProfile.getTargetGlucose());

    pump->pump();
    //log->logGlucose(time, glucose);
    //log->logInsulin(time, inuslin);

    //log->logTick();
}
