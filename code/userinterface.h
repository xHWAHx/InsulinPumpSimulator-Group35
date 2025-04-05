#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QObject>
#include <QStackedWidget>

class Login;
class HomeWindow;
class PumpController;
class BolusCalculator;
class CGMReader;
class BatteryManager;
class InsulinReserve;
class datalogger;
class ControlIQAlgorithm;
class Settings;
class Alert;

class UserInterface : public QObject {
    Q_OBJECT
public:
    explicit UserInterface(QStackedWidget *stack, QObject *parent = nullptr);

    void displayHomeScreen();
    void showLoginScreen();
    void navigateToBolusCalculator();
    void displayError(const QString &message);
    void refreshStatusBar();
    void showControlIQStats();
    void showOptions();
    bool checkPin(int pin);

public slots:
    void handlePin(int pin);
    void updateGlucoseForChart();
    void openBolusUI();
    void openSettings();
    void triggerEmergencyStop();

private:
    QStackedWidget *stackedWidget;
    Login *loginScreen;
    HomeWindow *homeScreen;

    PumpController *pumpController;
    BolusCalculator *bolusCalculator;
    CGMReader *cgmReader;
    BatteryManager *batteryManager;
    InsulinReserve *insulinReserve;
    datalogger *logger;
    ControlIQAlgorithm *controlIQ;

    const int correctPin = 1234;
};

#endif // USERINTERFACE_H
