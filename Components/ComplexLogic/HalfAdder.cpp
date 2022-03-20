#include "HalfAdder.h"
#include "CoreLogic.h"
#include "LogicCells/LogicHalfAdderCell.h"

HalfAdder::HalfAdder(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicHalfAdderCell>(), 2, 2, pDirection)
{
    mComponentText = components::complex_logic::HALF_ADDER_TEXT;
    mInputLabels = {"", ""};
    mOutputLabels = {"S", "C"};
}

HalfAdder::HalfAdder(const HalfAdder& pObj, const CoreLogic* pCoreLogic):
    HalfAdder(pCoreLogic, pObj.mDirection)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

IBaseComponent* HalfAdder::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new HalfAdder(*this, pCoreLogic);
}
