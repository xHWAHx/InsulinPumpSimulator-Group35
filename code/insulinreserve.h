#ifndef INSULINRESERVE_H
#include <QObject>
#define INSULINRESERVE_H

/**
 * @class InsulinReserve
 * @brief Manages the insulin reservoir for the pump simulator.
 *
 * Tracks available insulin units, supports consumption for deliveries,
 * low-level checks, and refilling to maximum capacity.
 */
class InsulinReserve : public QObject
{
    Q_OBJECT;
public:
    /**
     * @brief Constructs an InsulinReserve with full capacity.
     */
    InsulinReserve();
    
    /**
     * @brief Destroys the InsulinReserve instance.
     */
    ~InsulinReserve();
	
    /**
     * @brief Retrieves the current insulin units remaining.
     * @return Remaining insulin units.
     */
    double getInsulinRemaining();
    
    /**
     * @brief Consumes insulin from the reservoir.
     * @param amount Units of insulin to deploy.
     * @return Actual units deployed (may be less if reservoir is insufficient).
     */
    double useInsulin(double amount);
	
    /**
     * @brief Checks if insulin level is at or below the low threshold.
     * @return true if insulinRemaining <= lowAmount, false otherwise.
     */
    bool isInsulinLow();
public slots:
	/**
     * @brief Refills the insulin reservoir to maximum capacity.
     */
    void refillInsulin();
private:
	double insulinRemaining; // Measured in "units"
    static constexpr double maxAmount = 300; ///< Maximum reservoir capacity (units).
    static constexpr double lowAmount = maxAmount/10; ///< Low-level warning threshold.
};

#endif // INSULINRESERVE_H
