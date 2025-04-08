#ifndef BOLUSCALCULATOR_H
#define BOLUSCALCULATOR_H

#include <QWidget>

namespace Ui {
class BolusCalculator;
}

class BolusCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit BolusCalculator(QWidget *parent = nullptr);
    ~BolusCalculator();
    static double calculateBolus(double glucose, double carbs);
    static double suggestDose();
    static void overrideDose(double dose);
    static bool validateBolusInput(double dose);
    static double calculateCorrectionBolus(double glucose, double target, double correctionFactor);
    static double calculateCarbBolus(double carbs, double carbRatio);
    static double calculateTotalBolus(double glucose, double carbs, double target);
    static std::pair<double, double> splitBolus(double total, double percentage);


private:
    Ui::BolusCalculator *ui;
    static double overriddenDose;
    static bool doseOverridden;

signals:
    void backToHome();
private slots:
    void on_logoButton_clicked();
    void on_btnCalculate_clicked();
};

#endif // BOLUSCALCULATOR_H
