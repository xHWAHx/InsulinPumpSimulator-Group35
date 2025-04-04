#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QListWidgetItem>
#include "ui_settings.h"

class settings : public QWidget
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = nullptr);
    ~settings();

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
