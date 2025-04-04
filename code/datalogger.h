#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QObject>
#include <QList>
#include <QDateTime>
#include <QString>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>

struct LogEntry {
    QDateTime timestamp;
    QString eventType;
    QString description;

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["timestamp"] = timestamp.toString(Qt::ISODate);
        obj["eventType"] = eventType;
        obj["description"] = description;
        return obj;
    }
    
    static LogEntry fromJson(const QJsonObject &obj) {
        LogEntry entry;
        entry.timestamp = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
        entry.eventType = obj["eventType"].toString();
        entry.description = obj["description"].toString();
        return entry;
    }
};

struct GlucoseLogEntry {
    QDateTime timestamp;
    double glucose;

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["timestamp"] = timestamp.toString(Qt::ISODate);
        obj["glucose"] = glucose;
        return obj;
    }
    
    static GlucoseLogEntry fromJson(const QJsonObject &obj) {
        GlucoseLogEntry entry;
        entry.timestamp = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
        entry.glucose = obj["glucose"].toDouble();
        return entry;
    }
};

struct InsulinLogEntry {
    QDateTime timestamp;
    double dose;

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["timestamp"] = timestamp.toString(Qt::ISODate);
        obj["dose"] = dose;
        return obj;
    }
    
    static InsulinLogEntry fromJson(const QJsonObject &obj) {
        InsulinLogEntry entry;
        entry.timestamp = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
        entry.dose = obj["dose"].toDouble();
        return entry;
    }
};

struct ProfileLogData {
    QList<LogEntry> logs;
    QList<GlucoseLogEntry> glucoseLog;
    QList<InsulinLogEntry> insulinLog;

    QJsonObject toJson() const {
        QJsonObject obj;
        QJsonArray logsArray;
        for (const LogEntry &entry : logs) {
            logsArray.append(entry.toJson());
        }
        obj["logs"] = logsArray;
        
        QJsonArray glucoseArray;
        for (const GlucoseLogEntry &g : glucoseLog) {
            glucoseArray.append(g.toJson());
        }
        obj["glucoseLog"] = glucoseArray;
        
        QJsonArray insulinArray;
        for (const InsulinLogEntry &i : insulinLog) {
            insulinArray.append(i.toJson());
        }
        obj["insulinLog"] = insulinArray;
        
        return obj;
    }
    
    static ProfileLogData fromJson(const QJsonObject &obj) {
        ProfileLogData data;
        QJsonArray logsArray = obj["logs"].toArray();
        for (const QJsonValue &val : logsArray) {
            data.logs.append(LogEntry::fromJson(val.toObject()));
        }
        QJsonArray glucoseArray = obj["glucoseLog"].toArray();
        for (const QJsonValue &val : glucoseArray) {
            data.glucoseLog.append(GlucoseLogEntry::fromJson(val.toObject()));
        }
        QJsonArray insulinArray = obj["insulinLog"].toArray();
        for (const QJsonValue &val : insulinArray) {
            data.insulinLog.append(InsulinLogEntry::fromJson(val.toObject()));
        }
        return data;
    }
};

class datalogger : public QObject
{
    Q_OBJECT
public:
    explicit datalogger(QObject *parent = nullptr);

    // Logging functions:
    void logEvent(const QString &eventType, const QString &description, int profileId);
    void logGlucose(const QDateTime &timestamp, double glucose, int profileId);
    void logInsulin(const QDateTime &timestamp, double dose, int profileId);

    // Retrieval functions:
    QList<LogEntry> retrieveHistory(int profileId) const;
    QList<GlucoseLogEntry> retrieveGlucoseLog(int profileId) const;
    QList<InsulinLogEntry> retrieveInsulinLog(int profileId) const;

    // Export function:
    bool exportLogs(const QString &filePath) const;

    // Persistent storage:
    bool loadLogs();
    bool saveLogs();

signals:
    void logsUpdated();

private:
    QMap<int, ProfileLogData> m_profileLogs;
    QString m_logsFilePath;
};

#endif // DATALOGGER_H
