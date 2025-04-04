#ifndef HISTORY_H
#define HISTORY_H

#include <QDialog>

class datalogger;

namespace Ui {
    class Dialog;
}

class history : public QDialog
{
    Q_OBJECT
public:
    explicit history(datalogger *logger, QWidget *parent = nullptr);
    ~history();

signals:
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
    datalogger *m_logger;
};

#endif // HISTORY_H
