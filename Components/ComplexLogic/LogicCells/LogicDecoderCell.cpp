#include "LogicDecoderCell.h"

LogicDecoderCell::LogicDecoderCell(uint8_t pInputCount):
    LogicBaseCell(pInputCount, std::pow(2, pInputCount)),
    mOutputStates(std::pow(2, pInputCount), LogicState::LOW),
    mStateChanged(true),
    mInputCount(pInputCount),
    mPreviousValue(0)
{}

void LogicDecoderCell::LogicFunction()
{
    uint32_t value = 0;

    for (size_t i = 0; i < mInputCount; i++)
    {
        value += (mInputStates[i] == LogicState::HIGH) ? std::pow(2, i) : 0;
    }

    if (value != mPreviousValue && mOutputStates[mPreviousValue] != LogicState::LOW)
    {
        mOutputStates[mPreviousValue] = LogicState::LOW;
        mStateChanged = true;
    }

    if (mOutputStates[value] != LogicState::HIGH)
    {
        mOutputStates[value] = LogicState::HIGH;
        mStateChanged = true;
    }

    mPreviousValue = value;
}

LogicState LogicDecoderCell::GetOutputState(uint32_t pOutput) const
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

void LogicDecoderCell::OnSimulationAdvance()
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

void LogicDecoderCell::OnWakeUp()
{   
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mOutputStates = std::vector<LogicState>(std::pow(2, mInputCount), LogicState::LOW),
    mNextUpdateTime = UpdateTime::NOW;

    mPreviousValue = 0;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicDecoderCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}
