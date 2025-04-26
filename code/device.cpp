#include <device.h>
#include <ui_device.h>
#include <QTimer>
#include <controliqalgorithm.h>
#include <QDateTime>
#include <QSlider>

Device::Device(QWidget *parent)
    : QMainWindow{parent}
    , simulationRate(1)
    , poweredOn(false)
    , monitoring(false)
    , paused(false)
    , battery(new BatteryManager)
    , logger(DataLogger::instance(this))
    , insulin(new InsulinReserve)
    , bloodstream(new Bloodstream)
    , controlIQ(new ControlIQAlgorithm())
    , window(new Ui::Device)
    , tickClock(new QTimer(this))
{
    window->setupUi(this);

    Profile::loadProfiles();
    Profile::initDefaultProfile();
    Profile::selectProfileById(1);

    pump = new PumpController(insulin, logger, window->pumpErrorBox);
    interface = new UserInterface(pump, window->uiWidget);
    cgm = new CGMReader(window->cgmErrorBox);

    connect(window->powerButton, &QPushButton::released, this, &Device::power);
    connect(interface, &UserInterface::deviceUnlocked, this, &Device::startMonitoring);
    connect(tickClock, &QTimer::timeout, this, &Device::tick);
    connect(window->chargeBatteryButton, &QPushButton::released, battery, &BatteryManager::chargeBattery);
    connect(window->chargeBatteryButton, &QPushButton::released, this, [](){ Alert::reset(Alert::BATTERY_LOW); });
    connect(window->chargeBatteryButton, &QPushButton::released, this, [this](){ window->chargeBatteryButton->setText("Charge battery"); });
    connect(window->refillInsulinButton, &QPushButton::released, insulin, &InsulinReserve::refillInsulin);
    connect(battery, &BatteryManager::batteryDead, this, &Device::noPower);
    connect(window->pauseButton, &QPushButton::released, this, &Device::togglePaused);
    connect(window->simRateSlider, &QSlider::valueChanged, this, &Device::setSimRate);
    connect(window->carbButton, &QPushButton::released, this, &Device::simCarbIntake);

    logger->loadLogs();

    interface->hide(); // Device starts powered off
}

void Device::power(){
    if (poweredOn || battery->getBatteryLevel() == 0){
        poweredOn = false;
        monitoring = false;
        interface->hide();
        window->powerButton->setText("Power on");
        tickClock->stop();
    } else {
        poweredOn = true;
        interface->show();
        window->powerButton->setText("Power off");
        interface->showLoginScreen();
    }
}

void Device::noPower(){
        poweredOn = false;
        monitoring = false;
        interface->hide();
        window->powerButton->setText("Power on");
        window->chargeBatteryButton->setText("Charge battery\n(battery is dead)");
        tickClock->stop();
}

void Device::startMonitoring(){
    monitoring = true;
    interface->displayHomeScreen();
    if (not paused){
        tick(); // Updates the display immediately upon showing it
        tickClock->start(1000.0 / simulationRate);
    }
}

void Device::tick(){ // Each tick represents 5 minutes
    battery->drainBattery();

    if (monitoring) {
        this->monitor();
    }
}

void Device::monitor(){
    QDateTime time = QDateTime::currentDateTime();

    double batteryLevel = battery->getBatteryLevel();
    double glucose = cgm->getCurrentGlucoseLevel(bloodstream, profiles->getActiveProfile().getCorrectionFactor());
    double target = Profile::getActiveProfile().getTargetGlucose();
    double insulinReading = insulin->getInsulinRemaining();

    safetyChecks(glucose, target);

    // Pump logic
    if (glucose != -1){
        controlIQ->analyzeGlucoseData(glucose, logger, pump);

        pump->pump(bloodstream);

        logger->logGlucose(time, glucose);
        logger->logInsulin(time, bloodstream->getIOB());
    }
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

void Device::togglePaused(){
    if (paused) {
        paused = false;
        tickClock->start(1000/simulationRate);
        window->pauseButton->setText("Pause simulation");
    } else {
        paused = true;
        tickClock->stop();
        window->pauseButton->setText("Resume simulation");
    }
}

void Device::setSimRate(int rate){
    simulationRate = rate;
    if (this->poweredOn and not paused){
        tickClock->stop();
        tickClock->start(1000 / simulationRate);
    }
    window->simRateLabel->setText("Simulation rate: " + QString::number(simulationRate) + "x");
}

void Device::simCarbIntake(){
    double ratio = Profile::getActiveProfile().getCarbRatio();
    cgm->intakeGlucose(ratio * window->carbSpinBox->value());
}
