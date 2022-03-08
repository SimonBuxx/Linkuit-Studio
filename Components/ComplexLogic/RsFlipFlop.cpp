#include "RsFlipFlop.h"
#include "CoreLogic.h"
#include "LogicCells/LogicRsFlipFlopCell.h"

RsFlipFlop::RsFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicRsFlipFlopCell>(), 2, 2, pDirection)
{
    mComponentText = components::complex_logic::RS_FLIPFLOP_TEXT;
    mInputLabels = {"S", "R"};
    mOutputLabels = {"Q", "Q̅"};

#warning not suitable for gates with more inputs / turned gates
    mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 20)));
    mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 60)));
    mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(60, 20)));
    mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(60, 60)));

    if (mLogicCell)
    {
#warning implement global OnWakeUp
        QObject::connect(pCoreLogic, &CoreLogic::SimulationStartSignal, static_cast<LogicRsFlipFlopCell*>(mLogicCell.get()), &LogicRsFlipFlopCell::OnWakeUp);
    }
}

RsFlipFlop::RsFlipFlop(const RsFlipFlop& pObj, const CoreLogic* pCoreLogic):
    RsFlipFlop(pCoreLogic, pObj.mDirection)
{
    mComponentText = pObj.mComponentText;
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mInputLabels = pObj.mInputLabels;
    mOutputLabels = pObj.mOutputLabels;
    mOutputInverted = pObj.mOutputInverted;
    mInputInverted= pObj.mInputInverted;
};

IBaseComponent* RsFlipFlop::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new RsFlipFlop(*this, pCoreLogic);
}
