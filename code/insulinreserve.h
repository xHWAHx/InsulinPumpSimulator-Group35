#ifndef INSULINRESERVE_H
#include <QObject>
#define INSULINRESERVE_H


class InsulinReserve : public QObject
{
    Q_OBJECT;
public:
    InsulinReserve();
    ~InsulinReserve();
	double getInsulinRemaining();
    double useInsulin(double amount);
	bool isInsulinLow();
public slots:
	void refillInsulin();
private:
	double insulinRemaining; // Measured in "units"
    static constexpr double maxAmount = 300;
    static constexpr double lowAmount = maxAmount/10;
};

#endif // INSULINRESERVE_H
