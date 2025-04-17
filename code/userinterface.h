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

class Alert;

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
    void refreshStatusBar(double glucose, double battery, double insulin);
    void updateIOB(double iob);
    void showAlert(Alert *alert);
    void dismissAlert(Alert *alert);

public slots:
    void updateGlucoseForChart(double glucose);
    void openBolusUI();
    void openSettings();
    void openHistory();
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
    QTimer *pumpTimer;
    QWidget *lastPage;

    const int correctPin = 1234;

    void handleBolusCancelled(double amountDelivered);
};

#endif // USERINTERFACE_H
