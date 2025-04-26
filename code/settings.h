/**
 * @file settings.h
 * @brief Declares the Settings widget for creating and managing user profiles.
 *
 * The Settings class provides a Qt user interface to list existing profiles,
 * create new ones, update or delete selected profiles, and select an active profile.
 * It synchronizes with the Profile model and persists changes to JSON.
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class Settings;
}

/**
 * @brief Widget class for managing personal profiles.
 *
 * The Settings class provides a user interface for creating, updating, deleting,
 * selecting, and saving user profiles. It displays a list of existing profiles and
 * allows the user to modify profile details. The layout and widgets are defined in
 * the associated UI file (settings.ui).
 */
class Settings : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Constructs a new Settings widget.
     *
     * Initializes the UI components, connects signals/slots for profile operations,
     * and loads the current list of profiles.
     *
     * @param parent Pointer to the parent widget (default is nullptr).
     */
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void onCreateProfile();
    void onUpdateProfile();
    void onDeleteProfile();
    void onSelectProfile();
    void onSaveProfile();
    void onProfileListItemClicked(QListWidgetItem *item);

    void on_logoButton_clicked();

private:
    Ui::Settings *ui;
    void updateProfileList();
    QString currentProfileName() const;
    int currentProfileId() const;

signals:
    void backToHome();
};

#endif // SETTINGS_H
