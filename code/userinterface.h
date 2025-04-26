/**
 * @file userinterface.h
 * @brief Defines the UserInterface class for managing pump UI screens and user interactions.
 *
 * The UserInterface class controls screen transitions between login, home, bolus calculator,
 * history, and settings screens. It handles authentication, displays glucose/battery/insulin
 * data, manages alerts, and updates the CGM graph based on user and system events.
 */

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
#include "history.h"

class Alert;

namespace Ui {
    class UserInterface;
}

/**
 * @brief The UserInterface class manages navigation between screens and updates UI elements.
 * 
 * It handles user login, screen transitions, and displaying data such as glucose level, 
 * battery status, insulin information, and IOB (insulin on board).
 */

class UserInterface : public QWidget {
    Q_OBJECT
public:
  /**
     * @brief Constructs the UserInterface.
     * @param pump Pointer to the PumpController.
     * @param iob Pointer to the IOBTracker.
     * @param parent Optional parent QWidget.
     */
    explicit UserInterface(PumpController* pump, QWidget *parent = nullptr);
    
  /**
     * @brief Destructor for UserInterface.
     */
    ~UserInterface();

    /**
     * @brief Displays the Home screen.
     */
    void displayHomeScreen();

    /**
     * @brief Displays the Login screen.
     */
    void showLoginScreen();

    /**
     * @brief Updates the UI with current glucose, battery, insulin, and IOB values.
     * @param glucose Current glucose reading.
     * @param battery Battery level (0.0 to 1.0).
     * @param insulin Amount of insulin remaining.
     * @param iob Insulin on board.
     */
    void refresh(double glucose, double battery, double insulin, double iob);

    /**
     * @brief Updates the IOB display.
     * @param iob New IOB value to show.
     */
    void updateIOB(double iob);

    /**
     * @brief Displays an alert on the screen.
     * @param alert Pointer to the Alert object.
     */
    void showAlert(Alert *alert);

    /**
     * @brief Closes the current alert and returns to the previous screen.
     * @param alert Pointer to the Alert object.
     */
    void dismissAlert(Alert *alert);

public slots:
    /**
     * @brief Updates the glucose chart with a new reading.
     * @param glucose The glucose value to add to the graph.
     */
    void updateGlucoseForChart(double glucose);

    /**
     * @brief Opens the bolus calculator screen.
     */
    void openBolusUI();

    /**
     * @brief Opens the settings screen.
     */
    void openSettings();

    /**
     * @brief Opens the history/statistics screen.
     */
    void openHistory();

    /**
     * @brief Unlocks the device and emits the deviceUnlocked signal.
     */
    void unlock();

    /**
     * @brief Updates the bolus display (currently unused).
     * @param remainingBolus Amount of bolus insulin remaining.
     */
    void updateBolusDisplay(double remainingBolus);

signals:
    /**
     * @brief Emitted when the device is successfully unlocked.
     */
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
    QTimer *pumpTimer;
    QWidget *lastPage;

    void handleBolusCancelled();
};

#endif // USERINTERFACE_H
