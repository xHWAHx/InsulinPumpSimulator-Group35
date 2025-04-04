#include "settings.h"
#include "profile.h"
#include <QMessageBox>
#include <QDebug>

settings::settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    ui->spinBoxBasal->setMaximum(999.99);
    ui->spinBoxCarb->setMaximum(999.99);
    ui->spinBoxCorrection->setMaximum(999.99);
    ui->spinBoxTarget->setMaximum(999.99);

    connect(ui->buttonCreate, &QPushButton::clicked, this, &settings::onCreateProfile);
    connect(ui->buttonUpdate, &QPushButton::clicked, this, &settings::onUpdateProfile);
    connect(ui->buttonDelete, &QPushButton::clicked, this, &settings::onDeleteProfile);
    connect(ui->buttonSelect, &QPushButton::clicked, this, &settings::onSelectProfile);
    connect(ui->buttonSave, &QPushButton::clicked, this, &settings::onSaveProfile);
    connect(ui->buttonBack, &QPushButton::clicked, this, &settings::onBack);
    connect(ui->profileList, &QListWidget::itemClicked, this, &settings::onProfileListItemClicked);

    if (!profile::loadProfiles()) {
        QMessageBox::warning(this, "Error", "Failed to load profiles.");
    }
    updateProfileList();
}

settings::~settings()
{
    delete ui;
}

void settings::onCreateProfile()
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

    if (profile::createProfile(name, basalRate, carbRatio, correction, targetGlucose)) {
        QMessageBox::information(this, "Success", "Profile created successfully.");
        updateProfileList();
    } else {
        QMessageBox::warning(this, "Error", "Profile creation failed. Profile may already exist.");
    }
}

void settings::onUpdateProfile()
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

    if (profile::updateProfileById(id, name, basalRate, carbRatio, correction, targetGlucose)) {
        QMessageBox::information(this, "Success", "Profile updated successfully.");
        updateProfileList();
    } else {
        QMessageBox::warning(this, "Error", "Profile update failed.");
    }
}

void settings::onDeleteProfile()
{
    int id = currentProfileId();
    if (id == -1) {
        QMessageBox::warning(this, "Error", "No profile selected.");
        return;
    }
    if (profile::deleteProfileById(id)) {
        QMessageBox::information(this, "Success", "Profile deleted successfully.");
        updateProfileList();
    } else {
        QMessageBox::warning(this, "Error", "Profile deletion failed.");
    }
}

void settings::onSelectProfile()
{
    int id = currentProfileId();
    if (id == -1) {
        QMessageBox::warning(this, "Error", "No profile selected.");
        return;
    }
    if (profile::selectProfileById(id)) {
        QMessageBox::information(this, "Success", "Profile selected as active.");
    } else {
        QMessageBox::warning(this, "Error", "Profile selection failed.");
    }
}

void settings::onSaveProfile()
{
    if (profile::saveProfiles()) {
        QMessageBox::information(this, "Success", "Profiles saved successfully.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to save profiles.");
    }
}

void settings::onBack()
{
    close();
}

void settings::onProfileListItemClicked(QListWidgetItem *item)
{
    if (!item)
        return;

    int id = item->data(Qt::UserRole).toInt();
    profile p = profile::getProfileById(id);
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

void settings::updateProfileList()
{
    ui->profileList->clear();
    QList<profile> profiles = profile::getAllProfiles();
    for (const profile &p : profiles) {
        QListWidgetItem *item = new QListWidgetItem(p.getName());
        item->setData(Qt::UserRole, p.getId());
        ui->profileList->addItem(item);
    }
}

QString settings::currentProfileName() const
{
    QListWidgetItem *item = ui->profileList->currentItem();
    return item ? item->text() : QString();
}

int settings::currentProfileId() const
{
    QListWidgetItem *item = ui->profileList->currentItem();
    return item ? item->data(Qt::UserRole).toInt() : -1;
}
