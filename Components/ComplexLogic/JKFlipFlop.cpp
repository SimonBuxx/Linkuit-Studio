#include "JKFlipFlop.h"
#include "CoreLogic.h"
#include "LogicCells/LogicJKFlipFlopCell.h"

JKFlipFlop::JKFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicJKFlipFlopCell>(), 3, 2, pDirection)
{
    mComponentText = components::complex_logic::JK_FLIPFLOP_TEXT;
    mInputLabels = {"J", ">", "K"};
    mOutputLabels = {"Q", "Q̅"};
}

JKFlipFlop::JKFlipFlop(const JKFlipFlop& pObj, const CoreLogic* pCoreLogic):
    JKFlipFlop(pCoreLogic, pObj.mDirection)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

JKFlipFlop::JKFlipFlop(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    JKFlipFlop(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()))
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

IBaseComponent* JKFlipFlop::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new JKFlipFlop(*this, pCoreLogic);
}

QJsonObject JKFlipFlop::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::JK_FLIPFLOP;
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
