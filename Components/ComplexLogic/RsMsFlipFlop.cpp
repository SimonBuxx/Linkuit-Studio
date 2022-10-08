#include "RsMsFlipFlop.h"
#include "CoreLogic.h"
#include "LogicCells/LogicRsMsFlipFlopCell.h"

RsMasterSlaveFlipFlop::RsMasterSlaveFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicRsMsFlipFlopCell>(), 3, 2, pDirection)
{
    mSmallerDescription = true;
    mComponentText = components::complex_logic::RS_MS_FLIPFLOP_TEXT;
    mInputLabels = {"S", ">", "R"};
    mOutputLabels = {"Q", "Q̅"};
}

RsMasterSlaveFlipFlop::RsMasterSlaveFlipFlop(const RsMasterSlaveFlipFlop& pObj, const CoreLogic* pCoreLogic):
    RsMasterSlaveFlipFlop(pCoreLogic, pObj.mDirection)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

RsMasterSlaveFlipFlop::RsMasterSlaveFlipFlop(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    RsMasterSlaveFlipFlop(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()))
{
    setPos(SnapToGrid(QPointF(pJson["x"].toInt(), pJson["y"].toInt())));

    std::vector<bool> ininv, outinv;
    auto ininvArray = pJson["ininv"].toArray();
    for (const auto& inv : ininvArray)
    {
        ininv.push_back(inv.toBool());
    }
    GetLogicCell()->SetInputInversions(ininv);

    auto outinvArray = pJson["outinv"].toArray();
    for (const auto& inv : outinvArray)
    {
        outinv.push_back(inv.toBool());
    }
    GetLogicCell()->SetOutputInversions(outinv);
}

IBaseComponent* RsMasterSlaveFlipFlop::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new RsMasterSlaveFlipFlop(*this, pCoreLogic);
}

QJsonObject RsMasterSlaveFlipFlop::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::RS_MS_FLIPFLOP;
    json["x"] = x();
    json["y"] = y();
    json["dir"] = static_cast<int32_t>(mDirection);
    {
        QJsonArray ininv, outinv;

        for(const bool& inv : mLogicCell->GetInputInversions())
        {
            ininv.append(inv);
        }

        json["ininv"] = ininv;

        for(const bool& inv : mLogicCell->GetOutputInversions())
        {
            outinv.append(inv);
        }

        json["outinv"] = outinv;
    }

    return json;
}

SwVersion RsMasterSlaveFlipFlop::GetMinVersion(void) const
{
    return SwVersion(0, 0, 0);
}
