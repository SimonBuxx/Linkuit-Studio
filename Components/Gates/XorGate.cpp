#include "XorGate.h"

XorGate::XorGate(uint8_t pInputCount, Direction pDirection):
    AbstractGate(pInputCount, pDirection)
{
    mComponentText = components::gates::XOR_TEXT;
}

XorGate::XorGate(const XorGate& pObj):
    AbstractGate(pObj.mInputCount, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mOutputInverted = pObj.mOutputInverted;
    mInputInverted= pObj.mInputInverted;
};

BaseComponent* XorGate::CloneBaseComponent() const
{
    return new XorGate(*this);
}
