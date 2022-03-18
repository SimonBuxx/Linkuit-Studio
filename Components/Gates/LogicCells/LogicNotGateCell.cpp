#include "LogicNotGateCell.h"

LogicNotGateCell::LogicNotGateCell():
    LogicBaseCell(1, 1),
    mPreviousState(LogicState::LOW),
    mCurrentState(LogicState::LOW),
    mStateChanged(true)
{
    mOutputInverted[0] = true;
}

void LogicNotGateCell::LogicFunction()
{
    if (mCurrentState != mInputStates[0])
    {
         mCurrentState = mInputStates[0];
         mStateChanged = true;
    }
}

LogicState LogicNotGateCell::GetOutputState(uint32_t pOutput) const
{
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mCurrentState);
    }
    else
    {
        return mCurrentState;
    }
}

void LogicNotGateCell::OnSimulationAdvance()
{
    AdvanceUpdateTime();

    if (mStateChanged)
    {
        mStateChanged = false;
        NotifySuccessor(0, mCurrentState);
        mPreviousState = mCurrentState;

        emit StateChangedSignal();
    }
}

void LogicNotGateCell::OnWakeUp()
{
    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mPreviousState = LogicState::LOW;
    mCurrentState = LogicState::LOW;
    mNextUpdateTime = UpdateTime::NOW;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicNotGateCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates[0] = LogicState::LOW;
    mCurrentState = LogicState::LOW;
    mIsActive = false;
    emit StateChangedSignal();
}
