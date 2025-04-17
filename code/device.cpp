#include "device.h"
#include "ui_device.h"
#include "QTimer"
#include "alert.h"
#include "iobtracker.h"
#include <iostream>
#include "controliqalgorithm.h"
#include "QDateTime"
#include <QSlider>

Device::Device(QWidget *parent)
    : QMainWindow{parent}
    , poweredOn(false)
    , monitoring(false)
    , simulationRate(1)
    , battery(new BatteryManager)
    , logger(DataLogger::instance(this))
    , insulin(new InsulinReserve)
    , cgm(new CGMReader)
    , iobTracker(new IOBTracker)
    , pump(new PumpController(insulin, logger, iobTracker))
    , window(new Ui::Device)
    , tickClock(new QTimer(this))
{
    window->setupUi(this);
    interface = new UserInterface(pump, iobTracker, window->uiWidget);

    connect(window->powerButton, &QPushButton::released, this, &Device::power);
    connect(interface, &UserInterface::deviceUnlocked, this, &Device::startMonitoring);
    connect(tickClock, &QTimer::timeout, this, &Device::tick);
    connect(window->chargeBatteryButton, &QPushButton::released, battery, &BatteryManager::chargeBattery);
    connect(window->rateSlider, &QSlider::valueChanged, this, &Device::setSimRate);
    //connect(window->refillInsulinButton, &QPushButton::released, this, [=](){ insulin-> refillInsulin(); iobTracker-> clear(); });

    Profile::initDefaultProfile();
    Profile::selectProfileById(1);

    logger->loadLogs();

    interface->hide(); // because device starts powered off
}

void Device::power(){
    if (poweredOn || battery->getBatteryLevel() == 0){
        poweredOn = false;
        monitoring = false;
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
    monitoring = true;
    interface->displayHomeScreen();
    tickClock->start(1000.0 / simulationRate);
}

void Device::tick(){
    battery->drainBattery();

    if (monitoring) {
        this->monitor();
    }
}

void Device::monitor(){

    double batteryLevel = battery->getBatteryLevel();
    double glucose = cgm->getCurrentGlucoseLevel();
    double target= Profile::getActiveProfile().getTargetGlucose();
    double insulinReading = insulin->getInsulinRemaining();
    QDateTime time = QDateTime::currentDateTime();

    if (batteryLevel <= 0.15) {
        if (!batteryAlertShown) {
            batteryAlertShown= true;
            Alert *lowBattery = new Alert(this);
            lowBattery->showAlert("Low Battery", "Please recharge or replace the pump's battery.");
            logger->logEvent("Warning", QString("Low Battery"));
        }
    }

    if (insulinReading <= 10.0) {
        if(!insulinAlertShown) {
            insulinAlertShown= true;
            Alert *lowInsulin = new Alert(this);
            lowInsulin->showAlert("Low Insulin", "Insulin is running low. Please refill the reservoir.");
            logger->logEvent("Warning", QString("Low Insulin"));
    }
    }

       if (!cgm->isCGMConnected()) {
           if (!cgmAlertShown) {
           cgmAlertShown= true;
           Alert *cgmDisconnected = new Alert(this);
           cgmDisconnected->showAlert("CGM Disconnected", "Check CGM sensor connection.");
       }
    }

    if (glucose < 3.9 && !lowGlucoseAlertShown) {
        lowGlucoseAlertShown = true;
        pump->suspendBolus();  // Stop insulin delivery temporarily
        Alert* alert = new Alert(this);
        alert->showAlert("Low Glucose",
            "Glucose is below 3.9 mmol/L. Take 15g of fast-acting sugar. Bolus suspended.");
    }


    if (glucose > target + 2.0 && !highGlucoseAlertShown) {
        highGlucoseAlertShown = true;
        Alert* alert = new Alert(this);
        alert->showAlert("High Glucose",
            "Glucose is above target. Consider using the Bolus Calculator.");
    }

    if(glucose >= 4.0 && glucose <= target + 1.5){
        lowGlucoseAlertShown= false;
        highGlucoseAlertShown= false;
    }


    //pump logic
    pump->pump();
    double currentIOB= iobTracker-> getCurrentIOB(QDateTime::currentDateTime());

    interface->refreshStatusBar(glucose, batteryLevel, insulinReading);
    interface->updateIOB(currentIOB);

}

void Device::setSimRate(int rate){
    simulationRate = ceil((pow(double(rate)/10.0, 2)));
    if (this->poweredOn){
        tickClock->stop();
        tickClock->start(1000 / simulationRate);
    }
    window->rateLabel->setText("Simulation rate: " + QString::number(simulationRate) + "x");
}

