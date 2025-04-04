#include "datalogger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

datalogger::datalogger(QObject *parent)
    : QObject(parent),
    m_logsFilePath("./data/logs.json")
{
}

void datalogger::logEvent(const QString &eventType, const QString &description, int profileId)
{
    LogEntry entry;
    entry.timestamp = QDateTime::currentDateTime();
    entry.eventType = eventType;
    entry.description = description;

    if (!m_profileLogs.contains(profileId))
        m_profileLogs[profileId] = ProfileLogData();
    m_profileLogs[profileId].logs.append(entry);

    saveLogs();

    emit logsUpdated();
}

void datalogger::logGlucose(const QDateTime &timestamp, double glucose, int profileId)
{
    GlucoseLogEntry entry;
    entry.timestamp = timestamp;
    entry.glucose = glucose;
    
    if (!m_profileLogs.contains(profileId))
        m_profileLogs[profileId] = ProfileLogData();
    m_profileLogs[profileId].glucoseLog.append(entry);

    saveLogs();

    emit logsUpdated();
}

void datalogger::logInsulin(const QDateTime &timestamp, double dose, int profileId)
{
    InsulinLogEntry entry;
    entry.timestamp = timestamp;
    entry.dose = dose;
    
    if (!m_profileLogs.contains(profileId))
        m_profileLogs[profileId] = ProfileLogData();
    m_profileLogs[profileId].insulinLog.append(entry);

    saveLogs();

    emit logsUpdated();
}

QList<LogEntry> datalogger::retrieveHistory(int profileId) const
{
    if (m_profileLogs.contains(profileId))
        return m_profileLogs.value(profileId).logs;
    return QList<LogEntry>();
}

QList<GlucoseLogEntry> datalogger::retrieveGlucoseLog(int profileId) const
{
    if (m_profileLogs.contains(profileId))
        return m_profileLogs.value(profileId).glucoseLog;
    return QList<GlucoseLogEntry>();
}

QList<InsulinLogEntry> datalogger::retrieveInsulinLog(int profileId) const
{
    if (m_profileLogs.contains(profileId))
        return m_profileLogs.value(profileId).insulinLog;
    return QList<InsulinLogEntry>();
}

bool datalogger::exportLogs(const QString &filePath) const
{
    QJsonObject rootObj;
    QJsonObject profilesObj;
    for (auto it = m_profileLogs.constBegin(); it != m_profileLogs.constEnd(); ++it) {
        profilesObj.insert(QString::number(it.key()), it.value().toJson());
    }
    rootObj["profiles"] = profilesObj;

    QJsonDocument doc(rootObj);
    QFile file(filePath);
    QFileInfo info(file);
    QDir dir;
    if (!dir.exists(info.absolutePath())) {
        if (!dir.mkpath(info.absolutePath())) {
            qWarning() << "exportLogs: Failed to create directory:" << info.absolutePath();
            return false;
        }
    }
    if (!file.open(QIODevice::WriteOnly))
        return false;
    file.write(doc.toJson());
    file.close();
    return true;
}

bool datalogger::loadLogs()
{
    QFile file(m_logsFilePath);
    if (!file.exists()) {
        m_profileLogs.clear();
        return true;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "loadLogs: Could not open file:" << m_logsFilePath;
        return false;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "loadLogs: JSON document is not an object.";
        m_profileLogs.clear();
        return false;
    }
    QJsonObject rootObj = doc.object();
    QJsonObject profilesObj = rootObj["profiles"].toObject();
    m_profileLogs.clear();
    for (QString key : profilesObj.keys()) {
        int profileId = key.toInt();
        ProfileLogData logData = ProfileLogData::fromJson(profilesObj.value(key).toObject());
        m_profileLogs.insert(profileId, logData);
    }
    return true;
}

bool datalogger::saveLogs()
{
    QJsonObject rootObj;
    QJsonObject profilesObj;
    for (auto it = m_profileLogs.constBegin(); it != m_profileLogs.constEnd(); ++it) {
        profilesObj.insert(QString::number(it.key()), it.value().toJson());
    }
    rootObj["profiles"] = profilesObj;

    QJsonDocument doc(rootObj);
    QFile file(m_logsFilePath);
    QFileInfo info(file);
    QDir dir;
    if (!dir.exists(info.absolutePath())) {
        if (!dir.mkpath(info.absolutePath())) {
            qWarning() << "saveLogs: Failed to create directory:" << info.absolutePath();
            return false;
        }
    }
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "saveLogs: Could not open file for writing:" << m_logsFilePath;
        return false;
    }
    qint64 bytesWritten = file.write(doc.toJson());
    file.close();
    if (bytesWritten == -1) {
        qWarning() << "saveLogs: Failed to write logs to file.";
        return false;
    }
    return true;
}
