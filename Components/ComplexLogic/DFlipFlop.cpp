#include "DFlipFlop.h"
#include "CoreLogic.h"
#include "HelperFunctions.h"
//#include "LogicCells/LogicDFlipFlopCell.h"
#include "LogicCells/LogicCustomTestCell.h"

DFlipFlop::DFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicCustomTestCell>(), 3, 2, pDirection)
{
    mComponentText = components::complex_logic::D_FLIPFLOP_TEXT;
    mInputLabels = {"A", "D", ">"};
    mOutputLabels = {"Q", "Q̅"};
}

DFlipFlop::DFlipFlop(const DFlipFlop& pObj, const CoreLogic* pCoreLogic):
    DFlipFlop(pCoreLogic, pObj.mDirection)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

DFlipFlop::DFlipFlop(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    DFlipFlop(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()))
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

IBaseComponent* DFlipFlop::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new DFlipFlop(*this, pCoreLogic);
}

QJsonObject DFlipFlop::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::D_FLIPFLOP;
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

SwVersion DFlipFlop::GetMinVersion(void) const
{
    return SwVersion(0, 0, 0);
}
