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
	// Returns 0 if it was able to deploy the full amount,
	// otherwise returns the amount deployed
	if (amount <= insulinRemaining){
		insulinRemaining -= amount;
		return 0;
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
