#include "iobtracker.h"

IOBTracker::IOBTracker(double durationMinutes)
    : insulinDuration(durationMinutes) {}

void IOBTracker::addBolus(double units, const QDateTime &time) {
    bolusLog.append({time, units});
}

void IOBTracker::clear() {
    bolusLog.clear();
}

double IOBTracker::getCurrentIOB(const QDateTime &now) const {
    double totalIOB = 0.0;
    for (const auto &entry : bolusLog) {
        double elapsed = entry.time.secsTo(now) / 60.0;
        if (elapsed >= insulinDuration) continue;
        double fractionRemaining = 1.0 - (elapsed / insulinDuration);
        totalIOB += entry.units * fractionRemaining;
    }
    return totalIOB;
}
