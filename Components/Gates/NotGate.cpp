#include "NotGate.h"

NotGate::NotGate(Direction pDirection):
    AbstractGate(1, pDirection)
{
    mComponentText = components::gates::NOT_TEXT;
    mOutputInverted = true;
}

NotGate::NotGate(const NotGate& pObj):
    AbstractGate(pObj.mInputCount, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mOutputInverted = pObj.mOutputInverted;
    mInputInverted= pObj.mInputInverted;
};

BaseComponent* NotGate::CloneBaseComponent() const
{
    return new NotGate(*this);
}
