/**
 * @file device.h
 * @brief Defines the Device class, central orchestrator of the insulin pump simulator.
 *
 * The Device class initializes and connects all subsystems (pump, CGM, battery, UI, etc.),
 * manages the simulation lifecycle (power on/off, monitoring loop via QTimer ticks),
 * and enforces safety protocols through periodic checks and alerts.
 */
#ifndef DEVICE_H
#define DEVICE_H

#include <QMainWindow>
//#include <unistd.h>
#include <batterymanager.h>
#include <datalogger.h>
#include <insulinreserve.h>
#include <cgmreader.h>
#include <profile.h>
#include <pumpcontroller.h>
#include <userinterface.h>
#include <alert.h>
#include <bloodstream.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Device; }
QT_END_NAMESPACE

/**
 * @class Device
 * @brief Central orchestrator for the insulin pump simulator.
 *
 * Initializes and connects all subsystems (pump, CGM, battery, UI, etc.),
 * manages the simulation lifecycle (power, monitoring loop),
 * and enforces safety via periodic checks.
 */
class Device : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the Device controller and sets up UI and subsystems.
     * @param parent Optional parent widget.
     */
    explicit Device(QWidget *parent = nullptr);

public slots:
    /**
     * @brief Toggles device power on or off.
     */
	void power();
    
    /**
     * @brief Handles shutdown when power is lost or device is turned off.
     */
    void noPower();
    
    /**
     * @brief Simulation tick handler (invoked by QTimer).
     * Each tick represents 5 simulated minutes.
     */
    void tick();
    
    /**
     * @brief Begins monitoring loop after successful unlock.
     */
    void startMonitoring();
    
    /**
     * @brief Pauses or resumes the simulation tick loop.
     */
    void togglePaused();
    
    /**
     * @brief Simulates carbohydrate intake based on user input.
     */
    void simCarbIntake();

private:
    int simulationRate; // a rate of 1 means 1 second represents 5 minutes

    bool poweredOn; ///< Device power state
    bool monitoring; ///< Whether monitoring is active
    bool paused; ///< Whether simulation is paused
    
    BatteryManager *battery; ///< Manages battery level and drain
    DataLogger *logger; ///< Logs events, glucose, and insulin data
    InsulinReserve *insulin; ///< Tracks insulin reservoir
    CGMReader *cgm; ///< Simulates CGM readings
    Bloodstream *bloodstream; ///< Receives insulin injections
    ControlIQAlgorithm *controlIQ; ///< Automated basal adjustment algorithm
    Profile *profiles; ///< Manages user profiles
    PumpController *pump; ///< Executes insulin delivery logic
    UserInterface *interface; ///< Handles UI navigation and updates
    Alert *alerts; ///< Displays alert dialogs
    Ui::Device *window; ///< Generated UI components
    QTimer *tickClock; ///< Timer driving simulation ticks
    
    bool batteryAlertShown= false; ///< Prevents repeated battery alerts
    bool insulinAlertShown= false; ///< Prevents repeated insulin-low alerts
    bool cgmAlertShown= false; ///< Prevents repeated CGM disconnect alerts
    bool lowGlucoseAlertShown= false; ///< Prevents repeated low-glucose alerts 
    bool highGlucoseAlertShown= false; ///< Prevents repeated high-glucose alerts

    /**
     * @brief Executes one monitoring cycle: read sensors, update pump, log data, and refresh UI.
     */
    void monitor();
    
    /**
     * @brief Evaluates safety conditions and triggers alerts as needed.
     * @param glucose Current glucose reading from CGM
     * @param target  User-defined target glucose level
     */
    void safetyChecks(double glucose, double target);
    
    /**
     * @brief Updates simulation rate and adjusts timer interval accordingly.
     * @param rate New simulation speed factor
     */
    void setSimRate(int rate);
};

#endif // DEVICE_H
