#include "Counter.h"
#include "CoreLogic.h"
#include "HelperFunctions.h"
#include "LogicCells/LogicCounterCell.h"

Counter::Counter(const CoreLogic* pCoreLogic, Direction pDirection, uint8_t pBitWidth):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicCounterCell>(pBitWidth), 3, pBitWidth, pDirection, 2, false),
    mBitWidth(pBitWidth)
{
    Q_ASSERT(pBitWidth <= 32);
    mComponentText = components::complex_logic::COUNTER_TEXT;

    mInputLabels.push_back("R");
    mInputLabels.push_back("P");
    mInputLabels.push_back(">");

    for (size_t i = 0; i < mOutputCount; i++)
    {
        mOutputLabels.push_back(QString::number(2) + BuildSuperscriptString(i));
    }

    mDescriptionFontSize = 8;
}

Counter::Counter(const Counter& pObj, const CoreLogic* pCoreLogic):
    Counter(pCoreLogic, pObj.mDirection, pObj.mBitWidth)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

Counter::Counter(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    Counter(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()), pJson["bits"].toInt())
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

IBaseComponent* Counter::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new Counter(*this, pCoreLogic);
}

QJsonObject Counter::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::COUNTER;
    json["x"] = x();
    json["y"] = y();
    json["dir"] = static_cast<int32_t>(mDirection);
    json["bits"] = mBitWidth;
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

SwVersion Counter::GetMinVersion(void) const
{
    return SwVersion(0, 9, 7);
}
