#include "insulinreserve.h"

InsulinReserve::InsulinReserve()
	: insulinRemaining {maxAmount}
{ }

InsulinReserve::~InsulinReserve()
{ }

double InsulinReserve::getInsulinRemaining() {
	return insulinRemaining;
}

double InsulinReserve::useInsulin(double amount) {
	if (amount <= insulinRemaining){
		insulinRemaining -= amount;
        return amount;
	} else {
		double insulinDeployed = insulinRemaining;
		insulinRemaining = 0;
		return insulinDeployed;
	}
}

bool InsulinReserve::isInsulinLow() {
	return insulinRemaining <= lowAmount;
}

void InsulinReserve::refillInsulin() {
	insulinRemaining = maxAmount;
}
