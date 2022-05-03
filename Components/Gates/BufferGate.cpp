#include "BufferGate.h"
#include "CoreLogic.h"
#include "LogicCells/LogicBufferGateCell.h"

BufferGate::BufferGate(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractGate(pCoreLogic, std::make_shared<LogicBufferGateCell>(), 1, pDirection)
{
    mComponentText = components::gates::BUFFER_TEXT;
}

BufferGate::BufferGate(const BufferGate& pObj, const CoreLogic* pCoreLogic):
    BufferGate(pCoreLogic, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

IBaseComponent* BufferGate::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new BufferGate(*this, pCoreLogic);
}
