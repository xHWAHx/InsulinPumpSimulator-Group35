#ifndef BOLUSCALCULATOR_H
#define BOLUSCALCULATOR_H

#include <utility>

class BolusCalculator {
public:
    static double calculateBolus(double glucose, double carbs);
    static double suggestDose();
    static void overrideDose(double dose);
    static bool validateBolusInput(double dose);
    static double calculateCorrectionBolus(double glucose, double target, double correctionFactor);
    static double calculateCarbBolus(double carbs, double carbRatio);
    static double calculateTotalBolus(double glucose, double carbs, double target);
    static std::pair<double, double> splitBolus(double total, double percentage);

private:
    static double overriddenDose;
    static bool doseOverridden;
};

#endif // BOLUSCALCULATOR_H
