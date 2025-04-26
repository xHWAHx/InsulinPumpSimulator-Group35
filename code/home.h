/**
 * @file home.h
 * @brief Defines the Home screen widget for displaying real-time system status.
 *
 * The Home class provides a dashboard showing current glucose levels, insulin remaining,
 * battery status, bolus delivery status, and a dynamic CGM glucose trend chart.
 * It offers navigation to bolus calculator, settings, and history screens.
 */

#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QTimer>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Home;
}

/**
 * @brief The Home class displays the main home screen and CGM glucose chart.
 */
class Home : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for Home screen.
     * @param parent Optional QWidget parent.
     */
    explicit Home(QWidget *parent = nullptr);

    /**
     * @brief Destructor for Home screen.
     */
    ~Home();

    /**
     * @brief Initializes and configures the glucose chart.
     */
    void setupChart();

    /**
     * @brief Adds a new glucose reading to the chart.
     * @param value Glucose value in mmol/L.
     */
    void addGlucoseReading(double value);

    /**
     * @brief Updates the displayed device status (battery, glucose, insulin).
     */
    void updateStatus(double glucose, double battery, double insulin);

    /**
     * @brief Updates the displayed insulin-on-board (IOB) value.
     */
    void updateIOB(double iob);

    /**
     * @brief Updates the insulin progress bar and label.
     * @param insulinRemaining Insulin remaining in the reservoir.
     */
    void updateInsulinDisplay(double insulinRemaining);

    /**
     * @brief Updates the label showing bolus delivery status.
     * @param status Text showing the current bolus status.
     */
    void updateBolusStatus(const QString &status);

    /**
     * @brief Updates the countdown for extended bolus delivery.
     * @param seconds Remaining seconds.
     */
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
    int selectedGraphHours = 1;

private slots:
    /**
     * @brief Updates the displayed time and date.
     */
    void updateDateTime();

    /**
     * @brief Handles graph range changes (1h, 3h, 6h).
     */
    void onGraphRangeChanged(int index);
};

#endif // HOMEWINDOW_H
