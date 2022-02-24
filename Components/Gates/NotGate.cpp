#include "NotGate.h"
#include "CoreLogic.h"
#include "LogicCells/LogicNotGateCell.h"

NotGate::NotGate(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractGate(pCoreLogic, std::make_shared<LogicNotGateCell>(), 1, pDirection)
{
    mComponentText = components::gates::NOT_TEXT;
    mOutputInverted = true;
}

NotGate::NotGate(const NotGate& pObj, const CoreLogic* pCoreLogic):
    NotGate(pCoreLogic, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mOutputInverted = pObj.mOutputInverted;
    mInputInverted= pObj.mInputInverted;
    mLogicCell = std::make_shared<LogicNotGateCell>();
};

BaseComponent* NotGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new NotGate(*this, pCoreLogic);
}
