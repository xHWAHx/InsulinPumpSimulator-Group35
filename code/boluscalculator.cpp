#include "boluscalculator.h"
#include "ui_boluscalculator.h"
#include "profile.h"
#include "iobtracker.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QCheckBox>
#include <QDateTime>

BolusCalculator::BolusCalculator(PumpController* pump,
                                 DataLogger* logger,
                                 CGMReader* cgm,
                                 InsulinReserve* insulin,
                                 IOBTracker* iobTracker,
                                 QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BolusCalculator)
    , pump(pump)
    , logger(logger)
    , cgm(cgm)
    , insulinReserve(insulin)
    , iobTracker(iobTracker)
    , remainingExtendedDose(0.0)
    , countdownMinutes(0)
{
    ui->setupUi(this);

    // Setup UI and timers for extended bolus and countdown
    //extendedDoseTimer = new QTimer(this);
    //countdownTimer    = new QTimer(this);

    // Handle extended and countdown timer events
    //connect(extendedDoseTimer, &QTimer::timeout, this, &BolusCalculator::deliverExtendedDose);
    //connect(countdownTimer,    &QTimer::timeout, this, &BolusCalculator::updateCountdown);

    ui->overrideDoseInput->setReadOnly(true);
    ui->btnOverrideConfirm->setEnabled(false);

    // Enable override input only when checkbox is checked
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
bool   BolusCalculator::doseOverridden = false;

double BolusCalculator::calculateBolus(double glucose, double carbs) {

    // Use overridden dose if set, otherwise calculate based on profile
    if (doseOverridden) return overriddenDose;

    Profile profile = Profile::getActiveProfile();
    double carbDose = calculateCarbBolus(carbs, profile.getCarbRatio());
    double correctionDose = calculateCorrectionBolus(
        glucose, profile.getTargetGlucose(), profile.getCorrectionFactor());
    return carbDose + correctionDose;
}

double BolusCalculator::suggestDose() {
    Profile profile = Profile::getActiveProfile();
    return calculateCorrectionBolus(
        profile.getTargetGlucose(), profile.getTargetGlucose(), profile.getCorrectionFactor());
}

void BolusCalculator::overrideDose(double dose) {
    overriddenDose = dose;
    doseOverridden = true;
}

bool BolusCalculator::validateBolusInput(double dose) {
    // Prevent invalid override values
    return dose > 0 && dose <= 25.0;
}

// Calculates Correction Bolus
double BolusCalculator::calculateCorrectionBolus(double glucose, double target, double correctionFactor)
{
    if (correctionFactor <= 0) return 0;
    double diff = glucose - target;
    return (diff > 0) ? (diff / correctionFactor) : 0;
}

// Calculates food bolus
double BolusCalculator::calculateCarbBolus(double carbs, double carbRatio) {
    if (carbRatio <= 0) return 0;
    return carbs / carbRatio;
}

// Calculates Total Bolus (before IOB)
double BolusCalculator::calculateTotalBolus(double glucose,
                                            double carbs,
                                            double target)
{
    Profile profile = Profile::getActiveProfile();
    return calculateCarbBolus(carbs, profile.getCarbRatio()) +
           calculateCorrectionBolus(glucose, target, profile.getCorrectionFactor());
}

// Distrbutes Bolus overtime 
std::pair<double, double> BolusCalculator::splitBolus(double total, double percentage) {
    // Split total dose based on immediate percentage (used for extended bolus)
    if (percentage < 0 || percentage > 100) return {0.0, 0.0};
    double immediate = (percentage / 100.0) * total;
    return {immediate, total - immediate};
}

// Calculates and Outputs Recommended Dose on UI 
void BolusCalculator::on_btnCalculate_clicked()
{
    bool ok1, ok2;
    double glucose = ui->inputGlucose->text().toDouble(&ok1);
    double carbs   = ui->inputCarbs->text().toDouble(&ok2);

    if (!ok1 || !ok2) {
        ui->outputResult->setText("Invalid input");
        return;
    }

    // Reset override before calculating fresh dose
    doseOverridden = false;
    overriddenDose = 0.0;

    double dose = calculateBolus(glucose, carbs);
    ui->outputResult->setText(QString::number(dose, 'f', 2));
}

// Overrides and Displays Dose on UI 
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

// Delivers Bolus 
void BolusCalculator::on_btnDeliver_clicked()
{
    bool ok1, ok2;
    double glucose = ui->inputGlucose->text().toDouble(&ok1);
    double carbs   = ui->inputCarbs->text().toDouble(&ok2);
    double dose    = calculateBolus(glucose, carbs);

    if (!ok1 || !ok2 || dose <= 0) {
        QMessageBox::warning(this, "Invalid", "Please calculate bolus first.");
        return;
    }

    if (glucose < 3.9) {
        QMessageBox::warning(this, "Bolus Disabled",
                             "Glucose too low. Bolus delivery is disabled.");
        return;
    }

    //if (insulinReserve && insulinReserve->getInsulinRemaining() < dose) {
    //    QMessageBox::warning(this, "Insufficient Insulin",
    //                         "Not enough insulin to deliver the requested dose.");
    //    return;
    //}
    // Allows User to Customize Extended Dose Delivery 
    if (QMessageBox::question(this, "Extended Bolus",
                              "Would you like an extended dose?") == QMessageBox::Yes)
    {
        bool okNow, okTime;
        double nowPct   = QInputDialog::getDouble(
            this, "Deliver Now",
            "Enter % of dose to deliver now:",
            50, 0, 100, 1, &okNow);
        double laterPct = 100 - nowPct;
        int mins        = QInputDialog::getInt(
            this, "Delay Time",
            "Minutes until second dose:",
            30, 1, 240, 1, &okTime);

        // Validate % split and timing for extended bolus
        if (!okNow || !okTime || nowPct + laterPct != 100) {
            QMessageBox::warning(this, "Error", "Invalid percentages or time.");
            return;
        }

        // Immediate Dose to be Delivered 
        double nowDose   = dose * (nowPct / 100.0);
        // Second Portion of the Dose to be Delivered Later on 
        double laterDose = dose - nowDose;

        // Start timers and deliver the immediate part of extended dose
        pump->resumeBolus();
        countdownMinutes = mins;
        pump->bolusTimeRemainingUpdated(countdownMinutes);
        //countdownTimer->start(1000);

        pump->deliverBolus(nowDose, bolusRate, /*suppressTime=*/true);
        
        //logger->logInsulin(QDateTime::currentDateTime(), nowDose);
        //emit bolusStarted(QString("Delivering: %1 U").arg(nowDose, 0, 'f', 2));
        logger->logEvent("Extended Bolus", QString("Now: %1 units, Later: %2 units in %3 min").arg(nowDose).arg(laterDose).arg(mins));

        remainingExtendedDose = laterDose;
        //emit countdownActive(true);
        // Schedule the second part of the extended dose
        //extendedDoseTimer->start(mins * 60 * 1000);
    } else {
        // Stop any extended-dose timers
        //extendedDoseTimer->stop();
        //countdownTimer->stop();
        //emit countdownActive(false);

        // Manual delivery path if extended bolus is skipped
        if (QMessageBox::question(this, "Final Confirmation", QString("Deliver %1 units now?").arg(dose)) == QMessageBox::Yes)
        {
            pump->resumeBolus();
            pump->bolusTimeRemainingUpdated(0.0);
            pump->deliverBolus(dose, bolusRate, /*suppressTime=*/true);
            logger->logInsulin(QDateTime::currentDateTime(), dose);
            //emit bolusStarted(QString("Delivering: %1 U").arg(dose, 0, 'f', 2));
            logger->logEvent("Manual Bolus",
                             QString("Delivered %1 units").arg(dose));
        }
    }
}

void BolusCalculator::deliverExtendedDose() {
    if (pump && remainingExtendedDose > 0) {
        pump->resumeBolus();

        // Actual delivery of second portion of extended dose
        pump->deliverBolus(remainingExtendedDose, bolusRate, /*suppressTime=*/true);
        
        //if (iobTracker)
        //    iobTracker->addBolus(remainingExtendedDose, QDateTime::currentDateTime());

        if (logger) {
            logger->logInsulin(QDateTime::currentDateTime(), remainingExtendedDose);
            logger->logEvent("Extended Bolus Delivered", QString("Delivered extended dose of %1 units.").arg(remainingExtendedDose, 0, 'f', 2));
        }

        //QMessageBox::information(this, "Extended Dose Delivered", QString("%1 units have been delivered.").arg(remainingExtendedDose, 0, 'f', 2));
       
        remainingExtendedDose = 0;
        //emit bolusStarted("Delivering: 0.00U");
        pump->bolusTimeRemainingUpdated(0.0);
        //extendedDoseTimer->stop();
        //countdownTimer->stop();
    }
}

void BolusCalculator::updateCountdown() {
    // Countdown tick — update remaining time
    countdownMinutes -= 5; // because each tick is 5 minutes
    if (countdownMinutes <= 0) {
        //countdownTimer->stop();
        //emit countdownActive(false);
        countdownMinutes = 0;
        deliverExtendedDose();;
    }
    pump->bolusTimeRemainingUpdated(countdownMinutes);
}

void BolusCalculator::on_logoButton_clicked() {
    emit backToHome();
}

void BolusCalculator::on_btnCancelBolus_clicked()
{
    countdownTimer->stop();
    //extendedDoseTimer->stop();
    emit countdownActive(false);

    if (pump) {
        // Cancel bolus on user action
        pump->suspendBolus();
        if (logger) {
            logger->logEvent("Manual", "Bolus Delivery has been cancelled by user");
        }
        pump->bolusTimeRemainingUpdated(0.0);
        QMessageBox::information(
            this, "Cancelled",
            "Bolus Delivery has been cancelled");
    } else {
        QMessageBox::warning(
            this, "Error",
            "Pump not available to cancel bolus");
    }
}
