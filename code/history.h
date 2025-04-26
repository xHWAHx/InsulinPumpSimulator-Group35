/**
 * @file history.h
 * @brief Declares the History widget for viewing and filtering logged events.
 *
 * The History class provides a user interface for displaying the log entries maintained
 * by DataLogger. It supports text search and eventType filtering, and automatically
 * refreshes whenever new log entries arrive.
 */
#ifndef HISTORY_H
#define HISTORY_H

#include <QDialog>

class DataLogger;

namespace Ui {
    class History;
}

/**
 * @brief Dialog class for displaying the history log.
 *
 * The history class provides a user interface for viewing event logs retrieved from a DataLogger instance.
 * It supports filtering based on search queries and event types, as defined in the associated UI file (history.ui).
 */
class History : public QWidget
{
    Q_OBJECT
public:

    /**
     * @brief Constructs a new History dialog.
     *
     * Sets up the UI elements and connects signals/slots to allow interactive viewing and filtering of history logs.
     *
     * @param parent Pointer to the parent widget (default is nullptr).
     */
    explicit History(QWidget *parent = nullptr);

    ~History();

signals:
    /**
     * @brief Signal emitted when the user requests to return to the previous view.
     */
    void backToHome();
    void backRequested();

private slots:
    void onBackButtonClicked();
    void refreshHistory();
    void onSearch();
    void onFilterChanged(const QString &text);

    void on_logoButton_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::History *ui;
    DataLogger *m_logger;
};

#endif // HISTORY_H
