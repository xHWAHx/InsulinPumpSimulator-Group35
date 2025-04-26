/**
 * @file alert.h
 * @brief Defines the Alert system for raising and resetting device alerts.
 *
 * The Alert class manages critical notifications such as low battery, low insulin,
 * CGM disconnection, and glucose extremes. Alerts are displayed through the UserInterface
 * and logged into the DataLogger for history tracking.
 */

#ifndef ALERT_H
#define ALERT_H

#include <QWidget>
#include "userinterface.h"
#include "datalogger.h"

namespace Ui {
class Alert;
}

/**
 * @brief The Alert class displays warning or error alerts to the user.
 */
class Alert : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for Alert screen.
     * @param parent Optional parent widget.
     */
    explicit Alert(UserInterface *parent = nullptr);

    /**
     * @brief Destructor for Alert screen.
     */
    ~Alert();

    /// Predefined alert types
    static constexpr int BATTERY_LOW = 1;
    static constexpr int INSULIN_LOW = 2;
    static constexpr int CGM_DISCONNECTED = 3;
    static constexpr int PUMP_OCCLUSION = 6;
    static constexpr int GLUCOSE_LOW = 4;
    static constexpr int GLUCOSE_HIGH = 5;

    /**
     * @brief Raises a new alert of a specific type.
     * @param type Type of the alert.
     * @param interface Pointer to the user interface.
     * @param logger Pointer to the event logger.
     */
    static void raise(int type, UserInterface *interface, DataLogger *logger);

    /**
     * @brief Resets and clears a specific alert type.
     * @param type Type of the alert to reset.
     */
    static void reset(int type);

private:
    Ui::Alert *ui;

    /// Keeps track of which alerts are currently active.
    static QHash<int, bool> raisedAlerts;
};

#endif // ALERT_H
