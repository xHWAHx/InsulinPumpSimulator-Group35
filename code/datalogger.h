/**
 * @file datalogger.h
 * @brief Declares the DataLogger for recording events, glucose readings, and insulin doses.
 *
 * The DataLogger class provides methods to log general events (Info, Warning, Error, etc.),
 * timestamped glucose measurements, and insulin doses and persist them to a JSON file. 
 * It supports loading existing logs, exporting to a path of your choice,
 * and emits a logsUpdated() signal whenever new entries are added.
 */
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
 * @brief Contains all logs.
 */
struct LogData {
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
    
    static LogData fromJson(const QJsonObject &obj) {
        LogData data;
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

    /**
     * @brief Returns the singleton instance of DataLogger.
     *
     * This static method creates and returns a single, shared instance of DataLogger.
     * It ensures that only one instance is used throughout the application.
     *
     * @param parent Optional parent for the DataLogger instance.
     * @return DataLogger* Pointer to the singleton DataLogger instance.
     */
    static DataLogger* instance(QObject *parent = nullptr) {
        static DataLogger *s_instance = new DataLogger(parent);
        return s_instance;
    }

    // Logging functions:

    /**
     * @brief Logs a general event.
     *
     * Records an event with the current timestamp, event type, and description.
     *
     * @param eventType A string describing the type of event.
     *                  Valid Values:
     *                  - "Info"
     *                  - "Warning"
     *                  - "Error"
     *                  - "Manual Bolus"
     *                  - "Extended Bolus"
     * @param description A detailed description of the event.
     *
     * @note This function saves logs after adding the event and emits the logsUpdated signal.
     */
    void logEvent(const QString &eventType, const QString &description);

    /**
     * @brief Logs a glucose reading.
     *
     * Records a glucose log entry with the specified timestamp and glucose value.
     *
     * @param timestamp The time at which the glucose reading was taken.
     * @param glucose The glucose value.
     *
     * @note This function saves logs after logging the glucose entry and emits the logsUpdated signal.
     */
    void logGlucose(const QDateTime &timestamp, double glucose);

    /**
     * @brief Logs an insulin dose.
     *
     * Records an insulin log entry with the specified timestamp and dose amount.
     *
     * @param timestamp The time at which the insulin dose was administered.
     * @param dose The insulin dose amount.
     *
     * @note This function saves logs after logging the insulin entry and emits the logsUpdated signal.
     */
    void logInsulin(const QDateTime &timestamp, double dose);

    // Retrieval functions:

    /**
     * @brief Retrieves the event history.
     *
     * Returns a list of all log entries.
     *
     * @return QList<LogEntry> A list of log entries.
     */
    QList<LogEntry> retrieveHistory() const;

    /**
     * @brief Retrieves the glucose log.
     *
     * Returns the list of glucose log entries.
     *
     * @return QList<GlucoseLogEntry> A list of glucose log entries.
     */
    QList<GlucoseLogEntry> retrieveGlucoseLog() const;

    /**
     * @brief Retrieves the insulin log.
     *
     * Returns the list of insulin log entries.
     *
     * @return QList<InsulinLogEntry> A list of insulin log entries.
     */
    QList<InsulinLogEntry> retrieveInsulinLog() const;

    /**
     * @brief Exports all logs to a specified file.
     *
     * Saves all logs to the provided file path in JSON format.
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
    LogData m_logs;
    QString m_logsFilePath;
};

#endif // DATALOGGER_H
