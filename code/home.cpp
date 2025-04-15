#include "home.h"
#include "ui_home.h"
#include <QTime>
#include <QDate>

Home::Home(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Home)
    , currentTime(0)
{
    ui->setupUi(this);
    setupChart();

    clockTimer= new QTimer(this);
    clockTimer-> start(1000);

    chartTimer = new QTimer(this);
    chartTimer->start(3000);

    connect(ui->buttonBolus, &QPushButton::clicked, this, &Home::requestBolus);
    connect(ui->buttonOptions, &QPushButton::clicked, this, &Home::requestOptions);
    connect(ui->historyButton, &QPushButton::clicked, this, &Home::requestStats);
    connect(clockTimer, &QTimer::timeout, this, &Home::updateDateTime);


}

Home::~Home() {
    delete ui;
}

void Home::setupChart()
{
    series = new QLineSeries();

    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("CGM Glucose Trend");
    chart->legend()->hide();
    chart->setBackgroundBrush(Qt::black);
    chart->zoom(0.5);

    axisX = new QValueAxis;
    axisX->setRange(0, 180);
    axisX->setTitleText("Time (sec)");
    axisX->setLabelsColor(Qt::white);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(2, 20);
    axisY->setTitleText("Glucose (mmol/L)");
    axisY->setLabelsColor(Qt::white);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

void Home::addGlucoseReading(double value)
{
    series->append(currentTime *0.25, value);
    currentTime++;
    if (currentTime * 0.25 >= 180.0){
        axisX->setRange((currentTime* 0.25)- 180, currentTime * 0.25);
    }
}

void Home::updateStatus(double glucose, double battery, double insulin)
{
    int batteryPercent = static_cast<int>(battery * 100);
    ui->batteryBar->setValue(batteryPercent);
    ui->batteryLabel->setText(QString::number(batteryPercent) + "%");

    QString barColor;
    QString textColor;
    if (batteryPercent > 50) {
        barColor = "#00FF00";
        textColor = "#00FF00";
    } else if (batteryPercent > 20) {
        barColor = "#FFFF00";
        textColor = "#CCCC00";
    } else {
        barColor = "#FF0000";
        textColor = "#FF4444";
    }
    QString style = QString(
        "QProgressBar::chunk { background-color: %1; }"
        "QProgressBar { border: 1px solid #000; text-align: center; }"
    ).arg(barColor);
    ui->batteryBar->setStyleSheet(style);
    ui->batteryLabel->setStyleSheet("QLabel { color: " + textColor + "; font-weight: bold; }");

    ui->glucoseLabel->setText(QString::number(glucose, 'f', 1));
    ui->insulinLabel->setText(QString::number(insulin, 'f', 1) + " u");
}



void Home::updateDateTime() {
   QString timeStr = QTime::currentTime().toString("hh:mm AP");
   QString dateStr = QDate::currentDate().toString("dd MMM");
   ui->timeLabel->setText(timeStr);
   ui->dateLabel->setText(dateStr);
}


void Home::updateIOB(double iob) {
    ui-> iobLabel->setText(QString::number(iob, 'f', 1)+ "u");
}

void Home::updateInsulinDisplay(double insulinRemaining){
    ui-> insulinUnitsLabel-> setText(QString::number(insulinRemaining, 'f', 1) + " u");
    int percentage= static_cast<int>((insulinRemaining/300.0) * 100);
    ui-> insulinBar-> setValue(percentage);
}
