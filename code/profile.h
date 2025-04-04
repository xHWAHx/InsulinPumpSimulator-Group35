#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QList>
#include <QJsonObject>

class profile {
public:
    // Constructors
    profile();
    profile(const QString &name, double basalRate, double carbRatio, double correctionFactor, double targetGlucose, int id);

    // Getters
    int getId() const;
    QString getName() const;
    double getBasalRate() const;
    double getCarbRatio() const;
    double getCorrectionFactor() const;
    double getTargetGlucose() const;

    // Setters
    void setId(int id);
    void setName(const QString &name);
    void setBasalRate(double rate);
    void setCarbRatio(double ratio);
    void setCorrectionFactor(double factor);
    void setTargetGlucose(double target);

    // Serialization for persistent storage
    QJsonObject toJson() const;
    static profile fromJson(const QJsonObject &obj);

    // Static methods for profile management
    static bool createProfile(const QString &name, double basalRate, double carbRatio, double correctionFactor, double targetGlucose);
    static bool updateProfileById(int id, const QString &name, double newBasalRate, double newCarbRatio, double newCorrectionFactor, double newTargetGlucose);
    static bool deleteProfileById(int id);
    static bool selectProfileById(int id);
    static profile getProfileById(int id);
    static profile getActiveProfile();
    static QList<profile> getAllProfiles();

    // Methods to load/save profiles from/to a JSON file
    static bool loadProfiles();
    static bool saveProfiles();

private:
    int m_id;
    QString m_name;
    double m_basalRate;
    double m_carbRatio;
    double m_correctionFactor;
    double m_targetGlucose;

    // Static members for managing all profiles
    static QList<profile> s_profiles;
    static int s_nextId;
    static int s_activeProfileId;
    static QString s_profilesFilePath;
};

#endif // PROFILE_H
