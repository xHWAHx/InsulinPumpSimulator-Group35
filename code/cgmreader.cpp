#include "cgmreader.h"

CGMReader::CGMReader()
    : previousReading(startAmount)
    , randomGenerator()
    , norm(new std::normal_distribution<double>(0, volatility))
    , timeOfPreviousReading(std::chrono::steady_clock::now())

{
    CGMConnected = true;
}

double CGMReader::getCurrentGlucoseLevel(){
	auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedDuration {currentTime - timeOfPreviousReading};
    double elapsedSeconds = elapsedDuration.count() * timeScale;

    double rand = norm->operator()(randomGenerator);

    double reading = previousReading + meanReversion * (meanGlucose - previousReading) * elapsedSeconds + std::sqrt(elapsedSeconds) * rand;

	timeOfPreviousReading = currentTime;
	previousReading = reading;
	return reading;
}

bool CGMReader::isCGMConnected() {
	return CGMConnected;
}

