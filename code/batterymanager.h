#ifndef BATTERYMANAGER_H
#include <QObject>
#define BATTERYMANAGER_H


class BatteryManager : public QObject
{
    Q_OBJECT;
public:
    BatteryManager();
    ~BatteryManager();
    double getBatteryLevel();
	void drainBattery();
	bool isBatteryCritical();
	void alertLowBattery();

public slots:
    void chargeBattery();

private:
	double batteryLevel;
    static constexpr double criticalValue = 0.15;
};

#endif // BATTERYMANAGER_H
