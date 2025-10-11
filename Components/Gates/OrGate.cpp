#include "OrGate.h"
#include "CoreLogic.h"
#include "HelperFunctions.h"
#include "LogicCells/LogicOrGateCell.h"

OrGate::OrGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection):
    AbstractGate(pCoreLogic, std::make_shared<LogicOrGateCell>(pInputCount), pInputCount, pDirection)
{
    mComponentText = components::gates::OR_TEXT;
}

OrGate::OrGate(const OrGate& pObj, const CoreLogic* pCoreLogic):
    OrGate(pCoreLogic, pObj.mInputCount, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

OrGate::OrGate(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    OrGate(pCoreLogic, pJson["inputs"].toInt(), static_cast<Direction>(pJson["dir"].toInt()))
{
    setPos(SnapToGrid(QPointF(pJson["x"].toInt(), pJson["y"].toInt())));

    std::vector<bool> ininv;
    auto ininvArray = pJson["ininv"].toArray();
    for (const auto& inv : ininvArray)
    {
        ininv.push_back(inv.toBool());
    }
    GetLogicCell()->SetInputInversions(ininv);
    GetLogicCell()->SetOutputInversions(std::vector<bool>{pJson["outinv"].toBool()});
}

IBaseComponent* OrGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new OrGate(*this, pCoreLogic);
}

QJsonObject OrGate::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::OR_GATE;
    json["x"] = x();
    json["y"] = y();
    json["dir"] = static_cast<int32_t>(mDirection);
    json["inputs"] = mInputCount;

    {
        QJsonArray ininv;

        for(const bool& inv : mLogicCell->GetInputInversions())
        {
            ininv.append(inv);
        }

        json["ininv"] = ininv;

        json["outinv"] = QJsonValue(mLogicCell->GetOutputInversions()[0]);
    }

    return json;
}

SwVersion OrGate::GetMinVersion() const
{
    return SwVersion(0, 0, 0);
}
