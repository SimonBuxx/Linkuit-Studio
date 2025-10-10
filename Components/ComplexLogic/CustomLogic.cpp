#include "CustomLogic.h"
#include "CoreLogic.h"
#include "HelperFunctions.h"
#include "LogicCells/LogicCustomTestCell.h"

CustomLogic::CustomLogic(const CoreLogic* pCoreLogic, Direction pDirection, const CustomsLibrary& pLibrary, const QString& pFileId):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicCustomTestCell>(pFileId, pLibrary)),
    mFileId(pFileId),
    mLibrary(pLibrary)
{
    Init(mLogicCell->GetNumInputs(), mLogicCell->GetNumOutputs(), pDirection);
    mDescriptionFontSize = 8;

    ConfigureAppearance(mFileId);

    while (mInputLabels.size() < mInputCount)
    {
        mInputLabels.push_back("");
    }

    while (mOutputLabels.size() < mOutputCount)
    {
        mOutputLabels.push_back("");
    }
}

CustomLogic::CustomLogic(const CustomLogic& pObj, const CoreLogic* pCoreLogic):
    CustomLogic(pCoreLogic, pObj.mDirection, pObj.mLibrary, pObj.mFileId)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

CustomLogic::CustomLogic(const CoreLogic* pCoreLogic, const QJsonObject& pJson, const CustomsLibrary& pLibrary):
    CustomLogic(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()), pLibrary, pJson["FileName"].toString())
{
    setPos(SnapToGrid(QPointF(pJson["x"].toInt(), pJson["y"].toInt())));

    std::vector<bool> ininv, outinv;
    auto ininvArray = pJson["ininv"].toArray();
    for (const auto& inv : ininvArray)
    {
        ininv.push_back(inv.toBool());
    }
    GetLogicCell()->SetInputInversions(ininv);

    auto outinvArray = pJson["outinv"].toArray();
    for (const auto& inv : outinvArray)
    {
        outinv.push_back(inv.toBool());
    }
    GetLogicCell()->SetOutputInversions(outinv);
}

void CustomLogic::ConfigureAppearance(const QString& pFileId)
{
    const auto optJson = mLibrary.GetCustomJson(pFileId);

    if (!optJson.has_value())
    {
        qDebug() << "Error: Could not configure component appearance";
        return;
    }

    const auto json = optJson.value().get();

    if (!json->contains("Cells") || !(*json)["Cells"].isArray())
    {
        qWarning("JSON object does not contain a 'Cells' array.");
        return;
    }

    if (json->contains("Label") && (*json)["Label"].isString())
    {
        mComponentText = (*json)["Label"].toString("");
#warning magic numbers, make constants
        if (mComponentText.size() <= 3)
        {
            mDescriptionFontSize = components::complex_logic::DEFAULT_DESCRIPTION_FONT_SIZE;
        }
        else
        {
            mDescriptionFontSize = 8;
        }
    }

    // Get relevant data to determine component appearance

    QJsonArray cellsArray = (*json)["Cells"].toArray();

#warning In-/output order should be configurable
#warning Implement setter for in-/output labels that appends empty strings if necessary
    for (const QJsonValue& cellValue : cellsArray)
    {
        QJsonObject cellObj = cellValue.toObject();

        if (cellObj["Type"].toInt() == file::ComponentId::INPUT && cellObj.contains("Label") && (*json)["Label"].isString())
        {
            mInputLabels.push_back(cellObj["Label"].toString(""));
        }
        else if (cellObj["Type"].toInt() == file::ComponentId::OUTPUT && cellObj.contains("Label") && (*json)["Label"].isString())
        {
            mOutputLabels.push_back(cellObj["Label"].toString(""));
        }
    }
}

IBaseComponent* CustomLogic::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new CustomLogic(*this, pCoreLogic);
}

QJsonObject CustomLogic::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::CUSTOM_LOGIC;
    json["x"] = x();
    json["y"] = y();
    json["dir"] = static_cast<int32_t>(mDirection);
    {
        QJsonArray ininv, outinv;

        for(const bool& inv : mLogicCell->GetInputInversions())
        {
            ininv.append(inv);
        }

        json["ininv"] = ininv;

        for(const bool& inv : mLogicCell->GetOutputInversions())
        {
            outinv.append(inv);
        }

        json["outinv"] = outinv;
    }

    return json;
}

SwVersion CustomLogic::GetMinVersion(void) const
{
    return SwVersion(1, 1, 0);
}
