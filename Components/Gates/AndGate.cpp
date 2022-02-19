#include "AndGate.h"

AndGate::AndGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection):
    AbstractGate(pCoreLogic, pInputCount, pDirection)
{
    mComponentText = components::gates::AND_TEXT;
}

AndGate::AndGate(const AndGate& pObj, const CoreLogic* pCoreLogic):
    AndGate(pCoreLogic, pObj.mInputCount, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mOutputInverted = pObj.mOutputInverted;
    mInputInverted= pObj.mInputInverted;
};

BaseComponent* AndGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new AndGate(*this, pCoreLogic);
}
