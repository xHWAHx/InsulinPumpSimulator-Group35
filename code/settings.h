#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void onCreateProfile();
    void onUpdateProfile();
    void onDeleteProfile();
    void onSelectProfile();
    void onSaveProfile();
    void onBack();
    void onProfileListItemClicked(QListWidgetItem *item);

private:
    Ui::Settings *ui;
    void updateProfileList();
    QString currentProfileName() const;
    int currentProfileId() const;
};

#endif // SETTINGS_H
