#include "DFlipFlop.h"
#include "CoreLogic.h"
#include "LogicCells/LogicDFlipFlopCell.h"

DFlipFlop::DFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicDFlipFlopCell>(), 2, 2, pDirection)
{
    mComponentText = components::complex_logic::D_FLIPFLOP_TEXT;
    mInputLabels = {"D", ">"};
    mOutputLabels = {"Q", "Q̅"};
}

DFlipFlop::DFlipFlop(const DFlipFlop& pObj, const CoreLogic* pCoreLogic):
    DFlipFlop(pCoreLogic, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mInputLabels = pObj.mInputLabels;
    mOutputLabels = pObj.mOutputLabels;
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

IBaseComponent* DFlipFlop::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new DFlipFlop(*this, pCoreLogic);
}
