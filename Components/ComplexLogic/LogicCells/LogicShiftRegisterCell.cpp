#include "LogicShiftRegisterCell.h"

LogicShiftRegisterCell::LogicShiftRegisterCell(uint8_t pBitWidth):
    LogicBaseCell(2, pBitWidth + 1),
    mOutputStates(pBitWidth + 1, LogicState::LOW),
    mPrevInputStates(pBitWidth + 1, LogicState::LOW),
    mStateChanged(true),
    mBitWidth(pBitWidth)
{}

void LogicShiftRegisterCell::LogicFunction()
{
    if (mPrevInputStates[0] == LogicState::LOW && mInputStates[0] == LogicState::HIGH)
    {
        mOutputStates.insert(mOutputStates.begin(), mInputStates[1]);
        mOutputStates.pop_back();

        mStateChanged = true;
    }

    mPrevInputStates = mInputStates;
}

LogicState LogicShiftRegisterCell::GetOutputState(uint32_t pOutput) const
{
    Q_ASSERT(pOutput <= mBitWidth);
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mOutputStates[pOutput]);
    }
    else
    {
        return mOutputStates[pOutput];
    }
}

void LogicShiftRegisterCell::OnSimulationAdvance()
{
    AdvanceUpdateTime();

    if (mStateChanged)
    {
        mStateChanged = false;
        for (size_t i = 0; i < mOutputStates.size(); i++)
        {
            NotifySuccessor(i, mOutputStates[i]);
        }

        emit StateChangedSignal();
    }
}

void LogicShiftRegisterCell::OnWakeUp()
{   
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mPrevInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mNextUpdateTime = UpdateTime::NOW;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicShiftRegisterCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}
