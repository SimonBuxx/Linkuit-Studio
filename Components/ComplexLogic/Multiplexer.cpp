#include "Multiplexer.h"
#include "CoreLogic.h"
#include "LogicCells/LogicMultiplexerCell.h"

Multiplexer::Multiplexer(const CoreLogic* pCoreLogic, Direction pDirection, uint8_t pDigitCount):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicMultiplexerCell>(pDigitCount), pDigitCount + std::pow(2, pDigitCount), 1, pDirection, pDigitCount),
    mDigitCount(pDigitCount)
{
    Q_ASSERT(pDigitCount < 10);
    mComponentText = components::complex_logic::MULTIPLEXER_TEXT;
    for (size_t i = 0; i < mInputCount; i++)
    {
        if (i < pDigitCount)
        {
            mInputLabels.push_back(QString(QString::number(2) + helpers::SUPERSCRIPTS[i]));
        }
        else
        {
            mInputLabels.push_back(QString::fromStdString(std::to_string(i - pDigitCount)));
        }
    }

    for (size_t i = 0; i < mOutputCount; i++)
    {
        mOutputLabels.push_back("");
    }

    mSmallerDescription = true;
}

Multiplexer::Multiplexer(const Multiplexer& pObj, const CoreLogic* pCoreLogic):
    Multiplexer(pCoreLogic, pObj.mDirection, pObj.mDigitCount)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

IBaseComponent* Multiplexer::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new Multiplexer(*this, pCoreLogic);
}
