#include "RsFlipFlop.h"
#include "CoreLogic.h"
#include "LogicCells/LogicRsFlipFlopCell.h"

RsFlipFlop::RsFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicRsFlipFlopCell>(), 2, 2, pDirection)
{
    mComponentText = components::complex_logic::RS_FLIPFLOP_TEXT;
    mInputLabels = {"S", "R"};
    mOutputLabels = {"Q", "Q̅"};
}

RsFlipFlop::RsFlipFlop(const RsFlipFlop& pObj, const CoreLogic* pCoreLogic):
    RsFlipFlop(pCoreLogic, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mInputLabels = pObj.mInputLabels;
    mOutputLabels = pObj.mOutputLabels;
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

IBaseComponent* RsFlipFlop::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new RsFlipFlop(*this, pCoreLogic);
}
