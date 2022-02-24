#include "AndGate.h"
#include "CoreLogic.h"
#include "LogicCells/LogicAndGateCell.h"

AndGate::AndGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection):
    AbstractGate(pCoreLogic, std::make_shared<LogicAndGateCell>(pInputCount), pInputCount, pDirection)
{
    mComponentText = components::gates::AND_TEXT;

    QObject::connect(pCoreLogic, &CoreLogic::SimulationStopSignal, this, [&](){
        std::static_pointer_cast<LogicAndGateCell>(mLogicCell)->Shutdown();
    });
}

AndGate::AndGate(const AndGate& pObj, const CoreLogic* pCoreLogic):
    AndGate(pCoreLogic, pObj.mInputCount, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mOutputInverted = pObj.mOutputInverted;
    mInputInverted= pObj.mInputInverted;
    mLogicCell = std::make_shared<LogicAndGateCell>(mInputCount);
};

BaseComponent* AndGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new AndGate(*this, pCoreLogic);
}
