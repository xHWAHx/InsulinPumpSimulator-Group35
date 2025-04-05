#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QTimer>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

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
    Ui::HomeWindow *ui;
    QChart *chart;
    QLineSeries *series;
    QTimer *chartTimer;
    int currentTime;
};

#endif // HOMEWINDOW_H
