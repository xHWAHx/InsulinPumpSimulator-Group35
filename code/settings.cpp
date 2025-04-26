#include "settings.h"
#include "ui_settings.h"
#include "profile.h"
#include <QMessageBox>
#include <QDebug>

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    ui->spinBoxBasal->setMaximum(999.99);
    ui->spinBoxCarb->setMaximum(999.99);
    ui->spinBoxCorrection->setMaximum(999.99);
    ui->spinBoxTarget->setMaximum(999.99);

    connect(ui->buttonCreate, &QPushButton::clicked, this, &Settings::onCreateProfile);
    connect(ui->buttonUpdate, &QPushButton::clicked, this, &Settings::onUpdateProfile);
    connect(ui->buttonDelete, &QPushButton::clicked, this, &Settings::onDeleteProfile);
    connect(ui->buttonSelect, &QPushButton::clicked, this, &Settings::onSelectProfile);
    connect(ui->buttonSave, &QPushButton::clicked, this, &Settings::onSaveProfile);
    connect(ui->profileList, &QListWidget::itemClicked, this, &Settings::onProfileListItemClicked);

    if (!Profile::loadProfiles()) {
        QMessageBox::warning(this, "Error", "Failed to load profiles.");
    }
    updateProfileList();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::onCreateProfile()
{
    QString name = ui->lineEditName->text();
    double basalRate = ui->spinBoxBasal->value();
    double carbRatio = ui->spinBoxCarb->value();
    double correction = ui->spinBoxCorrection->value();
    double targetGlucose = ui->spinBoxTarget->value();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Profile name cannot be empty.");
        return;
    }

    if (Profile::createProfile(name, basalRate, carbRatio, correction, targetGlucose)) {
        QMessageBox::information(this, "Success", "Profile created successfully.");
        updateProfileList();
    } else {
        QMessageBox::warning(this, "Error", "Profile creation failed. Profile may already exist.");
    }
}

void Settings::onUpdateProfile()
{
    int id = currentProfileId();
    if (id == -1) {
        QMessageBox::warning(this, "Error", "No profile selected.");
        return;
    }
    QString name = ui->lineEditName->text();
    double basalRate = ui->spinBoxBasal->value();
    double carbRatio = ui->spinBoxCarb->value();
    double correction = ui->spinBoxCorrection->value();
    double targetGlucose = ui->spinBoxTarget->value();

    if (Profile::updateProfileById(id, name, basalRate, carbRatio, correction, targetGlucose)) {
        QMessageBox::information(this, "Success", "Profile updated successfully.");
        updateProfileList();
    } else {
        QMessageBox::warning(this, "Error", "Profile update failed.");
    }
}

void Settings::onDeleteProfile()
{
    int id = currentProfileId();
    if (id == -1) {
        QMessageBox::warning(this, "Error", "No profile selected.");
        return;
    }
    if (Profile::deleteProfileById(id)) {
        QMessageBox::information(this, "Success", "Profile deleted successfully.");
        updateProfileList();
    } else {
        QMessageBox::warning(this, "Error", "Profile deletion failed.");
    }
}

void Settings::onSelectProfile()
{
    int id = currentProfileId();
    if (id == -1) {
        QMessageBox::warning(this, "Error", "No profile selected.");
        return;
    }
    if (Profile::selectProfileById(id)) {
        QMessageBox::information(this, "Success", "Profile selected as active.");
    } else {
        QMessageBox::warning(this, "Error", "Profile selection failed.");
    }
}

void Settings::onSaveProfile()
{
    if (Profile::saveProfiles()) {
        QMessageBox::information(this, "Success", "Profiles saved successfully.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to save profiles.");
    }
}

void Settings::onProfileListItemClicked(QListWidgetItem *item)
{
    if (!item)
        return;

    int id = item->data(Qt::UserRole).toInt();
    Profile p = Profile::getProfileById(id);
    if (p.getId() == 0) {
        qWarning() << "onProfileListItemClicked: Profile not found, id:" << id;
        return;
    }

    ui->lineEditName->setText(p.getName());
    ui->spinBoxBasal->setValue(p.getBasalRate());
    ui->spinBoxCarb->setValue(p.getCarbRatio());
    ui->spinBoxCorrection->setValue(p.getCorrectionFactor());
    ui->spinBoxTarget->setValue(p.getTargetGlucose());
}

void Settings::updateProfileList()
{
    ui->profileList->clear();
    QList<Profile> profiles = Profile::getAllProfiles();
    for (const Profile &p : profiles) {
        QListWidgetItem *item = new QListWidgetItem(p.getName());
        item->setData(Qt::UserRole, p.getId());
        ui->profileList->addItem(item);
    }
}

QString Settings::currentProfileName() const
{
    QListWidgetItem *item = ui->profileList->currentItem();
    return item ? item->text() : QString();
}

int Settings::currentProfileId() const
{
    QListWidgetItem *item = ui->profileList->currentItem();
    return item ? item->data(Qt::UserRole).toInt() : -1;
}

void Settings::on_logoButton_clicked()
{
    emit backToHome();
    this-> close();
}

