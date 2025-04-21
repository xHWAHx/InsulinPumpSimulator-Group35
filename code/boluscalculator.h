#ifndef BOLUSCALCULATOR_H
#define BOLUSCALCULATOR_H

#include <QWidget>
#include <QTimer>
#include "pumpcontroller.h"
#include "datalogger.h"
#include "cgmreader.h"
#include "insulinreserve.h"
#include "iobtracker.h"

namespace Ui {
class BolusCalculator;
}

class BolusCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit BolusCalculator(PumpController* pump, DataLogger* logger, CGMReader* cgm, InsulinReserve* insulin, IOBTracker* iobTracker, QWidget *parent = nullptr);
    ~BolusCalculator();
    static double calculateBolus(double glucose, double carbs);
    static double suggestDose();
    static void overrideDose(double dose);
    static bool validateBolusInput(double dose);
    static double calculateCorrectionBolus(double glucose, double target, double correctionFactor);
    static double calculateCarbBolus(double carbs, double carbRatio);
    static double calculateTotalBolus(double glucose, double carbs, double target);
    static std::pair<double, double> splitBolus(double total, double percentage);
    void updateCountdown();

signals:
    void backToHome();
    void countdownActive(bool active);
    void bolusStarted(QString status);

private slots:
    void on_logoButton_clicked();
    void on_btnCalculate_clicked();
    void on_btnOverrideConfirm_clicked(); 
    void on_btnDeliver_clicked();
    void deliverExtendedDose();

    void on_btnCancelBolus_clicked();

private:
    Ui::BolusCalculator *ui;
    static double overriddenDose;
    static bool doseOverridden;

    PumpController* pump;
    DataLogger* logger;
    CGMReader* cgm;
    InsulinReserve* insulinReserve;
    IOBTracker* iobTracker;

    QTimer* extendedDoseTimer;
    QTimer* countdownTimer;
    double remainingExtendedDose;
    int countdownMinutes;
    static constexpr double bolusRate = 10;
};

#endif // BOLUSCALCULATOR_H
