#ifndef INSULINRESERVE_H
#define INSULINRESERVE_H


class InsulinReserve
{
public:
    InsulinReserve();
	double getInsulinRemaining();
    double useInsulin(double amount);
	bool isInsulinLow();
	void refillInsulin();
private:
	double insulinRemaining; // Measured in "units"
    static constexpr double maxAmount = 300;
    static constexpr double lowAmount = maxAmount/10;
};

#endif // INSULINRESERVE_H
