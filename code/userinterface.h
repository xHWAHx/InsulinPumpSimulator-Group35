#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QWidget>
#include <QStackedWidget>
#include "login.h"
#include "home.h"
#include "pumpcontroller.h"
#include "boluscalculator.h"
#include "cgmreader.h"
#include "batterymanager.h"
#include "insulinreserve.h"
#include "datalogger.h"
#include "controliqalgorithm.h"
#include "settings.h"
//#include "alert.h"
#include "history.h"
#include "iobtracker.h"

namespace Ui {
    class UserInterface;
}

class UserInterface : public QWidget {
    Q_OBJECT
public:
    explicit UserInterface(PumpController* pump, IOBTracker* iob, QWidget *parent = nullptr);
    ~UserInterface();

    void displayHomeScreen();
    void showLoginScreen();
    //void navigateToBolusCalculator();
    void displayError(const QString &message);
    void refreshStatusBar(double glucose, double battery, double insulin);
    //void showControlIQStats();
    //void showOptions();
    void updateIOB(double iob);

public slots:
    void updateGlucoseForChart(double glucose);
    void openBolusUI();
    void openSettings();
    void openHistory();
    void triggerEmergencyStop();
    void unlock();
    //void updateBolusDisplay(double remainingBolus, double rate, double deliveredThisTick);

signals:
    void deviceUnlocked();

private:
    Ui::UserInterface *ui;

    Login *loginScreen;
    Home *homeScreen;
    PumpController *pumpController;
    BolusCalculator *bolusCalculator;
    CGMReader *cgmReader;
    BatteryManager *batteryManager;
    InsulinReserve *insulinReserve;
    DataLogger *logger;
    ControlIQAlgorithm *controlIQ;
    Settings *settingsScreen;
    History *historyScreen;
    IOBTracker *iobTracker;

    const int correctPin = 1234;
};

#endif // USERINTERFACE_H
