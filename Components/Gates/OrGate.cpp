#include "OrGate.h"
#include "CoreLogic.h"
#include "LogicCells/LogicOrGateCell.h"

OrGate::OrGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection):
    AbstractGate(pCoreLogic, std::make_shared<LogicOrGateCell>(pInputCount), pInputCount, pDirection)
{
    mComponentText = components::gates::OR_TEXT;

#warning not suitable for gates with more inputs / turned gates
    mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 20)));
    mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 60)));
    mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(60, 40)));
}

OrGate::OrGate(const OrGate& pObj, const CoreLogic* pCoreLogic):
    OrGate(pCoreLogic, pObj.mInputCount, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mOutputInverted = pObj.mOutputInverted;
    mInputInverted= pObj.mInputInverted;
};

IBaseComponent* OrGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new OrGate(*this, pCoreLogic);
}
