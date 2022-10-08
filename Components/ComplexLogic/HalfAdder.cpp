#include "HalfAdder.h"
#include "CoreLogic.h"
#include "LogicCells/LogicHalfAdderCell.h"

HalfAdder::HalfAdder(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicHalfAdderCell>(), 2, 2, pDirection)
{
    mComponentText = components::complex_logic::HALF_ADDER_TEXT;
    mInputLabels = {"", ""};
    mOutputLabels = {"S", "C"};
}

HalfAdder::HalfAdder(const HalfAdder& pObj, const CoreLogic* pCoreLogic):
    HalfAdder(pCoreLogic, pObj.mDirection)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

HalfAdder::HalfAdder(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    HalfAdder(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()))
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

IBaseComponent* HalfAdder::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new HalfAdder(*this, pCoreLogic);
}

QJsonObject HalfAdder::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::HALF_ADDER;
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

SwVersion HalfAdder::GetMinVersion(void) const
{
    return SwVersion(0, 0, 0);
}
