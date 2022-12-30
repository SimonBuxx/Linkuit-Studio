#include "LogicHalfAdderCell.h"

LogicHalfAdderCell::LogicHalfAdderCell():
    LogicBaseCell(2, 2),
    mOutputStates(2, LogicState::LOW),
    mStateChanged(true)
{}

void LogicHalfAdderCell::LogicFunction()
{
    if ((mInputStates[0] == LogicState::HIGH && mInputStates[1] == LogicState::LOW) // One input high
        || (mInputStates[0] == LogicState::LOW && mInputStates[1] == LogicState::HIGH))
    {
        if (mOutputStates[0] != LogicState::HIGH)
        {
            mOutputStates[0] = LogicState::HIGH;
            mStateChanged = true;
        }
        if (mOutputStates[1] != LogicState::LOW)
        {
            mOutputStates[1] = LogicState::LOW;
            mStateChanged = true;
        }
    }
    else if (mInputStates[0] == LogicState::HIGH) // Both inputs high
    {
        if (mOutputStates[0] != LogicState::LOW)
        {
            mOutputStates[0] = LogicState::LOW;
            mStateChanged = true;
        }
        if (mOutputStates[1] != LogicState::HIGH)
        {
            mOutputStates[1] = LogicState::HIGH;
            mStateChanged = true;
        }
    }
    else // No input high
    {
        if (mOutputStates[0] != LogicState::LOW)
        {
            mOutputStates[0] = LogicState::LOW;
            mStateChanged = true;
        }
        if (mOutputStates[1] != LogicState::LOW)
        {
            mOutputStates[1] = LogicState::LOW;
            mStateChanged = true;
        }
    }
}

LogicState LogicHalfAdderCell::GetOutputState(uint32_t pOutput) const
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

void LogicHalfAdderCell::OnSimulationAdvance()
{
    AdvanceUpdateTime();

    if (mStateChanged)
    {
        mStateChanged = false;
        NotifySuccessor(0, mOutputStates[0]);
        NotifySuccessor(1, mOutputStates[1]);

        emit StateChangedSignal();
    }
}

void LogicHalfAdderCell::OnWakeUp()
{
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mOutputStates[0] = LogicState::LOW;
    mOutputStates[1] = LogicState::LOW;
    mNextUpdateTime = UpdateTime::NOW;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicHalfAdderCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}
