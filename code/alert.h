#ifndef ALERT_H
#define ALERT_H

#include <QWidget>

namespace Ui {
class Alert;
}

class Alert : public QWidget {
    Q_OBJECT

public:
    explicit Alert(QWidget *parent = nullptr);
    ~Alert();

    void showAlert(const QString &title, const QString &body);

private slots:
    void on_okButton_clicked();

private:
    Ui::Alert *ui;
};

#endif // ALERT_H
