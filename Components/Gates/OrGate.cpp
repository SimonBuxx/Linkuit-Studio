#include "OrGate.h"
#include "CoreLogic.h"
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
    mOutputInverted = pObj.mOutputInverted;
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
};

IBaseComponent* OrGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new OrGate(*this, pCoreLogic);
}
