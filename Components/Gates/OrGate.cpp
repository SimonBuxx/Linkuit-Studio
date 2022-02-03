#include "OrGate.h"

OrGate::OrGate(uint8_t pInputCount, Direction pDirection):
    AbstractGate(pInputCount, pDirection)
{
    mComponentText = components::gates::OR_TEXT;
}

OrGate::OrGate(const OrGate& pObj):
    AbstractGate(pObj.mInputCount, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mOutputInverted = pObj.mOutputInverted;
    mInputInverted= pObj.mInputInverted;
};

BaseComponent* OrGate::CloneBaseComponent() const
{
    return new OrGate(*this);
}
