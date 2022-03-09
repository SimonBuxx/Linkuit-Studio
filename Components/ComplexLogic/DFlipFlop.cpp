#include "DFlipFlop.h"
#include "CoreLogic.h"
#include "LogicCells/LogicDFlipFlopCell.h"

DFlipFlop::DFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicDFlipFlopCell>(), 2, 2, pDirection)
{
    mComponentText = components::complex_logic::D_FLIPFLOP_TEXT;
    mInputLabels = {"D", ">"};
    mOutputLabels = {"Q", "Q̅"};

#warning not suitable for gates with more inputs / turned gates
    mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 20)));
    mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 60)));
    mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(60, 20)));
    mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(60, 60)));

    if (mLogicCell)
    {
#warning implement global OnWakeUp
        QObject::connect(pCoreLogic, &CoreLogic::SimulationStartSignal, static_cast<LogicDFlipFlopCell*>(mLogicCell.get()), &LogicDFlipFlopCell::OnWakeUp);
    }
}

DFlipFlop::DFlipFlop(const DFlipFlop& pObj, const CoreLogic* pCoreLogic):
    DFlipFlop(pCoreLogic, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mInputLabels = pObj.mInputLabels;
    mOutputLabels = pObj.mOutputLabels;
    mOutputInverted = pObj.mOutputInverted;
    mInputInverted= pObj.mInputInverted;
};

IBaseComponent* DFlipFlop::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new DFlipFlop(*this, pCoreLogic);
}
