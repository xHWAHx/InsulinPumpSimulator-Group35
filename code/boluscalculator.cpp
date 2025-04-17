#include "boluscalculator.h"
#include "ui_boluscalculator.h"
#include "profile.h"
#include "iobtracker.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QCheckBox>

BolusCalculator::BolusCalculator(PumpController* pump, DataLogger* logger, CGMReader* cgm, InsulinReserve* insulin, IOBTracker* iobTracker, QWidget *parent)
    : QWidget(parent), ui(new Ui::BolusCalculator), pump(pump), logger(logger), cgm(cgm), insulinReserve(insulin), iobTracker(iobTracker), remainingExtendedDose(0.0), countdownSeconds(0)
{
    ui->setupUi(this);

    extendedDoseTimer = new QTimer(this);
    countdownTimer = new QTimer(this);

    connect(extendedDoseTimer, &QTimer::timeout, this, &BolusCalculator::deliverExtendedDose);
    connect(countdownTimer, &QTimer::timeout, this, &BolusCalculator::updateCountdown);

    ui->overrideDoseInput->setReadOnly(true);
    ui->btnOverrideConfirm->setEnabled(false);

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

    Profile profile = Profile::getActiveProfile();
    double carbDose = calculateCarbBolus(carbs, profile.getCarbRatio());
    double correctionDose = calculateCorrectionBolus(glucose, profile.getTargetGlucose(), profile.getCorrectionFactor());
    return carbDose + correctionDose;
}

double BolusCalculator::suggestDose() {
    Profile profile = Profile::getActiveProfile();
    return calculateCorrectionBolus(profile.getTargetGlucose(), profile.getTargetGlucose(), profile.getCorrectionFactor());
}

void BolusCalculator::overrideDose(double dose) {
    overriddenDose = dose;
    doseOverridden = true;
}

bool BolusCalculator::validateBolusInput(double dose) {
    return dose > 0 && dose <= 25.0;
}

double BolusCalculator::calculateCorrectionBolus(double glucose, double target, double correctionFactor) {
    if (correctionFactor <= 0) return 0;
    double diff = glucose - target;
    return (diff > 0) ? (diff / correctionFactor) : 0;
}

double BolusCalculator::calculateCarbBolus(double carbs, double carbRatio) {
    if (carbRatio <= 0) return 0;
    return carbs / carbRatio;
}

double BolusCalculator::calculateTotalBolus(double glucose, double carbs, double target) {
    Profile profile = Profile::getActiveProfile();
    return calculateCarbBolus(carbs, profile.getCarbRatio()) +
           calculateCorrectionBolus(glucose, target, profile.getCorrectionFactor());
}

std::pair<double, double> BolusCalculator::splitBolus(double total, double percentage) {
    if (percentage < 0 || percentage > 100) return {0.0, 0.0};
    double immediate = (percentage / 100.0) * total;
    return {immediate, total - immediate};
}

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

    if (glucose < 3.9){
        QMessageBox::warning(this, "Bolus Disabled", "Glucose too low. Bolus delivery is disabled.");
        return;
    }

    if (insulinReserve && insulinReserve->getInsulinRemaining() < dose) {
        QMessageBox::warning(this, "Insufficient Insulin", "Not enough insulin to deliver the requested dose.");
        return;
    }

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

        if (iobTracker) iobTracker->addBolus(nowDose, QDateTime::currentDateTime());

        if (QMessageBox::question(this, "Final Confirmation",
            QString("Now delivering: %1 units\nScheduled dose: %2 units in %3 minutes.")
            .arg(nowDose, 0, 'f', 2).arg(laterDose, 0, 'f', 2).arg(mins)) != QMessageBox::Yes)
            return;

        pump->deliverBolus(nowDose, nowDose / (mins / 60.0), /*suppressTime=*/false);//here
        logger->logInsulin(QDateTime::currentDateTime(), nowDose);
        logger->logEvent("Extended Bolus", QString("Now: %1 units, Later: %2 units in %3 min").arg(nowDose).arg(laterDose).arg(mins));

        remainingExtendedDose = laterDose;
        countdownSeconds = mins * 60;
        countdownTimer->start(1000);
        emit countdownActive(true);
        extendedDoseTimer->start(mins * 60 * 1000);
    } else {
        if (QMessageBox::question(this, "Final Confirmation", QString("Deliver %1 units now?").arg(dose)) == QMessageBox::Yes) {
            pump->deliverBolus(dose, 2.0, /*suppressTime=*/true);
            if (logger) {
                logger->logInsulin(QDateTime::currentDateTime(), dose);
                logger->logEvent("Manual Bolus", QString("Delivered %1 units").arg(dose));
            }
        }
    }
}

void BolusCalculator::deliverExtendedDose() {
    if (pump && remainingExtendedDose > 0) {
        pump->deliverBolus(remainingExtendedDose, 2.0, /*suppressTime=*/true); //here
        if (iobTracker) iobTracker->addBolus(remainingExtendedDose, QDateTime::currentDateTime());
        if (logger) {
            logger->logInsulin(QDateTime::currentDateTime(), remainingExtendedDose);
            logger->logEvent("Extended Bolus", QString("Delivered extended dose of %1 units.").arg(remainingExtendedDose, 0, 'f', 2));
        }
        QMessageBox::information(this, "Extended Dose Delivered", QString("%1 units have been delivered.").arg(remainingExtendedDose, 0, 'f', 2));
        remainingExtendedDose = 0;
        extendedDoseTimer->stop();
        countdownTimer->stop();
        if (pump) emit pump->bolusTimeRemainingUpdated(0.0);
    }
}

void BolusCalculator::updateCountdown() {
    countdownSeconds--;
    if (countdownSeconds <= 0) {
        countdownTimer->stop();
        emit countdownActive(false);
        emit pump->bolusTimeRemainingUpdated(0.0); //here
        return;
    }
    emit pump->bolusTimeRemainingUpdated(countdownSeconds);
}

void BolusCalculator::on_logoButton_clicked() {
    emit backToHome();
    close();
}

void BolusCalculator::on_btnCancelBolus_clicked()
{
    if (pump){
        pump-> suspendBolus();
        if (logger){
            logger-> logEvent("Manual", "Bolus Delivery has been cancelled by user");
        }
        QMessageBox::information(this, "Cancelled", "Bolus Delivery has been cancelled");
    } else {
        QMessageBox::warning(this, "Error", "Pump not availableto cancel bolus");
    }
}

