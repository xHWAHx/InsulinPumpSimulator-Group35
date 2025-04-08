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

UserInterface::UserInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserInterface)
{
    ui->setupUi(this);


    loginScreen = new Login();
    logger= new DataLogger(this);
    homeScreen = new Home();
    bolusCalculator = new BolusCalculator();
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
