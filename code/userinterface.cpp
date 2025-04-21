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

UserInterface::UserInterface(PumpController* pump, IOBTracker* iob, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserInterface)
    , pumpController(pump)
{
    ui->setupUi(this);

    loginScreen = new Login();
    logger = DataLogger::instance(this);
    homeScreen = new Home();
    bolusCalculator = new BolusCalculator(pumpController, logger, cgmReader, insulinReserve, iob, this);
    settingsScreen = new Settings();
    historyScreen = new History();

    ui->pageStack->addWidget(loginScreen);
    ui->pageStack->addWidget(homeScreen);
    ui->pageStack->addWidget(bolusCalculator);
    ui->pageStack->addWidget(settingsScreen);
    ui->pageStack->addWidget(historyScreen);

    connect(homeScreen, &Home::requestBolus, this, &UserInterface::openBolusUI);
    connect(homeScreen, &Home::requestOptions, this, &UserInterface::openSettings);
    connect(homeScreen, &Home::requestStats, this, &UserInterface::openHistory);
    connect(loginScreen, &Login::deviceUnlocked, this, &UserInterface::unlock);

    connect(settingsScreen, &Settings::backToHome, this, &UserInterface::displayHomeScreen);
    connect(bolusCalculator, &BolusCalculator::backToHome, this, &UserInterface::displayHomeScreen);
    connect(historyScreen, &History::backToHome, this, &UserInterface::displayHomeScreen);
    connect(pumpController, &PumpController::bolusCancelled, this, &UserInterface::handleBolusCancelled);

    //connect(pumpController, &PumpController::bolusDeliveryProgress, this, &UserInterface::updateBolusDisplay);
    
    //pumpTimer = new QTimer(this);
    //connect(pumpTimer, &QTimer::timeout, pumpController, &PumpController::pump);
    //pumpTimer->start(1000);

    //connect(bolusCalculator, &BolusCalculator::countdownActive,this, [this](bool active){
    //if (active){
    //    pumpTimer->stop();
    //}else {pumpTimer->start(1000);}
    //});

    connect(pumpController, &PumpController::bolusDeliveryProgress,
            this, [=](double remaining, double /*rate*/, double /*delivered*/) {
            QString status = QString("Delivering: %1 U").arg(remaining, 0, 'f', 2);
            homeScreen->updateBolusStatus(status); 
    });

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

//void UserInterface::updateBolusDisplay(double remainingBolus, double rate, double deliveredThisTick)
//{
//   QString status = QString("Bolus: %1 U remaining (delivered %2 U this tick at %3 U/hr)")
//                     .arg(remainingBolus, 0, 'f', 2)
//                     .arg(deliveredThisTick, 0, 'f', 2)
//                     .arg(rate, 0, 'f', 2);
//    ui->bolusStatusLabel->setText(status);
//}


void UserInterface::updateIOB(double iob){
    homeScreen->updateIOB(iob);
}

void UserInterface:: handleBolusCancelled(double delivered){
    homeScreen-> updateBolusStatus("Bolus Cancelled");
    //homeScreen-> updateTimeLeft("--:--");
    //homeScreen-> updateIOB(iobTracker-> getCurrentIOB(QDateTime::currentDateTime()));

    if (logger){
        logger-> logEvent("Warning", QString("Bolus cancelled after delivering %1 units").arg(delivered, 0, 'f', 2));
    }
}
