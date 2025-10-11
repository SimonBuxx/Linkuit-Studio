#include "JkMsFlipFlop.h"
#include "HelperFunctions.h"
#include "LogicCells/LogicJkMsFlipFlopCell.h"

JkMasterSlaveFlipFlop::JkMasterSlaveFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicJkMsFlipFlopCell>(), 3, 2, pDirection)
{
    mComponentText = components::complex_logic::JK_MS_FLIPFLOP_TEXT;
    mInputLabels = {"J", ">", "K"};
    mOutputLabels = {"Q", "Q̅"};

    mDescriptionFontSize = 10;
}

JkMasterSlaveFlipFlop::JkMasterSlaveFlipFlop(const JkMasterSlaveFlipFlop& pObj, const CoreLogic* pCoreLogic):
    JkMasterSlaveFlipFlop(pCoreLogic, pObj.mDirection)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

JkMasterSlaveFlipFlop::JkMasterSlaveFlipFlop(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    JkMasterSlaveFlipFlop(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()))
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

IBaseComponent* JkMasterSlaveFlipFlop::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new JkMasterSlaveFlipFlop(*this, pCoreLogic);
}

QJsonObject JkMasterSlaveFlipFlop::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::JK_MS_FLIPFLOP;
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

SwVersion JkMasterSlaveFlipFlop::GetMinVersion() const
{
    return SwVersion(0, 9, 8);
}
