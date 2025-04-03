#ifndef CGMREADER_H
#define CGMREADER_H
#include <chrono>
#include <random>
#include <memory>


class CGMReader
{
// All values are in mmol/L
public:
    CGMReader();
	double getCurrentGlucoseLevel();
	bool isCGMConnected(); // will we use this?
	void alertCGMDisconnected(); // idk how this would work
private:
	bool CGMConnected;
	double previousReading;
    std::default_random_engine randomGenerator;
    std::unique_ptr<std::normal_distribution<double>> norm;
    std::chrono::time_point<std::chrono::steady_clock> timeOfPreviousReading;
    static constexpr double meanReversion = 0.0001; // should never go above 1/timeScale
    static constexpr double meanGlucose = 11;
    static constexpr double volatility = 0.01;
    static constexpr double startAmount = 6;
    static constexpr double timeScale = 3600; // Glucose levels vary over time at the rate of timeScale faster than normal.
                                             // A timeScale of 1 means real time, 60 means 1 second simulates 1 minute.
};

#endif // CGMREADER_H
