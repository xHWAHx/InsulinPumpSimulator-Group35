#ifndef BATTERYMANAGER_H
#define BATTERYMANAGER_H


class BatteryManager
{
public:
    BatteryManager();
    double getBatteryLevel();
	void drainBattery();
	void chargeBattery(); 
	bool isBatteryCritical();
	void alertLowBattery();

private:
	double batteryLevel;
    static constexpr double criticalValue = 0.15;
};

#endif // BATTERYMANAGER_H
