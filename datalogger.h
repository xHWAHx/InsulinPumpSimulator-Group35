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

/**
 * @brief Represents a single log entry for general events.
 */
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

/**
 * @brief Represents a single glucose log entry.
 */
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

/**
 * @brief Represents a single insulin log entry.
 */
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

/**
 * @brief Contains all logs for a single profile.
 */
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

/**
 * @brief Manages data logging for events, glucose, and insulin entries.
 *
 * The DataLogger class provides functionality to record, save, load, and export log entries (might not need this).
 * It supports logging of general events as well as specific data for glucose and insulin.
 * Each log entry is associated with a profile identified by a unique profileId.
 */
class DataLogger : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a new DataLogger object.
     *
     * Initializes the DataLogger and sets the default log file path.
     *
     * @param parent Pointer to the parent QObject (default is nullptr).
     */
    explicit DataLogger(QObject *parent = nullptr);

    // Logging functions:

    /**
     * @brief Logs a general event.
     *
     * Records an event with the current timestamp, event type, and description.
     * The log entry is stored in the profile specified by profileId.
     *
     * @param eventType A string describing the type of event.
     * @param description A detailed description of the event.
     * @param profileId The identifier for the profile associated with this event.
     *
     * @note This function saves logs after adding the event and emits the logsUpdated signal.
     */
    void logEvent(const QString &eventType, const QString &description, int profileId);

    /**
     * @brief Logs a glucose reading.
     *
     * Records a glucose log entry with the specified timestamp and glucose value.
     * The entry is added to the glucose log for the profile identified by profileId.
     *
     * @param timestamp The time at which the glucose reading was taken.
     * @param glucose The glucose value.
     * @param profileId The identifier for the profile associated with this glucose reading.
     *
     * @note This function saves logs after logging the glucose entry and emits the logsUpdated signal.
     */
    void logGlucose(const QDateTime &timestamp, double glucose, int profileId);

    /**
     * @brief Logs an insulin dose.
     *
     * Records an insulin log entry with the specified timestamp and dose amount.
     * The entry is added to the insulin log for the profile identified by profileId.
     *
     * @param timestamp The time at which the insulin dose was administered.
     * @param dose The insulin dose amount.
     * @param profileId The identifier for the profile associated with this insulin entry.
     *
     * @note This function saves logs after logging the insulin entry and emits the logsUpdated signal.
     */
    void logInsulin(const QDateTime &timestamp, double dose, int profileId);

    // Retrieval functions:

    /**
     * @brief Retrieves the event history for a profile.
     *
     * Returns a list of all log entries for the profile identified by profileId.
     *
     * @param profileId The identifier for the profile whose history is to be retrieved.
     * @return QList<LogEntry> A list of log entries.
     */
    QList<LogEntry> retrieveHistory(int profileId) const;

    /**
     * @brief Retrieves the glucose log for a profile.
     *
     * Returns a list of glucose log entries for the profile identified by profileId.
     *
     * @param profileId The identifier for the profile whose glucose log is to be retrieved.
     * @return QList<GlucoseLogEntry> A list of glucose log entries.
     */
    QList<GlucoseLogEntry> retrieveGlucoseLog(int profileId) const;

    /**
     * @brief Retrieves the insulin log for a profile.
     *
     * Returns a list of insulin log entries for the profile identified by profileId.
     *
     * @param profileId The identifier for the profile whose insulin log is to be retrieved.
     * @return QList<InsulinLogEntry> A list of insulin log entries.
     */
    QList<InsulinLogEntry> retrieveInsulinLog(int profileId) const;

    /**
     * @brief Exports all logs to a specified file.
     *
     * Saves all logs from all profiles to the provided file path in JSON format.
     *
     * @param filePath The path to the file where logs will be exported.
     * @return true if the logs were exported successfully, false otherwise.
     * 
     * @note This function may not be necessary.
     */
    bool exportLogs(const QString &filePath) const;

    // Persistent storage functions:

    /**
     * @brief Loads logs from the log file.
     *
     * Reads logs from the default file path and updates the log.
     *
     * @return true if logs were loaded successfully, false otherwise.
     */
    bool loadLogs();

    /**
     * @brief Saves logs to the default log file.
     *
     * Writes the current logs to the default file path in JSON format.
     *
     * @return true if logs were saved successfully, false otherwise.
     */
    bool saveLogs();

signals:
    void logsUpdated();

private:
    QMap<int, ProfileLogData> m_profileLogs;
    QString m_logsFilePath;
};

#endif // DATALOGGER_H
