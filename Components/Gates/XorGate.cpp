#include "XorGate.h"
#include "CoreLogic.h"
#include "LogicCells/LogicXorGateCell.h"

XorGate::XorGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection):
    AbstractGate(pCoreLogic, std::make_shared<LogicXorGateCell>(pInputCount), pInputCount, pDirection)
{
    mComponentText = components::gates::XOR_TEXT;

#warning not suitable for gates with more inputs / turned gates
    mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 20)));
    mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 60)));
    mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(60, 40)));

    QObject::connect(pCoreLogic, &CoreLogic::SimulationAdvanceSignal, mLogicCell.get(), &LogicBaseCell::OnSimulationAdvance);
}

XorGate::XorGate(const XorGate& pObj, const CoreLogic* pCoreLogic):
    XorGate(pCoreLogic, pObj.mInputCount, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mOutputInverted = pObj.mOutputInverted;
    mInputInverted= pObj.mInputInverted;
};

IBaseComponent* XorGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new XorGate(*this, pCoreLogic);
}
