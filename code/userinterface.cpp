#include "userinterface.h"
#include "ui_userinterface.h"
#include "login.h"
#include "home.h"
#include "boluscalculator.h"
#include "settings.h"
#include "history.h"
#include "pumpcontroller.h"
#include "datalogger.h"
#include "controliqalgorithm.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

UserInterface::UserInterface(PumpController* pump, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserInterface)
    , pumpController(pump)
{
    ui->setupUi(this);


    loginScreen = new Login();
    logger= new DataLogger(this);
    homeScreen = new Home();
    bolusCalculator = new BolusCalculator(pumpController, logger, cgmReader, insulinReserve, this); 
    settingsScreen = new Settings();
    historyScreen = new History(logger);

    ui->pageStack->addWidget(loginScreen);
    ui->pageStack->addWidget(homeScreen);
    ui->pageStack->addWidget(bolusCalculator);
    ui->pageStack->addWidget(settingsScreen);
    ui->pageStack->addWidget(historyScreen);

    connect(homeScreen, &Home::requestBolus, this, &UserInterface::openBolusUI);
    connect(homeScreen, &Home::requestOptions, this, &UserInterface::openSettings);
    connect(homeScreen, &Home::requestStats, this, &UserInterface::openHistory);
    connect(homeScreen, &Home::requestEmergencyStop, this, &UserInterface::triggerEmergencyStop);
    connect(loginScreen, &Login::deviceUnlocked, this, &UserInterface::unlock);

    connect(settingsScreen, &Settings::backToHome, this, &UserInterface::displayHomeScreen);
    connect(bolusCalculator, &BolusCalculator::backToHome, this, &UserInterface::displayHomeScreen);
    connect(historyScreen, &History::backToHome, this, &UserInterface::displayHomeScreen);

    //connect(pumpController, &PumpController::bolusDeliveryProgress, this, &UserInterface::updateBolusDisplay);

    connect(pumpController, &PumpController::bolusDeliveryProgress, this, [=](double remaining, double rate, double delivered) {
    QString status = QString("💉 %1 U left (delivered %2 U @ %3 U/hr)")
                        .arg(remaining, 0, 'f', 2)
                        .arg(delivered, 0, 'f', 2)
                        .arg(rate, 0, 'f', 2);
    homeScreen->updateBolusStatus(status);
    });

    connect(pumpController, &PumpController::bolusTimeRemainingUpdated, homeScreen, &Home::updateBolusTimeRemaining);

    //showLoginScreen();
}

UserInterface::~UserInterface() {
    delete ui;
}

void UserInterface::unlock() {
    emit deviceUnlocked();
}

void UserInterface::showLoginScreen() {
    ui->pageStack->setCurrentWidget(loginScreen);
}

void UserInterface::displayHomeScreen() {
    ui->pageStack->setCurrentWidget(homeScreen);
}

void UserInterface::displayError(const QString &message) {
    QMessageBox::critical(nullptr, "Error", message);
}

void UserInterface::refreshStatusBar(double glucose, double battery, double insulin) {
    homeScreen->updateStatus(glucose, battery, insulin);
    this->updateGlucoseForChart(glucose);
}

void UserInterface::updateGlucoseForChart(double glucose) {
    homeScreen->addGlucoseReading(glucose);
}

void UserInterface::openBolusUI() {
    ui->pageStack->setCurrentWidget(bolusCalculator);
}

void UserInterface::openSettings() {
    ui->pageStack->setCurrentWidget(settingsScreen);
}

void UserInterface::openHistory() {
    ui->pageStack->setCurrentWidget(historyScreen);
}

void UserInterface::triggerEmergencyStop() {
    displayError("Emergency stop triggered!");
}

//void UserInterface::updateBolusDisplay(double remainingBolus, double rate, double deliveredThisTick)
//{
//   QString status = QString("Bolus: %1 U remaining (delivered %2 U this tick at %3 U/hr)")
//                     .arg(remainingBolus, 0, 'f', 2)
//                     .arg(deliveredThisTick, 0, 'f', 2)
//                     .arg(rate, 0, 'f', 2);
//    ui->bolusStatusLabel->setText(status);
//}
