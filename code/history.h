#ifndef HISTORY_H
#define HISTORY_H

#include <QDialog>

class DataLogger;

namespace Ui {
    class Dialog;
}

/**
 * @brief Dialog class for displaying the history log.
 *
 * The history class provides a user interface for viewing event logs retrieved from a DataLogger instance.
 * It supports filtering based on search queries and event types, as defined in the associated UI file (history.ui).
 */
class history : public QDialog
{
    Q_OBJECT
public:

    /**
     * @brief Constructs a new History dialog.
     *
     * Sets up the UI elements and connects signals/slots to allow interactive viewing and filtering of history logs.
     *
     * @param logger Pointer to the DataLogger instance used for retrieving log data.
     * @param parent Pointer to the parent widget (default is nullptr).
     */
    explicit history(DataLogger *logger, QWidget *parent = nullptr);
    ~history();

signals:
    /**
     * @brief Signal emitted when the user requests to return to the previous view.
     */
    void backRequested();

private slots:
    void onBackButtonClicked();
    void refreshHistory();
    void onSearch();
    void onFilterChanged(const QString &text);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::Dialog *ui;
    DataLogger *m_logger;
};

#endif // HISTORY_H
