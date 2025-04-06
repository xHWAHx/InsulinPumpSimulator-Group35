#include "home.h"
#include "ui_home.h"

Home::Home(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Home)
    , currentTime(0)
{
    ui->setupUi(this);
    setupChart();

    chartTimer = new QTimer(this);
    chartTimer->start(3000);

    connect(ui->buttonBolus, &QPushButton::clicked, this, &Home::requestBolus);
    connect(ui->buttonOptions, &QPushButton::clicked, this, &Home::requestOptions);
    connect(ui->buttonStats, &QPushButton::clicked, this, &Home::requestStats);
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

void Home::addGlucoseReading(double value)
{
    series->append(currentTime++, value);
    if (series->count() > 180)
        series->removePoints(0, series->count() - 180);
}

void Home::updateStatus(double glucose, double battery, double insulin)
{
    ui->glucoseLabel->setText(QString::number(glucose, 'f', 1) + " mmol/L");
    ui->batteryLabel->setText(QString::number(battery, 'f', 0) + "%");
    ui->insulinLabel->setText(QString::number(insulin, 'f', 1) + " u");
}
