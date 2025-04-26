/**
 * @file profile.h
 * @brief Declares the Profile model for user diabetes management settings.
 *
 * The Profile class encapsulates a single user profile, including basal insulin rate,
 * carbohydrate ratio, correction factor, and target glucose level. It provides static methods
 * to create, update, delete, select, load, and save a collection of named profiles to JSON,
 * and to retrieve the currently active profile.
 */
#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QList>
#include <QJsonObject>

/**
 * @brief Represents a user profile for managing diabetes-related data.
 *
 * The Profile class encapsulates user settings for diabetes management including
 * basal rate, carbohydrate ratio, correction factor, and target glucose levels.
 * It provides methods for profile creation, update, deletion, selection, and persistent storage.
 */
class Profile {
public:
    // Constructors:

    /**
     * @brief Default constructor.
     *
     * Creates an empty Profile with default values.
     */
    Profile();

    /**
     * @brief Constructs a new Profile object with specified parameters.
     *
     * @param name The name of the profile.
     * @param basalRate The basal insulin rate in units/hour.
     * @param carbRatio The carbohydrate ratio.
     * @param correctionFactor The correction factor for insulin dosing in .
     * @param targetGlucose The target blood glucose level.
     * @param id The unique identifier for the profile.
     */
    Profile(const QString &name, double basalRate, double carbRatio, double correctionFactor, double targetGlucose, int id);

    // Getters:
    int getId() const;
    QString getName() const;
    double getBasalRate() const;
    double getCarbRatio() const;
    double getCorrectionFactor() const;
    double getTargetGlucose() const;

    // Setters:
    void setId(int id);
    void setName(const QString &name);
    void setBasalRate(double rate);
    void setCarbRatio(double ratio);
    void setCorrectionFactor(double factor);
    void setTargetGlucose(double target);

    // Serialization for persistent storage:

    /**
     * @brief Serializes the profile to a JSON object.
     *
     * Converts the profile data to a JSON representation for storage.
     *
     * @return QJsonObject The JSON object representing the profile.
     */
    QJsonObject toJson() const;

    /**
     * @brief Constructs a Profile from a JSON object.
     *
     * Parses a JSON object to create a Profile instance.
     *
     * @param obj The JSON object containing profile data.
     * @return Profile The constructed Profile object.
     */
    static Profile fromJson(const QJsonObject &obj);

    // Static methods for profile management:

    /**
     * @brief Creates a new profile.
     *
     * Instantiates a new profile with the provided parameters and saves it.
     *
     * @param name The name of the profile.
     * @param basalRate The basal insulin rate.
     * @param carbRatio The carbohydrate ratio.
     * @param correctionFactor The correction factor.
     * @param targetGlucose The target blood glucose level.
     * @return true if the profile was created and saved successfully, false otherwise.
     */
    static bool createProfile(const QString &name, double basalRate, double carbRatio, double correctionFactor, double targetGlucose);

    /**
     * @brief Initializes the default profile if none exists.
     *
     * This function checks whether the profile data file exists at the expected path.
     * If the file does not exist, it creates a default profile with predefined parameters
     * and saves it. This is used during application startup to ensure that a usable 
     * profile is available.
     */
    static void initDefaultProfile();

    /**
     * @brief Updates an existing profile by its identifier.
     *
     * Modifies the profile with the given ID with new settings.
     *
     * @param id The identifier of the profile to update.
     * @param name The new name for the profile.
     * @param newBasalRate The new basal insulin rate.
     * @param newCarbRatio The new carbohydrate ratio.
     * @param newCorrectionFactor The new correction factor.
     * @param newTargetGlucose The new target blood glucose level.
     * @return true if the profile was updated and saved successfully, false otherwise.
     */
    static bool updateProfileById(int id, const QString &name, double newBasalRate, double newCarbRatio, double newCorrectionFactor, double newTargetGlucose);

    /**
     * @brief Deletes a profile by its identifier.
     *
     * Removes the profile with the given ID.
     *
     * @param id The identifier of the profile to delete.
     * @return true if the profile was deleted successfully, false otherwise.
     */
    static bool deleteProfileById(int id);

    /**
     * @brief Selects a profile to be the active profile.
     *
     * Sets the profile with the given ID as the active profile.
     *
     * @param id The identifier of the profile to select.
     * @return true if the profile was selected successfully, false otherwise.
     */
    static bool selectProfileById(int id);

    /**
     * @brief Retrieves a profile by its identifier.
     *
     * Returns the profile that matches the provided ID.
     *
     * @param id The identifier of the profile.
     * @return Profile for the matching profile, or an empty profile if not found.
     */
    static Profile getProfileById(int id);

    /**
     * @brief Retrieves the active profile.
     *
     * Returns the currently selected active profile.
     *
     * @return Profile for the active profile.
     */
    static Profile getActiveProfile();

    /**
     * @brief Retrieves all profiles.
     *
     * Returns a list of all available profiles.
     *
     * @return QList<Profile> A list of all profiles.
     */
    static QList<Profile> getAllProfiles();

    // Methods to load/save profiles from/to a JSON file:
    static bool loadProfiles();
    static bool saveProfiles();

private:
    int m_id;
    QString m_name;
    double m_basalRate;
    double m_carbRatio;
    double m_correctionFactor;
    double m_targetGlucose;

    // Static members for managing all profiles:
    static QList<Profile> s_profiles;
    static int s_nextId;
    static int s_activeProfileId;
    static QString s_profilesFilePath;
};

#endif // PROFILE_H
