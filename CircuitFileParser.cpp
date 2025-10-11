#include "CircuitFileParser.h"
#include "Configuration.h"

CircuitFileParser::CircuitFileParser(RuntimeConfigParser& pRuntimeConfigParser):
    mIsCircuitModified(false),
    mRuntimeConfigParser(pRuntimeConfigParser)
{}

void CircuitFileParser::LoadJson(const QFileInfo& pFileInfo, bool pIsFromRecents)
{
    QFile loadFile(pFileInfo.absoluteFilePath());

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        emit LoadCircuitFileFailedSignal(pFileInfo, pIsFromRecents);
        return;
    }

    QByteArray rawData = loadFile.readAll();

    if (file::SAVE_FORMAT == file::SaveFormat::BINARY)
    {
        rawData = qUncompress(rawData);

        if (rawData.isEmpty())
        {
            // if the data could not be decompressed
            emit LoadCircuitFileFailedSignal(pFileInfo, pIsFromRecents);
            return;
        }

        auto jsonDoc = QJsonDocument(QCborValue::fromCbor(rawData).toMap().toJsonObject());

        mCurrentFile = pFileInfo;
        if (jsonDoc.object().contains("uuid"))
        {
            // Load ID if it exists in file
            mUuid = jsonDoc.object()["uuid"].toString();
        }
        else
        {
            // Create new ID
            mUuid = QUuid::createUuid().toString();
        }
        mRuntimeConfigParser.AddRecentFilePath(mCurrentFile.value());
        emit LoadCircuitFileSuccessSignal(mCurrentFile.value(), jsonDoc.object());
    }
    else
    {
        auto jsonDoc = QJsonDocument::fromJson(rawData);

        mCurrentFile = pFileInfo;
        if (jsonDoc.object().contains("uuid"))
        {
            // Load ID if it exists in file
            mUuid = jsonDoc.object()["uuid"].toString();
        }
        else
        {
            // Create new ID
            mUuid = QUuid::createUuid().toString();
        }
        mRuntimeConfigParser.AddRecentFilePath(mCurrentFile.value());
        emit LoadCircuitFileSuccessSignal(mCurrentFile.value(), jsonDoc.object());
    }

    mRuntimeConfigParser.SetLastFilePath(pFileInfo.path());
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

    if (file::SAVE_FORMAT == file::SaveFormat::BINARY)
    {
        auto bin = QCborValue::fromJsonValue(pJson).toCbor();
        saveFile.write(qCompress(bin, 9));
    }
    else
    {
        auto json = QJsonDocument(pJson).toJson(QJsonDocument::Compact);
        saveFile.write(json);
    }

    mCurrentFile = pFileInfo;
    mIsCircuitModified = false;

    mRuntimeConfigParser.AddRecentFilePath(mCurrentFile.value());
    mRuntimeConfigParser.SetLastFilePath(pFileInfo.path());

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

const QString& CircuitFileParser::GetUuid() const
{
    return mUuid;
}

std::optional<QFileInfo> CircuitFileParser::GetFileInfo() const
{
    return mCurrentFile;
}
