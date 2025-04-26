/**
 * @file login.h
 * @brief Defines the Login screen widget for pump authentication.
 *
 * The Login class provides a simple numeric PIN entry interface to authenticate
 * users before accessing the device. It handles PIN validation and emits a signal
 * to unlock the device upon successful login.
 */

#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class Login;
}

/**
 * @brief The Login class manages PIN entry for unlocking the device.
 */
class Login : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for Login screen.
     * @param parent Optional QWidget parent.
     */
    explicit Login(QWidget *parent = nullptr);

    /**
     * @brief Destructor for Login screen.
     */
    ~Login();

signals:
    /**
     * @brief Emitted when the correct PIN is entered and device unlocks.
     */
    void deviceUnlocked();

private slots:
    void on_button1_clicked();
    void on_button2_clicked();
    void on_button3_clicked();
    void on_button4_clicked();
    void on_button5_clicked();
    void on_button6_clicked();
    void on_button7_clicked();
    void on_button8_clicked();
    void on_button9_clicked();
    void on_button0_clicked();
    void on_buttonOk_clicked();

private:
    Ui::Login *ui;

protected:
    /**
     * @brief Captures keyboard input for entering the PIN.
     */
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // LOGIN_H
