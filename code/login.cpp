#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QKeyEvent>


Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_button1_clicked()
{
    QString current= ui-> pinDisplay-> text();
    ui-> pinDisplay-> setText(current+ "1");
}


void Login::on_button2_clicked()
{
    QString current= ui-> pinDisplay-> text();
    ui-> pinDisplay-> setText(current+ "2");

}


void Login::on_button3_clicked()
{
    QString current= ui-> pinDisplay-> text();
    ui-> pinDisplay-> setText(current+ "3");

}


void Login::on_button4_clicked()
{
    QString current= ui-> pinDisplay-> text();
    ui-> pinDisplay-> setText(current+ "4");
}


void Login::on_button5_clicked()
{
    QString current= ui-> pinDisplay-> text();
    ui-> pinDisplay-> setText(current+ "5");

}


void Login::on_button6_clicked()
{
    QString current= ui-> pinDisplay-> text();
    ui-> pinDisplay-> setText(current+ "6");
}


void Login::on_button7_clicked()
{
    QString current= ui-> pinDisplay-> text();
    ui-> pinDisplay-> setText(current+ "7");
}


void Login::on_button8_clicked()
{
    QString current= ui-> pinDisplay-> text();
    ui-> pinDisplay-> setText(current+ "8");
}


void Login::on_button9_clicked()
{
    QString current= ui-> pinDisplay-> text();
    ui-> pinDisplay-> setText(current+ "9");
}


void Login::on_button0_clicked()
{
    QString current= ui-> pinDisplay-> text();
    ui-> pinDisplay-> setText(current+ "0");
}


void Login::on_buttonOk_clicked()
{
    QString pin= ui-> pinDisplay-> text();
    if (pin== "1234"){
        emit deviceUnlocked();
        ui-> pinDisplay-> clear();
    } else{
        QMessageBox:: warning(this, "Error", "Incorrect Pin, Try again");
        ui-> pinDisplay-> clear();
    }
}

void Login::keyPressEvent(QKeyEvent *event)
{
    QString current = ui->pinDisplay->text();
    if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
        QString digit = event->text();
        ui->pinDisplay->setText(current + digit);
    } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        on_buttonOk_clicked();
    } else if (event->key() == Qt::Key_Backspace) {
        ui->pinDisplay->setText(current.left(current.length() - 1));
    }

    QWidget::keyPressEvent(event);
}
