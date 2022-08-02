#ifndef RUNTIMECONFIGPARSER_H
#define RUNTIMECONFIGPARSER_H

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <vector>

class RuntimeConfigParser
{
public:
    RuntimeConfigParser(void);

    bool LoadRuntimeConfig(const QString& pPath);

    bool SaveRuntimeConfig(const QString& pPath);

    bool IsWelcomeDialogEnabledOnStartup(void) const;
    void IsWelcomeDialogEnabledOnStartup(bool pShowOnStartup);

    const std::vector<QFileInfo>& GetRecentFilePaths(void) const;
    void AddRecentFilePath(const QFileInfo& pFilePath);

protected:
    std::vector<QFileInfo> mRecentFiles;

    bool mIsWelcomeDialogEnabledOnStartup;
};

#endif // RUNTIMECONFIGPARSER_H
