/**
 * @file boluscalculator.h
 * @brief Defines the BolusCalculator widget for calculating and delivering insulin boluses.
 *
 * The BolusCalculator class provides a user interface for computing insulin doses based
 * on current glucose readings, carbohydrate intake, and user overrides. It supports
 * correction bolus, carb bolus, total bolus calculations, dose overrides, and extended
 * dose delivery with countdown management.
 */
#ifndef BOLUSCALCULATOR_H
#define BOLUSCALCULATOR_H

#include <QWidget>
#include "pumpcontroller.h"
#include "datalogger.h"
#include "cgmreader.h"
#include "insulinreserve.h"

namespace Ui {
class BolusCalculator;
}

/**
 * @class BolusCalculator
 * @brief QWidget subclass for insulin bolus calculation and delivery control.
 *
 * Manages UI interactions for computing recommended insulin doses, overriding
 * inputs, validating entries, and dispatching bolus commands to the PumpController.
 * Also handles extended dose timing and countdown display.
 */
class BolusCalculator : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the BolusCalculator widget.
     *
     * @param pump Pointer to the PumpController for sending bolus commands.
     * @param logger Pointer to DataLogger for event logging.
     * @param cgm Pointer to CGMReader for obtaining glucose data.
     * @param insulin Pointer to InsulinReserve for checking insulin availability.
     * @param parent Optional parent QWidget.
     */
    explicit BolusCalculator(PumpController* pump, DataLogger* logger, CGMReader* cgm, InsulinReserve* insulin, QWidget *parent = nullptr);

    /**
     * @brief Cleans up the BolusCalculator widget / deallocates memory.
     */
    ~BolusCalculator();

    /**
     * @brief Calculate a bolus recommendation from current glucose and carb values.
     *
     * Uses both correction and carb ratios to compute a suggested dose.
     *
     * @param glucose Current blood glucose level (mg/dL).
     * @param carbs Amount of carbohydrates to process (grams).
     * @return Recommended bolus amount (units).
     */
    static double calculateBolus(double glucose, double carbs);
     
    /**
     * @brief Suggest dose based on user profile metrics.
     *
     * @return Suggested baseline bolus amount (units).
     */
    static double suggestDose();

     /**
     * @brief Override the calculated bolus with a user-specified value.
     *
     * @param dose The manual dose to use instead of calculated values.
     */
    static void overrideDose(double dose);

     /**
     * @brief Validate that a bolus input falls within acceptable clinical ranges.
     *
     * @param dose The bolus amount to validate.
     * @return True if the dose is within valid limits, false otherwise.
     */
    static bool validateBolusInput(double dose);

    /**
     * @brief Compute the correction bolus based on glucose deviation.
     *
     * @param glucose Current blood glucose (mg/dL).
     * @param target Target blood glucose (mg/dL).
     * @param correctionFactor Insulin sensitivity factor (mg/dL per unit).
     * @return Units required to correct to target level.
     */
    static double calculateCorrectionBolus(double glucose, double target, double correctionFactor);

    /**
     * @brief Compute the carbohydrate bolus based on carb intake.
     *
     * @param carbs Grams of carbohydrates consumed.
     * @param carbRatio Grams of carbs covered per unit of insulin.
     * @param correctionFactor Sensitivity factor for adjustments (mg/dL per unit).
     * @return Units required to cover carbohydrate intake.
     */
    static double calculateCarbBolus(double carbs, double carbRatio, double correctionFactor);

     /**
     * @brief Compute total bolus by combining correction and carb boluses.
     *
     * @param glucose Current blood glucose level (mg/dL).
     * @param carbs Carbohydrate intake (grams).
     * @param target Target blood glucose level (mg/dL).
     * @return Sum of correction and carb bolus units.
     */
    static double calculateTotalBolus(double glucose, double carbs, double target);
 
    /**
     * @brief Split a total bolus into immediate and extended portions.
     *
     * @param total Total bolus units.
     * @param percentage Percentage delivered immediately (1% - 100%).
     * @return Pair of <immediateUnits, extendedUnits>.
     */
    static std::pair<double, double> splitBolus(double total, double percentage);

    /**
     * @brief Update the countdown timer display for extended dose delivery.
     *
     * Called periodically to refresh remaining time in the UI.
     */
    void updateCountdown();

signals:
     /**
     * @brief Navigate back to the home screen.
     */
    void backToHome();

    /**
     * @brief Indicates whether the countdown is active.
     *
     * @param active True if countdown is running, false if stopped.
     */
    void countdownActive(bool active);

    /**
     * @brief Emitted when a bolus delivery begins.
     *
     * @param status Human-readable status message (e.g., "Immediate", "Extended").
     */
    void bolusStarted(QString status);

private slots:
     /**
     * @brief Handle click on the logo button to return home.
     */
    void on_logoButton_clicked();
    /**
     * @brief Calculate bolus when calculate button is pressed.
     */
    void on_btnCalculate_clicked();
    /**
     * @brief Confirm override dose when override button is pressed.
     */
    void on_btnOverrideConfirm_clicked(); 
    /**
     * @brief Trigger delivery of the calculated bolus.
     */
    void on_btnDeliver_clicked();
    /**
     * @brief Handle extended dose delivery logic.
     */
    void deliverExtendedDose();
    /**
     * @brief Cancel any ongoing bolus and update UI/log.
     */
    void on_btnCancelBolus_clicked();

private:
    Ui::BolusCalculator *ui; 
    static double overriddenDose;
    static bool doseOverridden;

    PumpController* pump;
    DataLogger* logger;
    CGMReader* cgm;
    InsulinReserve* insulinReserve;

    QTimer* extendedDoseTimer;
    QTimer* countdownTimer;
    double remainingExtendedDose;
    int countdownMinutes;
    static constexpr double bolusRate = 10;
};

#endif // BOLUSCALCULATOR_H
