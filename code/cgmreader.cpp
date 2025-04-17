#include "cgmreader.h"

CGMReader::CGMReader()
    : reading(startAmount)
{
    CGMConnected = true;
}

double CGMReader::getCurrentGlucoseLevel(){
    double randomVariance = QRandomGenerator::global()->generateDouble() - 0.5;

    reading += increasePerSecond + volatility * randomVariance;

	return reading;
}

bool CGMReader::isCGMConnected() {
	return CGMConnected;
}
