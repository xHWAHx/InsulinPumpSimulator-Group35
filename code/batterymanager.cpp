#include "batterymanager.h"

BatteryManager::BatteryManager()
    : batteryLevel(1.0)
{ }

BatteryManager::~BatteryManager()
{ }

double BatteryManager::getBatteryLevel(){
	return batteryLevel;
}

void BatteryManager::drainBattery(){ 
	// Drains battery level by 1%.
	// Would not exist in a real device.
    batteryLevel -= 0.001;
	if (batteryLevel <= 0){
		batteryLevel = 0;
        emit batteryDead();
	}
}

void BatteryManager::chargeBattery(){
	// Charges the battery to full instantly
	batteryLevel = 1;
}

bool BatteryManager::isBatteryCritical(){
	return batteryLevel <= criticalValue;
}
