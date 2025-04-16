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
    void updateIOB(double iob);
    void updateInsulinDisplay(double insulinRemaining);
    //13/04/2025 
    void updateBolusStatus(const QString &status);
    void updateBolusTimeRemaining(double seconds);


signals:
    void requestBolus();
    void requestOptions();
    void requestEmergencyStop();
    void requestStats();

private:
    Ui::Home *ui;
    QChart *chart;
    QLineSeries *series;
    QTimer *chartTimer;
    int currentTime;
    QTimer *clockTimer;
    QValueAxis *axisX;
    int selectedGraphHours= 3;

private slots:
   void updateDateTime();
   void onGraphRangeChanged(int index);
};

#endif // HOMEWINDOW_H
