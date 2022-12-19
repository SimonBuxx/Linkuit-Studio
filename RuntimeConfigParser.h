#ifndef RUNTIMECONFIGPARSER_H
#define RUNTIMECONFIGPARSER_H

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <vector>

///
/// \brief The RuntimeConfigParser class parses the runtime config file
///
class RuntimeConfigParser
{
public:
    /// \brief Constructor for the RuntimeConfigParser
    RuntimeConfigParser(void);

    /// \brief Loads the runtime config from the given file path
    /// \param pPath: The path to the runtime config
    /// \return True, if the file was loaded sucessfully
    bool LoadRuntimeConfig(const QString& pPath);

    /// \brief Saves the runtime configto the given file path
    /// \param pPath: The path to save the runtime config into
    /// \return True, if the file was saved sucessfully
    bool SaveRuntimeConfig(const QString& pPath);

    /// \brief Returns true if the welcome dialog should be displayed on startup
    /// \return True, if the welcome dialog should be displayed on startup
    bool IsWelcomeDialogEnabledOnStartup(void) const;

    /// \brief Setter for whether the welcome dialog should be displayed on startup
    /// \param pShowOnStartup: If true, the welcome dialog will be displayed on startup
    void IsWelcomeDialogEnabledOnStartup(bool pShowOnStartup);

    /// \brief Getter for the list of recent files
    /// \return Vector of recent files
    const std::vector<QFileInfo>& GetRecentFilePaths(void) const;

    /// \brief Adds a file's info to the list of recent files
    /// \param pFilePath: The file info to add
    void AddRecentFilePath(const QFileInfo& pFilePath);

    /// \brief Getter for the last file path
    /// \return The path of the last opened or saved file
    const QString& GetLastFilePath(void) const;

    /// \brief Setter for the last file path
    /// \param pLastFilePath: The path of the last opened or saved file
    void SetLastFilePath(const QString& pLastFilePath);

protected:
    std::vector<QFileInfo> mRecentFiles;

    bool mIsWelcomeDialogEnabledOnStartup;

    QString mLastFilePath;
};

#endif // RUNTIMECONFIGPARSER_H
