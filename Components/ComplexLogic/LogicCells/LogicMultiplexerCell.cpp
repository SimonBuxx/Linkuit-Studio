#include "LogicMultiplexerCell.h"

LogicMultiplexerCell::LogicMultiplexerCell(uint8_t pDigitCount):
    LogicBaseCell(pDigitCount + std::pow(2, pDigitCount), 1),
    mOutputStates(1, LogicState::LOW),
    mStateChanged(true),
    mDigitCount(pDigitCount)
{}

void LogicMultiplexerCell::LogicFunction()
{
    uint32_t input = 0;

    for (size_t i = 0; i < mDigitCount; i++)
    {
        input += (mInputStates[i] == LogicState::HIGH) ? std::pow(2, i) : 0;
    }

    if (mOutputStates[0] != mInputStates[input + mDigitCount])
    {
        mOutputStates[0] = mInputStates[input + mDigitCount];
        mStateChanged = true;
    }
}

LogicState LogicMultiplexerCell::GetOutputState(uint32_t pOutput) const
{
    Q_ASSERT(pOutput <= 1);
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mOutputStates[pOutput]);
    }
    else
    {
        return mOutputStates[pOutput];
    }
}

void LogicMultiplexerCell::OnSimulationAdvance()
{
    AdvanceUpdateTime();

    if (mStateChanged)
    {
        mStateChanged = false;
        NotifySuccessor(0, mOutputStates[0]);

        emit StateChangedSignal();
    }
}

void LogicMultiplexerCell::OnWakeUp()
{   
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mOutputStates[0] = LogicState::LOW;
    mNextUpdateTime = UpdateTime::NOW;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicMultiplexerCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}
