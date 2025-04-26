#include "alert.h"
#include "ui_alert.h"

QHash<int, bool> Alert::raisedAlerts;

Alert::Alert(UserInterface *parent)
    : QWidget(parent)
    , ui(new Ui::Alert)
{
    ui->setupUi(this);
    connect(ui->okButton, &QPushButton::released, parent, [this, parent](){parent->dismissAlert(this);});
}

Alert::~Alert() {
    delete ui;
}

void Alert::raise(int type, UserInterface *interface, DataLogger *logger) {
    if (not raisedAlerts[type]){
        Alert *raisedAlert = new Alert(interface);
        switch(type){
            case BATTERY_LOW:
                raisedAlert->ui->alertBody->setText("Battery low. Please plug in the device to charge.");
                logger->logEvent("Warning", QString("Low Battery"));
                break;
            case INSULIN_LOW:
                raisedAlert->ui->alertBody->setText("Insulin is running low. Please refill the reservoir.");
                logger->logEvent("Warning", QString("Low Insulin"));
                break;
            case CGM_DISCONNECTED:
                raisedAlert->ui->alertBody->setText("CGM Disconnected, check sensor connection");
                logger->logEvent("Warning", QString("CGM disconnected"));
                break;
            case PUMP_OCCLUSION:
                raisedAlert->ui->alertBody->setText("Insulin pump is occluded, insulin pumping has been suspended. Please check insertion point");
                logger->logEvent("Warning", QString("Pump occluded"));
                break;
            case GLUCOSE_LOW:
                raisedAlert->ui->alertBody->setText("Glucose is below 3.9 mmol/L. Take 15g of fast-acting sugar. Bolus suspended.");
                logger->logEvent("Warning", QString("Glucose went below minimum safe level"));
                break;
            case GLUCOSE_HIGH:
                raisedAlert->ui->alertBody->setText("Glucose is above target. Consider using the Bolus Calculator.");
                logger->logEvent("Warning", QString("Glucose went above maximum safe level"));
                break;
        }
        interface->showAlert(raisedAlert);
        raisedAlerts[type] = true;
    }
}

void Alert::reset(int type) {
    raisedAlerts[type] = false;
}
