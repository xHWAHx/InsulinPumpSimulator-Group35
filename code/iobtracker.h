// iobtracker.h
#ifndef IOBTRACKER_H
#define IOBTRACKER_H

#include <QDateTime>
#include <QList>

class IOBTracker {
public:
    IOBTracker(double durationMinutes = 240.0);

    void addBolus(double units, const QDateTime &time);
    double getCurrentIOB(const QDateTime &now = QDateTime::currentDateTime()) const;
    void clear();

private:
    struct BolusEntry {
        QDateTime time;
        double units;
    };

    QList<BolusEntry> bolusLog;
    double insulinDuration;
};

#endif // IOBTRACKER_H

