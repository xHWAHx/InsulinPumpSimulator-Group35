#include "boluscalculator.h"
#include "ui_boluscalculator.h"
#include "profile.h"

BolusCalculator::BolusCalculator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BolusCalculator)
{
    ui->setupUi(this);
}

BolusCalculator::~BolusCalculator()
{
    delete ui;
}

double BolusCalculator::overriddenDose = 0.0;
bool BolusCalculator::doseOverridden = false;

double BolusCalculator::calculateBolus(double glucose, double carbs) {
    if (doseOverridden) return overriddenDose;

    // gets value from current profile
    Profile profile = Profile::getActiveProfile();
    double carbRatio = profile.getCarbRatio();
    double correctionFactor = profile.getCorrectionFactor();
    double targetGlucose = profile.getTargetGlucose();

    double carbDose = calculateCarbBolus(carbs, carbRatio);
    double correctionDose = calculateCorrectionBolus(glucose, targetGlucose, correctionFactor);

    return carbDose + correctionDose;
}

double BolusCalculator::suggestDose() {
    // recommends dose based on target glucose
    Profile profile = Profile::getActiveProfile();
    return calculateCorrectionBolus(profile.getTargetGlucose(), profile.getTargetGlucose(), profile.getCorrectionFactor());
}

void BolusCalculator::overrideDose(double dose) {
    overriddenDose = dose;
    doseOverridden = true;
}

bool BolusCalculator::validateBolusInput(double dose) {
    return dose > 0 && dose <= 25.0;  // saftey range, modifiable
}

double BolusCalculator::calculateCorrectionBolus(double glucose, double target, double correctionFactor) {
    if (correctionFactor <= 0) return 0;
    double diff = glucose - target;
    return (diff > 0) ? (diff / correctionFactor) : 0; // true if glucose lvl is high
}

double BolusCalculator::calculateCarbBolus(double carbs, double carbRatio) {
    if (carbRatio <= 0) return 0;
    return carbs / carbRatio;
}

double BolusCalculator::calculateTotalBolus(double glucose, double carbs, double target) {
    Profile profile = Profile::getActiveProfile();
    double carbDose = calculateCarbBolus(carbs, profile.getCarbRatio());
    double correctionDose = calculateCorrectionBolus(glucose, target, profile.getCorrectionFactor());
    return carbDose + correctionDose;
}

std::pair<double, double> BolusCalculator::splitBolus(double total, double percentage) {
    // ensures the requested dose percentages make sense
    if (percentage < 0 || percentage > 100) return {0.0, 0.0};
    double immediate = (percentage / 100.0) * total;
    double extended = total - immediate;
    return {immediate, extended};
}

void BolusCalculator::on_logoButton_clicked()
{
    emit backToHome();
    this-> close();
}

