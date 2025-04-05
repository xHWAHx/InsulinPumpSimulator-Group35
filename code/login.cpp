#include "login.h"
#include "HomeWindow.h"
#include "ui_login.h"
#include <QMessageBox>

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
        this-> close();
        HomeWindow *home= new HomeWindow ();
        home-> show();
    } else{
        QMessageBox:: warning(this, "Error", "Incorrect Pin, Try again");
        ui-> pinDisplay-> clear();

    }
}

