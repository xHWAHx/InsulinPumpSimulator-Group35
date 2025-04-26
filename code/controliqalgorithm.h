/**
 * @file controliqalgorithm.h
 * @brief Implements the Control-IQ insulin delivery algorithm interface.
 *
 * Provides methods to analyze glucose readings, adjust basal rates, and
 * direct pump actions based on algorithmic decisions.
 */
#ifndef CONTROLIQALGORITHM_H
#define CONTROLIQALGORITHM_H

#include <vector>

class DataLogger;
class PumpController;

/**
 * @class ControlIQAlgorithm
 * @brief Core algorithm for dynamic basal insulin control.
 *
 * The ControlIQAlgorithm class processes continuous glucose monitoring data,
 * calculates necessary insulin adjustments, and issues commands to the pump.
 */
class ControlIQAlgorithm {
public:
    /**
     * @brief Analyze a glucose data point and trigger pump actions.
     *
     * Evaluates a single glucose reading, logs any relevant events,
     * and instructs the PumpController to adjust insulin delivery as needed.
     *
     * @param data Latest blood glucose measurement (mg/dL).
     * @param logger DataLogger instance for recording algorithm events.
     * @param pump PumpController instance for executing insulin commands.
     */
    static void analyzeGlucoseData(double data, DataLogger* logger, PumpController* pump);
     /**
     * @brief Adjust the basal insulin rate on the pump.
     *
     * Sends a command to update the pump's basal delivery rate.
     *
     * @param pump PumpController instance controlling the insulin pump.
     * @param rate New basal rate to set (units per hour).
     */
    static void adjustBasalRate(PumpController* pump, double rate);
    /**
     * @brief Current basal rate applied by the Control-IQ algorithm.
     */

    static double currentRate;
};

#endif // CONTROLIQALGORITHM_H
