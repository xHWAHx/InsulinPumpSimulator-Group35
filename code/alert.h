#ifndef ALERT_H
#define ALERT_H

#include <QWidget>
#include "userinterface.h"
#include "datalogger.h"

namespace Ui {
class Alert;
}

class Alert : public QWidget
{
    Q_OBJECT

public:
    explicit Alert(UserInterface *parent = nullptr);
    ~Alert();

    //void showAlert(const QString &title, const QString &body);

    static constexpr int BATTERY_LOW = 1;
    static constexpr int INSULIN_LOW = 2;
    static constexpr int CGM_DISCONNECTED = 3;
    static constexpr int PUMP_OCCLUSION = 6;
    static constexpr int GLUCOSE_LOW = 4;
    static constexpr int GLUCOSE_HIGH = 5;

    static void raise(int type, UserInterface *interface, DataLogger *logger);
    static void reset(int type);

private:
    Ui::Alert *ui;

    static QHash<int, bool> raisedAlerts;
};

#endif // ALERT_H
