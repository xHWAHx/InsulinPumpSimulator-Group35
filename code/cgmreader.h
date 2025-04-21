#ifndef CGMREADER_H
#define CGMREADER_H
#include <QRandomGenerator>
#include "iobtracker.h"
#include "bloodstream.h"
#include <QCheckBox>

class CGMReader
{
// All values are in mmol/L, and each tick is assumed to be 1 second
public:
    CGMReader(QCheckBox *errorCheckBox);
    double getCurrentGlucoseLevel(Bloodstream *blood, double correctionFactor);
    bool isCGMConnected();
private:
	bool CGMConnected;
    double reading;
    QCheckBox *errorCheckBox;
    QRandomGenerator randomGen;
    static constexpr double volatility = 0.8; // how much the increasePerHour can randomly vary, as a multiplier
    static constexpr double startAmount = 6.0; // in mmol/L
    static constexpr double increasePerHour = 4; // in mmol/L per hour
    static constexpr double insulinUsageRate = 6; // in units per hour
};

#endif // CGMREADER_H
