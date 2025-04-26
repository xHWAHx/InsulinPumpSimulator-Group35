#ifndef IOBTRACKER_H
#define IOBTRACKER_H

#include <QDateTime>
#include <QList>

/**
 * @brief The IOBTracker class tracks insulin on board over time.
 */
class IOBTracker {
public:
    /**
     * @brief Constructs an IOBTracker with a given insulin activity duration.
     * @param durationMinutes Duration insulin remains active (default 240 minutes).
     */
    IOBTracker(double durationMinutes = 240.0);

    /**
     * @brief Adds a bolus entry at a given time.
     * @param units Units of insulin delivered.
     * @param time Time of the bolus.
     */
    void addBolus(double units, const QDateTime &time);

    /**
     * @brief Calculates the current insulin on board.
     * @param now The current time (default is system time).
     * @return Current IOB value.
     */
    double getCurrentIOB(const QDateTime &now = QDateTime::currentDateTime()) const;

    /**
     * @brief Clears all recorded bolus entries.
     */
    void clear();

private:
    struct BolusEntry {
        QDateTime time;
        double units;
    };

    QList<BolusEntry> bolusLog; ///< List of past bolus entries.
    double insulinDuration;     ///< Active insulin duration in minutes.
};

#endif // IOBTRACKER_H
