#include "BufferGate.h"
#include "CoreLogic.h"
#include "HelperFunctions.h"
#include "LogicCells/LogicBufferGateCell.h"

BufferGate::BufferGate(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractGate(pCoreLogic, std::make_shared<LogicBufferGateCell>(), 1, pDirection)
{
    mComponentText = components::gates::BUFFER_TEXT;
}

BufferGate::BufferGate(const BufferGate& pObj, const CoreLogic* pCoreLogic):
    BufferGate(pCoreLogic, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

BufferGate::BufferGate(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    BufferGate(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()))
{
    setPos(SnapToGrid(QPointF(pJson["x"].toInt(), pJson["y"].toInt())));

    GetLogicCell()->SetInputInversions(std::vector<bool>{pJson["ininv"].toBool()});
    GetLogicCell()->SetOutputInversions(std::vector<bool>{pJson["outinv"].toBool()});
}

IBaseComponent* BufferGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new BufferGate(*this, pCoreLogic);
}

QJsonObject BufferGate::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::BUFFER_GATE;
    json["x"] = x();
    json["y"] = y();
    json["dir"] = static_cast<int32_t>(mDirection);

    json["ininv"] = QJsonValue(mLogicCell->GetInputInversions()[0]);
    json["outinv"] = QJsonValue(mLogicCell->GetOutputInversions()[0]);

    return json;
}

SwVersion BufferGate::GetMinVersion(void) const
{
    return SwVersion(0, 0, 0);
}
