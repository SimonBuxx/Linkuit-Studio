#include "Demultiplexer.h"
#include "CoreLogic.h"
#include "HelperFunctions.h"
#include "LogicCells/LogicDemultiplexerCell.h"

Demultiplexer::Demultiplexer(const CoreLogic* pCoreLogic, Direction pDirection, uint8_t pBitWidth):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicDemultiplexerCell>(pBitWidth), pBitWidth + 1, std::pow(2, pBitWidth), pDirection, pBitWidth),
    mBitWidth(pBitWidth)
{
    Q_ASSERT(pBitWidth < 10);
    mComponentText = components::complex_logic::DEMULTIPLEXER_TEXT;
    for (size_t i = 0; i < mInputCount; i++)
    {
        if (i < pBitWidth)
        {
            mInputLabels.push_back(QString(QString::number(2) + helpers::SUPERSCRIPTS[i]));
        }
        else
        {
            mInputLabels.push_back("");
        }
    }

    for (size_t i = 0; i < mOutputCount; i++)
    {
        mOutputLabels.push_back(QString::fromStdString(std::to_string(i)));
    }

    mDescriptionFontSize = 8;
}

Demultiplexer::Demultiplexer(const Demultiplexer& pObj, const CoreLogic* pCoreLogic):
    Demultiplexer(pCoreLogic, pObj.mDirection, pObj.mBitWidth)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

Demultiplexer::Demultiplexer(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    Demultiplexer(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()), pJson["bits"].toInt())
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

IBaseComponent* Demultiplexer::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new Demultiplexer(*this, pCoreLogic);
}

QJsonObject Demultiplexer::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::DEMULTIPLEXER;
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

SwVersion Demultiplexer::GetMinVersion() const
{
    return SwVersion(0, 0, 0);
}
