#include "cgmreader.h"

CGMReader::CGMReader(QCheckBox *errorCheckBox)
    : CGMConnected(true)
    , reading(startAmount)
    , errorCheckBox(errorCheckBox)
{
}

double CGMReader::getCurrentGlucoseLevel(Bloodstream *blood, double correctionFactor){
    double randomVariance = (QRandomGenerator::global()->generateDouble() - 0.5) * volatility * 2;

    reading += (increasePerHour/12 + increasePerHour/12 * randomVariance);
    double absorbed = std::max(0.0, std::min(insulinUsageRate/12, blood->getIOB()));
    reading -= absorbed * correctionFactor;
    blood->absorbUnits(absorbed);

    if (CGMConnected) {
        return reading;
    } else {
        return -1;
    }
}

bool CGMReader::isCGMConnected() {
    CGMConnected = not errorCheckBox->isChecked();
    return CGMConnected;
}

void CGMReader::intakeGlucose(double glucose){
    reading += glucose;
}
