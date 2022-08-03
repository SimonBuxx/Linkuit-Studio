#include "CircuitFileParser.h"
#include "Configuration.h"

CircuitFileParser::CircuitFileParser(RuntimeConfigParser& pRuntimeConfigParser):
    mIsCircuitModified(false),
    mRuntimeConfigParser(pRuntimeConfigParser)
{}

void CircuitFileParser::LoadJson(const QFileInfo& pFileInfo)
{
    QFile loadFile(pFileInfo.absoluteFilePath());

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        emit LoadCircuitFileFailedSignal(pFileInfo);
        return;
    }

    QByteArray rawData = loadFile.readAll();

    QJsonDocument jsonDoc(file::SAVE_FORMAT == file::SaveFormat::JSON
                    ? QJsonDocument::fromJson(rawData)
                    : QJsonDocument(QCborValue::fromCbor(rawData).toMap().toJsonObject()));

    mCurrentFile = pFileInfo;

    mRuntimeConfigParser.AddRecentFilePath(mCurrentFile.value());

    emit LoadCircuitFileSuccessSignal(mCurrentFile.value(), jsonDoc.object());
}

void CircuitFileParser::SaveJson(const QJsonObject& pJson)
{
    if (mCurrentFile.has_value())
    {
        SaveJsonAs(mCurrentFile.value(), pJson);
    }
}

void CircuitFileParser::SaveJsonAs(const QFileInfo& pFileInfo, const QJsonObject& pJson)
{
    QFile saveFile(pFileInfo.absoluteFilePath());

    if (!saveFile.open(QIODevice::WriteOnly))
    {
        emit SaveCircuitFileFailedSignal(pFileInfo);
        return;
    }

    saveFile.write(file::SAVE_FORMAT == file::SaveFormat::JSON
                   ? QJsonDocument(pJson).toJson()
                   : QCborValue::fromJsonValue(pJson).toCbor());

    mCurrentFile = pFileInfo;
    mIsCircuitModified = false;

    mRuntimeConfigParser.AddRecentFilePath(mCurrentFile.value());

    emit SaveCircuitFileSuccessSignal(mCurrentFile.value());}

void CircuitFileParser::ResetCurrentFileInfo()
{
    mCurrentFile = std::nullopt;
    mIsCircuitModified = false;
}

bool CircuitFileParser::IsFileOpen() const
{
    return mCurrentFile.has_value();
}

bool CircuitFileParser::IsCircuitModified() const
{
    return mIsCircuitModified;
}

void CircuitFileParser::MarkAsModified()
{
    if (!mIsCircuitModified)
    {
        mIsCircuitModified = true;
        emit CircuitModifiedSignal();
    }
}

std::optional<QFileInfo> CircuitFileParser::GetFileInfo() const
{
    return mCurrentFile;
}
