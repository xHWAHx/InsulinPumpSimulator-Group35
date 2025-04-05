#include "HomeWindow.h"
#include "ui_HomeWindow.h"

HomeWindow::HomeWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::HomeWindow), currentTime(0)
{
    ui->setupUi(this);
    setupChart();

    chartTimer = new QTimer(this);
    connect(chartTimer, &QTimer::timeout, this, &HomeWindow::requestCGMValue);
    chartTimer->start(3000);

    connect(ui->buttonBolus, &QPushButton::clicked, this, &HomeWindow::requestBolus);
    connect(ui->buttonOptions, &QPushButton::clicked, this, &HomeWindow::requestOptions);
    connect(ui->buttonEmergency, &QPushButton::clicked, this, &HomeWindow::requestEmergencyStop);
    connect(ui->buttonStats, &QPushButton::clicked, this, &HomeWindow::requestStats);
    connect(ui->buttonRefresh, &QPushButton::clicked, this, &HomeWindow::requestStatusRefresh);
}

HomeWindow::~HomeWindow() {
    delete ui;
}

void HomeWindow::setupChart()
{
    series = new QLineSeries();

    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("CGM Glucose Trend");
    chart->legend()->hide();
    chart->setBackgroundBrush(Qt::black);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 180);
    axisX->setTitleText("Time (min)");
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

void HomeWindow::addGlucoseReading(double value)
{
    series->append(currentTime++, value);
    if (series->count() > 180)
        series->removePoints(0, series->count() - 180);
}

void HomeWindow::updateStatus(double glucose, double battery, double insulin)
{
    ui->glucoseLabel->setText(QString::number(glucose, 'f', 1) + " mmol/L");
    ui->batteryLabel->setText(QString::number(battery, 'f', 0) + "%");
    ui->insulinLabel->setText(QString::number(insulin, 'f', 1) + " u");
}
