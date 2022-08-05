#include "Encoder.h"
#include "CoreLogic.h"
#include "LogicCells/LogicEncoderCell.h"

Encoder::Encoder(const CoreLogic* pCoreLogic, Direction pDirection, uint8_t pOutputCount):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicEncoderCell>(pOutputCount + 1), std::pow(2, pOutputCount), pOutputCount + 1, pDirection, 0, false, true)
{
    mComponentText = components::complex_logic::ENCODER_TEXT;
    for (size_t i = 0; i < mInputCount; i++)
    {
        mInputLabels.push_back(QString::number(i));
    }

    for (size_t i = 0; i < mOutputCount - 1; i++)
    {
        mOutputLabels.push_back(QString(QString::number(2) + helpers::SUPERSCRIPTS[i]));
    }

    mOutputLabels.push_back(QString("V"));

    mSmallerDescription = true;
}

Encoder::Encoder(const Encoder& pObj, const CoreLogic* pCoreLogic):
    Encoder(pCoreLogic, pObj.mDirection, pObj.mOutputCount - 1)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

Encoder::Encoder(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    Encoder(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()), pJson["outputs"].toInt())
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

IBaseComponent* Encoder::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new Encoder(*this, pCoreLogic);
}

QJsonObject Encoder::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::ENCODER;
    json["x"] = x();
    json["y"] = y();
    json["dir"] = static_cast<int32_t>(mDirection);
    json["outputs"] = mOutputCount - 1;
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
