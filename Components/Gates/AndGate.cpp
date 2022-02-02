#include "AndGate.h"

AndGate::AndGate(uint8_t pInputCount, Direction pDirection):
    AbstractGate(pInputCount, pDirection)
{
    mComponentText = components::gates::AND_TEXT;
}

AndGate::AndGate(const AndGate& pObj):
    AbstractGate(pObj.mInputCount, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
};

BaseComponent* AndGate::CloneBaseComponent() const
{
    return new AndGate(*this);
}
