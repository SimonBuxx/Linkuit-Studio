#include "FullAdder.h"
#include "CoreLogic.h"
#include "LogicCells/LogicFullAdderCell.h"

FullAdder::FullAdder(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicFullAdderCell>(), 3, 2, pDirection)
{
    mComponentText = components::complex_logic::FULL_ADDER_TEXT;
    mInputLabels = {"", "", ""};
    mOutputLabels = {"S", "C"};
}

FullAdder::FullAdder(const FullAdder& pObj, const CoreLogic* pCoreLogic):
    FullAdder(pCoreLogic, pObj.mDirection)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

IBaseComponent* FullAdder::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new FullAdder(*this, pCoreLogic);
}
