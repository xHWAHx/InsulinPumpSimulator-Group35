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
    connect(ui-> comboGraphRange, QOverload<int>:: of(&QComboBox::currentIndexChanged), this, &Home::onGraphRangeChanged);
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
    QString xAxisLabel;

    axisX->setTitleText(xAxisLabel);
    axisX->setLabelsColor(Qt::white);
    axisX->setLabelsFont(QFont("sans serif", 10, -1, false));

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(2, 20);
    axisY->setTitleText("Glucose (mmol/L)");
    axisY->setLabelsColor(Qt::white);
    axisY->setLabelsFont(QFont("sans serif", 8, -1, false));

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

void Home::addGlucoseReading(double value)

{
    double timeInHours = double(currentTime)/12;
    if (value != -1) {
        series->append(timeInHours, value);
    }
    currentTime++;

    axisX->setRange(timeInHours - selectedGraphHours, timeInHours);
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

    if (glucose != -1) {
        ui->glucoseLabel->setText(QString::number(glucose, 'f', 1));
    } else {
        ui->glucoseLabel->setText("- ");
    }
    updateInsulinDisplay(insulin);
}

void Home::updateDateTime() {
   QString timeStr = QTime::currentTime().toString("hh:mm AP");
   QString dateStr = QDate::currentDate().toString("dd MMM");
   ui->timeLabel->setText(timeStr);
   ui->dateLabel->setText(dateStr);
}

void Home::updateBolusStatus(const QString &status) {
    ui->labelBolusStatus->setText(status);
}

void Home::updateBolusTimeRemaining(double seconds) {
    if (seconds == 0){
        ui->labelBolusTimeRemaining->setText("");
    } else {
        int minutes = static_cast<int>(seconds / 60);
        int secs = static_cast<int>(seconds) % 60;
        QString formatted = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0'));

        ui->labelBolusTimeRemaining->setText("Extended dose in: " + formatted);
    }
}

void Home::updateIOB(double iob) {
    ui-> iobLabel->setText(QString::number(iob, 'f', 1)+ "u");
}

void Home::updateInsulinDisplay(double insulinRemaining){
    ui-> insulinUnitsLabel-> setText(QString::number(insulinRemaining, 'f', 1) + " u");
    int percentage= static_cast<int>((insulinRemaining/300.0) * 100);
    ui-> insulinBar-> setValue(percentage);
}

void Home:: onGraphRangeChanged(int index){
    switch(index){
        case 0: selectedGraphHours= 1; break;
        case 1: selectedGraphHours= 3; break;
        case 2: selectedGraphHours= 6; break;
    }
}

