#include "LogicEncoderCell.h"

LogicEncoderCell::LogicEncoderCell(uint8_t pOutputCount):
    LogicBaseCell(std::pow(2, pOutputCount), pOutputCount),
    mOutputStates(pOutputCount, LogicState::LOW),
    mStateChanged(true),
    mOutputCount(pOutputCount),
    mPreviousValue(-1)
{}

void LogicEncoderCell::LogicFunction()
{
    int32_t value = -1;

    for (size_t i = 0; i < std::pow(2, mOutputCount - 1); i++)
    {
        if (mInputStates[i] == LogicState::HIGH)
        {
            value = i;
        }
    }

    if (value != mPreviousValue)
    {
        if (value >= 0 && mOutputStates[mOutputCount - 1] == LogicState::LOW)
        {
            mOutputStates[mOutputCount - 1] = LogicState::HIGH;
            mStateChanged = true;
        }
        else if (value < 0 && mOutputStates[mOutputCount - 1] == LogicState::HIGH)
        {
            mOutputStates[mOutputCount - 1] = LogicState::LOW;
            mStateChanged = true;
        }

        uint8_t remainder = 0;
        uint8_t val = (value >= 0) ? value : 0;
        for (uint8_t i = 0; i < mOutputCount - 1; i++)
        {
            remainder = val % 2;
            val /= 2;
            auto newState = (remainder == 1) ? LogicState::HIGH : LogicState::LOW;
            if (mOutputStates[i] != newState)
            {
                mOutputStates[i] = newState;
                mStateChanged = true;
            }
        }
    }

    mPreviousValue = value;
}

LogicState LogicEncoderCell::GetOutputState(uint32_t pOutput) const
{
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mOutputStates[pOutput]);
    }
    else
    {
        return mOutputStates[pOutput];
    }
}

void LogicEncoderCell::OnSimulationAdvance()
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

void LogicEncoderCell::OnWakeUp()
{
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    //mOutputStates = std::vector<LogicState>(std::pow(2, mInputCount), LogicState::LOW),
    mNextUpdateTime = UpdateTime::NOW;

    mPreviousValue = -1;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicEncoderCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}

