#include "AndGate.h"
#include "CoreLogic.h"
#include "LogicCells/LogicAndGateCell.h"

AndGate::AndGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection):
    AbstractGate(pCoreLogic, std::make_shared<LogicAndGateCell>(pInputCount), pInputCount, pDirection)
{
    mComponentText = components::gates::AND_TEXT;
}

AndGate::AndGate(const AndGate& pObj, const CoreLogic* pCoreLogic):
    AndGate(pCoreLogic, pObj.mInputCount, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

AndGate::AndGate(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    AndGate(pCoreLogic, pJson["inputs"].toInt(), static_cast<Direction>(pJson["dir"].toInt()))
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

IBaseComponent* AndGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new AndGate(*this, pCoreLogic);
}

QJsonObject AndGate::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::AND_GATE;
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
