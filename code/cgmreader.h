/**
 * @file cgmreader.h
 * @brief Defines the CGMReader class for simulating continuous glucose monitoring.
 *
 * The CGMReader class simulates glucose readings based on insulin activity and
 * random natural variations. It also simulates sensor connection loss and recovery.
 */

#ifndef CGMREADER_H
#define CGMREADER_H

#include <QRandomGenerator>
#include "bloodstream.h"
#include <QCheckBox>
#include <QSpinBox>

/**
 * @class CGMReader
 * @brief Simulates a continuous glucose monitor (CGM).
 *
 * Produces glucose level readings influenced by insulin activity and carbohydrate intake,
 * with options to simulate sensor errors and disconnections.
 */
class CGMReader
{
public:
    /**
     * @brief Constructs a CGMReader with an associated error checkbox UI component.
     * @param errorCheckBox Pointer to the checkbox that simulates CGM errors.
     */
    CGMReader(QCheckBox *errorCheckBox);

    /**
     * @brief Gets the current simulated glucose level.
     * @param blood Pointer to the Bloodstream to account for insulin absorption effects.
     * @param correctionFactor Correction sensitivity factor.
     * @return Current glucose level in mmol/L.
     */
    double getCurrentGlucoseLevel(Bloodstream *blood, double correctionFactor);

    /**
     * @brief Simulates glucose intake from carbohydrate consumption.
     * @param glucose Amount of glucose intake in mmol/L.
     */
    void intakeGlucose(double glucose);

    /**
     * @brief Checks if the CGM sensor is connected.
     * @return True if CGM is connected, false if disconnected.
     */
    bool isCGMConnected();

private:
	bool CGMConnected;
    double reading;
    QCheckBox *errorCheckBox;
    QSpinBox *carbSpinBox;
    QRandomGenerator randomGen;
    static constexpr double volatility = 0.8; // how much the increasePerHour can randomly vary, as a coefficient
    static constexpr double startAmount = 6.0; // in mmol/L
    static constexpr double increasePerHour = 2; // in mmol/L per hour
    static constexpr double insulinUsageRate = 2; // in units per hour
};

#endif // CGMREADER_H
