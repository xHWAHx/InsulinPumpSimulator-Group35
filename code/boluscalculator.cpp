#include "boluscalculator.h"
#include "ui_boluscalculator.h"
#include "profile.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QCheckBox>

BolusCalculator::BolusCalculator(PumpController* pump, DataLogger* logger, CGMReader* cgm, InsulinReserve* insulin, QWidget *parent): QWidget(parent),ui(new Ui::BolusCalculator),pump(pump),logger(logger),cgm(cgm),insulinReserve(insulin){
   
    ui->setupUi(this);

     // override input initially read-only 
     ui->overrideDoseInput->setReadOnly(true);
     // disable confirm button
     ui->btnOverrideConfirm->setEnabled(false);
 
     // checkbox toggles dose input
     connect(ui->overrideCheckbox, &QCheckBox::stateChanged, this, [=](int state) {
         ui->overrideDoseInput->setReadOnly(state != Qt::Checked);
         ui->btnOverrideConfirm->setEnabled(state == Qt::Checked);
     });
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

// outputs recommended dose
void BolusCalculator::on_btnCalculate_clicked()
{
    bool ok1, ok2;
    double glucose = ui->inputGlucose->text().toDouble(&ok1);
    double carbs = ui->inputCarbs->text().toDouble(&ok2);

    if (!ok1 || !ok2) {
        ui->outputResult->setText("Invalid input");
        return;
    }

    doseOverridden = false;
    overriddenDose = 0.0;

    double dose = calculateBolus(glucose, carbs);
    ui->outputResult->setText(QString::number(dose, 'f', 2));
}

// overrides dose
void BolusCalculator::on_btnOverrideConfirm_clicked()
{
    bool ok;
    double dose = ui->overrideDoseInput->text().toDouble(&ok);

    if (!ok || !validateBolusInput(dose)) {
        ui->outputResult->setText("Invalid override");
        return;
    }

    overrideDose(dose);
    ui->outputResult->setText(QString("%1 (Overridden)").arg(dose, 0, 'f', 2));
}

void BolusCalculator::on_btnDeliver_clicked()
{
    bool ok1, ok2;
    double glucose = ui->inputGlucose->text().toDouble(&ok1);
    double carbs = ui->inputCarbs->text().toDouble(&ok2);
    double dose = calculateBolus(glucose, carbs);

    if (!ok1 || !ok2 || dose <= 0) {
        QMessageBox::warning(this, "Invalid", "Please calculate bolus first.");
        return;
    }

    if (insulinReserve && insulinReserve->getInsulinRemaining() < dose) {
        QMessageBox::warning(this, "Insufficient Insulin", "Not enough insulin to deliver the requested dose.");
        return;
    }

    QString summary = QString("Confirm Bolus Delivery?\n\nGlucose: %1 mmol/L\nCarbs: %2 g\nDose: %3 units").arg(glucose).arg(carbs).arg(dose);

    if (QMessageBox::question(this, "Confirm Dose", summary) != QMessageBox::Yes)
        return;

    if (QMessageBox::question(this, "Extended Bolus", "Would you like an extended dose?") == QMessageBox::Yes) {
        bool okNow, okLater, okTime;
        double now = QInputDialog::getDouble(this, "Deliver Now", "Enter % of dose to deliver now:", 50, 0, 100, 1, &okNow);
        double later = QInputDialog::getDouble(this, "Deliver Later", "Enter % of dose to deliver later:", 50, 0, 100, 1, &okLater);
        int mins = QInputDialog::getInt(this, "Delay Time", "Minutes until second dose:", 30, 1, 240, 1, &okTime);

        if (!okNow || !okLater || !okTime || now + later != 100) {
            QMessageBox::warning(this, "Error", "Invalid percentages or time.");
            return;
        }

        double nowDose = dose * (now / 100.0);
        double laterDose = dose - nowDose;

        QString confirm = QString("Now delivering: %1 units\nScheduled dose: %2 units in %3 minutes.").arg(nowDose, 0, 'f', 2).arg(laterDose, 0, 'f', 2).arg(mins);

        if (QMessageBox::question(this, "Final Confirmation", confirm) == QMessageBox::Yes) {
            double rate = nowDose / (mins / 60.0);  
            if (pump) pump->deliverBolus(nowDose, rate);
            if (logger) {
                logger->logInsulin(QDateTime::currentDateTime(), nowDose);
                logger->logEvent("Extended Bolus", QString("Now: %1 units, Later: %2 units in %3 min")
                                 .arg(nowDose, 0, 'f', 2).arg(laterDose, 0, 'f', 2).arg(mins));
            }
            QMessageBox::information(this, "Delivered", confirm);
        }

    } else {
        if (QMessageBox::question(this, "Final Confirmation", QString("Deliver %1 units now?").arg(dose)) == QMessageBox::Yes) {
            if (pump) pump->deliverBolus(dose, 2.0);
            if (logger) {
                logger->logInsulin(QDateTime::currentDateTime(), dose);
                logger->logEvent("Manual Bolus", QString("Delivered %1 units").arg(dose));
            }
            QMessageBox::information(this, "Delivered", QString("Now delivering: %1 units").arg(dose));
        }
    }
}

void BolusCalculator::on_logoButton_clicked()
{
    emit backToHome();
    this-> close();
}
