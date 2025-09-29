#include "DMsFlipFlop.h"
#include "HelperFunctions.h"
#include "LogicCells/LogicDMsFlipFlopCell.h"

DMasterSlaveFlipFlop::DMasterSlaveFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicDMsFlipFlopCell>(), 2, 2, pDirection)
{
    mComponentText = components::complex_logic::D_MS_FLIPFLOP_TEXT;
    mInputLabels = {"D", ">"};
    mOutputLabels = {"Q", "Q̅"};

    mDescriptionFontSize = 10;
}

DMasterSlaveFlipFlop::DMasterSlaveFlipFlop(const DMasterSlaveFlipFlop& pObj, const CoreLogic* pCoreLogic):
    DMasterSlaveFlipFlop(pCoreLogic, pObj.mDirection)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

DMasterSlaveFlipFlop::DMasterSlaveFlipFlop(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    DMasterSlaveFlipFlop(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()))
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

IBaseComponent* DMasterSlaveFlipFlop::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new DMasterSlaveFlipFlop(*this, pCoreLogic);
}

QJsonObject DMasterSlaveFlipFlop::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::D_MS_FLIPFLOP;
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

SwVersion DMasterSlaveFlipFlop::GetMinVersion(void) const
{
    return SwVersion(0, 9, 8);
}
