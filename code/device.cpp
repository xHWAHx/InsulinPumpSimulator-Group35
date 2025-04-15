#include "device.h"
#include "ui_device.h"
#include "QTimer"
#include "alert.h"
#include "iobtracker.h"
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
    , iobTracker(new IOBTracker)
    , pump(new PumpController(insulin, log))
    , window(new Ui::Device)
    , tickClock(new QTimer(this))
{
    window->setupUi(this);
    interface = new UserInterface(pump, window->uiWidget);

    connect(window->powerButton, &QPushButton::released, this, &Device::power);

    connect(interface, &UserInterface::deviceUnlocked, this, &Device::startMonitoring);
    connect(tickClock, &QTimer::timeout, this, &Device::tick);

    connect(window->chargeBatteryButton, &QPushButton::released, battery, &BatteryManager::chargeBattery);
    connect(window->refillInsulinButton, &QPushButton::released, insulin, &InsulinReserve::refillInsulin);

    Profile::initDefaultProfile();
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
    tickClock->start(2250);
}

void Device::tick(){
   static int tickCounter = 0;
   tickCounter++;

   std::cout << "Ticking\n" << std::flush;

   if (tickCounter >= 60) {
       battery->drainBattery();
       tickCounter = 0;
   }

   double batteryLevel = battery->getBatteryLevel();
   double glucose = cgm->getCurrentGlucoseLevel();
   double insulinReading = insulin->getInsulinRemaining();
   double currentIOB= iobTracker-> getCurrentIOB(QDateTime::currentDateTime());
   QDateTime time = QDateTime::currentDateTime();
   interface->refreshStatusBar(glucose, batteryLevel, insulinReading);
   interface->updateIOB(currentIOB);


   if (batteryLevel <= 0.15) {
       if (!batteryAlertShown) {
           batteryAlertShown= true;
           Alert *lowBattery = new Alert(this);
           lowBattery->showAlert("Low Battery", "Please recharge or replace the pump's battery.");
       }
   }

   if (insulinReading <= 10.0) {
       if(!insulinAlertShown) {
           insulinAlertShown= true;
           Alert *lowInsulin = new Alert(this);
           lowInsulin->showAlert("Low Insulin", "Insulin is running low. Please refill the reservoir.");
   }
}

   if (!cgm->isCGMConnected()) {
       if (!cgmAlertShown) {
           cgmAlertShown= true;
           Alert *cgmDisconnected = new Alert(this);
           cgmDisconnected->showAlert("CGM Disconnected", "Check CGM sensor connection.");
   }
}

   //pump logic
   pump->pump();

}


