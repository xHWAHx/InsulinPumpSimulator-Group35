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
    , battery(new BatteryManager)
    , logger(DataLogger::instance(this))
    , insulin(new InsulinReserve)
    , bloodstream(new Bloodstream)
    , controlIQ(new ControlIQAlgorithm())
    //, iobTracker(new IOBTracker(10.0))
    , window(new Ui::Device)
    , tickClock(new QTimer(this))
{
    window->setupUi(this);

    Profile::loadProfiles();
    Profile::initDefaultProfile();
    Profile::selectProfileById(1);

    pump = new PumpController(insulin, logger, iobTracker, window->pumpErrorBox);
    interface = new UserInterface(pump, iobTracker, window->uiWidget);
    cgm = new CGMReader(window->cgmErrorBox);

    connect(window->powerButton, &QPushButton::released, this, &Device::power);
    connect(interface, &UserInterface::deviceUnlocked, this, &Device::startMonitoring);
    connect(tickClock, &QTimer::timeout, this, &Device::tick);
    connect(window->chargeBatteryButton, &QPushButton::released, battery, &BatteryManager::chargeBattery);
    connect(window->chargeBatteryButton, &QPushButton::released, this, [](){ Alert::reset(Alert::BATTERY_LOW); });
    connect(window->refillInsulinButton, &QPushButton::released, this, [this](){ insulin->refillInsulin(); iobTracker-> clear(); });
    connect(battery, &BatteryManager::batteryDead, this, &Device::noPower);

    logger->loadLogs();

    interface->hide(); // device starts powered off
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

void Device::noPower(){
        poweredOn = false;
        monitoring = false;
        interface->hide();
        window->powerLabel->setText("Device is powered off  (battery died)");
        tickClock->stop();
}

void Device::startMonitoring(){
    monitoring = true;
    interface->displayHomeScreen();
    tick(); // updates the display immediately upon showing it
    tickClock->start(1000.0 / simulationRate);
}

void Device::tick(){ // each tick represents 5 minutes
    battery->drainBattery();

    if (monitoring) {
        this->monitor();
    }
}

void Device::monitor(){
    QDateTime time = QDateTime::currentDateTime();

    double batteryLevel = battery->getBatteryLevel();
    double glucose = cgm->getCurrentGlucoseLevel(bloodstream, profiles->getActiveProfile().getCorrectionFactor());
    double target= Profile::getActiveProfile().getTargetGlucose();
    double insulinReading = insulin->getInsulinRemaining();

    safetyChecks(glucose, target);

    //pump logic
    if (glucose != -1){
        controlIQ->analyzeGlucoseData(glucose, logger, pump);

        pump->pump(bloodstream);

        logger->logGlucose(time, glucose);
    }
    //logger->logInsulin(time, currentIOB);
    double currentIOB = bloodstream->getIOB();

    interface->refresh(glucose, batteryLevel, insulinReading, currentIOB);
}

void Device::safetyChecks(double glucose, double target){
    if (battery->isBatteryCritical()){
        alerts->raise(Alert::BATTERY_LOW, interface, logger);
    }

    if (insulin->isInsulinLow()){
        alerts->raise(Alert::INSULIN_LOW, interface, logger);
    }

    if (cgm->isCGMConnected()) {
        alerts->reset(Alert::CGM_DISCONNECTED);
    } else {
        alerts->raise(Alert::CGM_DISCONNECTED, interface, logger);
    }

    if (not window->pumpErrorBox->isChecked()) {
        alerts->reset(Alert::PUMP_OCCLUSION);
    } else {
        alerts->raise(Alert::PUMP_OCCLUSION, interface, logger);
    }

    if (glucose == -1){
        pump->suspendBolus();  // Stop insulin delivery temporarily
    } else if (glucose < 3.9) {
        pump->suspendBolus();  // Stop insulin delivery temporarily
        alerts->raise(Alert::GLUCOSE_LOW, interface, logger);
    } else if (glucose > target + 2.0) {
        alerts->raise(Alert::GLUCOSE_HIGH, interface, logger);
    } else if (glucose <= target + 0.5 and glucose >= target -0.5) {
        alerts->reset(Alert::GLUCOSE_HIGH);
        alerts->reset(Alert::GLUCOSE_LOW);
    }
}
