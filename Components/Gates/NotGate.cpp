#include "NotGate.h"
#include "CoreLogic.h"
#include "LogicCells/LogicNotGateCell.h"

NotGate::NotGate(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractGate(pCoreLogic, std::make_shared<LogicNotGateCell>(), 1, pDirection)
{
    mComponentText = components::gates::NOT_TEXT;
}

NotGate::NotGate(const NotGate& pObj, const CoreLogic* pCoreLogic):
    NotGate(pCoreLogic, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

NotGate::NotGate(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    NotGate(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()))
{
    setPos(SnapToGrid(QPointF(pJson["x"].toInt(), pJson["y"].toInt())));

    GetLogicCell()->SetInputInversions(std::vector<bool>{pJson["ininv"].toBool()});
    GetLogicCell()->SetOutputInversions(std::vector<bool>{pJson["outinv"].toBool()});
}

IBaseComponent* NotGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new NotGate(*this, pCoreLogic);
}

QJsonObject NotGate::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::NOT_GATE;
    json["x"] = x();
    json["y"] = y();
    json["dir"] = static_cast<int32_t>(mDirection);

    json["ininv"] = QJsonValue(mLogicCell->GetInputInversions()[0]);
    json["outinv"] = QJsonValue(mLogicCell->GetOutputInversions()[0]);

    return json;
}
