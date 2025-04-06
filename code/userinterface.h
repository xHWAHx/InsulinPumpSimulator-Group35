#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QWidget>
#include <QStackedWidget>

class Login;
class Home;
class PumpController;
class BolusCalculator;
class CGMReader;
class BatteryManager;
class InsulinReserve;
class DataLogger;
class ControlIQAlgorithm;
class Settings;
class Alert;

namespace Ui {
    class UserInterface;
}

class UserInterface : public QWidget {
    Q_OBJECT
public:
    explicit UserInterface(QWidget *parent = nullptr);
    ~UserInterface();

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

    const int correctPin = 1234;
};

#endif // USERINTERFACE_H
