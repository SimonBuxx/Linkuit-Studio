#include "RsClockedFlipFlop.h"
#include "CoreLogic.h"
#include "LogicCells/LogicRsClockedFlipFlopCell.h"

RsClockedFlipFlop::RsClockedFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicRsClockedFlipFlopCell>(), 3, 2, pDirection)
{
    mComponentText = components::complex_logic::RS_FLIPFLOP_TEXT;
    mInputLabels = {"R", ">", "S"};
    mOutputLabels = {"Q", "Q̅"};
}

RsClockedFlipFlop::RsClockedFlipFlop(const RsClockedFlipFlop& pObj, const CoreLogic* pCoreLogic):
    RsClockedFlipFlop(pCoreLogic, pObj.mDirection)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

RsClockedFlipFlop::RsClockedFlipFlop(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    RsClockedFlipFlop(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()))
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

IBaseComponent* RsClockedFlipFlop::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new RsClockedFlipFlop(*this, pCoreLogic);
}

QJsonObject RsClockedFlipFlop::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::RS_CLOCKED_FLIPFLOP;
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

SwVersion RsClockedFlipFlop::GetMinVersion(void) const
{
    return SwVersion(0, 9, 7);
}
