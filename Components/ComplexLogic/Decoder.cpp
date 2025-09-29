#include "Decoder.h"
#include "CoreLogic.h"
#include "HelperFunctions.h"
#include "LogicCells/LogicDecoderCell.h"

Decoder::Decoder(const CoreLogic* pCoreLogic, Direction pDirection, uint8_t pInputCount):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicDecoderCell>(pInputCount), pInputCount, std::pow(2, pInputCount), pDirection, 0, false, true)
{
    mComponentText = components::complex_logic::DECODER_TEXT;
    for (size_t i = 0; i < mInputCount; i++)
    {
        mInputLabels.push_back(QString(QString::number(2) + helpers::SUPERSCRIPTS[i]));
    }

    for (size_t i = 0; i < mOutputCount; i++)
    {
        mOutputLabels.push_back(QString::number(i));
    }

    mDescriptionFontSize = 8;
}

Decoder::Decoder(const Decoder& pObj, const CoreLogic* pCoreLogic):
    Decoder(pCoreLogic, pObj.mDirection, pObj.mInputCount)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

Decoder::Decoder(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    Decoder(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()), pJson["inputs"].toInt())
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

IBaseComponent* Decoder::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new Decoder(*this, pCoreLogic);
}

QJsonObject Decoder::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::DECODER;
    json["x"] = x();
    json["y"] = y();
    json["dir"] = static_cast<int32_t>(mDirection);
    json["inputs"] = mInputCount;
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

SwVersion Decoder::GetMinVersion(void) const
{
    return SwVersion(0, 0, 0);
}
