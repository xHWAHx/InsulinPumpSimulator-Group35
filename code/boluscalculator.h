#ifndef BOLUSCALCULATOR_H
#define BOLUSCALCULATOR_H
#include <utility>

class BolusCalculator
{
public:
    static double calculateBolus(double glucose, double carbs);
    static double suggestDose();
    static void overrideDose(double dose);
    static bool validateBolusInput(double dose);
    static double calculateCorrectionDose(double glucose, double target, double sensitivity);
    static double calculateCarbBolus(double carbs, double ratio);
    static std::pair<double, double> splitBolus(double totalDose, double percentage);
};

#endif // BOLUSCALCULATOR_H
