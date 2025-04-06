#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QTimer>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Home;
}

class Home : public QWidget { // made QWidget instead of QMainWindow because the main window will contain each page
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

    void setupChart();
    void addGlucoseReading(double value);
    void updateStatus(double glucose, double battery, double insulin);

signals:
    void requestBolus();
    void requestOptions();
    void requestEmergencyStop();
    void requestStats();
    void requestStatusRefresh();
    void requestCGMValue();

private:
    Ui::Home *ui;
    QChart *chart;
    QLineSeries *series;
    QTimer *chartTimer;
    int currentTime;
};

#endif // HOMEWINDOW_H
