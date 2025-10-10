#include "CustomsLibrary.h"


static constexpr auto TEST_JSON = R"(
{
    "Label": "HA",
    "Cells": [
        {
            "UID": 0,
            "Type": 12,
            "InputInversions": [false, false],
            "OutputInversions": [false, false],
            "OutputCells": [[7, 0, 0], [8, 0, 1]]
        },
        {
            "UID": 9,
            "Type": 29,
            "InputInversions": [false, false],
            "OutputInversions": [false, false],
            "OutputCells": [],
            "FileName": "aluslice.lsc",
            "FileID": 1234,
            "FileDate": 1234
        },
        {
            "UID": 1,
            "Type": 7,
            "OutputCells": [[5, 0, 0]],
            "Label": ""
        },
        {
            "UID": 2,
            "Type": 7,
            "OutputCells": [[6, 0, 0]],
            "Label": ""
        },
        {
            "UID": 3,
            "Type": 10,
            "OutputCells": [],
            "Label": "S"
        },
        {
            "UID": 4,
            "Type": 10,
            "OutputCells": [],
            "Label": "C"
        },
        {
            "UID": 5,
            "Type": 0,
            "OutputCells": [[0, 0, 0]]
        },
        {
            "UID": 6,
            "Type": 0,
            "OutputCells": [[0, 1, 0]]
        },
        {
            "UID": 7,
            "Type": 0,
            "OutputCells": [[3, 0, 0]]
        },
        {
            "UID": 8,
            "Type": 0,
            "OutputCells": [[4, 0, 0]]
        }
    ]
}
    )";

CustomsLibrary::CustomsLibrary(QObject *pParent)
    : QObject{pParent}
{
    AddCustomJson("Test", QJsonDocument::fromJson(TEST_JSON).object());
}

std::optional<std::shared_ptr<QJsonObject>> CustomsLibrary::GetCustomJson(const QString& pFileId) const
{
    if (mCustoms.find(pFileId) == mCustoms.end())
    {
        return std::nullopt;
    }

    return std::make_shared<QJsonObject>(mCustoms.at(pFileId));
}

void CustomsLibrary::AddCustomJson(const QString& pFileId, const QJsonObject& pJson)
{
    mCustoms.insert(std::make_pair(pFileId, pJson));
}
