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
    if (elapsedSeconds < 5.0)
        elapsedSeconds= 1.0;

    double rand = norm->operator()(randomGenerator);
    double drift= meanReversion * (meanGlucose-previousReading) * elapsedSeconds;
    double noise= rand * std::sqrt(elapsedSeconds);

    double reading = previousReading + drift+ noise;

    previousReading = reading;
    timeOfPreviousReading = currentTime;
	return reading;
}

bool CGMReader::isCGMConnected() {
	return CGMConnected;
}

