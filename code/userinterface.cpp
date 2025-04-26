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
#include "alert.h"

UserInterface::UserInterface(PumpController* pump, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserInterface)
    , pumpController(pump)
{
    ui->setupUi(this);

    loginScreen = new Login();
    logger = DataLogger::instance(this);
    homeScreen = new Home();
    bolusCalculator = new BolusCalculator(pumpController, logger, cgmReader, insulinReserve, this);
    settingsScreen = new Settings();
    historyScreen = new History();

    ui->pageStack->addWidget(loginScreen);
    ui->pageStack->addWidget(homeScreen);
    ui->pageStack->addWidget(bolusCalculator);
    ui->pageStack->addWidget(settingsScreen);
    ui->pageStack->addWidget(historyScreen);

    connect(loginScreen, &Login::deviceUnlocked, this, &UserInterface::unlock);

    connect(homeScreen, &Home::requestBolus, this, &UserInterface::openBolusUI);
    connect(homeScreen, &Home::requestOptions, this, &UserInterface::openSettings);
    connect(homeScreen, &Home::requestStats, this, &UserInterface::openHistory);

    connect(settingsScreen, &Settings::backToHome, this, &UserInterface::displayHomeScreen);
    connect(bolusCalculator, &BolusCalculator::backToHome, this, &UserInterface::displayHomeScreen);
    connect(historyScreen, &History::backToHome, this, &UserInterface::displayHomeScreen);

    connect(pumpController, &PumpController::bolusCancelled, this, &UserInterface::handleBolusCancelled);
    connect(pumpController, &PumpController::bolusDeliveryProgress, this, &UserInterface::updateBolusDisplay);
    connect(pumpController, &PumpController::bolusTimeRemainingUpdated, homeScreen, &Home::updateBolusTimeRemaining);

    connect(bolusCalculator, &BolusCalculator::bolusStarted, homeScreen, &Home::updateBolusStatus);

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

void UserInterface::refresh(double glucose, double battery, double insulin, double iob) {
    homeScreen->updateStatus(glucose, battery, insulin);
    homeScreen->updateIOB(iob);
    bolusCalculator->updateCountdown();
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

void UserInterface::showAlert(Alert *alert) {
    if (ui->pageStack->currentIndex() < 5){
        lastPage = ui->pageStack->currentWidget();
    }
    ui->pageStack->addWidget(alert);
    ui->pageStack->setCurrentWidget(alert);
}

void UserInterface::dismissAlert(Alert *alert) {
    ui->pageStack->removeWidget(alert);
    delete alert;
    int numPages = ui->pageStack->count();
    if (numPages > 5){
        ui->pageStack->setCurrentIndex(numPages);
    } else {
        ui->pageStack->setCurrentWidget(lastPage);
    }
}

void UserInterface::updateBolusDisplay(double remainingBolus) {
    if (remainingBolus == 0) {
        homeScreen->updateBolusStatus(QString("Bolus complete"));
    } else {
        homeScreen->updateBolusStatus(QString("Bolus remaining:\n%1 U").arg(remainingBolus, 0, 'f', 2));
    }
}

void UserInterface::updateIOB(double iob){
    homeScreen->updateIOB(iob);
}

void UserInterface::handleBolusCancelled(){
    homeScreen->updateBolusStatus("Bolus Cancelled");
}
