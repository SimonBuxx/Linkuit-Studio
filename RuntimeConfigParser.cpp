#include "RuntimeConfigParser.h"
#include "HelperFunctions.h"
#include "Configuration.h"

RuntimeConfigParser::RuntimeConfigParser() :
    mRecentFiles{},
    mIsWelcomeDialogEnabledOnStartup(gui::SHOW_WELCOME_DIALOG_ON_STARTUP) // default values
{}

bool RuntimeConfigParser::LoadRuntimeConfig(const QString& pPath)
{
    QFile loadFile(pPath);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QByteArray rawData = loadFile.readAll();

    QJsonObject json(QJsonDocument::fromJson(rawData).object());

    if (!json.contains(file::runtime_config::JSON_WELCOME_DIALOG_ENABLED_IDENTIFIER)
            || !json.contains(file::runtime_config::JSON_RECENT_FILES_IDENTIFIER))
    {
        return false;
    }

    mIsWelcomeDialogEnabledOnStartup = json[file::runtime_config::JSON_WELCOME_DIALOG_ENABLED_IDENTIFIER].toBool();

    auto recentFiles = json[file::runtime_config::JSON_RECENT_FILES_IDENTIFIER].toArray();

    for (uint32_t fileIdx = 0; fileIdx < recentFiles.size(); fileIdx++)
    {
        auto path = recentFiles[fileIdx].toString();
        mRecentFiles.push_back(QFileInfo(path));
    }

    return true;
}

bool RuntimeConfigParser::SaveRuntimeConfig(const QString& pPath)
{
    QFile saveFile(pPath);

    if (!saveFile.open(QIODevice::WriteOnly))
    {
        return false;
    }

    auto json = QJsonObject();

    if (mRecentFiles.size() > 5)
    {
        mRecentFiles.resize(5); // New recent files have to be appended to front
    }

    QJsonArray recentFiles;

    for (const auto& filePath : mRecentFiles)
    {
        recentFiles.append(QJsonValue(filePath.filePath()));
    }

    json[file::runtime_config::JSON_WELCOME_DIALOG_ENABLED_IDENTIFIER] = mIsWelcomeDialogEnabledOnStartup;
    json[file::runtime_config::JSON_RECENT_FILES_IDENTIFIER] = recentFiles;

    saveFile.write(QJsonDocument(json).toJson());

    return true;
}

bool RuntimeConfigParser::IsWelcomeDialogEnabledOnStartup() const
{
    return mIsWelcomeDialogEnabledOnStartup;
}

void RuntimeConfigParser::IsWelcomeDialogEnabledOnStartup(bool pShowOnStartup)
{
    mIsWelcomeDialogEnabledOnStartup = pShowOnStartup;
    SaveRuntimeConfig(GetRuntimeConfigAbsolutePath());
}

const std::vector<QFileInfo>& RuntimeConfigParser::GetRecentFilePaths() const
{
    return mRecentFiles;
}

void RuntimeConfigParser::AddRecentFilePath(const QFileInfo &pFilePath)
{
    auto pos = std::find(mRecentFiles.begin(), mRecentFiles.end(), pFilePath);
    if (pos != mRecentFiles.end())
    {
        mRecentFiles.erase(pos);
    }

    mRecentFiles.insert(mRecentFiles.begin(), pFilePath);
    if (mRecentFiles.size() > 5)
    {
        mRecentFiles.resize(5);
    }

    SaveRuntimeConfig(GetRuntimeConfigAbsolutePath());
}
