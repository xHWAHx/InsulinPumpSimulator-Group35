#include "userinterface.h"
#include "ui_userinterface.h"
#include "login.h"
#include "home.h"
#include "boluscalculator.h"
#include "settings.h"
//#include "alert.h" //
#include "pumpcontroller.h"
#include "datalogger.h"
#include "controliqalgorithm.h"
#include <QMessageBox>
#include <QDateTime>

UserInterface::UserInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserInterface)
{
    ui->setupUi(this);

    loginScreen = new Login();
    homeScreen = new Home();

    ui->pageStack->addWidget(loginScreen);
    ui->pageStack->addWidget(homeScreen);

    //connect(loginScreen, &Login::pinSubmitted, this, &UserInterface::handlePin);
    connect(homeScreen, &Home::requestBolus, this, &UserInterface::openBolusUI);
    connect(homeScreen, &Home::requestOptions, this, &UserInterface::openSettings);
    connect(homeScreen, &Home::requestEmergencyStop, this, &UserInterface::triggerEmergencyStop);
    connect(homeScreen, &Home::requestStats, this, &UserInterface::showControlIQStats);
    connect(homeScreen, &Home::requestStatusRefresh, this, &UserInterface::refreshStatusBar);
    connect(homeScreen, &Home::requestCGMValue, this, &UserInterface::updateGlucoseForChart);


}

UserInterface::~UserInterface(){
    delete ui;
}

void UserInterface::showLoginScreen() {
    ui->pageStack->setCurrentWidget(loginScreen);
}

void UserInterface::displayHomeScreen() {
    ui->pageStack->setCurrentWidget(homeScreen);
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
    homeScreen-> updateStatus(glucose, battery, insulin);
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
    BolusCalculator *bolus = new BolusCalculator();
    //bolus->show();
}

void UserInterface::openSettings() {
    Settings *settings = new Settings();
    settings->show();
}

void UserInterface::triggerEmergencyStop() {
    pumpController->triggerEmergencyStop();
    displayError("Emergency stop triggered!");
}
