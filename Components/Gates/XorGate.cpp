#include "XorGate.h"
#include "CoreLogic.h"
#include "LogicCells/LogicXorGateCell.h"

XorGate::XorGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection):
    AbstractGate(pCoreLogic, std::make_shared<LogicXorGateCell>(pInputCount), pInputCount, pDirection)
{
    mComponentText = components::gates::XOR_TEXT;
}

XorGate::XorGate(const XorGate& pObj, const CoreLogic* pCoreLogic):
    XorGate(pCoreLogic, pObj.mInputCount, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mOutputInverted = pObj.mOutputInverted;
    mInputInverted= pObj.mInputInverted;
    mLogicCell = std::make_shared<LogicXorGateCell>(mInputCount);
};

BaseComponent* XorGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new XorGate(*this, pCoreLogic);
}
