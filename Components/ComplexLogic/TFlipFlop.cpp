#include "TFlipFlop.h"
#include "CoreLogic.h"
#include "LogicCells/LogicTFlipFlopCell.h"

TFlipFlop::TFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicTFlipFlopCell>(), 2, 2, pDirection)
{
    mComponentText = components::complex_logic::T_FLIPFLOP_TEXT;
    mInputLabels = {"T", ">"};
    mOutputLabels = {"Q", "Q̅"};
}

TFlipFlop::TFlipFlop(const TFlipFlop& pObj, const CoreLogic* pCoreLogic):
    TFlipFlop(pCoreLogic, pObj.mDirection)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

TFlipFlop::TFlipFlop(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    TFlipFlop(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()))
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

IBaseComponent* TFlipFlop::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new TFlipFlop(*this, pCoreLogic);
}

QJsonObject TFlipFlop::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::T_FLIPFLOP;
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

SwVersion TFlipFlop::GetMinVersion(void) const
{
    return SwVersion(0, 0, 0);
}
