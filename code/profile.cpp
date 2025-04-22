#include "profile.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

QList<Profile> Profile::s_profiles;
int Profile::s_nextId = 1;
int Profile::s_activeProfileId = -1;
QString Profile::s_profilesFilePath = "./data/profiles.json";

Profile::Profile()
    : m_id(0),
      m_basalRate(0.0),
      m_carbRatio(0.0),
      m_correctionFactor(0.0),
      m_targetGlucose(0.0)
{
}

Profile::Profile(const QString &name, double basalRate, double carbRatio, double correctionFactor, double targetGlucose, int id)
    : m_id(id),
      m_name(name),
      m_basalRate(basalRate),
      m_carbRatio(carbRatio),
      m_correctionFactor(correctionFactor),
      m_targetGlucose(targetGlucose)
{
}

int Profile::getId() const { return m_id; }
QString Profile::getName() const { return m_name; }
double Profile::getBasalRate() const { return m_basalRate; }
double Profile::getCarbRatio() const { return m_carbRatio; }
double Profile::getCorrectionFactor() const { return m_correctionFactor; }
double Profile::getTargetGlucose() const { return m_targetGlucose; }

void Profile::setId(int id) { m_id = id; }
void Profile::setName(const QString &name) { m_name = name; }
void Profile::setBasalRate(double rate) { m_basalRate = rate; }
void Profile::setCarbRatio(double ratio) { m_carbRatio = ratio; }
void Profile::setCorrectionFactor(double factor) { m_correctionFactor = factor; }
void Profile::setTargetGlucose(double target) { m_targetGlucose = target; }

QJsonObject Profile::toJson() const {
    QJsonObject obj;
    obj["id"] = m_id;
    obj["name"] = m_name;
    obj["basalRate"] = m_basalRate;
    obj["carbRatio"] = m_carbRatio;
    obj["correctionFactor"] = m_correctionFactor;
    obj["targetGlucose"] = m_targetGlucose;
    return obj;
}

Profile Profile::fromJson(const QJsonObject &obj) {
    return Profile(
        obj["name"].toString(),
        obj["basalRate"].toDouble(),
        obj["carbRatio"].toDouble(),
        obj["correctionFactor"].toDouble(),
        obj["targetGlucose"].toDouble(),
        obj["id"].toInt()
    );
}

bool Profile::createProfile(const QString &name, double basalRate, double carbRatio, double correctionFactor, double targetGlucose) {
    Profile newProfile(name, basalRate, carbRatio, correctionFactor, targetGlucose, s_nextId++);
    s_profiles.append(newProfile);
    return saveProfiles();
}

void Profile::initDefaultProfile() {
    QFile profileFile(s_profilesFilePath);
    if (!profileFile.exists()) {
        createProfile("Default", 0.8, 0.09, 2.5, 5.5);
    }
}

bool Profile::updateProfileById(int id, const QString &name, double newBasalRate, double newCarbRatio, double newCorrectionFactor, double newTargetGlucose) {
    for (int i = 0; i < s_profiles.size(); i++) {
        if (s_profiles[i].getId() == id) {
            s_profiles[i].setName(name);
            s_profiles[i].setBasalRate(newBasalRate);
            s_profiles[i].setCarbRatio(newCarbRatio);
            s_profiles[i].setCorrectionFactor(newCorrectionFactor);
            s_profiles[i].setTargetGlucose(newTargetGlucose);
            return saveProfiles();
        }
    }
    qWarning() << "updateProfileById: Profile not found, id:" << id;
    return false;
}

bool Profile::deleteProfileById(int id) {
    if (id == 1) {
        qWarning() << "deleteProfileById: Cannot delete the default profile (id 1).";
        return false;
    }
    for (int i = 0; i < s_profiles.size(); i++) {
        if (s_profiles[i].getId() == id) {
            s_profiles.removeAt(i);
            if (s_activeProfileId == id)
                s_activeProfileId = 1;
            return saveProfiles();
        }
    }
    qWarning() << "deleteProfileById: Profile not found, id:" << id;
    return false;
}

bool Profile::selectProfileById(int id) {
    for (const Profile &p : s_profiles) {
        if (p.getId() == id) {
            s_activeProfileId = id;
            return saveProfiles();
        }
    }
    qWarning() << "selectProfileById: Profile not found, id:" << id;
    return false;
}

Profile Profile::getProfileById(int id) {
    for (const Profile &p : s_profiles) {
        if (p.getId() == id)
            return p;
    }
    qWarning() << "getProfileById: Profile not found, id:" << id;
    return Profile();
}

Profile Profile::getActiveProfile() {
    return getProfileById(s_activeProfileId);
}

QList<Profile> Profile::getAllProfiles() {
    return s_profiles;
}

bool Profile::loadProfiles() {
    QFile file(s_profilesFilePath);
    if (!file.exists()) {
        s_profiles.clear();
        s_activeProfileId = -1;
        s_nextId = 1;
        return true;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "loadProfiles: Could not open file for reading:" << s_profilesFilePath;
        return false;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "loadProfiles: JSON document is not an object.";
        s_profiles.clear();
        s_activeProfileId = -1;
        s_nextId = 1;
        return false;
    }

    QJsonObject rootObj = doc.object();
    s_activeProfileId = rootObj["activeProfileId"].toInt(-1);
    QJsonArray profilesArray = rootObj["profiles"].toArray();

    s_profiles.clear();
    int maxId = 0;
    for (int i = 0; i < profilesArray.size(); i++) {
        QJsonObject obj = profilesArray[i].toObject();
        Profile p = Profile::fromJson(obj);
        s_profiles.append(p);
        if (p.getId() > maxId)
            maxId = p.getId();
    }
    s_nextId = maxId + 1;
    return true;
}

bool Profile::saveProfiles() {
    QJsonObject rootObj;
    QJsonArray profilesArray;
    for (const Profile &p : s_profiles) {
        profilesArray.append(p.toJson());
    }
    rootObj["profiles"] = profilesArray;
    rootObj["activeProfileId"] = s_activeProfileId;

    QJsonDocument doc(rootObj);
    QFile file(s_profilesFilePath);
    QFileInfo info(file);
    QDir dir;
    if (!dir.exists(info.absolutePath())) {
        if (!dir.mkpath(info.absolutePath())) {
            qWarning() << "saveProfiles: Failed to create directory:" << info.absolutePath();
            return false;
        }
    }
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "saveProfiles: Could not open file for writing:" << s_profilesFilePath;
        return false;
    }
    qint64 bytesWritten = file.write(doc.toJson());
    file.close();
    if (bytesWritten == -1) {
        qWarning() << "saveProfiles: Failed to write JSON data to file.";
        return false;
    }
    return true;
}
