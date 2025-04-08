#include "datalogger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

DataLogger::DataLogger(QObject *parent)
    : QObject(parent),
    m_logsFilePath("./data/logs.json")
{
}

void DataLogger::logEvent(const QString &eventType, const QString &description)
{
    LogEntry entry;
    entry.timestamp = QDateTime::currentDateTime();
    entry.eventType = eventType;
    entry.description = description;

    m_logs.logs.append(entry);
    saveLogs();
    emit logsUpdated();
}

void DataLogger::logGlucose(const QDateTime &timestamp, double glucose)
{
    GlucoseLogEntry entry;
    entry.timestamp = timestamp;
    entry.glucose = glucose;
    
    m_logs.glucoseLog.append(entry);
    saveLogs();
    emit logsUpdated();
}

void DataLogger::logInsulin(const QDateTime &timestamp, double dose)
{
    InsulinLogEntry entry;
    entry.timestamp = timestamp;
    entry.dose = dose;
    
    m_logs.insulinLog.append(entry);
    saveLogs();
    emit logsUpdated();
}

QList<LogEntry> DataLogger::retrieveHistory() const
{
    return m_logs.logs;
}

QList<GlucoseLogEntry> DataLogger::retrieveGlucoseLog() const
{
    return m_logs.glucoseLog;
}

QList<InsulinLogEntry> DataLogger::retrieveInsulinLog() const
{
    return m_logs.insulinLog;
}

bool DataLogger::exportLogs(const QString &filePath) const
{
    QJsonObject rootObj = m_logs.toJson();
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

bool DataLogger::loadLogs()
{
    QFile file(m_logsFilePath);

    if (!file.exists()) {
        m_logs = LogData();
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
        m_logs = LogData();
        return false;
    }

    QJsonObject rootObj = doc.object();
    m_logs = LogData::fromJson(rootObj);
    return true;
}

bool DataLogger::saveLogs()
{
    QJsonObject rootObj = m_logs.toJson();
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
