#include "Demultiplexer.h"
#include "CoreLogic.h"
#include "LogicCells/LogicDemultiplexerCell.h"

Demultiplexer::Demultiplexer(const CoreLogic* pCoreLogic, Direction pDirection, uint8_t pDigitCount):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicDemultiplexerCell>(pDigitCount), pDigitCount + 1, std::pow(2, pDigitCount), pDirection, pDigitCount),
    mDigitCount(pDigitCount)
{
    Q_ASSERT(pDigitCount < 10);
    mComponentText = components::complex_logic::DEMULTIPLEXER_TEXT;
    for (size_t i = 0; i < mInputCount; i++)
    {
        if (i < pDigitCount)
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

    mSmallerDescription = true;
}

Demultiplexer::Demultiplexer(const Demultiplexer& pObj, const CoreLogic* pCoreLogic):
    Demultiplexer(pCoreLogic, pObj.mDirection, pObj.mDigitCount)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

IBaseComponent* Demultiplexer::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new Demultiplexer(*this, pCoreLogic);
}
