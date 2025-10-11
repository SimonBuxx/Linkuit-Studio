#include "RsFlipFlop.h"
#include "CoreLogic.h"
#include "HelperFunctions.h"
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
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

RsFlipFlop::RsFlipFlop(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    RsFlipFlop(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()))
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

IBaseComponent* RsFlipFlop::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new RsFlipFlop(*this, pCoreLogic);
}

QJsonObject RsFlipFlop::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::RS_FLIPFLOP;
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

SwVersion RsFlipFlop::GetMinVersion() const
{
    return SwVersion(0, 0, 0);
}
