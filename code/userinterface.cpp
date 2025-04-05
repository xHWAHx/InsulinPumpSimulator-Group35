#include "userinterface.h"
#include "login.h"
#include "HomeWindow.h"
#include "boluscalculator.h"
#include "settings.h"
#include "alert.h"
#include <QMessageBox>
#include <QDateTime>

UserInterface::UserInterface(QStackedWidget *stack, QObject *parent)
    : QObject(parent), stackedWidget(stack)
{
    loginScreen = new Login();
    homeScreen = new HomeWindow();

    pumpController = new PumpController();
    bolusCalculator = new BolusCalculator();
    cgmReader = new CGMReader();
    batteryManager = new BatteryManager();
    insulinReserve = new InsulinReserve();
    logger = new datalogger();
    controlIQ = new ControlIQAlgorithm();

    stackedWidget->addWidget(loginScreen);
    stackedWidget->addWidget(homeScreen);

    connect(loginScreen, &Login::pinSubmitted, this, &UserInterface::handlePin);
    connect(homeScreen, &HomeWindow::requestBolus, this, &UserInterface::openBolusUI);
    connect(homeScreen, &HomeWindow::requestOptions, this, &UserInterface::openSettings);
    connect(homeScreen, &HomeWindow::requestEmergencyStop, this, &UserInterface::triggerEmergencyStop);
    connect(homeScreen, &HomeWindow::requestStats, this, &UserInterface::showControlIQStats);
    connect(homeScreen, &HomeWindow::requestStatusRefresh, this, &UserInterface::refreshStatusBar);
    connect(homeScreen, &HomeWindow::requestCGMValue, this, &UserInterface::updateGlucoseForChart);

    showLoginScreen();
}

void UserInterface::showLoginScreen() {
    stackedWidget->setCurrentWidget(loginScreen);
}

void UserInterface::displayHomeScreen() {
    stackedWidget->setCurrentWidget(homeScreen);
}

bool UserInterface::checkPin(int pin) {
    return pin == correctPin;
}

void UserInterface::handlePin(int pin) {
    if (checkPin(pin)) {
        displayHomeScreen();
    } else {
        displayError("Incorrect PIN. Try again.");
    }
}

void UserInterface::displayError(const QString &message) {
    QMessageBox::critical(nullptr, "Error", message);
}

void UserInterface::refreshStatusBar() {
    double glucose = cgmReader->getCurrentGlucoseLevel();
    double battery = batteryManager->getBatteryLevel();
    double insulin = insulinReserve->getInsulinRemaining();

    if (batteryManager->isBatteryCritical() || insulinReserve->isInsulinLow() || !cgmReader->isCGMConnected()) {
        Alert *alert = new Alert();
        alert->show();
    }

    homeScreen->updateStatus(glucose, battery, insulin);
    logger->logGlucose(QDateTime::currentDateTime(), glucose, 1);
}

void UserInterface::updateGlucoseForChart() {
    double glucose = cgmReader->getCurrentGlucoseLevel();
    homeScreen->addGlucoseReading(glucose);
}

void UserInterface::showControlIQStats() {
    std::vector<double> data = { cgmReader->getCurrentGlucoseLevel() };
    controlIQ->analyzeGlucoseData(data);
    QMessageBox::information(nullptr, "ControlIQ", "Stats updated");
}

void UserInterface::openBolusUI() {
    BolusCalculatorUI *bolus = new BolusCalculatorUI();
    bolus->show();
}

void UserInterface::openSettings() {
    Settings *settings = new Settings();
    settings->show();
}

void UserInterface::triggerEmergencyStop() {
    pumpController->triggerEmergencyStop();
    displayError("Emergency stop triggered!");
}
