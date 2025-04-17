#ifndef CGMREADER_H
#define CGMREADER_H
#include <QRandomGenerator>


class CGMReader
{
// All values are in mmol/L, and each tick is assumed to be 1 second
public:
    CGMReader();
	double getCurrentGlucoseLevel();
	bool isCGMConnected(); // will we use this?
	void alertCGMDisconnected(); // idk how this would work
private:
	bool CGMConnected;
    double reading;
    QRandomGenerator randomGen;
    static constexpr double volatility = 0.4;
    static constexpr double startAmount = 6.0;
    static constexpr double increasePerSecond = 0.01;
};

#endif // CGMREADER_H
