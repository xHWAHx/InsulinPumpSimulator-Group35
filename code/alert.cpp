#include "alert.h"
#include "ui_alert.h"

Alert::Alert(QWidget *)
    : QWidget(nullptr)
    , ui(new Ui::Alert)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt:: CustomizeWindowHint | Qt::WindowTitleHint);
    setAttribute(Qt::WA_DeleteOnClose);
}

Alert::~Alert() {
    delete ui;
}

void Alert::showAlert(const QString &title, const QString &body) {
    ui->alertTitle->setText(title);
    ui->alertBody->setText(body);
    this-> setWindowModality(Qt::ApplicationModal);
    this->show();
}

void Alert::on_okButton_clicked() {
    this->close();
}
