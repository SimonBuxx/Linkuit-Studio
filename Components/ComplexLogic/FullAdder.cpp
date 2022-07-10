#include "FullAdder.h"
#include "CoreLogic.h"
#include "LogicCells/LogicFullAdderCell.h"

FullAdder::FullAdder(const CoreLogic* pCoreLogic, Direction pDirection):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicFullAdderCell>(), 3, 2, pDirection)
{
    mComponentText = components::complex_logic::FULL_ADDER_TEXT;
    mInputLabels = {"", "", ""};
    mOutputLabels = {"S", "C"};
}

FullAdder::FullAdder(const FullAdder& pObj, const CoreLogic* pCoreLogic):
    FullAdder(pCoreLogic, pObj.mDirection)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

FullAdder::FullAdder(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    FullAdder(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()))
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

IBaseComponent* FullAdder::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new FullAdder(*this, pCoreLogic);
}

QJsonObject FullAdder::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::FULL_ADDER;
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
