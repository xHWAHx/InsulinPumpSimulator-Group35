#ifndef CGMREADER_H
#define CGMREADER_H
#include <QRandomGenerator>
#include "bloodstream.h"
#include <QCheckBox>
#include <QSpinBox>

class CGMReader
{
// All values are in mmol/L, and each tick is assumed to be 1 second
public:
    CGMReader(QCheckBox *errorCheckBox);
    double getCurrentGlucoseLevel(Bloodstream *blood, double correctionFactor);
    void intakeGlucose(double glucose);
    bool isCGMConnected();
private:
	bool CGMConnected;
    double reading;
    QCheckBox *errorCheckBox;
    QSpinBox *carbSpinBox;
    QRandomGenerator randomGen;
    static constexpr double volatility = 0.8; // how much the increasePerHour can randomly vary, as a coefficient
    static constexpr double startAmount = 6.0; // in mmol/L
    static constexpr double increasePerHour = 2; // in mmol/L per hour
    static constexpr double insulinUsageRate = 2; // in units per hour
};

#endif // CGMREADER_H
