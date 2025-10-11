#include "CustomLogic.h"
#include "CoreLogic.h"
#include "HelperFunctions.h"
#include "LogicCells/LogicCustomCell.h"

CustomLogic::CustomLogic(const CoreLogic* pCoreLogic, Direction pDirection, const CustomsLibrary& pLibrary, const CircuitId& pCircuitId):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicCustomCell>(pCircuitId, pLibrary)),
    mCircuitId(pCircuitId),
    mLibrary(pLibrary)
{
    Init(mLogicCell->GetNumInputs(), mLogicCell->GetNumOutputs(), pDirection);
    mDescriptionFontSize = 8;

    ConfigureAppearance(mCircuitId);

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
    CustomLogic(pCoreLogic, pObj.mDirection, pObj.mLibrary, pObj.mCircuitId)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

CustomLogic::CustomLogic(const CoreLogic* pCoreLogic, const QJsonObject& pJson, const CustomsLibrary& pLibrary):
    CustomLogic(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()), pLibrary, CircuitId(pJson["uuid"].toString(), pJson["timestamp"].toInt()))
{
    setPos(SnapToGrid(QPointF(pJson["x"].toInt(), pJson["y"].toInt())));

    std::vector<bool> ininv, outinv;
    auto ininvArray = pJson["ininv"].toArray();
    for (const auto& inv : std::as_const(ininvArray))
    {
        ininv.push_back(inv.toBool());
    }
    GetLogicCell()->SetInputInversions(ininv);

    auto outinvArray = pJson["outinv"].toArray();
    for (const auto& inv : std::as_const(outinvArray))
    {
        outinv.push_back(inv.toBool());
    }
    GetLogicCell()->SetOutputInversions(outinv);
}

void CustomLogic::ConfigureAppearance(const CircuitId& pCircuitId)
{
    const auto optJson = mLibrary.GetCustomJson(pCircuitId);

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
        if (mComponentText.size() <= components::complex_logic::MAX_CHARS_FOR_BIG_DESCRIPTION)
        {
            mDescriptionFontSize = components::complex_logic::DEFAULT_DESCRIPTION_FONT_SIZE;
        }
        else
        {
            mDescriptionFontSize = components::complex_logic::SMALL_DESCRIPTION_FONT_SIZE;
        }
    }

    // Get relevant data to determine component appearance

    QJsonArray cellsArray = (*json)["Cells"].toArray();

    for (const QJsonValue& cellValue : std::as_const(cellsArray))
    {
        QJsonObject cellObj = cellValue.toObject();

        if ((cellObj["Type"].toInt() == file::ComponentId::INPUT || cellObj["Type"].toInt() == file::ComponentId::BUTTON) && cellObj.contains("Label") && (*json)["Label"].isString())
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
    
    json["uuid"] = std::get<0>(mCircuitId);
    json["timestamp"] = std::get<1>(mCircuitId);

    return json;
}

SwVersion CustomLogic::GetMinVersion() const
{
    return SwVersion(1, 1, 0);
}
