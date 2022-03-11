#include "AndGate.h"
#include "CoreLogic.h"
#include "LogicCells/LogicAndGateCell.h"

AndGate::AndGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection):
    AbstractGate(pCoreLogic, std::make_shared<LogicAndGateCell>(pInputCount), pInputCount, pDirection)
{
    mComponentText = components::gates::AND_TEXT;

#warning not suitable for gates with more inputs / turned gates
    mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 20)));
    mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 60)));
    mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(60, 40)));
}

AndGate::AndGate(const AndGate& pObj, const CoreLogic* pCoreLogic):
    AndGate(pCoreLogic, pObj.mInputCount, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mOutputInverted = pObj.mOutputInverted;
    mInputInverted = pObj.mInputInverted;
};

IBaseComponent* AndGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new AndGate(*this, pCoreLogic);
}
