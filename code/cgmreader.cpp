#include "cgmreader.h"
#include "iobtracker.h"
#include "iostream"

CGMReader::CGMReader()
    : reading(startAmount)
{
    CGMConnected = true;
}

double CGMReader::getCurrentGlucoseLevel(Bloodstream *blood, double correctionFactor){
    double randomVariance = (QRandomGenerator::global()->generateDouble() - 0.5) * volatility * 2;

    reading += (increasePerHour/12 + increasePerHour/12 * randomVariance);
    double absorbed = std::max(0.0, std::min(insulinUsageRate/12, correctionFactor * blood->getIOB()));
    reading -= absorbed;
    blood->absorbUnits(absorbed);

	return reading;
}

bool CGMReader::isCGMConnected() {
    return CGMConnected;
}
