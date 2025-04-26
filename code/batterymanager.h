/**
 * @file batterymanager.h
 * @brief Defines the BatteryManager class for simulating pump battery behavior.
 *
 * The BatteryManager class monitors and manages the battery level of the insulin pump device,
 * allowing draining over time, manual recharging, and detection of critical battery levels.
 */

#ifndef BATTERYMANAGER_H
#define BATTERYMANAGER_H

#include <QObject>

/**
 * @class BatteryManager
 * @brief Simulates the battery behavior of the device.
 *
 * Manages battery draining, charging, and triggers a warning when battery reaches a critical threshold.
 */
class BatteryManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a BatteryManager object with a full battery.
     */
    BatteryManager();

    /**
     * @brief Destructor for BatteryManager.
     */
    ~BatteryManager();

    /**
     * @brief Gets the current battery level.
     * @return Battery level as a fraction between 0.0 and 1.0.
     */
    double getBatteryLevel();

    /**
     * @brief Drains the battery based on usage.
     */
    void drainBattery();

    /**
     * @brief Checks if the battery level is below the critical threshold.
     * @return True if battery is critically low, otherwise false.
     */
    bool isBatteryCritical();

    /**
     * @brief Raises a low battery warning alert.
     */
    void alertLowBattery();

public slots:
    /**
     * @brief Charges the battery back to full.
     */
    void chargeBattery();

signals:
    /**
     * @brief Signal emitted when battery is completely depleted.
     */
    void batteryDead();

private:
    double batteryLevel;           ///< Current battery level.
    static constexpr double criticalValue = 0.15; ///< Critical battery threshold (15%).
};

#endif // BATTERYMANAGER_H
