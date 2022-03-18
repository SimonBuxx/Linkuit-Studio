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

IBaseComponent* AndGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new AndGate(*this, pCoreLogic);
}
