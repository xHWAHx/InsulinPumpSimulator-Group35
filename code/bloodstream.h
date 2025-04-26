/**
 * @file bloodstream.h
 * @brief Defines the Bloodstream class for tracking insulin absorption and insulin on board (IOB).
 *
 * The Bloodstream class models how insulin enters and remains in the bloodstream,
 * allowing calculation of Insulin On Board (IOB) at any given time.
 */

#ifndef BLOODSTREAM_H
#define BLOODSTREAM_H

#include <QObject>

/**
 * @class Bloodstream
 * @brief Simulates insulin activity within the bloodstream.
 *
 * Tracks the amount of active insulin and processes injections and absorption over time.
 */
class Bloodstream : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a Bloodstream object with zero insulin on board.
     * @param parent Optional parent QObject.
     */
    explicit Bloodstream(QObject *parent = nullptr);

    /**
     * @brief Absorbs units of insulin over time, decreasing IOB.
     * @param insulin Amount of insulin absorbed.
     */
    void absorbUnits(double insulin);

    /**
     * @brief Injects new insulin into the bloodstream, increasing IOB.
     * @param insulin Amount of insulin injected.
     */
    void injectUnits(double insulin);

    /**
     * @brief Returns the current insulin on board (IOB).
     * @return The current IOB in units.
     */
    double getIOB();

signals:
    // No custom signals yet.

private:
    double insulinOnBoard;
};

#endif // BLOODSTREAM_H
